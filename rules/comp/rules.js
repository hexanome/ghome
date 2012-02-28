var redisbase = require("./redisbase.js"),
    sensors = require("./sensors.js"),
    actuators = require("./actuators.js"),
    tableRule = "rule",
    tableCondition = "condition",
    tableAction = "action";

// Export methods.
exports.getCondition = getCondition;
exports.getConditions = getConditions;
exports.getConditionsFromRule = getConditionsFromRule;
exports.getConditionsFromSensorAndProperty = getConditionsFromSensorAndProperty;
exports.addCondition = addCondition;
exports.updateCondition = updateCondition;
exports.deleteCondition = deleteCondition;

exports.getAction = getAction;
exports.getActions = getActions;
exports.getActionsFromRule = getActionsFromRule;
exports.addAction = addAction;
exports.updateAction = updateAction;
exports.deleteAction = deleteAction;

exports.getRule = getRule;
exports.getRules = getRules;
exports.addRule = addRule;
exports.updateRule = updateRule;
exports.deleteRule = deleteRule;

// Conditions
function getCondition(conditionId, cb) {
  redisbase.getSingleItem(tableCondition, conditionId, function (err, result) {
    cb(err, processCondition(result));
  });
}

function getConditions(cb) {
  redisbase.getAllItems(tableCondition, function (err, results) {
    cb(err, processConditions(results));
  });
}

function getConditionsFromRule(ruleId, cb) {
  redisbase.getItemsFromSec(tableCondition, "ruleId", ruleId, function (err, results) {
    cb(err, processConditions(results));
  });
}

function getConditionsFromSensorAndProperty(sensorId, propertyId, cb) {
  redisbase.getItemsFromSec(tableCondition, "sensorAndPropertyId", "{0};{1}".format(sensorId, propertyId), function (err, results) {
    cb(err, processConditions(results));
  });
}

function processConditions(conditions) {
    for (var i = 0; i < conditions.length; i++) {
      conditions[i] = processCondition(conditions[i]);
    }

    return conditions;
}

function processCondition(condition) {
    if (!condition) {
      return null;
    }

    var sensorAndPropertyId = condition.sensorAndPropertyId.split(";");
    condition.sensorId = sensorAndPropertyId[0];
    condition.sensorPropertyId = sensorAndPropertyId[1];

    delete condition["sensorAndPropertyId"];  

    return condition;
}

function addCondition(condition, cb) {
  if (!condition.parentId) {
    condition.parentId = null;
  }

  // We concatenate the sensorId and sensorPropertyId in one property.
  condition["sensorAndPropertyId"] = "{0};{1}".format(condition.sensorId, condition.sensorPropertyId);
  delete condition["sensorId"];
  delete condition["sensorPropertyId"];

  redisbase.addItem(tableCondition, condition, cb,
    [], // Join.
    ["sensorAndPropertyId", "ruleId"]); // Secondary Index.
}

function updateCondition(condition, cb) {if (!condition.parentId) {
    condition.parentId = null;
  }

  // We concatenate the sensorId and sensorPropertyId in one property.
  condition["sensorAndPropertyId"] = "{0};{1}".format(condition.sensorId, condition.sensorPropertyId);
  delete condition["sensorId"];
  delete condition["sensorPropertyId"];

  redisbase.updateItem(tableCondition, condition, cb,
    ["sensorAndPropertyId", "ruleId"]); // Secondary Index.  
}

function deleteCondition(conditionId, cb) {
  redisbase.deleteItem(tableCondition, conditionId, cb);
}

// Actions

function getAction(actionId, cb) {
  redisbase.getSingleItem(tableAction, actionId, cb);
}

function getActionsFromRule(ruleId, cb) {
  redisbase.getItemsFromSec(tableAction, "ruleId", ruleId, cb);  
}

function getActions(cb) {
  redisbase.getAllItems(tableAction, cb);
}

function addAction(action, cb) {
  redisbase.addItem(tableAction, action, cb, 
  [], 
  ["ruleId"]);
}

function updateAction(action, cb) {
  redisbase.addItem(tableAction, action, cb, ["ruleId"]); 
}

function deleteAction(actionId, cb) {
  redisbase.deleteItem(tableAction, actionId, cb);
}

// Rules

function getRule(ruleId, cb) {
  redisbase.getSingleItem(tableRule, ruleId, cb);
}

function getRules(cb) {
  redisbase.getAllItems(tableRule, cb);
}

function addRule(rule, cb) {
  redisbase.addItem(tableRule, rule, cb);
}

function updateRule(rule, cb) {
  redisbase.updateItem(tableRule, rule, cb);  
}

function deleteRule(ruleId, cb) {
  redisbase.deleteItem(tableRule, ruleId, cb);
}