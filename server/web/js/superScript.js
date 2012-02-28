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

  query.action = 'addActuatorProperty';
  query.data = {
    name: Scout('#newActuatorPropertyName').value,
    index: Scout('#newActuatorPropertyIndex').value,
    actuatorTypeId: Scout('#newActuatorPropertyTypeId').value
  };
  console.log(query.data);
  query.resp = function () {
    document.location = '/actuator-types/' + Scout('#newActuatorPropertyTypeId').value;
  }
});

Scout('#newActuator').on('click', function(query) {
  var actuatorTypeId = Scout('#newActuatorType').options[Scout('#newActuatorType').selectedIndex].value;

  query.action = 'addActuator';
  query.data = {
    name: Scout('#newActuatorName').value,
    oemId: Scout('#newActuatorOemId').value,
    actuatorTypeId: actuatorTypeId
  };
  console.log(query.data);
  query.resp = function (id) {
    document.location = '/actuators';
  }
});

Scout('#newRule').on('click', function(query) {
  query.action = 'addRule';
  query.data = {
    name: Scout('#newRuleName').value,
  };
  console.log(query.data);
  query.resp = function (id) {
    document.location = '/rules';
  }
});

Scout('#newCondition').on('click', function(query) {
  var ruleId = Scout('#newConditionRuleId');
  query.action = 'addCondition';
  query.data = {
    sensorTypeId: Scout('#newConditionType').options[Scout('#newConditionType').selectedIndex].value,
    parentId: Scout('#newConditionParent').options[Scout('#newConditionParent').selectedIndex].value,
    sensorId: Scout('#newConditionSensor').options[Scout('#newConditionSensor').selectedIndex].value,
    sensorPropertyId: Scout('#newConditionSensorProperty').options[Scout('#newConditionSensorProperty').selectedIndex].value,
    value: Scout('#newConditionThreshold').value,
    order: Scout('#newConditionOperator').selectedIndex
  }
  query.resp = function (id) {
    document.location = '/rules/' + ruleId;
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
