# Smart Bike monitoring
The purpose of this project is to develop an embedded system for monitoring a smart-bike. 
Through various sensors placed along the bicycle, speed, altitude, number of compressions, lean angle and 
other parameters are detected with the aim of providing the user engaged on an enduro descent the 
opportunity to evaluate his sports performance. The information obtained from the sensors is processed 
by an Arduino Nano 33 IoT microcontroller equipped with a WiFi module, also located on the vehicle, 
and subsequently sent to a cloud. Communication with the latter is carried out through APIs released 
by a framework called Measurify. It acts as an interface between the detection system and the smartphone 
application. The latter, developed with Flutter, allows the user to analyze the information obtained 
along the descent through a graphical representation of the data.

## Install Hardware and Software
First you have to load the EdgeSW (insert your wifi credentials before) on the Arduino board and connect all 
the sensors and electronics to it. Then make a USB Debug on your phone of the Smart Bike app (it also need your wifi credentials).
Connect the Arduino to your wifi or hotspot, so it will be able to post on server your bike descent measures and then 
open the SmartBike app to see all the measurements made by the system as list of value and graphics.
