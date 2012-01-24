var async = require('async');

var redisClient;

function configure(dbClient) {
  redisClient = dbClient;
}

function getSensorType(typeId, cb) {
  var result = null;

  redisClient.hgetall("sensor_type:" + typeId, function (err, obj) {
    if (err) {
      cb(err);
      return;
    }

    cb(null, obj);
  }
}

function getSensorTypes(cb) {
  var result = [];

  redisClient.smembers("sensor_types", function (err, replies) {
    if (err) {
      cb(err);
      return;
    }

    async.forEach(replies, function (reply, cb2) {
      getSensorType(reply, function(err, st) {
        if (err) {
          cb2(err);
          return;
        }
        cb2(null, st);
      });
    }, function(err) {});
  }
}

function addSensorType(sensorType, cb) {
  var sensorId = newGuid();

  redisClient.multi()
    .hmset("sensor_type:" + sensorId, sensorType)
    .sadd("sensor_types", sensorId)
    .exec(function (err) {
      cb(err, sensorId);
    });
}

function newGuid() {
  "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx".replace(/[xy]/g, function(c) {
    var r = Math.random()*16|0, v = c == 'x' ? r : (r&0x3|0x8);
    return v.toString(16);
  });
}
