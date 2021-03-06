/*
	UDP And HTTP Server
	Context: node.js

	Serve a web page to a browser with a control panel
	Read control panel and send results to Arduino 

	Web server provided by modules "http" and "express"

	Communication with Arduino is via a UDP socket
	provided by module "dgram"

	Communication with the web client (web browser) 
	is via a UDP socket provided by webSockets.
	Websockets creates a socket on top of the HTTP protocol
	The webSockets module is "socket.io"

	created 7 March 2019
	by Michael Shiloh

	Change log:

	14 Mar 2019 - ms - better comments and cleaned up code
										 send bytes instead of buffers to Arduino
										 receive button events from Arduino and send to web page
*/

/* UDP server talks to Arduino */
var dgram = require('dgram');
var arduinoUDPServer = dgram.createSocket('udp4')
var MY_PORT_FOR_ARDUINO = 7000;
var ARDUINO_PORT_FOR_ME= 5000;
var ARDUINO_IP_ADDRESS = '10.225.161.127';
var nodemailer = require('nodemailer'); // This is the added require call to use nodemailer

var transporter = nodemailer.createTransport({
 service: 'gmail',
 auth: {
        user: 'henoktemboo@gmail.com',
        pass: 'tdashuluqa'
    }
}); //This sets up the transporter for the nodemailer including the service and authentication.

const mailOptions = {
  from: 'henoktemboo@gmail.com', // sender address
  to: 'henimagne@gmail.com', // list of receivers
  subject: 'Notification', // Subject line
  html: '<p>You are playing your music too loud </p>'// plain text body
};


const http = require('http')
const express = require('express'); // web server application
const app = express();        // instantiate express server
const httpServer = http.Server(app);  // connects http library to server
const HTTP_SERVER_PORT = 8000; 

// Express creates the simple web page
// The argument says where to find pages and scripts
app.use(express.static('public'));  

// websockets so that webpage can talk back to server
const webSocket = require('socket.io')(httpServer);  

/* Arduino UDP server callback functions */



function ArduinoUDPServerIsListening() {
	console.log('Arduino UDP Server is listening');
}

function ArduinoUDPServerReceivedMessage(message, sender) {

	// If the message is a byte we need to read a byte

	// Now send a message to the web browser to change color depending on the received signal from arduino
	if (message.readUInt8(0) == 0 ) {
		console.log( "received a 0");
		
		webSocket.emit('PotRotation', );
	}

	else if (message.readUInt8(0) == 1 ) {
		console.log( "received a 1");
		webSocket.emit('PotRotation', '#4c5d77');
	
	}

	else if (message.readUInt8(0) == 2 ) {
		console.log( "received a 2");
		webSocket.emit('PotRotation', '#723870');
	
	}
	else if (message.readUInt8(0) == 3 ) {
		console.log( "received a 3");
		webSocket.emit('PotRotation', '#376335');
	
	}
	else if (message.readUInt8(0) == 4 ) {
		console.log( "received a 4");
		webSocket.emit('PotRotation', '#7c822a');
	
	}
	else if (message.readUInt8(0) == 5 ) {
		console.log( "received a 5");
		webSocket.emit('PotRotation', '#35305b');
	
	}else if (message.readUInt8(0) == 6 ) {
		console.log( "received a 6");
		webSocket.emit('PotRotation', '#672e84');
	
	}

	else if (message.readUInt8(0) == 7 ) {
		console.log( "received a 7");
		webSocket.emit('PotRotation', '#63182d');
	
	}
	else if (message.readUInt8(0) == 8 ) {
		console.log( "received a 8");
		webSocket.emit('PotRotation', '#235556');
	
	}
	else if (message.readUInt8(0) == 9 ) {
		console.log( "received a 9");
		webSocket.emit('PotRotation', '#3b7f51');
	
	}
	else if (message.readUInt8(0) == 10 ) {
		console.log( "received a 10");
		webSocket.emit('PotRotation', '#35161f');
	
	}

	
	// This second byte from the arduino represents volume, and here we send info to the browser to set the volume of audio playing depending 
	// on the signal received from arduino
	if (message.readUInt8(1) == 0 ) {
		console.log( "received a 0");
		webSocket.emit('volRotation', 0);
	}

	else if (message.readUInt8(1) == 1 ) {
		console.log( "received a 1");
		webSocket.emit('volRotation', 0.2);
	
	}

	else if (message.readUInt8(1) == 2 ) {
		console.log( "received a 2");
		webSocket.emit('volRotation', 0.4);
	
	}
	else if (message.readUInt8(1) == 3 ) {
		console.log( "received a 3");
		webSocket.emit('volRotation', 0.6);
	
	}
	else if (message.readUInt8(1) == 4) {
		console.log( "received a 4");
		webSocket.emit('volRotation', 0.8);
	
	}
	else if (message.readUInt8(1) == 5) {
		console.log( "received a 5");
		webSocket.emit('volRotation', 1);
	
	}

	//Read the third byte of the message from arduino and send the play state to the website.
	if (message.readUInt8(2)== 0){
		console.log( "received a 0");
		webSocket.emit('ButtonReleased1', 0);
	}
	else if (message.readUInt8(2)== 1){
		console.log( "received a 1");
		webSocket.emit('ButtonPressed1', 1);
	}

    //Read the fourth byte of the message from arduino and send the pause state to the website.
	if (message.readUInt8(3)== 0){
		console.log( "received a 0");
		webSocket.emit('ButtonReleased2', 0);
	}
	else if (message.readUInt8(3)== 1){
		console.log( "received a 1");
		webSocket.emit('ButtonPressed2', 1);
	}	

}

/* Register the UDP callback functions */
arduinoUDPServer.bind( MY_PORT_FOR_ARDUINO );
arduinoUDPServer.on( 'listening', ArduinoUDPServerIsListening);
arduinoUDPServer.on( 'message', ArduinoUDPServerReceivedMessage);

/* HTTP callback functions */

httpServer.listen(HTTP_SERVER_PORT, () => {
	console.log('web server: Listening at', httpServer.address());
});

httpServer.on('connection', () => {
  console.log("web server: An HTTP client has connected")
});


webSocket.on('connect', (socket) => {
	// array for the message
	// newLEDState[0] = LED number 
	// newLEDState[1] = LED state 
	const SIZEOF_LED_DATA = 10;
	var newLEDState = new Uint8Array(SIZEOF_LED_DATA ); 
  
	console.log('Web server socket: Client connected');

	//when received sendmail message from client, call teh sendmail function of the transporter.

  	socket.on('sendmail', () => {
		console.log('Send mail');
		transporter.sendMail(mailOptions, function (err, info) {
	    if(err)
		    console.log(err)
	   	else
			console.log(info);
		newLEDState[0] = 0;  // Red is LED number 0
		newLEDState[1] = 1; // turn on the LED
    	// Send the message to Arduino
    	arduinoUDPServer.send(
			newLEDState,
			0, // offset to the message start in the buffer
			SIZEOF_LED_DATA,
			ARDUINO_PORT_FOR_ME, 
			ARDUINO_IP_ADDRESS);
 	});
		
  });
  

  // if you get the 'disconnect' message, say the user disconnected
  socket.on('disconnect', () => {
    console.log('Web server socket: user disconnected');
  });




});
