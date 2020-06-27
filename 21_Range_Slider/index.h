const char indexPage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<title>ESP32 Web Server</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
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
 background: #dddddd;
 padding: 30px;
 box-sizing: border-box;
 color: #000000;
 margin:20px;
 box-shadow: 0px 2px 18px -4px rgba(0,0,0,0.75);
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

.slidecontainer {
  width: 100%;
}

.slider {
  -webkit-appearance: none;
  width: 100%;
  height: 15px;
  border-radius: 5px;
  background: #454140;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
}

.slider:hover {
  opacity: 1;
}

.slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  appearance: none;
  width: 25px;
  height: 25px;
  border-radius: 50%;
  background: #4CAF50;
  cursor: pointer;
}

.slider::-moz-range-thumb {
  width: 25px;
  height: 25px;
  border-radius: 50%;
  background: #4CAF50;
  cursor: pointer;
}
</style>
</head>
<body onload="SocketInit()">

<div class="container">
 <div class="card">
 <h1>Range Slider</h1><br>
 
  <div class="slidecontainer">
    <input type="range" min="0" max="1023" value="512" class="slider" id="myRange" />
    <h2> <span id="demo">..</span></h2>
  </div>
 
 </div>
</div>
<script>
 var Socket;
 function SocketInit(){
   Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
 }
   
 var slider = document.getElementById("myRange");
 var output = document.getElementById("demo");
 
 output.innerHTML = slider.value;
 
 slider.oninput = function(){
   output.innerHTML = this.value;
   Socket.send("#"+document.getElementById("myRange").value);
 } 
</script>

</body>
</html>
)=====";
