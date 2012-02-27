var async = require("async"),
    ruleEngine = require("./rule-engine.js"),
    com = require("./com.js"),
    sensors = require("./sensors.js"),
    actuators = require("./actuators.js"),
    actuatorCallback = null;

// Export methods.
exports.startServer = startServer;
exports.newSensorValue = newSensorValue;

function startServer(newActuatorCallback) {
  // We set the callback for actuators.
  actuatorCallback = newActuatorCallback;

  com.registerForNotifications(function (err, actions) {
    if (err) {
      return;
    }

    findActuatorsFromActions(results);
  });
}

function newSensorValue(oemId, sensorPropertyIndex, newValue) {
  // First, we find the corresponding sensor and property.
  sensors.getSensorFromOem(oemId, function (err, sensor) {
    if (err) {
      return err;
    }

    // We now retrieve the properties with the same type as this sensor.
    console.log("SensorName: " + sensor.name);

    sensors.getSensorPropertiesFromType(sensor.sensorTypeId, function (err2, properties) {
      // We find the first property with index "propertyIndex".
      var currentProperty = null;
      for (var i = 0; i < properties.length; i++) {
        var property = properties[i];
        if (property.index == sensorPropertyIndex) {
          currentProperty = property;
        }
      }

      if (currentProperty == null) {
        cb("error");
        return;
      }

      // We can at this point store the newly retrieved value in the database.
      sensors.addSensorPropertyValue({
        "value" : newValue,
        "sensorId" : sensor.id,
        "sensorPropertyId" : currentProperty.id        
      }, function (err3, sensorPropertyValueId) {
        // We can now reprocess the rules, and then notifify the actuators.
        processRules(sensor.id, currentProperty.id, function (err4, actions) {
          findActuatorsFromActions(actions);
        });
      });
    });
  });
}

function findActuatorsFromActions(actions) {
  if (!actions) {
    return;
  }

  async.map(actions, function (action, cb) {
    // We retrieve the actuator corresponding to this action.
    actuators.getActuator(action.actuatorId, function (err, actuator) {
      if (err) {
        cb(err);
        return;
      }

      // We now retrieve the corresponding actuatorProperty.
      actuators.getActuatorProperty(action.actuatorPropertyId, function (err2, actuatorProperty) {
        if (err2) {
          cb(err2);
          return;
        }

        cb(null, {
          "oemId" : actuator.oemId,
          "propertyIndex" : actuatorProperty.index,
          "newValue" : action.value
        });
      })
    })
  }, function (err, oemActuators) {
    if (err) {
      return;
    }

    for (var i = 0; i < oemActuators.length; i++) {
      actuatorCallback(oemActuators[i]);
    }
  });
}