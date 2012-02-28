/* server.js: run this with Node.js in the publish/ folder to start your server.
 * Copyright © 2011 Jan Keromnes, Thaddee Tyl. All rights reserved.
 * Code covered by the LGPL license. */


// Options
var options = {
  port: +process.argv[2],
  secure: process.argv[3] === 'yes',
  debug: +process.argv[4]
};

// Let's rock'n'roll!
var camp = require('./camp/camp').start(options);


// Import useful modules
var actuatordb = require ('../rules/comp/actuators.js'),
    comdb = require ('../rules/comp/com.js'),
    ruledb = require ('../rules/comp/rules.js'),
    ruleutils = require ('../rules/comp/condition-utils.js'),
    sensordb = require ('../rules/comp/sensors.js');


// Sensor types
camp.handle('/sensor-types/?(.*)', function (query, path) {
  path[0] = '/layout.html';
  if ( path[1].length > 0 ) {
    var data = {page: 'sensor-properties'};
    sensordb.getSensorPropertiesFromType(path[1], function (err, properties) {
      if (err) throw err;
      //console.error(JSON.stringify(types));
      data.properties = properties;
      data.sensorTypeId = path[1];
      camp.server.emit('gotsensortypes', data);
    });
  } else {
    var data = {page: 'sensor-types'};
    sensordb.getSensorTypes (function (err, types) {
      if (err) throw err;
      //console.error(JSON.stringify(types));
      data.types = types;
      camp.server.emit('gotsensortypes', data);
    });
  }
}, function gotsensortypes (data) {
  return data;
});


// Add Sensor
camp.addDiffer('addSensor', function(data) {
  sensordb.addSensor(data, function(err,id) {
    camp.server.emit('addSensorProperty', id);
  });
}, function(data) {return data;});


// Add Sensor Type
camp.addDiffer('addSensorType', function(data) {
  sensordb.addSensorType(data, function(err,typeId) {
    camp.server.emit('addSensorType', typeId);
  });
}, function(data) {return data;});


// Add Sensor Property
camp.addDiffer('addSensorProperty', function(data) {
  sensordb.addSensorProperty(data, function(err,id) {
    camp.server.emit('addSensorProperty', id);
  });
}, function(data) {return data;});


// Add Actuator
camp.addDiffer('addActuator', function(data) {
  actuatordb.addActuator(data, function(err,id) {
    camp.server.emit('addActuator', id);
  });
}, function(data) {return data;});


// Add Actuator Type
camp.addDiffer('addActuatorType', function(data) {
  actuatordb.addActuatorType(data, function(err,id) {
    camp.server.emit('addActuatorType', id);
  });
}, function(data) {return data;});


// Add Actuator Property
camp.addDiffer('addActuatorProperty', function(data) {
  actuatordb.addActuatorProperty(data, function(err,id) {
    camp.server.emit('addActuatorProperty', id);
  });
}, function(data) {return data;});


// Add Rule
camp.addDiffer('addRule', function(data) {
  ruledb.addRule(data, function(err,id) {
    camp.server.emit('addRule', id);
  });
}, function(data) {return data;});


// Sensors
camp.handle('/sensors/?(.*)', function (query, path) {
  path[0] = '/layout.html';
  if ( path[1].length > 0 ) {
    var data = {page: 'sensor-values'};
    sensordb.getSensorValuesFromSensor(path[1], function (err, values) {
      if (err) throw err;
      //console.error(JSON.stringify(types));
      data.values = values;
      camp.server.emit('gotsensortypes', data);
    });
  } else {
    var data = {page: 'sensors'};
    sensordb.getSensors (function (err, sensors) {
      if (err) throw err;
      //console.error(JSON.stringify(sensors));
      data.sensors = sensors;
      camp.server.emit('gotsensors', data);
    });
  }
}, function gotsensors (data) {
  return data;
});


// Actuator types
camp.handle('/actuator-types/?(.*)', function (query, path) {
  path[0] = '/layout.html';
  var data = {page: 'actuator-types'};
  actuatordb.getActuatorTypes (function (err, types) {
    if (err) throw err;
    //console.error(JSON.stringify(types));
    data.types = types;
    camp.server.emit('gotactuatortypes', data);
  });
}, function gotactuatortypes (data) {
  return data;
});


// Actuators
camp.handle('/actuators/?(.*)', function (query, path) {
  path[0] = '/layout.html';
  var data = {page: 'actuators'};
  actuatordb.getActuators (function (err, actuators) {
    if (err) throw err;
    //console.error(JSON.stringify(actuators));
    data.actuators = actuators;
    camp.server.emit('gotactuators', data);
  });
}, function gotactuators (data) {
  return data;
});


// Rules
camp.handle('/rules/?(.*)', function (query, path) {
  path[0] = '/layout.html';
  var data = {page: 'rules'};
  ruledb.getRules (function (err, rules) {
    if (err) throw err;
    //console.error(JSON.stringify(rules));
    data.rules = rules;
    camp.server.emit('gotrules', data);
  });
}, function gotrules (data) {
  return data;
});

/*
// Get sensor IDs and names
sensors.getSensors (function (err, res) {
  if (err) throw err;

  for (var i = 0; i < res.length; i++) {
    sname[res[i].id] = res[i].name;input
  }

  // Get property IDs and names
  sensors.getSensorProperties(function (err, res) {
    if (err) throw err;

    for (var i = 0; i < res.length; i++) {
      pname[res[i].id] = ''; // TODO '°C'? 'km'? 'L'?
    }

    // Get values measured by the sensors
    sensors.getSensorPropertyValues (function (err, res) {
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


// Get rules

rules.getRules (function (err, rools) {
  if (err) throw err;

  for (var i in rools) {
    rules.getConditionsFromRule (rools[i].id, function (err, res) {
      if (err) throw err;

      var root = rutils.findRootCondition (res);
      rutils.buildConditionChilds(res, root);

      // FIXME debug
      root.childs.push({
        "order": "1",
        "value": "2",
        "type": "0",
        "name": "Prout",
        "parentId": "null",
        "ruleId": "ad8c9995-2025-4224-af67-605554a5d07d",
        "id": "ce5037f6-0ed5-4ca4-966e-2bb1a3ca5d9d",
        "sensorId": "683e9dac-4fef-4b49-a983-320dbc309dc2",
        "sensorPropertyId": "5969a49c-24e4-4e9f-8c7c-7d4871082f33",
        "children": []
      });

      root.name = rools[i].name;

      // BLAME quentez for this
      (function correct(arr) {
        if (arr.childs) arr.children = arr.childs;
        if (arr.children) for (var i in arr.children) correct(arr.children[i]);
      })(root)

      r.push(root);

    });
  }


  r = [{name: "Troll", type: 0, value: 2, children: [
    {name: "Trall", type: 1, value: 2, children: []},
    {name: "Prout", type: 2, value: 2, children: []}]}
  ]

});


// Render Express™ pages
exports.index = function(req, res) {
  res.render('index', {
    // mothereffing template key:values
    sensors: s,
    rules: r
  });
};

*/
