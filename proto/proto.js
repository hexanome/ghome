// Common protocol primitives.

// `data` is a buffer.
function read(data) {
  // This is a textual protocol.
  var sdata = ('' + data).trim(),
      atoms = sdata.split(/\s+/);
  return {
    str: sdata,
    comm: atoms[0],
    args: atoms.slice(1)
  };
}

module.exports = read;
