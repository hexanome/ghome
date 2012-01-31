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

function getSensorProperties(cb) {
  redisbase.getAllItems(tableSensorProperty, cb);
}

function addSensorProperty(sensorProperty, cb) {
  redisbase.addItem(tableSensorProperty, sensorProperty, cb, [{
    "table" : tableSensorType,
    "name" : "sensorType"
  }]);
}

function deleteSensorProperty(propertyId, cb) {
  redisbase.deleteItem(tableSensorProperty, propertyId, cb);
}

// Sensors

function getSensor(sensorId, cb) {
  redisbase.getSingleItem(tableSensor, sensorId, cb);
}

function getSensors(cb) {
  redisbase.getAllItems(tableSensor, cb);
}

function addSensor(sensor, cb) {
  redisbase.addItem(tableSensor, sensor, cb, [{
    "table" : tableSensorType,
    "name" : "sensorType"
  }]);
}

function deleteSensor(sensorId, cb) {
  redisbase.deleteItem(tableSensor, sensorId, cb);
}

// SensorPropertyValues

function getSensorPropertyValue(propertyValueId, cb) {
  redisbase.getSingleItem(tableSensorPropertyValue, propertyValueId, cb);
}

function getSensorPropertyValues(cb) {
  redisbase.getAllItems(tableSensorPropertyValue, cb);
}

function addSensorPropertyValue(propertyValue, cb) {
  redisbase.addItem(tableSensorPropertyValue, propertyValue, cb, [{
    "table" : tableSensor,
    "name" : "sensor"
  }, {
    "table" : tableSensorProperty,
    "name" : "sensorProperty"
  }]);
}

function deleteSensorPropertyValue(propertyValueId, cb) {
  redisbase.deleteItem(tableSensorPropertyValue, propertyValueId, cb);
}

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
exports.getSensors = getSensors;
exports.addSensor = addSensor;
exports.deleteSensor = deleteSensor;

exports.getSensorPropertyValue = getSensorPropertyValue;
exports.getSensorPropertyValues = getSensorPropertyValues;
exports.addSensorPropertyValue = addSensorPropertyValue;
exports.deletePropertyValue = deleteSensorPropertyValue;