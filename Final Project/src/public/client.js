/* 
This is the websocket library which will allow us to send messages
back to the web server 
*/
var socket = io();
var obj = {col: "blue.jpg"};
var audio1 = document.getElementById("audio1");
var audio2 = document.getElementById("audio2"); 
var audio3 = document.getElementById("audio3");
var audio4 = document.getElementById("audio4");
var audio5 = document.getElementById("audio5");
var audio6 = document.getElementById("audio6");
var placeholder = document.getElementById("placeholder");
placeholder.src = "green.jpg";
placeholder.style.opacity = 100;


socket.on('event', (data) => {
  console.log('received data from webserver: ' + data);
});

socket.on('PotRotation', (data) => {
  console.log('received button pressed event from webserver: ' + data);
  //setvolume(data);
  setcolor(obj, data);
});

socket.on('buttonReleased', (data) => {
  console.log('received button released from webserver: ' + data);
  pausemusic();
});


//placeholder.src = obj.col;

function setvolume(vol)
{
  audio1.volume = vol;
  audio2.volume = vol;
  audio3.volume = vol;
  audio4.volume = vol;
  audio5.volume = vol;
  audio6.volume = vol;
}
function pausemusic()
{
  audio1.pause();
  audio2.pause();
  audio3.pause();
  audio4.pause();
  audio5.pause();
  audio6.pause();
}
function setcolor(obj, data)
{
  obj.col = data + ".jpg";
}

function redLEDOn() {
  socket.emit('redLEDOn'); 
}

function redLEDOff() {
  socket.emit('redLEDOff');
}

function greenLEDOn() {
  socket.emit('greenLEDOn'); 
}

function greenLEDOff() {
  socket.emit('greenLEDOff');
}

function blueLEDOn() {
  socket.emit('blueLEDOn'); 
}

function blueLEDOff() {
  socket.emit('blueLEDOff');
}
