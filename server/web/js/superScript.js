Scout('#newSensorType').on('click', function (query) {
  query.action = 'addSensorType';
  query.data = {
    name: Scout('#newSensorTypeName').value
  };
  query.resp = function (typeId) {
    console.log('New Sensor Type sais yo!',typeId);
    document.location = '/sensor-types';
  }
});