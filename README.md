# CANopen-ESP32-nodes
Playing around with CANopenNode to fully expand the true potential of CAN bus on ESP32 while obeying ESP-idf rules.

## Forks
The Forks folder consists of projects I've seen over the internet, and I've tried to re-write in ESP-idf platform:
 + [Alexander Miller's implementation](https://github.com/xXAM22Xx/CANopenESP32) uses the Apache License 2.0
   + Only implements the Master Node.  Alexander has other CANopen devices to play with like the [Dunker motor](https://www.dunkermotoren.com/en/)
   + It works, but I don't know how PDO mapping nor the object dictionary works.
 + [Guillermo-ruffino's implementation](https://github.com/CANopenNode/CANopenNode/issues/198#issuecomment-658429391):
   + Consists of a LSS Slave node.

## How to use

1. Download [esp-idf](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html), and follow their instructions.
2. Enter into terminal
   1. `idf.py menuconfig`
	    + You will see a GUI, but we can skip this.
	      + `Ctrl + C` to exit
	 2. Build and upload to port (I'm on linux so `/dev/ttyUSB0` and my second port is `/dev/ttyUSB1`):
	    + `idf.py build && idf.py -p /dev/ttyUSB0 flash monitor`
	 3. You can exit the monitor by pressing `Ctrl + ]`.
	 4. To view the serial again:
	    + `idf.py -p /dev/ttyUSB0 monitor`
3. Most of the code here requires multiple terminals to be open to see the output for each CAN node.  So, redo step 2 for as many nodes.

## License
```
Apache License
Version 2.0, January 2004
http://www.apache.org/licenses/

TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION
...
```
