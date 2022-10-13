/* 
 * Written by 3tawi
 * Lien vidéo: https://youtu.be/Haiq1Ytl-lY
*/
 
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
    <title>Great Projects</title>
    <script>
    var cival;
    var amval;
    const amvalarr = ["Manual", "Auto"];
    const weekday = ["Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"];

function DisplayCurrentTime() {
  var dt = new Date();
  var dow = weekday[dt.getDay()];
  var da = dt.toLocaleDateString();
  var ti = dt.toLocaleTimeString(true);
  document.getElementById("datetime").innerHTML = (dow + " " + da + " " + ti);
        setTimeout('DisplayCurrentTime()', 1000);
}
function myFunctionPre() {
  var strLine = "modepre";
  var request = new XMLHttpRequest();
  request.open("GET", strLine, false);
  request.send(null);
  GetArduinodata();
}
function myFunctionNex() {
  var strLine = "modenex";
  var request = new XMLHttpRequest();
  request.open("GET", strLine, false);
  request.send(null);
  GetArduinodata();
}
function myFunctionMA() {
  var strLine = "modeauto";
  var request = new XMLHttpRequest();
  request.open("GET", strLine, false);
  request.send(null);
  GetArduinodata();
}
function myFunctionS1() {
  var n = document.getElementById("mySpeed0");
  var strLine = "mySpeed0?Speed0=" + n.value;
  var request = new XMLHttpRequest();
  request.open("GET", strLine, false);
  request.send(null);
  GetArduinodata();
}
function myFunctionS2() {
  var n = document.getElementById("mySpeed1");
  var strLine = "mySpeed1?Speed1=" + n.value;
  var request = new XMLHttpRequest();
  request.open("GET", strLine, false);
  request.send(null);
  GetArduinodata();
}
function myFunctionS3() {
  var n = document.getElementById("mySpeed2");
  var strLine = "mySpeed2?Speed2=" + n.value;
  var request = new XMLHttpRequest();
  request.open("GET", strLine, false);
  request.send(null);
  GetArduinodata();
}
function myFunctionS4() {
  var n = document.getElementById("mySpeed3");
  var strLine = "mySpeed3?Speed3=" + n.value;
  var request = new XMLHttpRequest();
  request.open("GET", strLine, false);
  request.send(null);
  GetArduinodata();
}
function myFunctionS5() {
  var n = document.getElementById("mySpeed4");
  var strLine = "mySpeed4?Speed4=" + n.value;
  var request = new XMLHttpRequest();
  request.open("GET", strLine, false);
  request.send(null);
  GetArduinodata();
}
function myFunctionPatt() {
  var n = document.getElementById("Modepat");
  var strLine = "modepat?Pattern=" + n.value;
  var request = new XMLHttpRequest();
  request.open("GET", strLine, false);
  request.send(null);
  GetArduinodata();
}
function myFunctionRest() {
  var strLine = "restesp";
  var request = new XMLHttpRequest();
  request.open("GET", strLine, false);
  request.send(null);
}
function GetArduinodata() {
  var strstat = "readstat";
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
            cival      = this.responseXML.getElementsByTagName('analog')[0].childNodes[0].nodeValue;
            amval      = this.responseXML.getElementsByTagName('analog')[1].childNodes[0].nodeValue;
    }
    document.getElementById("cimesg").innerHTML = "Mode " + cival;
    document.getElementById("ammesg").innerHTML = amvalarr[amval];
  }
  xhttp.open("GET", strstat, true);
  xhttp.send();
        }
      document.addEventListener('DOMContentLoaded', function() {
        DisplayCurrentTime(),GetArduinodata();
      }, false);
    </script>
    <style>

