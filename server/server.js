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
camp.handle('/sensor-types', function (query, path) {
  var data = {page: 'sensor-types'};
  path[0] = '/layout.html';
  sensordb.getSensorTypes (function (err, types) {
    if (err) throw err;
    console.error(JSON.stringify(types));
    data.types = types;
    camp.server.emit('gotsensortypes', data);
  });
}, function gotsensortypes (data) {
  return data;
});


// Sensors
camp.handle('/sensors', function (query, path) {
  var data = {page: 'sensors'};
  path[0] = '/layout.html';
  sensordb.getSensors (function (err, sensors) {
    if (err) throw err;
    console.error(JSON.stringify(sensors));
    data.sensors = sensors;
    camp.server.emit('gotsensors', data);
  });
}, function gotsensors (data) {
  return data;
});


// Actuator types
camp.handle('/actuator-types', function (query, path) {
  var data = {page: 'actuator-types'};
  path[0] = '/layout.html';
  actuatordb.getActuatorTypes (function (err, types) {
    if (err) throw err;
    console.error(JSON.stringify(types));
    data.types = types;
    camp.server.emit('gotactuatortypes', data);
  });
}, function gotactuatortypes (data) {
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
