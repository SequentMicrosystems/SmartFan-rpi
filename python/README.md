[![sfan](../images/sequent.jpg)](https://www.sequentmicrosystems.com)

# sfan

This is the python library to control the [Smart Fan Card for Raspberry Pi](https://sequentmicrosystems.com/products/smart-fan-hat-for-raspberry-pi).

## Install

```bash
~$ sudo apt-get update
~$ sudo apt-get install build-essential python-pip python-dev python-smbus git
~$ git clone https://github.com/SequentMicrosystems/SmartFan-rpi.git
~$ cd SmartFan-rpi/python/
~/SmartFan-rpi/python$ sudo python setup.py install
```
If you use python3.x :
```
~$ sudo apt-get update
~$ sudo apt-get install build-essential python3-pip python3-dev python3-smbus git
~$ git clone https://github.com/SequentMicrosystems/SmartFan-rpi.git
~$ cd SmartFan-rpi/python/
~/SmartFan-rpi/python$ sudo python3 setup.py install
```
## Update

```bash
~$ cd SmartFan-rpi/
~/SmartFan-rpi$ git pull
~$ cd SmartFan-rpi/python/
~/SmartFan-rpi/python$ sudo python setup.py install
```
If you use python3.x repace the last line with:
```
~/SmartFan-rpi/python$ sudo python3 setup.py install
```
## Usage 

Now you can import the sfan library and use its functions. To test, read processor temperature from the Smart Fan with stack level 0:

```bash
~$ python
Python 2.7.9 (default, Sep 17 2016, 20:26:04)
[GCC 4.9.2] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> import sfan
>>> sfan.getProcTemp(0)
31
>>>
```


## Functions

### setPower(stack, power)

Set the Smart Fan power

**stack** - stack level, set with jumper [0/1]

**power** - Fan power  [0..100]


### getPower(stack)

Return the fan power [0..100]

**stack** - stack level, set with jumper [0/1]


### setSafetyTemp(stack, temp):

Set the Smart Fan safety temperature. If fan processor temperature exceeds this value fan will run 100%

**stack** - stack level, set with jumper [0/1]

**tenp** - Safety temperature in degree Celsius  [30..80]


### getSafetyTemp(stack)

Return the fan safety temperature in degree Celsius [30..80]

**stack** - stack level, set with jumper [0/1]


### getProcTemp(stack)

Return the fan processor temperature in degree Celsius

**stack** - stack level, set with jumper [0/1]


### setLedBlink(stack, blink):

Turn the Smart Fan led blink on or off  

**stack** - stack level, set with jumper [0/1]

**blink** - Led blinking enable [0/1]


### getLedBlink(stack)

Return the fan led blinking [0/1]

**stack** - stack level, set with jumper [0/1]


### setStopInt(stack, seconds)

Set the stop interval in seconds. The fan will run 100% specified seconds then stop.

**stack** - stack level, set with jumper [0/1]

**seconds** - how long fan will run [1..65000]


### getStopInt(stack)

Return remaining time until fan will stop

**stack** - stack level, set with jumper [0/1]
