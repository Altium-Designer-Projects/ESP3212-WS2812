const char indexPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
<title>WebSocket Tutorial</title>
</head>

<body onload="SocketInit()">

<h1>WebSockets Test</h1>

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
}

function onError(event){      // Called when a WebSocket error occurs
  console.log("ERROR: " + event.data);
}
function SocketSend(){
  websocket.send("Hello Arduino");
  console.log("Hello Socket");
}  

setInterval('SocketSend()', 2000);
    
</script>
</body>
</html>
)rawliteral";
