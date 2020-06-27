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
 margin-top:50px;
 margin-left:auto;
 margin-right:auto;
 align-items: center;
}
h1{
 font-family:"Segoe UI";
 font-size:40px;
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
</style>

</head>
<body onload="SocketInit()">  
<div class="container">

<h1>Arduino AJAX JSON</h1><br>
<h2 id="idName">...</h2><br>
<h2 id="idYear">...</h2><br>
<h2 id="idCity">...</h2><br>
<h2 id="idAdc">...</h2><br>

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
  
  var obj = JSON.parse(event.data);
  document.getElementById("idName").innerHTML = obj.name;   
  document.getElementById("idYear").innerHTML = obj.year;
  document.getElementById("idCity").innerHTML = obj.city;  
  document.getElementById("idAdc").innerHTML = obj.adc;  
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
)=====";
