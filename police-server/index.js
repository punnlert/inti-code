// for socket
const path = require('path');
const express = require('express');
const fs = require('fs');

const app = express();
const server = require('http').createServer(app);
// socket.io
var io = require('socket.io')(server, {
  cors: {
    origin: "*",
  }
});

//for serial comm
const { SerialPort } = require('serialport');
const { DelimiterParser } = require('@serialport/parser-delimiter');

const dotenv = require('dotenv').config({
  path: path.join(__dirname, '.env'),
});

const envVariables = process.env;
const { SERIAL_PORT, BAUDRATE } = envVariables;

const port = new SerialPort({ path: SERIAL_PORT, baudRate: BAUDRATE | 0 });

app.get('/', (req, res) => {
  res.sendFile(__dirname + '/index.html');
});

io.on('connection', (socket) => {
  console.log('a user connected')
  const parser = port.pipe(new DelimiterParser({ delimiter: '\n' }));
    parser.on('data', (data) => {
    let str = '' + data;
    console.log('emit: ' + str);
    io.emit('button data', str);
  });
});

server.listen(3000, () => {
    console.log('listening on *:3000');
});
