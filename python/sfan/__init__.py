import smbus
import struct

# bus = smbus.SMBus(1)    # 0 = /dev/i2c-0 (port I2C0), 1 = /dev/i2c-1 (port I2C1)

DEVICE_ADDRESS = 0x03  # 7 bit address (will be left shifted to add the read write bit)


def setPower(stack, power):
    if stack < 0 or stack > 1:
        raise ValueError('Invalid stack level [0..1]')
    if power < 0 or power > 100:
        raise ValueError('Invalid power value [0..100]')
    POWER_ADDRESS = 0
    try:
        bus = smbus.SMBus(1)
        bus.write_byte_data(DEVICE_ADDRESS + stack, POWER_ADDRESS, power)
    except Exception as e:
        print(e)
        bus.close()
        return -1
    bus.close()
    return 1


def getPower(stack):
    if stack < 0 or stack > 1:
        raise ValueError('Invalid stack level [0..1]')
    POWER_ADDRESS = 0
    try:
        bus = smbus.SMBus(1)
        val = bus.read_byte_data(DEVICE_ADDRESS + stack, POWER_ADDRESS);
    except Exception as e:
        print(e)
        bus.close()
        return -1
    bus.close()
    return val


def setSafetyTemp(stack, temp):
    if stack < 0 or stack > 1:
        raise ValueError('Invalid stack level [0..1]')
    if temp < 30 or temp > 80:
        raise ValueError('Invalid safety temperature value [30..80]')
    SAFETY_TEMP_ADDRESS = 3
    try:
        bus = smbus.SMBus(1)
        bus.write_byte_data(DEVICE_ADDRESS + stack, SAFETY_TEMP_ADDRESS, temp)
    except Exception as e:
        print(e)
        bus.close()
        return -1
    bus.close()
    return 1


def getSafetyTemp(stack):
    if stack < 0 or stack > 1:
        raise ValueError('Invalid stack level [0..1]')
    SAFETY_TEMP_ADDRESS = 3
    try:
        bus = smbus.SMBus(1)
        val = bus.read_byte_data(DEVICE_ADDRESS + stack, SAFETY_TEMP_ADDRESS);
    except Exception as e:
        print(e)
        bus.close()
        return -1
    bus.close()
    return val


def getProcTemp(stack):
    if stack < 0 or stack > 1:
        raise ValueError('Invalid stack level [0..1]')
    TEMP_ADDRESS = 2
    try:
        bus = smbus.SMBus(1)
        val = bus.read_byte_data(DEVICE_ADDRESS + stack, TEMP_ADDRESS);
    except Exception as e:
        print(e)
        bus.close()
        return -1
    bus.close()
    return val


def setLedBlink(stack, blink):
    val = 0
    if stack < 0 or stack > 1:
        raise ValueError('Invalid stack level [0..1]')
    if blink == 0:
        val = 1
    BLINK_ADDRESS = 1
    try:
        bus = smbus.SMBus(1)
        bus.write_byte_data(DEVICE_ADDRESS + stack, BLINK_ADDRESS, val)
    except Exception as e:
        print(e)
        bus.close()
        return -1
    bus.close()
    return 1


def getLedBlink(stack):
    if stack < 0 or stack > 1:
        raise ValueError('Invalid stack level [0..1]')
    BLINK_ADDRESS = 1
    try:
        bus = smbus.SMBus(1)
        val = bus.read_byte_data(DEVICE_ADDRESS + stack, BLINK_ADDRESS);
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
