// Dependencies
var redis = require("redis"),
    redisClient = redis.createClient(),
    sensors = require("comp/sensors.js");

sensors.configure(redisClient);

