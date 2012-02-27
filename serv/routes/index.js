
// Import other modules
var actuators = require('../../rules/comp/actuators.js'),
    com = require('../../rules/comp/com.js'),
    sensors = require('../../rules/comp/sensors.js');

var sensors = {};

sensors.getSensorPropertyValues (function (err,res) {
  if(err) throw err;
  for (var i = 0 ; i < res.length ; i++) {
    sensors[res.sensorId] = sensors[res.sensorId] || {};
    sensors[res.sensorId][res.sensorPropertyId] = res.value;
  }
  console.log(sensors);
});

exports.index = function(req, res) {
  res.render('index', {
    // mothereffing template key:values
    sensors : [{id:'s1',val:'42°C'}, {id:'s2',val:'closed'}]
  });
};

