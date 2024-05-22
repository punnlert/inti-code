const { SerialPort } = require('serialport');
const { DelimiterParser } = require('@serialport/parser-delimiter');
const path = require('path');

const dotenv = require('dotenv').config({
  path: path.join(__dirname, '.env'),
});

const envVariables = process.env;
const { SERIAL_PORT, BAUDRATE } = envVariables;

const port = new SerialPort({ path: SERIAL_PORT, baudRate: BAUDRATE | 0 });

const parser = port.pipe(new DelimiterParser({ delimiter: '\n' }));
parser.on('data', (data) => {
  let str = '' + data;
  console.log(str);
});
