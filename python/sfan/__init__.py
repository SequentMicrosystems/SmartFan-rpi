import smbus2 as smbus
import struct
import RPi.GPIO as GPIO

# bus = smbus.SMBus(1)    # 0 = /dev/i2c-0 (port I2C0), 1 = /dev/i2c-1 (port I2C1)

DEVICE_ADDRESS = 0x03  # 7 bit address (will be left shifted to add the read write bit)
DEVICE_ALT_ADDRESS = 0x2C


def checkCard(stack):
    if stack < 0 or stack > 1:
        raise ValueError('Invalid stack level [0..1]')
    bus = smbus.SMBus(1)
    I2C_MEM_REVISION_HW_MAJOR_ADD = 100
    address = DEVICE_ADDRESS + stack
    try:
        val = bus.read_byte_data(address, I2C_MEM_REVISION_HW_MAJOR_ADD)
    except:
        address = DEVICE_ALT_ADDRESS + (stack ^ 1)
        try:
            val = bus.read_byte(address)
        except Exception as e:
            print(e)
            address = -1
    bus.close()
    return address


def setPower(stack, power):
    if power < 0 or power > 100:
        raise ValueError('Invalid power value [0..100]')
    POWER_ADDRESS = 0
    address = checkCard(stack)
    if address < 0:
        return -1
    if address > 4:
        val = 255 - power * 2.55
        power = int(val)
        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)
        GPIO.setup(12, GPIO.OUT)
        if power < 255:
           GPIO.output(12, GPIO.HIGH)
        else:
            GPIO.output(12, GPIO.LOW)
    try:
        bus = smbus.SMBus(1)
        bus.write_byte_data(address, POWER_ADDRESS, power)
    except Exception as e:
        print(e)
        bus.close()
        return -1
    bus.close()
    return 1


def getPower(stack):
    address = checkCard(stack)
    if address < 0:
        return -1
    POWER_ADDRESS = 0
    try:
        bus = smbus.SMBus(1)
        if address > 4:
            val = bus.read_byte(address)
        else:
            val = bus.read_byte_data(address, POWER_ADDRESS)
    except Exception as e:
        print(e)
        bus.close()
        return -1
    bus.close()
    if address > 4:
        val = int((255 - val) / 2.55)
    return val


def setSafetyTemp(stack, temp):
    address = checkCard(stack)
    if address < 0:
        return -1
    if address > 4:
        raise ValueError('Not available for this hardware version')
    if temp < 30 or temp > 80:
        raise ValueError('Invalid safety temperature value [30..80]')
    SAFETY_TEMP_ADDRESS = 3
    try:
        bus = smbus.SMBus(1)
        bus.write_byte_data(address, SAFETY_TEMP_ADDRESS, temp)
    except Exception as e:
        print(e)
        bus.close()
        return -1
    bus.close()
    return 1


def getSafetyTemp(stack):
    address = checkCard(stack)
    if address < 0:
        return -1
    if address > 4:
        raise ValueError('Not available for this hardware version')
    SAFETY_TEMP_ADDRESS = 3
    try:
        bus = smbus.SMBus(1)
        val = bus.read_byte_data(address, SAFETY_TEMP_ADDRESS);
    except Exception as e:
        print(e)
        bus.close()
        return -1
    bus.close()
    return val


def getProcTemp(stack):
    address = checkCard(stack)
    if address < 0:
        return -1
    if address > 4:
        raise ValueError('Not available for this hardware version')
    TEMP_ADDRESS = 2
    try:
        bus = smbus.SMBus(1)
        val = bus.read_byte_data(address, TEMP_ADDRESS);
    except Exception as e:
        print(e)
        bus.close()
        return -1
    bus.close()
    return val


def setLedBlink(stack, blink):
    val = 0
    address = checkCard(stack)
    if address < 0:
        return -1
    if address > 4:
        raise ValueError('Not available for this hardware version')
    if blink == 0:
        val = 1
    BLINK_ADDRESS = 1
    try:
        bus = smbus.SMBus(1)
        bus.write_byte_data(address, BLINK_ADDRESS, val)
    except Exception as e:
        print(e)
        bus.close()
        return -1
    bus.close()
    return 1


def getLedBlink(stack):
    address = checkCard(stack)
    if address < 0:
        return -1
    if address > 4:
        raise ValueError('Not available for this hardware version')
    BLINK_ADDRESS = 1
    try:
        bus = smbus.SMBus(1)
        val = bus.read_byte_data(address, BLINK_ADDRESS);
        if val == 0:
            val = 1
        else:
            val = 0
    except Exception as e:
        print(e)
        bus.close()
        return -1
    bus.close()
    return val


def setStopInt(stack, seconds):
    address = checkCard(stack)
    if address < 0:
        return -1
    if address > 4:
        raise ValueError('Not available for this hardware version')
    I2C_MEM_TIME_TO_STOP_SET = 5
    try:
        bus = smbus.SMBus(1)
        bus.write_word_data(address, I2C_MEM_TIME_TO_STOP_SET, int(seconds))
    except Exception as e:
        print(e)
        bus.close()
        return -1
    bus.close()
    return 1


def getStopInt(stack):
    address = checkCard(stack)
    if address < 0:
        return -1
    if address > 4:
        raise ValueError('Not available for this hardware version')
    I2C_MEM_TIME_TO_STOP_REM = 7
    try:
        bus = smbus.SMBus(1)
        val = bus.read_word_data(address, I2C_MEM_TIME_TO_STOP_REM)
    except Exception as e:
        print(e)
        bus.close()
        return -1
    bus.close()
    return val
