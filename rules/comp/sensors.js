var redisClient;

function configure(dbClient) {
  redisClient = dbClient;
}

function getSensorType(typeId) {
  var result = null;

  redisClient.hgetall("sensor_type:" + typeId, function (err, obj) {
    if (err) {
      return;
    }

    result = obj;
  }

  return result;
}

function getSensorTypes() {
  var result = [];

  redisClient.smembers("sensor_types", function (err, replies) {
    if (err) {
      return;
    }

    replies.forEach(function (reply, i) {
      var sensorType = getSensorType(reply);
      if (sensorType) {
        result.push(sensorType);
      }
    }
  }

  return result;
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
