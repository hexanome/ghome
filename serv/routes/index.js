
// Import useful modules
var actuators = require ('../../rules/comp/actuators.js'),
    com = require ('../../rules/comp/com.js'),
    rules = require ('../../rules/comp/rules.js'),
    rutils = require ('../../rules/comp/condition-utils.js'),
    sensors = require ('../../rules/comp/sensors.js');


// Variables we work on
var s = [],     // s = [ { name: 'Sensor1', val: '3°C' }, ... ]
    sname = {}, // sname[sensorId] = sensorName
    pname = {}, // pname[propertyId] = propertyName
    r = [];


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
rules.getConditions (function (err, conditions) {
  if (err) throw err;
  var root = rutils.findRootCondition (conditions);
  rutils.buildConditionChilds(conditions, root);

  // FIXME debug
  root.childs.push({
  "order": "1",
  "value": "2",
  "type": "0",
  "parentId": "null",
  "ruleId": "ad8c9995-2025-4224-af67-605554a5d07d",
  "id": "ce5037f6-0ed5-4ca4-966e-2bb1a3ca5d9d",
  "sensorId": "683e9dac-4fef-4b49-a983-320dbc309dc2",
  "sensorPropertyId": "5969a49c-24e4-4e9f-8c7c-7d4871082f33",
  "childs": []});

  console.log(JSON.stringify(root, null, 2));

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
