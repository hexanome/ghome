var redis = require("redis"),
    redisClient1 = redis.createClient(),
    redisClient2 = redis.createClient(),
    ruleEngine = require("./rule-engine.js"),
    channelName = "instant_com";

function registerForNotifications(cb) {
  // We setup the callback for new messages.
  redisClient1.on("message", function (channel, message) {
    if (channel == channelName) {
      // First, we reprocess the rules corresponding to this message.
      ruleEngine.processRules(message.sensorId, message.sensorPropertyId, function (err, results) {
        if (err) {
          cb(err);
          return;
        }

        // Then, we call the callback with the list of actions to trigger.
        cb(err, results);
      });
    }
  });

  // We subscribe to the channel.
  redisClient1.subscribe(channelName);
}

function sendUpdateMessage(sensorId, sensorPropertyId) {
  // We send a message to the channel containing the sensorId and sensorPropertyId.
  redisClient2.publish(channelName, {
    "sensorId" : sensorId,
    "sensorPropertyId" : sensorPropertyId
  });
}

// Export methods.
exports.registerForNotifications = registerForNotifications;
exports.sendUpdateMessage = sendUpdateMessage;