const fs = require('fs');

const wasmBuffer = fs.readFileSync('./main.wasm');
const wasmModule = new WebAssembly.Module(wasmBuffer);
const wasmInstance = new WebAssembly.Instance(wasmModule);

console.log(wasmInstance.exports.add(1, 3));