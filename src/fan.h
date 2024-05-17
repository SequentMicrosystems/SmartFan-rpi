#ifndef SMARTFAN_H_
#define SMARTFAN_H_

#include <stdint.h>

#define RETRY_TIMES	10
typedef enum
{
	I2C_MEM_FAN_POWER = 0,
	I2C_MEM_BLINK_OFF,
	I2C_MEM_INT_TEMP,
	I2C_MEM_INT_TEMP_FS,
	I2C_MEM_TEMP_SAFETY,
	I2C_MEM_TIME_TO_STOP_SET, //2bytes
	I2C_MEM_TIME_TO_STOP_REM = I2C_MEM_TIME_TO_STOP_SET + 2,
	I2C_MEM_BUTTON_STATE = I2C_MEM_TIME_TO_STOP_REM + 2,
	I2C_MEM_BUTTON_LATCH,
	I2C_MEM_BUTTON_INT_DISABLE,

	I2C_MEM_REVISION_HW_MAJOR_ADD = 100,
	I2C_MEM_REVISION_HW_MINOR_ADD,
	I2C_MEM_REVISION_MAJOR_ADD,
	I2C_MEM_REVISION_MINOR_ADD,

	SLAVE_BUFF_SIZE,
} I2C_MEM_ADD;

#define FAN_PWM_MAX	100
#define FAN_PWM_MIN	0

#define SAFETY_TEMP_MIN	30
#define SAFETY_TEMP_MAX 80

#define STACK_MIN	0
#define STACK_MAX 1

#define STOP_INT_MIN	1
#define STOP_INT_MAX 65000

#define ERROR	-1
#define OK		0
#define FAIL	-1
#define ARG_ERR -2
#define ARG_CNT_ERR -3

#define SLAVE_OWN_ADDRESS_BASE 0x03
#define SLAVE_ALT_ADDRESS_BASE 0x2C

#define FAN_V4_ENABLE_PIN 12

typedef uint8_t u8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;
typedef int32_t s32;

typedef enum
{
	OFF = 0,
	ON,
	STATE_COUNT
} OutStateEnumType;

int doBoardInit(int stack);

#endif //SMARTFAN_H_
