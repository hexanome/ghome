// Dependencies
var sensors = require("./comp/sensors.js"),
    rules = require("./comp/rules.js"),
    ruleEngine = require("./comp/rule-engine.js");

// Test set for the ruleEngine.

// First, we create a sensorType.
sensors.addSensorType({ "name" : "Type1" }, function (err, typeId) {

  // Now, we add a sensor of this Type.
  sensors.addSensor({
    "name" : "Sensor1",
    "oemId" : "1234",
    "sensorTypeId" : typeId
  }, function (err2, sensorId) {

    // We then add a property to this sensorType.
    sensors.addSensorProperty({
      "name" : "Property1",
      "type" : 0,
      "index" : 0,
      "sensorTypeId" : typeId
    }, function (err3, sensorPropertyId) {

      // And a propertyValue for this pair of sensor and sensorProperty.
      sensors.addSensorPropertyValue({
        "value" : 3,
        "sensorId" : sensorId,
        "sensorPropertyId" : sensorPropertyId        
      }, function (err4, sensorPropertyValueId) {

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
            "value" : 2, // The threshold limit.
            "order" : 0 // The condition is "<".
          }, function (err6, conditionId) {

            // We create an action corresponding to that rule.
            rules.addAction({
              "ruleId" : ruleId,
              "actuatorId" : "1",
              "actuatorPropertyId" : "2"
            }, function (err7, actionId) {

              // Finally, we try to validate the rules.
              ruleEngine.processEvent("1234", 0, function (err8, actions) {
                if (err8) {
                  console.log(err8);
                  return;
                }

                if (actions.length == 0) {
                  console.log("Rien à voir");
                } else {
                  console.dir(actions[0]);
                }
              });
            });
          });
        });
      });
    });
  });
});
