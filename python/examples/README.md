# loop

This is a usage example of the Smart fan and his python library.
The example implements a PID loop that maintains the raspberry temperature by controlling the fan power.

## Usage 

The script must be called with temperature parameter -t <value>. The parameter -v makes the script to report the Raspberry CPU temperature and fan power while running.

## Example

The command
```bash
$ python loop.py -t 50 -v
```
keep Raspberry temperature at 50 degrees Celsius.

## Future work

The loop.py implement a basic PID loop as a Class, the constructor set the 3 coefficients: Kp, Ki, Kd (proportional, integral, and derivative) feel free to tune the loop by editing the following line:
```bash
fanPid = PID(10, .8, .1)
```
Once you are happy with the loop you can run the script at startup to maintain your Raspberry at the proper temperature.
