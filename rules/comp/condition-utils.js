// Export methods
exports.buildConditionChildren = buildConditionChildren;
exports.findRootCondition = findRootCondition;
exports.findConditionFromId = findConditionFromId;
exports.findConditionsFromParent = findConditionsFromParent;

function buildConditionChildren(conditions, rootCondition) {
  rootCondition.children = findConditionsFromParent(conditions, rootCondition.id);

  for (var i = 0; i < rootCondition.children.length; i++) {
    buildConditionChildren(conditions, rootCondition.children[i]);
  }
}

function findRootCondition(conditions) {
  for (var i = 0; i < conditions.length; i++) {
    var condition = conditions[i];
    if (condition.parentId == "null") {
      return condition;
    }
  }

  return null;
}

function findConditionFromId(conditions, conditionsId) {
  for (var i = 0; i < conditions.length; i++) {
    var condition = conditions[i];
    if (condition.id == conditionsId) {
      return condition;
    }
  }

  return null;
}

function findConditionsFromParent(conditions, parentId) {
  var children = [];

  for (var i = 0; i < conditions.length; i++) {
    var condition = conditions[i];    
    if (condition.parentId == parentId) {
      children.push(condition);
    }
  }

  return children;
}