body {
   text-align: center;
   background-color: #ddccff;
}
.slider {
  -webkit-appearance: none;
  width: 100%;
  height: 15px;
  background: #ffff00;
  outline: none;
  opacity: 0.7;
  -webkit-transition: .2s;
  transition: opacity .2s;
}
.slider:hover {
  opacity: 1;
}
.slider::-moz-range-thumb {
  width: 15px;
  height: 25px;
  background: #FF0F50;
  cursor: pointer;
}
.data-input {
  width: 100%;
  background-color: #000000;
  font-size:26px;
  color:red;
  border: 5px solid #444444;
  -moz-border-radius: 7px;
  -webkit-border-radius: 7px;
}
.datta-d {
  width: 100%;
  background-color: #ffffff;
  font-size:26px;
  color:red;
  border: 5px solid #444444;
  -moz-border-radius: 7px;
  -webkit-border-radius: 7px;
}
  .button-blue {
  text-shadow: 1px 1px #ff0000;
  padding: 2px 2px 2px 2px;
  width: 100%;
  border: #ff0000 solid 3px;
  background-color: #4444ff;
  color:white;
  font-size:20px;
  padding-bottom:2px;
  font-weight:700;
  -moz-border-radius: 7px;
  -webkit-border-radius: 7px;
}
table {
  text-align: center;
  width:100%;
  color: #0000ff;
}
.Date{
  padding-top: 0px;
  color: #ff00ff;
  text-shadow: 2px 2px #ffff00;
  text-align: center;
  font-style: blod;
  font-size:26px;
}
/* Full-width input fields */
input[type=text], input[type=password] {
  width: 100%;
  padding: 12px 20px;
  margin: 8px 0;
  display: inline-block;
  border: 1px solid #ccc;
  box-sizing: border-box;
}
.container {
  padding: 16px;
}
span.psw {
  float: right;
  padding-top: 16px;
}
/* The Modal (background) */
.modal {
  display: none; /* Hidden by default */
  position: fixed; /* Stay in place */
  z-index: 1; /* Sit on top */
  left: 0;
  top: 0;
  width: 100%; /* Full width */
  height: 100%; /* Full height */
  overflow: auto; /* Enable scroll if needed */
  background-color: rgb(0,0,255); /* Fallback color */
  background-color: rgba(0,0,255,0.4); /* Black w/ opacity */
  padding-top: 60px;
}
/* Modal Content/Box */
.modal-content {
  background-color: #fefefe;
  margin: 5% auto 15% auto; /* 5% from the top, 15% from the bottom and centered */
  border: 1px solid #888;
  width: 50%; /* Could be more or less, depending on screen size */
}
/* Add Zoom Animation */
.animate {
  -webkit-animation: animatezoom 0.5s;
  animation: animatezoom 0.5s
}

@-webkit-keyframes animatezoom {
  from {-webkit-transform: scale(0)} 
  to {-webkit-transform: scale(1)}
}
  
@keyframes animatezoom {
  from {transform: scale(0)} 
  to {transform: scale(1)}
}

/* Change styles for span and cancel button on extra small screens */
@media screen and (max-width: 300px) {
  span.psw {
     display: block;
     float: none;
  }
  .cancelbtn {
     width: 100%;
  }
}
/* Set a style for all buttons */
button {
  background-color: #4CAF50;
  color: white;
  padding: 14px 20px;
  margin: 8px 0;
  border: none;
  cursor: pointer;
  width: 100%;
}

button:hover {
  opacity: 0.8;
}

   </style>
</head>
<body>
<tr>
     <div class="data-input">
       <table>
         <form action="/" method="POST">
            <td style="width:40%">
                <b class="Date"><span id="datetime"></span></b></td>
            <td style="width:22%">
                <input type="number" name="Pattern" min="1" max="16" value="1" class="button-blue" style="width:20%" id="Modepat">
                <button class="button-blue" onclick="myFunctionPatt()" style="width:40%">Set Mode</button></td>
            <td style="width:18%">
                <b class="button-blue" style="width:98%"><span id="cimesg"></span></b></td>
            <td style="width:18%">
                <button class="button-blue" style="width:98%" onclick="myFunctionMA()"><span id="ammesg"></span></button></td>
         </form>
       </table>
     </div>
