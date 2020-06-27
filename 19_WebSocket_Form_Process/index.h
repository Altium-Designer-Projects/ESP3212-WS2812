const char indexPage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
<title>WebSocket Tutorial</title>
</head>
<style>
input[type=text], select {
  width: 500px;
  padding: 12px 20px;
  margin: 8px 0;
  display: inline-block;
  border: 1px solid #ccc;
  border-radius: 4px;
  box-sizing: border-box;
}

input[type=submit] {
  width:250px;
  background-color: #4CAF50;
  color: white;
  padding: 14px 20px;
  margin: 8px 0;
  border: none;
  border-radius: 4px;
  cursor: pointer;
}

input[type=submit]:hover {
  background-color: #45a049;
}

div {
  width:500px;
  border-radius: 5px;
  background-color: #f2f2f2;
  padding: 20px;
}
</style>
</head>
<body onload="SocketInit()">
  
<div>
    <label for="fname">First Name</label>
    <input type="text" id="firstname" name="firstname&" placeholder="Your name.." >

    <label for="lname">Last Name</label>
    <input type="text" id="lastname" name="lastname&" placeholder="Your last name..">

    <label for="country">Country</label>
    <select id="country" name="country&">
      <option value="Turkiye">Turkiye</option>
      <option value="Australia">Australia</option>
      <option value="Canada">Canada</option>
      <option value="USA">USA</option>
    </select>
  
    <input type="submit" value="Submit" onclick = "SocketSend();">

</div>  

<script>

var url = 'ws://' + window.location.hostname + ':81/';

function SocketInit(){    // This is called when the page finishes loading
  wsConnect(url);         // Connect to WebSocket server
}

function wsConnect(url){  // Call this to connect to the WebSocket server
  websocket = new WebSocket(url);  // Connect to WebSocket server
  // Assign callbacks
  websocket.onopen    = function(event) { onOpen(event) };
  websocket.onclose   = function(event) { onClose(event) };
  websocket.onmessage = function(event) { onMessage(event) };
  websocket.onerror   = function(event) { onError(event) };
}

function onOpen(event){             // Called when a WebSocket connection is established with the server
  console.log("Connected");         // Log connection state
}    

function onClose(event){              // Called when the WebSocket connection is closed
  console.log("Disconnected");        // Log disconnection state
  setTimeout(function() { wsConnect(url) }, 1000);       // Try to reconnect after a few seconds
}

function onMessage(event){   // Called when a message is received from the server
  console.log("Received: " + event.data);
  document.getElementById("rxConsole").value += event.data;
}

function onError(event){      // Called when a WebSocket error occurs
  console.log("ERROR: " + event.data);
}

function SocketSend(){
 // websocket.send("...");
 var StringBuffer = "";
       
 StringBuffer  = document.getElementById("firstname").name + document.getElementById("firstname").value;
 StringBuffer += document.getElementById("lastname").name + document.getElementById("lastname").value;
 StringBuffer += document.getElementById("country").name + document.getElementById("country").value;
      
 websocket.send("?" + StringBuffer + "#"); 
 StringBuffer = ""; 
}

</script>
</body>
</html>
)=====";
