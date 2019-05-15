/* 
This is the websocket library which will allow us to send messages
back to the web server 
*/
var socket = io();
var obj = {col: "blue.jpg"};
document.addEventListener("DOMContentLoaded", function(){
  var audio1 = document.getElementById("audio1");
  var audio2 = document.getElementById("audio2"); 
  var audio3 = document.getElementById("audio3");
  var audio4 = document.getElementById("audio4");
  var audio5 = document.getElementById("audio5");
  var audio6 = document.getElementById("audio6");
  var audio7 = document.getElementById("audio7");
  var audio8 = document.getElementById("audio8");
  var audio9 = document.getElementById("audio9");
  var pausestate= 0;


  //document.getElementById('newsletter').style.background='red';

});

//document.getElementById("header").style.color = "green";

/*document.getElementsByTagName("header")[0].style.backgroundColor = "red";*/

//var x = document.getElementById('newsletter');

//.style.background = "red";

//x.style.background = "red"


socket.on('event', (data) => {
  console.log('received data from webserver: ' + data);
});

socket.on('PotRotation', (data) => {
  console.log('received button pressed event from webserver: ' + data);
  //setvolume(data);
  /*document.getElementsByTagName("header")[0].style.background = data;
  document.getElementById("newsletter").style.background = data;*/
  document.body.style.backgroundColor = data;
 /* setcolor(obj, data);*/
});

socket.on('volRotation', (data) => {
  console.log('received button pressed event from webserver: ' + data);
  //setvolume(data);
  setvolume(data);
 /* setcolor(obj, data);*/
});

socket.on('ButtonPressed1', (data) => {
  console.log('received button released from webserver: ' + data);
  pausemusic();
});

setTimeout(mailer, 10000);

function mailer()
{
  if (audio1.volume > 0.9 && pausestate =1)
  {
    socket.emit('sendmail');
  }
}
//placeholder.src = obj.col;

function setvolume(vol)
{
  audio1.volume = vol;
  audio2.volume = vol;
  audio3.volume = vol;
  audio4.volume = vol;
  audio5.volume = vol;
  audio6.volume = vol;
  audio7.volume = vol;
  audio8.volume = vol;
  audio9.volume = vol;
}
function pausemusic()
{
  audio1.pause();
  audio2.pause();
  audio3.pause();
  audio4.pause();
  audio5.pause();
  audio6.pause();
  audio7.pause();
  audio8.pause();
  audio9.pause();
  pausestate = 1;
}
/*function setcolor(obj, data)
{
  obj.col = data + ".jpg";
}*/

/*function redLEDOn() {
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
}*/
