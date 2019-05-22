/* 
This is the websocket library which will allow us to send messages
back to the web server 
*/
var socket = io();
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



});


socket.on('event', (data) => {
  console.log('received data from webserver: ' + data);
});

socket.on('PotRotation', (data) => {
  console.log('received potentiometer event from webserver: ' + data);
  document.body.style.backgroundColor = data;
});

socket.on('volRotation', (data) => {
  console.log('received button pressed event from webserver: ' + data);
  setvolume(data);
});

socket.on('ButtonPressed1', (data) => {
  console.log('received button1 pressed from webserver: ' + data);
  pausemusic();
});
socket.on('ButtonPressed2', (data) => {
  console.log('received button2 pressed from webserver: ' + data);
  audio1.play();
});



//setTimeout(mailer, 10000);

/*function mailer()
{
  if (audio1.volume > 0.9 && pausestate =1)
  {
    socket.emit('sendmail');
  }
}*/

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
