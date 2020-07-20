# CANopen-ESP32-nodes
Playing around with CANopenNode to fully expand the true potential of CAN bus on ESP32 while obeying ESP-idf rules.
Link to this repo on [Github](https://github.com/nathanRamaNoodles/CANopen-ESP32-nodes) 
<!-- TOC depthFrom:1 depthTo:6 withLinks:1 updateOnSave:1 orderedList:0 -->

- [CANopen-ESP32-nodes](#canopen-esp32-nodes)
				- [Why use CAN?](#why-use-can)
				- [Why use CANopen?](#why-use-canopen)
	- [Forks](#forks)
	- [Requirements](#requirements)
	- [How to use](#how-to-use)
	- [License](#license)

<!-- /TOC -->

##### Why use CAN?
  + Better wired communication between multiple ESP32s and other CAN enabled chips like STM32.
  + High reliability over long distances.
  + Great for home automation.
  + Widely used protocol by big industries.
##### Why use CANopen?
  + Like anything else, a higher level protocol is essential to avoid re-inventing the wheel, and to guarantee reliability in a commercial product.

## Forks
The Forks folder consists of projects I've seen over the internet, and I've tried to re-write in ESP-idf platform:
 + [Alexander Miller's implementation](https://github.com/xXAM22Xx/CANopenESP32) uses the Apache License 2.0
   + Only implements the Master Node.  Alexander has other CANopen devices to play with like the [Dunker motor](https://www.dunkermotoren.com/en/)
   + It works, but I don't know how PDO mapping nor the object dictionary works.
 + [Guillermo-ruffino's implementation](https://github.com/CANopenNode/CANopenNode/issues/198#issuecomment-658429391):
   + Consists of a LSS Slave node.

## Requirements
You will need the following materials:
+ ESP32 (2 or more)
+ SN65HVD230 (or SN65HVD233) CAN bus transceiver chip for each ESP32.
+ A PC (preferably Linux)

## How to use

1. Download [esp-idf](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html), and follow their instructions.
2. Enter into a root directory for one of the nodes for this repo, like `cd ./Forks/Alexander_Miller/node_one/`
3. Enter into terminal
   1. `idf.py menuconfig`
	    + You will see a GUI, but we can skip this.
	      + `Ctrl + C` to exit
	 2. Build and upload to port (I'm on linux so `/dev/ttyUSB0` and my second port is `/dev/ttyUSB1`):
	    + `idf.py build && idf.py -p /dev/ttyUSB0 flash monitor`
	 3. You can exit the monitor by pressing `Ctrl + ]`.
	 4. To view the serial again:
	    + `idf.py -p /dev/ttyUSB0 monitor`
4. Most of the code here requires multiple terminals to be open to see the output for each CAN node.  So, redo step 2 and 3 for as many nodes.

**Note**: if builds fail, then try to run `idf.py fullclean`. and run build `idf.py build` again

## License
```
Apache License
Version 2.0, January 2004
http://www.apache.org/licenses/

TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION
...
```
