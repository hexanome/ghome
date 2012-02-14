var redisbase = require("./redisbase.js"),
    tableSensorType = "sensor_type",
    tableSensorProperty = "sensor_property",
    tableSensor = "sensor",
    tableSensorPropertyValue = "sensor_property_value";

function configure(dbClient) {
  redisbase.configure(dbClient);
}

// SensorTypes

function getSensorType(typeId, cb) {
  redisbase.getSingleItem(tableSensorType, typeId, cb);
}

function getSensorTypes(cb) {
  redisbase.getAllItems(tableSensorType, cb);
}

function addSensorType(sensorType, cb) {
  redisbase.addItem(tableSensorType, sensorType, cb);
}

function deleteSensorType(sensorTypeId, cb) {
  redisbase.deleteItem(tableSensorType, sensorTypeId, cb);
}

// SensorProperties

function getSensorProperty(propertyId, cb) {
  redisbase.getSingleItem(tableSensorProperty, propertyId, cb);
}

function getSensorPropertyFromType(sensorTypeId, cb) {
  redisbase.getSingleItemFromSec(tableSensorProperty, "sensorTypeId", sensorTypeId, cb);
}

function getSensorProperties(cb) {
  redisbase.getAllItems(tableSensorProperty, cb);
}

function addSensorProperty(sensorProperty, cb) {
  redisbase.addItem(tableSensorProperty, sensorProperty, cb, [], ["sensorTypeId"]);
}

function deleteSensorProperty(propertyId, cb) {
  redisbase.deleteItem(tableSensorProperty, propertyId, cb);
}

// Sensors

function getSensor(sensorId, cb) {
  redisbase.getSingleItem(tableSensor, sensorId, cb);
}

function getSensorFromOem(oemId, cb) {
  redisbase.getSingleItemFromSec(tableSensor, "oemId", oemId, cb);
}

function getSensors(cb) {
  redisbase.getAllItems(tableSensor, cb);
}

function addSensor(sensor, cb) {
  redisbase.addItem(tableSensor, sensor, cb, [], ["oemId", "sensorTypeId"]);
}

function deleteSensor(sensorId, cb) {
  redisbase.deleteItem(tableSensor, sensorId, cb);
}

// SensorPropertyValues

function getSensorPropertyValue(propertyValueId, cb) {
  redisbase.getSingleItem(tableSensorPropertyValue, propertyValueId, cb);
}

function getSensorPropertyValueFromSensorAndProperty(sensorId, propertyId, cb) {
  redisbase.getSingleItemFromSec(tableSensorPropertyValue, "sensorAndPropertyId","{0};{1}".format(sensorId, propertyId), cb);
}

function getSensorPropertyValues(cb) {
  redisbase.getAllItems(tableSensorPropertyValue, cb);
}

function addSensorPropertyValue(propertyValue, cb) {
  redisbase.addItem(tableSensorPropertyValue, propertyValue, cb, [], ["sensorAndPropertyId"]);
}

function deleteSensorPropertyValue(propertyValueId, cb) {
  redisbase.deleteItem(tableSensorPropertyValue, propertyValueId, cb);
}

// Export variables
exports.tableSensorType = tableSensorType;
exports.tableSensor = tableSensor;
exports.tableSensorProperty = tableSensorProperty;
exports.tableSensorPropertyValue = tableSensorPropertyValue;

// Export methods
exports.configure = configure;

exports.getSensorType = getSensorType;
exports.getSensorTypes = getSensorTypes;
exports.addSensorType = addSensorType;
exports.deleteSensorType = deleteSensorType;

exports.getSensorProperty = getSensorProperty;
exports.getSensorProperties = getSensorProperties;
exports.addSensorProperty = addSensorProperty;
exports.deleteSensorProperty = deleteSensorProperty;

exports.getSensor = getSensor;
exports.getSensorFromOem = getSensorFromOem;
exports.getSensors = getSensors;
exports.addSensor = addSensor;
exports.deleteSensor = deleteSensor;

exports.getSensorPropertyValue = getSensorPropertyValue;
exports.getSensorPropertyValues = getSensorPropertyValues;
exports.addSensorPropertyValue = addSensorPropertyValue;
exports.deletePropertyValue = deleteSensorPropertyValue;