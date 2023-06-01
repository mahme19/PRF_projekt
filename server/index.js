// ------------------------------ setup the server ------------------------------
var express = require('express');
var app = express();
var server = require('http').createServer(app);
var path = require('path');
app.use(express.static(path.join(__dirname, '')));

//server variables
var currentLEDValue = 0;

var currentLightLevelValue = 0;
var currentLDRSensorValue = 0;
var currentStepperValue = 0;



//start server
server.listen(3000, function () {
    console.log("Server listening on port: 3000\n");
});

//on HTTP GET "/" (e.g.: http://localhost:3000/)
app.get('/', function (req, res) {
    //send index.html to client browser
    res.sendFile(__dirname + '/index.html');
});





// ------------------------------ Find and print the server IP-address ------------------------------ 
var networkInterfaces = require('os').networkInterfaces();
var address;

Object.keys(networkInterfaces).forEach(function(interface) {
    networkInterfaces[interface].filter(function(details) {
        if (details.family === 'IPv4' && details.internal === false) {
            address = details.address;
        }
    });
});

console.log("Server IP-address: " + address + "\n");




//---------------------------------SETUP MQTT--------------------
var mqtt = require('mqtt');


var clientId = 'mqttjs_' + Math.random().toString(8).substr(2, 4) 

const host = 'broker.hivemq.com';

var LDR_SENSOR = 'PRF/2023/LDR'
var STEPPER_MOTOR = 'PRF/2023/STEPPER'
var LIGHT_LEVEL = 'PRF/2023/LIGHT'

var commands = 'PRF/2023/COMMANDS'


const connectUrl = `mqtt://${host}`
var client = mqtt.connect(connectUrl, {
  clientId,
  clean: true,
  connectTimeout: 4000,
  reconnectPeriod: 1000,
  resubscribe: true
});

client.on('connect', function () {
    
    console.log('client connected:' + clientId)
    client.subscribe(LDR_SENSOR);
    client.subscribe(STEPPER_MOTOR);
    client.subscribe(LIGHT_LEVEL);
});


client.on('message', function (topic, message, packet) {
    
    if(topic == LDR_SENSOR){
        console.log(topic+"light: "+message.toString());
        currentLDRSensorValue = message.toString();
        EmitLDRSensorValue();
    } else if (topic == STEPPER_MOTOR){
        currentStepperValue = message.toString();
        EmitStepperValue();
    } else if (topic ==  LIGHT_LEVEL){

        currentLightLevelValue = message.toString();
        console.log(topic+ message.toString());
        EmitLEDLevelValue();
    }
 
});


// Needs to subscribe to topics related to sensorpin/ldr/light_level


// needs to publish input from users to MQTT and the arudino needs to subscribe to this data


client.on("error", function(err) { 
    console.log("Error: " + err) 
    if(err.code == "ENOTFOUND") { 
        console.log("Network error, make sure you have an active internet connection") 
    } 
}); 

client.on("close", function() { 
    console.log("Connection closed by client") 
}); 

client.on("reconnect", function() { 
    console.log("Client trying a reconnection") 
}); 

client.on("offline", function() { 
    console.log("Client is currently offline") 
});  


// ------------------------------ setup Socket.IO ------------------------------ 
var io = require('socket.io')(server);




//on new client connection
io.on('connection', function(IOsocket) {
    console.log("Client has connected" + "\n");

    //on client disconnection
    IOsocket.on('disconnect', function () {
        console.log("Client has disconnected" + "\n");
    });

    //on "UpdateCurrentLEDValue"
    IOsocket.on('UpdateCurrentLEDValue', function (data) {
        console.log("Current LED Value received from client: " + data + "\n");
        currentLEDValue = data;
        client.publish(commands, currentLEDValue);

    });
});


function EmitLDRSensorValue() {
    io.emit('CurrentSensorValue', currentLDRSensorValue);
}

function EmitStepperValue() {
    io.emit('CurrentStepperValue', currentStepperValue);
}

function EmitLEDLevelValue() {
    io.emit('CurrentLEDLevelValue', currentLightLevelValue);
}

