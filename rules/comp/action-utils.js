var utils = require("./utils.js");

// Export methods
exports.removeActionDuplicates = removeActionDuplicates;

function removeActionDuplicates(actions) {
  var actuatorAndPropertyIds = [];

  var i = 0;
  while (i < actions.length) {
    var action = actions[i];
    var actuatorAndPropertyId = "{0};{1}".format(action.actuatorId, action.actuatorPropertyId);

    if (actuatorAndPropertyIds.indexOf(actuatorAndPropertyId) === -1) {
      // If the actuatorAndPropertyId wasn't already processed, we add it to the array.
      actuatorAndPropertyIds.push(actuatorAndPropertyId);
      i++;
    } else {
      // Otherwise, we remove the action from the action list.
      actions.splice(i, 1);
    }
  }

  return actions;
}