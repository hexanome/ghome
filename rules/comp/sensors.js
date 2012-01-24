var redisbase = require("./redisbase.js"),
    tableSensorType = "sensor_type",
    tableSensorProperty = "sensor_property";

function configure(dbClient) {
  redisbase.configure(dbClient);
}

function getSensorType(typeId, cb) {
  redisbase.getSingleItem(tableSensorType, typeId, cb);
}

function getSensorTypes(cb) {
  redisbase.getAllItems(tableSensorType, cb);
}

function addSensorType(sensorType, cb) {
  redisbase.addItem(tableSensorType, sensorType, cb);
}

function addSensorProperty(sensorProperty, cb) {
  redisbase.addItem(tableSensorProperty, sensorProperty, cb, {
    "table" : tableSensorType,
    "name" : "sensorType"
  });
}

function getSensorProperty(propertyId, cb) {
  redisbase.getSingleItem(tableSensorProperty, propertyId, cb);
}

exports.configure = configure;
exports.addSensorType = addSensorType;
exports.getSensorType = getSensorType;
exports.getSensorTypes = getSensorTypes;
exports.addSensorProperty = addSensorProperty;
exports.getSensorProperty = getSensorProperty;