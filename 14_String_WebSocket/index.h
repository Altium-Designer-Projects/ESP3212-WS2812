const char indexPage[] PROGMEM = R"rawliteral(
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
 background:#ECF0F1;
 font-family:Segoe UI,Frutiger,Frutiger Linotype,Dejavu Sans,Helvetica Neue,Arial,sans-serif;
 text-align:center;
}

.container{
 width:960px;
 margin-top:50px;
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
 margin:20px; 
 box-sizing: border-box;
 color: #000000;
 box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
}

h1{
 font-family:"Segoe UI";
 font-size:60px;
 font-style:bold;
 font-variant:normal;
 font-weight:600;
 line-height:34px;
} 
h2{
 font-family:"Segoe UI";
 font-size:28px;
 font-style:bold;
 font-variant:normal;
 font-weight:400;
 line-height:28px;
} 
textarea{
  width: 100%;
  height: 500px;
  margin: 8px 0;
  padding: 12px 12px;
  box-sizing: border-box;
  border: 2px solid #ccc;
  border-radius: 4px;
  background-color: #f8f8f8;
  font-size: 18px;
  resize: none;
  border-radius: 4px;
}

p{
 font-family:"Segoe UI";
 font-size:12px;
 font-style:normal;
 font-variant:normal;
 font-weight:400;
 line-height:14px;
} 
 
input[type=text] {
  width: 400px;
  padding: 12px 20px;
  margin: 8px 0;
  box-sizing: border-box;
  border: 1px solid #555;
  outline: none;
}
input[type=text]:focus {
  background-color: #D3D3D3;
  border: 2px solid #696969;
}

textarea{
  width: 100%;
  height: 500px;
  margin: 8px 0;
  padding: 12px 12px;
  box-sizing: border-box;
  border: 2px solid #ccc;
  border-radius: 4px;
  background-color: #f8f8f8;
  font-size: 18px;
  resize: none;
  border-radius: 4px;
}

</style>
</head>
<body onload="SocketInit()">  

<div class="container">
 
  <h1>Raw Data</h1><br>
  <div class="card">
   <textarea id="rxConsole"></textarea>
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
  websocket.send("...");
} 

</script>
</body>
</html>
)rawliteral";
