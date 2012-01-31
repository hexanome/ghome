// Dependencies
var redis = require("redis"),
  redisClient = redis.createClient(),
  sensors = require("./comp/sensors.js");

// Configuration des services DB sensors.
sensors.configure(redisClient);

sensors.addSensorType({ "name" : "Test type" }, function (err, typeId) {

  sensors.addSensor({ 
    "name" : "Sensor1", 
    "oemId" : "1234",
    "sensorType" : {
      "id" : typeId
    } 
  }, function (err, sensorId) {
    sensors.getSensorFromOem(1234, function (err, reply) {
      console.dir(reply);
    });
  });
});
