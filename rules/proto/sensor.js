// Read sensor data.

var proto = require('./proto');

// `data` is a buffer.
function read(data) {
  var d = proto(data);
  d.sensor = d.args[0];     // Sensor id.
  d.values = d.args.slice(1);
  return d;
}

exports.read = read;
