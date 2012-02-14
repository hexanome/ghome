// mum.js spawns you all.
//
// If you're worried about the name, keep in mind that it could have been a lot
// worse.  `Mother-effing-mum.js`, for instance.
//
// Dearly designed by Thaddee Tyl.


var util = require('util'),   // Might be handy (isn't right now).

    // Database elements.
    redis = require("redis"),
    redisClient = redis.createClient(),
    sensordb = require("./comp/sensors.js"),

    // Children elements.
    readSensor = require('./proto/sensor').read,
    spawn = require('child_process').spawn,
    sensorProg = '../modulesTraduction/EnOceanModuleTraduction/' +
              'EnOceanModuleTraduction/sensor',
    actuatorProg = '../modulesTraduction/EnOceanModuleTraduction/' +
              'EnOceanModuleTraduction/actuator';


// START DATA BASE.
//

sensordb.configure(redisClient);


// KIDS THESE DAYS.
//


// They are now officially born.

sensorKid = spawn(sensorProg);
actuatorKid = spawn(actuatorProg);


// The sensor kid is very loud, he has a lot to say.

sensorKid.stdout.on('data', function(data) {
  console.log('stdout from sensorKid:', readSensor(data));
});

sensorKid.on('exit', function(code) {
  if (code !== 0) {
    console.log('Kid died and said ' + code);
  }
});
