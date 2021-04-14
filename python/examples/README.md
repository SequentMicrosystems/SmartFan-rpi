# loop

This is a usage example of the Smart fan and his python library.
The example implements a PID loop that maintains the raspberry temperature by controlling the fan power.

## Usage 

First install the python libary, instructions [here](https://github.com/SequentMicrosystems/SmartFan-rpi/tree/main/python),
then install the gpiozero library for your prefered python version:

Python 3:
```bash
pi@raspberrypi:~$ sudo apt install python3-gpiozero
```
Python 2:
```bash
pi@raspberrypi:~$ sudo apt install python-gpiozero
```

The script must be called with temperature parameter -t <value>. The parameter -v makes the script to report the Raspberry CPU temperature and fan power while running.

## Example

The command
```bash
$ cd ~/SmartFan-rpi/python/examples/
~/SmartFan-rpi/python/examples $ python loop.py -t 50 -v
```
keep Raspberry temperature at 50 degrees Celsius.

## Future work

The loop.py implement a basic PID loop as a Class, the constructor set the 3 coefficients: Kp, Ki, Kd (proportional, integral, and derivative) feel free to tune the loop by editing the following line:
```bash
fanPid = PID(10, .8, .1)
```
For testing you can use stress tests to load the CPU. Install:
```bash
$ sudo apt install stress-ng mesa-utils
```
Example load the CPU for 10 seconds:
```bash
~$ stress-ng --cpu 8 --io 4 --vm 2 --vm-bytes 128M --fork 4 --timeout 10s
```

Once you are happy with the loop you can run the script at startup to maintain your Raspberry at the proper temperature.
