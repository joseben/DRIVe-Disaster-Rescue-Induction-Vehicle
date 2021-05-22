# DRIVe-Disaster-Rescue-Induction-Vehicle

The main purpose of this project is to use it as an object transporter robot during disasters.

DRIVe can also be used in case of a pandemic such as the Covid '19 pandemic. It can transport the medical supplies and Oxygen cylinders to isolated places or containment zones and reduce human interactions. The onboard camera can also evaluate the situation of the locality and people.

https://youtu.be/nldsgOFV0nQ

## Featues

- Instead of the usual Arduino UNO + wifi/ Bluetooth module combination which is bulky and complicated, DRIVe has an ESP32 board that has all these features packed into a single board, making it more compact.
- DRIVe can be operated remotely.
- DRIVe can be controlled in two ways, Autonomous mode, and Manual mode.
- DRIVe can be controlled manually either by using Hand gestures or by using a Joystick
- DRIVe has an onboard camera with Day&Night vision and the live video can be streamed to any device.
- The camera also has a built-in two-way audio system so that communications with people inside the containment zones are also made possible So that necessary relief material could be arranged for them.
- Obstacle avoidance features are also added so that DRIVe could avoid obstacles if any, during the journey.


## Manual Mode

The hand gesture controller consists of a NodeMCU which is a microcontroller similar to Arduino with a wifi module build into it, which again makes the controller small and compact along with a 3-axis accelerometer. With the unit fixed to a hand glove, as the hand is moved around, DRIVe moves accordingly. As a future improvement, with the addition of a flex sensor, the speed and movement of DRIVe can be controlled

## Autonomous Mode

In Autonomous mode, The coordinates of the destination are entered into a web browser. (To implement the web interface feature flask and WebSockets have been used). A server is set up in a computer system, the web page and DRIVe are 2 clients connected to it. Once the coordinates are entered the server sends the coordinates to DRIVe and DRIVe calculates the distance and the required GPS course to the destination. Once everything is set DRIVe begins its journey.

Obstacle avoidance features are also added so that DRIVe could avoid obstacles if any, during the journey.

## Performance and Improvements 

As of now DRIVe just calculates the shortest direct path to the destination due to the inefficiency and inaccuracy of the GPS and compass modules. 
With further development of DRIVe and using more accurate devices, we can use it with services like Google Earth to identify the roads and shortest paths through them.

Also with the addition of RGBD cameras and AI into DRIVe, it would be able to recognize water, fire, humans, or even obstacles more accurately.

In the future, ROS could be implemented on a Rasberry Pi and a LIDAR system can be added for better obstacle avoidance and mapping. Computer vision can also be implemented with the onboard camera to achieve better results

## General Thought
I have been planning to improvise DRIVe and add this system to a drone so that it can be used in more terrains and have better features. But as of now the GPS and compass I have are not very accurate so DRIVe provides just satisfactory results. So with further improvements and testing in the future, I hope i could apply DRIVe to a Drone 
