// Export methods
exports.buildConditionChilds = buildConditionChilds;
exports.findRootCondition = findRootCondition;
exports.findConditionFromId = findConditionFromId;
exports.findConditionsFromParent = findConditionsFromParent;

function buildConditionChilds(conditions, rootCondition) {
  rootCondition.childs = findConditionsFromParent(conditions, rootCondition.id);

  for (var i = 0; i < rootCondition.childs.length; i++) {
    buildConditionChilds(conditions, rootCondition.childs[i]);
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
  var childs = [];

  for (var i = 0; i < conditions.length; i++) {
    var condition = conditions[i];    
    if (condition.parentId == parentId) {
      childs.push(condition);
    }
  }

  return childs;
}