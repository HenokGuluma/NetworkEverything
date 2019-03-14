function greenledON() {
    console.log ("sending a message to turn on the LED");
    
    socket.emit('greenled', this.value);
    
    // send a packet which will be read by the arduino and assigns variables a value of high.  
}

function greenledOFF() {
    console.log ("sending a message to turn off the LED");
    
    socket.emit('greenled', this.value);
    // send a packet which will be read by the arduino and assigns variables a value of low. 
}
//---------------------------------------------------------------------------------------------------
function blueledON() {
    console.log ("sending a message to turn on the LED");
    
    socket.emit('blueled', this.value);
    
    // send a packet which will be read by the arduino and assigns variables a value of high.  
}

function blueledOFF() {
    console.log ("sending a message to turn off the LED");
    
    socket.emit('blueled', this.value);
    // send a packet which will be read by the arduino and assigns variables a value of low. 
}
//-------------------------------------------------------------------------------------------------------------
function redledON() {
    console.log ("sending a message to turn on the LED");
    
    socket.emit('redled', this.value);
    
    // send a packet which will be read by the arduino and assigns variables a value of high.  
}

function redledOFF() {
    console.log ("sending a message to turn off the LED");
    
    socket.emit('redled', this.value);
    // send a packet which will be read by the arduino and assigns variables a value of low. 
}
