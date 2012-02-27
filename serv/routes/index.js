
// Import useful modules
var actuators = require('../../rules/comp/actuators.js'),
    com = require('../../rules/comp/com.js'),
    sensors = require('../../rules/comp/sensors.js');


// Variables we work on
var s = [],     // s = [ { name: 'Sensor1', val: '3°C' }, ... ]
    sname = {}, // sname[sensorId] = sensorName
    pname = {}; // pname[propertyId] = propertyName


// Get sensor IDs and names
sensors.getSensors (function (err, res) {
  if (err) throw err;

  for (var i = 0; i < res.length; i++) {
    sname[res[i].id] = res[i].name;
  }

  // Get property IDs and names
  sensors.getSensorProperties(function (err, res) {
    if (err) throw err;

    for (var i = 0; i < res.length; i++) {
      pname[res[i].id] = ''; // TODO '°C'? 'km'? 'L'?
    }

    // Get values measured by the sensors
    sensors.getSensorPropertyValues (function (err,res) {
      if (err) throw err;

      for (var i = 0 ; i < res.length ; i++) {
        var sensor = {
          name: sname[res[i].sensorId],
          val: res[i].value + pname[res[i].sensorPropertyId]
        };
        if (sensor.name) s.push(sensor);
      }

    });
  });
});


// Render Express™ pages
exports.index = function(req, res) {
  res.render('index', {
    // mothereffing template key:values
    sensors : s
  });
};
