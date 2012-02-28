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

Scout('#newSensorProperty').on('click', function(query) {
  var unit = Scout('#newSensorPropertyUnit').options[Scout('#newSensorPropertyUnit').selectedIndex].innerHTML;

  query.action = 'addSensorProperty';
  query.data = {
    name: Scout('#newSensorPropertyName').value,
    index: Scout('#newSensorPropertyIndex').value,
    unit: unit,
    sensorTypeId: Scout('#newSensorPropertyTypeId').value
  };
  console.log(query.data);
  query.resp = function () {
    document.location = '/sensor-types/' + Scout('#newSensorPropertyTypeId').value;
  }
});

$(document).ready(function () {
  $(".btNewElement").click(function () {
    var modalId = $(this).attr("href");
    var modal = $(modalId);

    modal.find(".editMode").html("New");
    modal.find(".btn-danger").hide();
  }); 
});