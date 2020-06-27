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
body{
 width:100%;
 background:#ECF0F1;
 font-family:Segoe UI,Frutiger,Frutiger Linotype,Dejavu Sans,Helvetica Neue,Arial,sans-serif;
 text-align:center;
}

.container{
 width:960px;
 padding-top:50px;
 margin-left:auto;
 margin-right:auto;
 align-items: center;
}

.card{
 max-width: 100%;
 border:none;
 border-radius:8px;
 color: white;
 outline: none;
 background: #B0C4DE;
 padding: 30px;
 box-sizing: border-box;
 color: #000000;
 margin:20px;
 box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
}
h1{
 font-family:"Segoe UI";
 font-size:40px;
 font-style:normal;
 font-variant:normal;
 font-weight:600;
 line-height:34px;
} 

input[type=text] {
  padding: 12px 12px;
  margin: 8px 0;
  box-sizing: border-box;
  border: 1px solid transparent;
  outline: none;
  -moz-appearance: none;
  -webkit-appearance: none;
  align-items: center;
  border-radius: 4px;
  display: inline-flex;
  font-size: 2rem;
  height: 2.5em;
  justify-content: flex-start;
  line-height: 1.5;
  position: relative;
  vertical-align: top;
  background-color: white;
  border-color: #dbdbdb;
  color: #363636;
  box-shadow: inset 0 0.0625em 0.125em rgba(10, 10, 10, 0.05);
  max-width: 100%;
  width: 100%;  
}
input[type=text]:focus {
  outline: none;
  border: 2px solid #696969;
}

</style>
</head>
<body onload="SocketInit()">  

<div class="container">
  
  <h1>String Processing</h1><br>
  
  <div class="card">
    <input type="text" id="txBar" onkeydown = "if(event.keyCode == 13) SocketSend();" /> // enter button
  </div>
  
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
 websocket.send("?"+document.getElementById("txBar").value);
 document.getElementById("txBar").value = "";  
}

</script>
</body>
</html>
)=====";
