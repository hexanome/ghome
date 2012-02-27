var async = require("async"),
    rules = require("./rules.js"),
    sensors = require("./sensors.js"),
    actuators = require("./actuators.js"),
    conditionUtils = require("./condition-utils.js");

function configure(dbClient) {
  rules.configure(dbClient);
  sensors.configure(dbClient);
  actuators.configure(dbClient);
}

function processEvent(sensorOemId, propertyIndex, cb) {
  // First, we find the corresponding sensor.
  sensors.getSensorFromOem(sensorOemId, function (err, sensor) {
    // We now retrieve the properties with the same type as this sensor.
    console.log("SensorName: " + sensor.name);

    sensors.getSensorPropertiesFromType(sensor.sensorTypeId, function (err, properties) {
      // We find the first property with index "propertyIndex".
      var currentProperty = null;
      for (var i = 0; i < properties.length; i++) {
        var property = properties[i];
        if (property.index == propertyIndex) {
          currentProperty = property;
        }
      }

      if (currentProperty == null) {
        cb("ERROR1");
        return;
      }

      console.log("PropertyId: " + currentProperty.id);
      console.log("PropertyName: " + currentProperty.name);

      // Now that we have the sensor and the property, we process the rules.
      processRules(sensor.id, property.id, cb);
    });
  });
}

function processRules(sensorId, propertyId, cb) {
  // We now have the sensor, and the property. We must retrieve the last value for that property.
  sensors.getSensorPropertyValueFromSensorAndProperty(sensorId, propertyId, function (err3, propertyValue) {
    // We now have the last propertyValue for the corresponding sensor and property.
    // We are now going to fetch all the rule conditions targeting this sensor/property pair.
    rules.getConditionsFromSensorAndProperty(sensorId, propertyId, function (err4, conditions) {
      // For each condition, we are now going to retrieve the corresponding rule, and validate it.
      var allActions = [];

      async.forEach(conditions, function (condition, cb2) {
        // We retrieve the rule.
        rules.getRule(condition.ruleId, function (err5, rule) {
          // We are now going to validate this rule, and if validated, we will add it to the array.
          validateRule(rule, function (err6, validated) {
            // We must now retrieve the action corresponding to the rule if it was validated, and add it to the resulting array.
            if (!validated) {
              cb2(err6);
            }

            rules.getActionsFromRule(rule.id, function (err7, actions) {
              allActions = allActions.concat(actions);
              cb2(err7);
            });
          });
        });
      }, function (err5) {
        // We now return the full list of actions that we must run.
        cb(err5, allActions);
      });
    });
  });  
}

function validateRule(rule, cb) {
  // First, we need to retrieve and build the whole condition tree for this rule.
  rules.getConditionsFromRule(rule.id, function (err, conditions) {
    console.log("Starting the validation of " + conditions.length + " conditions.");

    // We find the root condition of the condition tree.
    var rootCondition = conditionUtils.findRootCondition(conditions);
    if (rootCondition == null) {
      cb(null, false);
      return;
    }

    console.log("Found the rootCondition:" + rootCondition.id);

    // We build the tree of conditions form this root.
    conditionUtils.buildConditionChilds(conditions, rootCondition);

    // We validate each condition in the tree, starting with the root.
    validateCondition(rootCondition, function (err2, validated) {
      cb(null, validated);      
    });
  });
}

// Utility methods for working on conditions.

function validateCondition(rootCondition, cb) {
  console.log("Validating condition: " + rootCondition.id);

  if (rootCondition.type == 0) {
    // This is the case where we must validate the value of a sensor.

    // First, we retrieve the the SensorPropertyValue corresponding to this condition.
    sensors.getSensorPropertyValueFromSensorAndProperty(rootCondition.sensorId, rootCondition.sensorPropertyId, function (err, sensorPropertyValue) {
      if (err) {
        cb(err);
        return;
      }

      console.log("We compare this condition:");
      console.dir(rootCondition);
      console.log("To this propertyValue:");
      console.dir(sensorPropertyValue);

      // We compare the threshold of the condition with the current propertyValue.
      var validated = false;
      if ((rootCondition.value < sensorPropertyValue.value && rootCondition.order == 0)
      || (rootCondition.value == sensorPropertyValue.value && rootCondition.order == 1)
      || (rootCondition.value > sensorPropertyValue.value && rootCondition.order == 2)) {
        validated = true;
      }

      cb(null, validated);
    });
  } else if (rootCondition.type == 1) {
    // This is an "AND" condition, all the childs must be validated.
    if (rootCondition.childs.length == 0) {
      cb(null, false);
      return;
    }

    var validated = true;

    async.forEach(rootCondition.childs, function (child, cb2) {
      validateCondition(child, function (err, childValidated) {
        if (!childValidated) {
          validated = false;
        }
        cb2(null);
      });
    }, function (err) {
      cb(err, validated);
    });
  } else if (rootCondition.type == 2) {
    // This is an "OR" condition, one of the childs must be validated.
    var validated = false;

    async.forEach(rootCondition.childs, function (child, cb2) {
      validateCondition(child, function (err, childValidated) {
        if (childValidated) {
          validated = true;
        }
        cb2(null);
      });
    }, function (err) {
      cb(err, validated);
    });
  } else if (rootCondition.type == 3) {
    // This is a "NOT" condition. Returns the opposite of the first condition.
    if (rootCondition.childs.length == 0) {
      cb(null, false);
      return;
    }

    validateCondition(rootCondition.childs[0], function (err, childValidated) {
      cb(err, !childValidated);
    });
  }
}

// Export methods.
exports.configure = configure;
exports.processEvent = processEvent;
exports.processRules = processRules;