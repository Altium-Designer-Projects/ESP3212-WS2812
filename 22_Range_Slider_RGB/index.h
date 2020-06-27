const char indexPage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
<title>WebSocket Tutorial</title>
</head>
<body>
LED Control:<br/><br/>

 R <input type="range" value="0" min="0" max="255" step="1" id="myRangeRed" oninput="sendRGB();" /><br/>
 G <input type="range" value="0" min="0" max="255" step="1" id="myRangeGreen" oninput="sendRGB();" /><br/>
 B <input type="range" value="0" min="0" max="255" step="1" id="myRangeBlue" oninput="sendRGB();" /><br/>
 

<script>
var Socket = new WebSocket('ws://'+location.hostname+':81/', ['arduino']);

Socket.onopen = function(){
  Socket.send("Connect " + new Date()); 
};

Socket.onerror = function(error){
  console.log("WebSocket Error ", error);
};

Socket.onmessage = function(event){
  console.log("Server: ", event.data);
};

function sendRGB(){
 var redRead = parseInt(document.getElementById("myRangeRed").value).toString(16);
 var greenRead = parseInt(document.getElementById("myRangeGreen").value).toString(16);
 var blueRead = parseInt(document.getElementById("myRangeBlue").value).toString(16);
 
 if(redRead.length < 2){
    redRead = '0' + redRead;
 }
 
 if(greenRead.length < 2){
    greenRead = '0' + greenRead;
 }
 
 if(blueRead.length < 2){
    blueRead = '0' + blueRead;
 }
 
 var result = '#'+redRead+greenRead+blueRead;
 console.log("RGB: " + result);
 
 Socket.send(result);
 
}
</script>
</body>
</html>

)=====";
