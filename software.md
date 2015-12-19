Software
========

The bracelet is not smart. 
It's only streaming data from the accelerometer.

ESP8266 firmware (written in *lua* or *arduino*):
- Wire (read data from the accelerometer)
- TCP server to stream the data 

The software has to:
- receive and store raw data from the device (*node.js* web server)
- recognize patterns using mathematical model (KNN with *python*)
- execute command (*node.js* & *bash* / *powershell*)

Features:
- Config mode - (re)Map any gesture to any command
- Machine Learning mode - create new gestures with ease
- FreeFly mode - turn-off the gestures and control the cursor
  * amazing for fruit ninja game

And finally - it has to be **user-frienly** and **portable**, so there will be no need to install libs, dependencies and so on. 

**Just plug & play**!
