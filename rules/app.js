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

    sensors.deleteSensor(sensorId, function (err) {
      console.log(err ? "Error" : "Success");
    });

  })

  // Ajout d'une sensorProperty.
  // sensors.addSensorProperty({
  //   "name" : "Test property",
  //   "sensorType" : {
  //     "id" : typeId
  //   }
  // }, function (err, propertyId) {
  //   console.log("We now try to delete the sensorType.");

  //   sensors.deleteSensorProperty(propertyId, function (err) {
  //     console.log(err ? "Error" : "Success");
  //   });
  // })
});