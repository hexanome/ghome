
// Import other modules
var actuators = require('../../rules/comp/actuators.js'),
    com = require('../../rules/comp/com.js'),
    sensors = require('../../rules/comp/sensors.js');

sensors.getSensors(function(thing){
  console.error(JSON.stringify(thing));
});

exports.index = function(req, res) {
  res.render('index', {
    // mothereffing template key:values
    sensors : [{id:'s1',val:'42°C'}, {id:'s2',val:'closed'}]
  });
};