</tr>
    <tr>
        <table>
         <form action="/" method="POST">
            <td style="width:25%">
            <div class="data-input">
                <button class="button-blue" onclick="myFunctionPre()">&laquo; Previous</span></button>
            </div></td>
            <td style="width:25%">
            <div class="data-input">
                <button class="button-blue" onclick="myFunctionNex()">Next &raquo;</span></button>
            </div></td>
            <td style="width:25%">
            <div class="data-input">
                <button class="button-blue" onclick="myFunctionRest()">Rest Esp32</button>
            </div>
            </td>
         </form>
        <td style='width:25%'>
    <div  class="data-input">
    <button onclick="document.getElementById('id01').style.display='block'" class="button-blue">Connect to Wi-Fi</button>
  </div>
    <div id="id01" class="modal">
  <form class="modal-content animate" action="/mywifi" method="POST">
    <div  class="data-input">
    <div class="slidecontainer">
      <label for="usname"><b>Wifi Ssid</b></label>
      <input type="text" placeholder="Enter Ssid" name="usname" required>
      <label for="pssw"><b>Password</b></label>
      <input type="password" placeholder="Enter Password" name="pssw">
      <button type="submit" class="button-blue"><b>Connect</b></button>
    </div>
    </div>
  </form>
</div>

<script>
// Get the modal
var modal = document.getElementById('id01');

// When the user clicks anywhere outside of the modal, close it
window.onclick = function(event) {
    if (event.target == modal) {
        modal.style.display = "none";
    }
}
</script></td>
        </table>
</tr>
<tr>
    <table>
         <form action="/" method="POST">
        <div class="data-input">
            <input style="width:80%" type="range" name="Speed0" min="0" max="50" value="7" class="slider" id="mySpeed0" onchange="myFunctionS1()">
            <b> <span id="speed0"></span></b>
            <script>
var slider0 = document.getElementById("mySpeed0");
var output0 = document.getElementById("speed0");
output0.innerHTML = slider0.value;

slider0.oninput = function() {
  output0.innerHTML = this.value;
}
</script>
        </div>
         </form>
        </table>
</tr>
<tr>
    <table>
         <form action="/" method="POST">
        <div class="data-input">
            <input style="width:80%" type="range" name="Speed1" min="1" max="200" value="20" class="slider" id="mySpeed1" onchange="myFunctionS2()">
            <b> <span id="speed1"></span></b>
            <script>
var slider1 = document.getElementById("mySpeed1");
var output1 = document.getElementById("speed1");
output1.innerHTML = slider1.value;

slider1.oninput = function() {
  output1.innerHTML = this.value;
}
</script>
        </div>
         </form>
        </table>
</tr>
<tr>
    <table>
         <form action="/" method="POST">
        <div class="data-input">
            <input style="width:80%" type="range" name="Speed2" min="1" max="6" value="2" class="slider" id="mySpeed2" onchange="myFunctionS3()">
            <b> <span id="speed2"></span></b>
            <script>
var slider2 = document.getElementById("mySpeed2");
var output2 = document.getElementById("speed2");
output2.innerHTML = slider2.value;

slider2.oninput = function() {
  output2.innerHTML = this.value;
}
</script>
        </div>
         </form>
        </table>
</tr>
<tr>
    <table>
         <form action="/" method="POST">
        <div class="data-input">
            <input style="width:80%" type="range" name="Speed3" min="2" max="240" value="8" class="slider" id="mySpeed3" onchange="myFunctionS4()">
            <b> <span id="speed3"></span></b>
            <script>
var slider3 = document.getElementById("mySpeed3");
var output3 = document.getElementById("speed3");
output3.innerHTML = slider3.value;

slider3.oninput = function() {
  output3.innerHTML = this.value;
}
</script>
        </div>
         </form>
        </table>
</tr>
<tr>
    <table>
         <form action="/" method="POST">
        <div class="data-input">
            <input style="width:80%" type="range" name="Speed4" min="-10" max="10" value="0" class="slider" id="mySpeed4" onchange="myFunctionS5()">
            <b> <span id="speed4"></span></b>
            <script>
var slider4 = document.getElementById("mySpeed4");
var output4 = document.getElementById("speed4");
output4.innerHTML = slider4.value;

slider4.oninput = function() {
  output4.innerHTML = this.value;
}
</script>
        </div>
         </form>
        </table>
</tr>
<tr>
    <table>
         <form action="/" method="POST">
        <div class="data-input">
            <input type="submit" value="Refresh" class="button-blue" style="width:98%">
        </div>
         </form>
    </table>
</tr>
</body>
</html>
)=====";
