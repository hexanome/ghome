var async = require("async"),
    rules = require("./rules.js"),
    sensors = require("./sensors.js"),
    actuators = require("./actuators.js");

function configure(dbClient) {
  rules.configure(dbClient);
  sensors.configure(dbClient);
  actuators.configure(dbClient);
}

function processEvent(sensorOemId, propertyIndex, cb) {
  // First, we find the corresponding sensor.
  sensors.getSensorFromOem(sensorOemId, function (err, sensor) {
    // We now retrieve the properties with the same type as this sensor.
    sensors.getSensorPropertyFromType(sensor.sensorTypeId, function (err, properties) {
      // We find the first property with index "propertyIndex".
      var currentProperty = null;

      async.forEach(properties, function (property, cb2) {
        if (property.index == propertyIndex) {
          currentProperty = property;
        }

        cb2(null);
      }, function (err2) {
        if (currentProperty == null) {
          cb("ERROR");
          return;
        }

        // We now have the sensor, and the property. We must retrieve the last value for that property.
        sensors.getSensorPropertyValueFromSensorAndProperty(sensor.id, currentProperty.id, function (err3, propertyValue) {
          // We now have the last propertyValue for the corresponding sensor and property.
          // We are now going to fetch all the rule conditions targeting this sensor/property pair.
          sensors.getConditionsFromSensorAndProperty(sensor.id, currentProperty.id, function (err4, conditions) {
            // For each condition, we are now going to retrieve the corresponding rules, and store them in an array.
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
      });
    });
  });
}

function validateRule(rule, cb) {
  // First, we need to retrieve and build the whole condition tree for this rule.
  rules.getConditionsFromRule(rule.id, function (err, conditions) {
    // We find the root condition of the condition tree.
    var rootCondition = findConditionFromId(conditions, rule.rootConditionId);
    if (rootCondition == null) {
      cb(null, false);
      return;
    }

    // We build the tree of conditions form this root.
    buildConditionChilds(conditions, rootCondition);

    // We validate each condition in the tree, starting with the root.
    validateCondition(rootCondition, function (err2, validated) {
      cb(null, validated);      
    });
  });
}

// Utility methods for working on conditions.

function buildConditionChilds(conditions, rootCondition) {
  rootCondition.childs = findConditionsFromParent(conditions, rootCondition.id);

  for (var i = 0; i < rootCondition.childs.length; i++) {
    buildConditionChilds(conditions, rootCondition.childs[i]);
  }
}

function validateCondition(rootCondition, cb) {
  if (rootCondition.type == 0) {
  } else if (rootCondition.type == 1) {
    // This is an "AND" condition, all the childs must be validated.
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

  }
}

function findConditionFromId(conditions, conditionsId) {
  for (var i = 0; i < conditions.length; i++) {
    if (condition.id == conditionsId) {
      return condition;
    }
  }

  return null;
}

function findConditionsFromParent(conditions, parentId) {
  var childs = [];

  for (var i = 0; i < conditions.length; i++) {
    if (condition.parentId == parentId) {
      childs.push(condition);
    }
  }

  return childs;
}