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

Scout('#newSensor').on('click', function(query) {
  var sensorTypeId = Scout('#newSensorType').options[Scout('#newSensorType').selectedIndex].value;

  query.action = 'addSensor';
  query.data = {
    name: Scout('#newSensorName').value,
    oemId: Scout('#newSensorOemId').value,
    sensorTypeId: sensorTypeId
  };
  console.log(query.data);
  query.resp = function (id) {
    document.location = '/sensors';
  }
});

Scout('#newActuatorType').on('click', function (query) {
  query.action = 'addActuatorType';
  query.data = {
    name: Scout('#newActuatorTypeName').value
  };
  query.resp = function (typeId) {
    console.log('New Actuator Type sais yo!',typeId);
    document.location = '/actuator-types';
  }
});

Scout('#newActuatorProperty').on('click', function(query) {
  var unit = Scout('#newActuatorPropertyUnit').options[Scout('#newActuatorPropertyUnit').selectedIndex].innerHTML;

  query.action = 'addActuatorProperty';
  query.data = {
    name: Scout('#newActuatorPropertyName').value,
    index: Scout('#newActuatorPropertyIndex').value,
    unit: unit,
    sensorTypeId: Scout('#newActuatorPropertyTypeId').value
  };
  console.log(query.data);
  query.resp = function () {
    document.location = '/actuator-types/' + Scout('#newActuatorPropertyTypeId').value;
  }
});

$(document).ready(function () {
  // We customize the modal when displayed from a "New Element" link.
  $(".btNewElement").click(function () {
    var modalId = $(this).attr("href");
    var modal = $(modalId);

    modal.find(".editMode").html("New");
    modal.find(".btn-danger").hide();
  });

  // We customize the modal when displayed from an "Edit" link.
  $(".btEditElement").click(function () {
    var modalId = $(this).attr("href");
    var modal = $(modalId);

    modal.find(".editMode").html("Edit");
    modal.find(".btn-danger").show();
  });
});
