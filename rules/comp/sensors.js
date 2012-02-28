var redisbase = require("./redisbase.js"),
    tableSensorType = "sensor_type",
    tableSensorProperty = "sensor_property",
    tableSensor = "sensor",
    tableSensorPropertyValue = "sensor_property_value";

// Export variables
exports.tableSensorType = tableSensorType;
exports.tableSensor = tableSensor;
exports.tableSensorProperty = tableSensorProperty;
exports.tableSensorPropertyValue = tableSensorPropertyValue;

// Export methods
exports.getSensorType = getSensorType;
exports.getSensorTypes = getSensorTypes;
exports.addSensorType = addSensorType;
exports.updateSensorType = updateSensorType;
exports.deleteSensorType = deleteSensorType;

exports.getSensorProperty = getSensorProperty;
exports.getSensorPropertyFromType = getSensorPropertyFromType;
exports.getSensorPropertiesFromType = getSensorPropertiesFromType;
exports.getSensorProperties = getSensorProperties;
exports.addSensorProperty = addSensorProperty;
exports.updateSensorProperty = updateSensorProperty;
exports.deleteSensorProperty = deleteSensorProperty;

exports.getSensor = getSensor;
exports.getSensorFromOem = getSensorFromOem;
exports.getSensors = getSensors;
exports.addSensor = addSensor;
exports.updateSensor = updateSensor;
exports.deleteSensor = deleteSensor;

exports.getSensorPropertyValue = getSensorPropertyValue;
exports.getSensorPropertyValueFromSensorAndProperty = getSensorPropertyValueFromSensorAndProperty;
exports.getSensorPropertyValues = getSensorPropertyValues;
exports.addSensorPropertyValue = addSensorPropertyValue;
exports.updateSensorPropertyValue = updateSensorPropertyValue;
exports.deleteSensorPropertyValue = deleteSensorPropertyValue;

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

function updateSensorType(sensorType, cb) {
  redisbase.updateItem(tableSensorType, sensorType, cb);
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

function getSensorPropertiesFromType(sensorTypeId, cb) {
  redisbase.getItemsFromSec(tableSensorProperty, "sensorTypeId", sensorTypeId, cb);
}

function addSensorProperty(sensorProperty, cb) {
  redisbase.addItem(tableSensorProperty, sensorProperty, cb, [], ["sensorTypeId"]);
}

function updateSensorProperty(sensorProperty, cb) {
  redisbase.updateItem(tableSensorProperty, sensorProperty, cb, ["sensorTypeId"]);
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

function updateSensor(sensor, cb) {
  redisbase.updateItem(tableSensor, sensor, cb, ["oemId", "sensorTypeId"]);
}

function deleteSensor(sensorId, cb) {
  redisbase.deleteItem(tableSensor, sensorId, cb);
}

// SensorPropertyValues

function getSensorPropertyValue(propertyValueId, cb) {
  redisbase.getSingleItem(tableSensorPropertyValue, propertyValueId, function (err, result) {
    cb(err, processSensorPropertyValue(result));
  });
}

function getSensorPropertyValueFromSensorAndProperty(sensorId, propertyId, cb) {
  redisbase.getSingleItemFromSec(tableSensorPropertyValue, "sensorAndPropertyId","{0};{1}".format(sensorId, propertyId), function (err, result) {
    cb(err, processSensorPropertyValue(result));
  });
}

function getSensorPropertyValues(cb) {
  redisbase.getAllItems(tableSensorPropertyValue, function (err, results) {
    cb(err, processSensorPropertyValues(results));
  });
}

function processSensorPropertyValues(sensorPropertyValues) {
    for (var i = 0; i < sensorPropertyValues.length; i++) {
      sensorPropertyValues[i] = processSensorPropertyValue(sensorPropertyValues[i]);
    }

    return sensorPropertyValues;
}

function processSensorPropertyValue(sensorPropertyValue) {
    if (!sensorPropertyValue) {
      return null;
    }

    var sensorAndPropertyId = sensorPropertyValue.sensorAndPropertyId.split(";");
    sensorPropertyValue.sensorId = sensorAndPropertyId[0];
    sensorPropertyValue.sensorPropertyId = sensorAndPropertyId[1];

    delete sensorPropertyValue["sensorAndPropertyId"];  

    return sensorPropertyValue;
}

function addSensorPropertyValue(propertyValue, cb) {
  propertyValue["sensorAndPropertyId"] = "{0};{1}".format(propertyValue.sensorId, propertyValue.sensorPropertyId);
  delete propertyValue["sensorId"];
  delete propertyValue["sensorPropertyId"];

  redisbase.addItem(tableSensorPropertyValue, propertyValue, cb, [], ["sensorAndPropertyId"]);
}

function updateSensorPropertyValue(propertyValue, cb) {
  propertyValue["sensorAndPropertyId"] = "{0};{1}".format(propertyValue.sensorId, propertyValue.sensorPropertyId);
  delete propertyValue["sensorId"];
  delete propertyValue["sensorPropertyId"];

  redisbase.updateItem(tableSensorPropertyValue, propertyValue, cb, ["sensorAndPropertyId"]);
}

function deleteSensorPropertyValue(propertyValueId, cb) {
  redisbase.deleteItem(tableSensorPropertyValue, propertyValueId, cb);
}