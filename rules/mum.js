// mum.js tells everyone the rules.
//
// She also tells you the temperature.
//
// If you're worried about the name, keep in mind that it could have been a lot
// worse.  `Mother-effing-mum.js`, for instance.
//
// Dearly designed by Thaddee Tyl.


var util = require('util'),   // Might be handy (isn't right now).
    fs = require('fs'),

    protocol = require('./proto/col'),

    // Database elements.
    startServer = require("./comp/rules-server").startServer,
    newSensorValue = require("./comp/rules-server").newSensorValue;




// FIFOs
//
// (and sensor readings on the fly)
//

fs.createReadStream('./from-sensor').on('data', function(data) {
  var d = protocol.read(data);
  console.log(util.inspect(d));
  for (var i = 0; i < d.values.length; i++) {
    newSensorValue(d.sensor, i, d.values[i]);
  }
});

var toActuator = fs.createWriteStream('./to-actuator');


// ACTUATOR MECHANICS!
//

startServer(function (oemActuators) {
  // oemActuators is a list of actuators.
  // Each oemActuator looks like this:
  //
  //      "oemId" : actuator.oemId,
  //      "propertyIndex" : actuatorProperty.index,
  //      "newValue" : action.value

  oemActuators.forEach(function(oem) {
    toActuator.write(protocol.write(oem.oemID, 0, oem.newValue));
  })
});

