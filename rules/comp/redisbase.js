var async = require("async"),
  utils = require("utils.js"), 
  redisClient;

function configure(dbClient) {
  redisClient = dbClient;
}

function addItem(table, item, cb, foreignKeys) {
  var sensorId = newGuid();

  async.forEach(foreignKeys, function (cb2, foreignKey) {
    item["foreign:{0}:{1}".format(foreignKey.table, foreignKey.name)] = item[foreignKey.name].id;
    delete item[foreignKey.name];
  })

  redisClient.hmset("sensor_type:" + sensorId, sensorType, function (err) {
    cb(err, sensorId);
	});	
}

function getSingleItem(table, id, cb) {
  var result = null;

  redisClient.hgetall(table + ":" + typeId, function (err, obj) {
    if (err) {
      cb(err);
      return;
    }

    // We check for foreign keys.
    var foreignKeys = [];

    for (var key in obj) {
      if (key.startsWith("foreign")) {
        foreignKeys.push(key);                
      }
    }

    async.forEach(foreignKeys, function (cb2, foreignKey) {
      var foreignKeyParts = foreignKey.split(":");
      getSingleItem(foreignKeyParts[1], obj[foreignKey], function (err, fkObj) {
        if (err) {
          cb2(err);
          return;
        }

        obj[foreignKeyParts[2]] = fkObj;
        delete obj[foreignKey];
        cb2(null);
      });
    }, function (err) {
      if (err) {
        cb(err);
        return;
      }

      // We set the id for this object.
      obj.id = typeId;

      cb(null, obj);
    });
  });
}

function getAllItems(table, cb) {
	redisClient.keys(table + ":*", function (err, keys) {
    if (err) {
      cb(err);
      return;
    }

    async.map(keys, function (key, cb2) {
      // To replace with "getSingleItem"
      redisClient.hgetall(key, function (err2, st) {
        if (err) {
          cb2(err);
          return;
        }

        cb2(null, st);
      });
    }, function(err, sts) {
      cb(null, sts);
    });
  });
}

exports.configure = configure;
exports.addItem = addItem;
exports.getSingleItem = getSingleItem;
exports.getAllItems = getAllItems;