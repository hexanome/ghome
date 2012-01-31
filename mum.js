// mum.js spawns you all.
//
// If you're worried about the name, keep in mind that it could have been a lot
// worse.  `Mother-effing-mum.js`, for instance.
//
// Dearly designed by Thaddee Tyl.


var util = require('util'),
    readSensor = require('./proto/sensor').read,
    spawn = require('child_process').spawn,
    sensorProg = './modulesTraduction/EnOceanModuleTraduction/' +
              'EnOceanModuleTraduction/sensor',
    actuatorProg = './modulesTraduction/EnOceanModuleTraduction/' +
              'EnOceanModuleTraduction/actuator';


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

