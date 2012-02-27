var redisbase = require("./redisbase.js"),
    tableActuatorType = "actuator_type",
    tableActuatorProperty = "actuator_property",
    tableActuator = "actuator",
    tableActuatorPropertyValue = "actuator_property_value";

// Export variables.
exports.tableActuatorType = tableActuatorType;
exports.tableActuator = tableActuator;
exports.tableActuatorProperty = tableActuatorProperty;
exports.tableActuatorPropertyValue = tableActuatorPropertyValue;

// Export methods.
exports.getActuatorType = getActuatorType;
exports.getActuatorTypes = getActuatorTypes;
exports.addActuatorType = addActuatorType;
exports.deleteActuatorType = deleteActuatorType;

exports.getActuatorProperty = getActuatorProperty;
exports.getActuatorProperties = getActuatorProperties;
exports.addActuatorProperty = addActuatorProperty;
exports.deleteActuatorProperty = deleteActuatorProperty;

exports.getActuator = getActuator;
exports.getActuatorFromOem = getActuatorFromOem;
exports.getActuators = getActuators;
exports.addActuator = addActuator;
exports.deleteActuator = deleteActuator;

exports.getActuatorPropertyValue = getActuatorPropertyValue;
exports.getActuatorPropertyValues = getActuatorPropertyValues;
exports.addActuatorPropertyValue = addActuatorPropertyValue;
exports.deleteActuatorPropertyValue = deleteActuatorPropertyValue;

// ActuatorTypes

function getActuatorType(typeId, cb) {
  redisbase.getSingleItem(tableActuatorType, typeId, cb);
}

function getActuatorTypes(cb) {
  redisbase.getAllItems(tableActuatorType, cb);
}

function addActuatorType(actuatorType, cb) {
  redisbase.addItem(tableActuatorType, actuatorType, cb);
}

function deleteActuatorType(actuatorTypeId, cb) {
  redisbase.deleteItem(tableActuatorType, actuatorTypeId, cb);
}

// ActuatorProperties

function getActuatorProperty(propertyId, cb) {
  redisbase.getSingleItem(tableActuatorProperty, propertyId, cb);
}

function getActuatorProperties(cb) {
  redisbase.getAllItems(tableActuatorProperty, cb);
}

function addActuatorProperty(actuatorProperty, cb) {
  redisbase.addItem(tableActuatorProperty, actuatorProperty, cb, [], ["actuatorTypeId"]);
}

function deleteActuatorProperty(propertyId, cb) {
  redisbase.deleteItem(tableActuatorProperty, propertyId, cb);
}

// Actuators

function getActuator(actuatorId, cb) {
  redisbase.getSingleItem(tableActuator, actuatorId, cb);
}

function getActuatorFromOem(oemId, cb) {
  redisbase.getSingleItemFromSec(tableActuator, "oemId", oemId, cb);
}

function getActuators(cb) {
  redisbase.getAllItems(tableActuator, cb);
}

function addActuator(actuator, cb) {
  redisbase.addItem(tableActuator, actuator, cb, [], ["oemId", "actuatorTypeId"]);
}

function deleteActuator(actuatorId, cb) {
  redisbase.deleteItem(tableActuator, actuatorId, cb);
}

// ActuatorPropertyValues
function getActuatorPropertyValue(propertyValueId, cb) {
  redisbase.getSingleItem(tableActuatorPropertyValue, propertyValueId, function (err, result) {
    cb(err, processActuatorPropertyValue(result));
  });
}

function getActuatorPropertyValueFromActuatorAndProperty(actuatorId, propertyId, cb) {
  redisbase.getSingleItemFromSec(tableActuatorPropertyValue, "actuatorAndPropertyId","{0};{1}".format(actuatorId, propertyId), function (err, result) {
    cb(err, processActuatorPropertyValue(result));
  });
}

function getActuatorPropertyValues(cb) {
  redisbase.getAllItems(tableActuatorPropertyValue, function (err, results) {
    cb(err, processActuatorPropertyValues(results));
  });
}

function processActuatorPropertyValues(actuatorPropertyValues) {
    for (var i = 0; i < actuatorPropertyValues.length; i++) {
      actuatorPropertyValues[i] = processActuatorPropertyValue(actuatorPropertyValues[i]);
    }

    return actuatorPropertyValues;
}

function processActuatorPropertyValue(actuatorPropertyValue) {
    if (!actuatorPropertyValue) {
      return null;
    }

    var actuatorAndPropertyId = actuatorPropertyValue.actuatorAndPropertyId.split(";");
    actuatorPropertyValue.actuatorId = actuatorAndPropertyId[0];
    actuatorPropertyValue.actuatorPropertyId = actuatorAndPropertyId[1];

    delete actuatorPropertyValue["actuatorAndPropertyId"];  

    return actuatorPropertyValue;
}

function addActuatorPropertyValue(actuatorPropertyValue, cb) {
  // We concatenate the actuatorId and actuatorPropertyId in one property.
  actuatorPropertyValue["actuatorAndPropertyId"] = "{0};{1}".format(actuatorPropertyValue.actuatorId, actuatorPropertyValue.actuatorPropertyId);
  delete actuatorPropertyValue["actuatorId"];
  delete actuatorPropertyValue["actuatorPropertyId"];

  redisbase.addItem(tableActuatorPropertyValue, actuatorPropertyValue, cb, [], ["actuatorAndPropertyId"]);
}

function deleteActuatorPropertyValue(propertyValueId, cb) {
  redisbase.deleteItem(tableActuatorPropertyValue, propertyValueId, cb);
}