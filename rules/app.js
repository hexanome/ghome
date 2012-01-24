// Dependencies
var redis = require("redis"),
  redisClient = redis.createClient(),
  sensors = require("./comp/sensors.js");

// Configuration des services DB sensors.
sensors.configure(redisClient);

sensors.addSensorType({ "name" : "Test type" }, function (err, typeId) {
  // Ajout d'une sensorProperty.
  sensors.addSensorProperty({
    "name" : "Test property",
    "sensorType" : {
      "id" : typeId
    }
  }, function (err, propertyId) {
    console.log("Success");

    sensors.getSensorProperty(propertyId, function (err, sensorProperty) {
      console.dir(sensorProperty);
      console.log(sensorProperty.sensorType.id);
    });
  })
});