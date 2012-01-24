// Dependencies
var redis = require("redis"),
    redisClient = redis.createClient(),
    sensors = require("./comp/sensors.js");

sensors.configure(redisClient);
sensors.addSensorType({ "test" : "MAMA" }, function (err, sensorId) {
	if (err) {
		console.log(err);
	}

	console.log(sensorId);
});