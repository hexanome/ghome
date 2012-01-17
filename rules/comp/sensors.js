var redisClient;

var udf = undefined;

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

    cb(udf, obj);
  }
}

function getSensorTypes(cb) {
  var result = [];

  redisClient.smembers("sensor_types", function (err, replies) {
    if (err) {
      cb(err);
      return;
    }

    var processed = 0, total = replies.length;
    replies.forEach(function (reply, i) {
      getSensorType(reply, function(err, st) {
        if (err) {
          cb(err);
        }
        result.push(st);
        processed++;
        if (processed == total) {
          cb(udf, result);
        }
      });
    }
  }
}

function addSensorType(sensorType) {
  var sensorId = newGuid();
  redisClient.hmset("sensor_type:" + sensorId, sensorType);
  return sensorId;
}

function newGuid() {
  'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function(c) {
    var r = Math.random()*16|0, v = c == 'x' ? r : (r&0x3|0x8);
    return v.toString(16);
  });
}
