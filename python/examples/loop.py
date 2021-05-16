#!/usr/bin/python

import sys, getopt, time
import sfan as f
from gpiozero import CPUTemperature
import datetime


class PID:
    kp = 1
    ki = 0
    kd = 0
    iSum = 0
    iLimHi = 100
    iLimLo = 0
    lastErr = 0
    dTerm = 0
    outLimLo = 0
    outLimHi = 100

    def __init__(self, p, i, d):
        self.kp = p
        self.ki = i
        self.kd = d

    def update(self, err, dt):
        self.iSum += self.ki * err * dt;
        if self.iSum > self.iLimHi:
            self.iSum = self.iLimHi
        if self.iSum < self.iLimLo:
            self.iSum = self.iLimLo
        self.dTerm = (err - self.lastErr)/dt #optional filter might be added
        self.lastErr = err;
        out = self.ki * err + self.iSum + self.dTerm;
        if out > self.outLimHi:
            out = self.outLimHi
        if out < self.outLimLo:
            out = self.outLimLo
        return out

def WriteToLog(temperature, fanPower):
    f = open("SmartFan.csv", "a")
    now = datetime.datetime.now()
    f.write(now.strftime("%Y-%m-%d %H:%M:%S") + "," + str(temperature) + "," + str(fanPower) + "\n")
    f.close()

def main(argv):
    setTemp = 100
    verb = False
    log = False
    logInt = 60    
    try:
        opts, args = getopt.getopt(argv, "hl:t:v", ["help", "temp=", "verbose", "log="])
    except getopt.error as err:
        # Output error, and return with an error code
        print(str(err))
        sys.exit(2)
    for opt, arg in opts:
        if opt in ("-h", "--help"):
            print('loop.py -t <temperature>  Maintain CPU temperature at set level (deg C)')
            sys.exit()
        elif opt in ("-v", "--verbose"):
            verb = True
        elif opt in ("-t", "--temp"):
            setTemp = int(arg)
        elif opt in ("-l", "--log"):
            log = True
            if len(arg) > 0:
                logInt = int(arg)
    if setTemp < 30 or setTemp > 80:
        print('Invalid set temperature must be between 30 and 80 deg C!')
        sys.exit(3)
    cpu = CPUTemperature()
    fanPid = PID(10, .8, .1)
    logCount = 0
    while(1):
        time.sleep(1)
        t = cpu.temperature
        out = int(fanPid.update(t-setTemp, 1))
        f.setPower(0, out)
        if verb:
            #print("CPU temperature:" + str(int(t)) + " FAN power: " + str(int(out)))
            sys.stdout.write("CPU temperature: %d   FAN power: %d%% \r" % (int(t), int(out)))
            sys.stdout.flush()
        if log:
            logCount+= 1
            if logCount >= logInt:
                logCount = 0
                WriteToLog(t, out)    

if __name__ == "__main__":
    main(sys.argv[1:])
