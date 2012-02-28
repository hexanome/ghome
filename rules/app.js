// Dependencies
var sensors = require("./comp/sensors.js"),
    actuators = require("./comp/actuators.js"),
    rules = require("./comp/rules.js"),
    ruleEngine = require("./comp/rule-engine.js");

// Test set for the ruleEngine.

sensors.getSensors(function (err, results) {
  for (var i = 0; i < results.length; i++) {
    console.dir(results[i]);
  }
});


function createPlug(cb) {
  actuators.addActuatorType({ "name" : "Prise électrique" },
    function (err, typeId) {
      actuators.addActuator({
        "name" : "Prise 1",
        "oemId" : "FF9F1E04",
        "actuatorTypeId" : typeId
      }, function (err2, actuatorId) {
        actuators.addActuatorProperty({
          "name" : "Allumé",
          "type" : 0,
          "index" : 0,
          "actuatorTypeId" : typeId
        }, function (err3, actuatorPropertyId) {
          actuators.addActuatorPropertyValue({
            "value" : 1,
            "actuatorId" : actuatorId,
            "actuatorPropertyId" : actuatorPropertyId        
          }, function (err4, actuatorPropertyValueId) {
            cb(typeId, actuatorId, actuatorPropertyId, actuatorPropertyValueId);
          })
        });
     });
  });
}


function createSwitch(cb) {
  // First, we create a sensorType.
  sensors.addSensorType({ "name" : "Interrupteur" }, function (err, typeId) {

    // Now, we add a sensor of this Type.
    sensors.addSensor({
      "name" : "Interrupteur 1",
      "oemId" : "0021CBE5",
      "sensorTypeId" : typeId
    }, function (err2, sensorId) {

      // We then add a property to this sensorType.
      sensors.addSensorProperty({
        "name" : "Appuyé",
        "type" : 0,
        "index" : 3,
        "sensorTypeId" : typeId,
        "unit" : "°C"
      }, function (err3, sensorPropertyId) {

        // And a propertyValue for this pair of sensor and sensorProperty.
        sensors.addSensorPropertyValue({
          "value" : 3,
          "sensorId" : sensorId,
          "sensorPropertyId" : sensorPropertyId,
          "date" : +new Date()      
        }, function (err4, sensorPropertyValueId) {
          cb(typeId, sensorId, sensorPropertyId, sensorPropertyValueId);
        });
      });
    });
  });
}


createSwitch(function(typeId, sensorId,
                      sensorPropertyId, sensorPropertyValueId) {
  createPlug(function(typePlugId, actuatorId,
                      actuatorPropertyId, actuatorPropertyValueId) {
    // Now let's see for the "rules" part.
    // We start by creating a new rule.
    rules.addRule({
      "name" : "Rule1"
    }, function (err5, ruleId) {

      // We add a root condition to this rule.
      rules.addCondition({
        "type" : 0, // This specifies it is a condition related to a sensor.
        "ruleId" : ruleId,
        "sensorId" : sensorId,
        "sensorPropertyId" : sensorPropertyId,
        "value" : 0, // The threshold limit.
        "order" : 0 // The condition is "<".
      }, function (err6, conditionId) {

        // We create an action corresponding to that rule.
        rules.addAction({
          "ruleId" : ruleId,
          "actuatorId" : actuatorId,
          "actuatorPropertyId" : actuatorPropertyId,
          "value" : 1
        }, function (err7, actionId) {

          // Finally, we try to validate the rules.
          /*ruleEngine.processEvent("0021CBE5", 0, function (err8, actions) {
            if (err8) {
              console.log('err8: ' + err8);
              return;
            }

            if (actions.length == 0) {
              console.log("Rien à voir");
            } else {
              console.dir('actions 0: ' + actions[0]);
            }

            // Deletion tests.
            // sensors.deleteSensorType(typeId);
            // sensors.deleteSensor(sensorId);
            // sensors.deleteSensorProperty(sensorPropertyId);
            // sensors.deleteSensorPropertyValue(sensorPropertyValueId);

            // rules.deleteRule(ruleId);
            // rules.deleteCondition(conditionId);
            // rules.deleteAction(actionId);
          });*/
        });
      });
    });
  });
});
