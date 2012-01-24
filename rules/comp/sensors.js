var redisbase = require("redisbase.js"),
    tableSensorType = "sensorType",
    tableSensorProperty = "sensorProperty";

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

function getSensorProperty(propertyId, cb) {
  redisbase.getSingleItem(tableSensorType, propertyId, cb);
}

exports.configure = configure;
exports.addSensorType = addSensorType;
exports.getSensorType = getSensorType;
exports.getSensorTypes = getSensorTypes;