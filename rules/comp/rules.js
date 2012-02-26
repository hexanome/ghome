var redisbase = require("./redisbase.js"),
    sensors = require("./sensors.js"),
    actuators = require("./actuators.js"),
    tableRule = "rule",
    tableCondition = "condition",
    tableAction = "action";

function configure(dbClient) {
  redisbase.configure(dbClient);
}

// Conditions
function getCondition(conditionId, cb) {
  redisbase.getSingleItem(tableCondition, conditionId, cb);
}

function getConditions(cb) {
  redisbase.getAllItems(tableCondition, cb);
}

function getConditionsFromRule(ruleId, cb) {
  redisbase.getItemsFromSec(tableCondition, "ruleId", ruleId, cb);
}

function getConditionsFromSensorAndProperty(sensorId, propertyId, cb) {
  redisbase.getItemsFromSec(tableCondition, "sensorAndPropertyId", "{0};{1}".format(sensorId, propertyId), cb);
}

function addCondition(condition, cb) {
  redisbase.addItem(tableCondition, condition, cb,
    [], // Join.
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
  [{
    "table" : actuators.tableActuator,
    "name" : "actuator"
  }, {
    "table" : actuators.tableActuatorProperty,
    "name" : "actuatorProperty"
  }], ["ruleId"]);
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

function deleteRule(ruleId, cb) {
  redisbase.deleteItem(tableRule, ruleId, cb);
}

// Export methods.
exports.configure = configure;

exports.getCondition = getCondition;
exports.getConditions = getConditions;
exports.getConditionsFromSensorId = getConditionsFromSensorId;
exports.addCondition = addCondition;
exports.deleteCondition = deleteCondition;

exports.getAction = getAction;
exports.getActions = getActions;
exports.addAction = addAction;
exports.deleteAction = deleteAction;

exports.getRule = getRule;
exports.getRules = getRules;
exports.addRule = addRule;
exports.deleteRule = deleteRule;