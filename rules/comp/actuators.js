var redisbase = require("./redisbase.js"),
    tableActuatorType = "actuator_type",
    tableActuatorProperty = "actuator_property",
    tableActuator = "actuator",
    tableActuatorPropertyValue = "actuator_property_value";

function configure(dbClient) {
  redisbase.configure(dbClient);
}

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
  redisbase.addItem(tableActuatorProperty, actuatorProperty, cb, [{
    "table" : tableActuatorType,
    "name" : "actuatorType"
  }]);
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
  redisbase.addItem(tableActuator, actuator, cb, [{
    "table" : tableActuatorType,
    "name" : "actuatorType"
  }], ["oemId"]);
}

function deleteActuator(actuatorId, cb) {
  redisbase.deleteItem(tableActuator, actuatorId, cb);
}

// ActuatorPropertyValues

function getActuatorPropertyValue(propertyValueId, cb) {
  redisbase.getSingleItem(tableActuatorPropertyValue, propertyValueId, cb);
}

function getActuatorPropertyValues(cb) {
  redisbase.getAllItems(tableActuatorPropertyValue, cb);
}

function addActuatorPropertyValue(propertyValue, cb) {
  redisbase.addItem(tableActuatorPropertyValue, propertyValue, cb, [{
    "table" : tableActuator,
    "name" : "actuator"
  }, {
    "table" : tableActuatorProperty,
    "name" : "actuatorProperty"
  }]);
}

function deleteActuatorPropertyValue(propertyValueId, cb) {
  redisbase.deleteItem(tableActuatorPropertyValue, propertyValueId, cb);
}

// Export variables.
exports.tableActuatorType = tableActuatorType;
exports.tableActuator = tableActuator;
exports.tableActuatorProperty = tableActuatorProperty;
exports.tableActuatorPropertyValue = tableActuatorPropertyValue;

// Export methods.
exports.configure = configure;

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