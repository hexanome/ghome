// Dependencies
var redis = require("redis"),
    redisClient = redis.createClient(),
    sensors = require("./comp/sensors.js");

// Configuration des services DB sensors.
sensors.configure(redisClient);

// Ajout d'un sensor en base.
sensors.addSensorType({ "test" : "MAMA" }, function (err, sensorId) {
  if (err) {
    console.log(err);
  }

  console.log(sensorId);

  // Récupération de la liste des sensors.
  sensors.getSensorTypes(function (err, sts) {
    sts.forEach(function (st) {
      console.dir(st);
    });
  });
}); 

pipe.listen(function () {
  // Reception pipe

  // Decodage

  // MAJ DB

  // Lecture Regles

  // Ecriture sur pipe pour actuators.
})

base.listen(function () {
  // Decodage

  // Ecriture sur pipe pour actuators.
})
