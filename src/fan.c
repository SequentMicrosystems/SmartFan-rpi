/*
 * ioplus.c:
 *	Command-line interface to the Raspberry
 *	Pi's IOPLUS card.
 *	Copyright (c) 2016-2020 Sequent Microsystem
 *	<http://www.sequentmicrosystem.com>
 ***********************************************************************
 *	Author: Alexandru Burcea
 ***********************************************************************
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "fan.h"
#include "comm.h"
#include "cli.h"

#define VERSION_BASE	(int)1
#define VERSION_MAJOR	(int)0
#define VERSION_MINOR	(int)0

#define UNUSED(X) (void)X      /* To avoid gcc/g++ warnings */

char *warranty =
	"	       Copyright (c) 2016-2021 Sequent Microsystems\n"
		"                                                             \n"
		"		This program is free software; you can redistribute it and/or modify\n"
		"		it under the terms of the GNU Leser General Public License as published\n"
		"		by the Free Software Foundation, either version 3 of the License, or\n"
		"		(at your option) any later version.\n"
		"                                    \n"
		"		This program is distributed in the hope that it will be useful,\n"
		"		but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
		"		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
		"		GNU Lesser General Public License for more details.\n"
		"			\n"
		"		You should have received a copy of the GNU Lesser General Public License\n"
		"		along with this program. If not, see <http://www.gnu.org/licenses/>.";

void usage(void)
{
	int i = 0;
	while (gCmdArray[i] != NULL)
	{
		if (gCmdArray[i]->name != NULL)
		{
			if (strlen(gCmdArray[i]->usage1) > 2)
			{
				printf("%s", gCmdArray[i]->usage1);
			}
			if (strlen(gCmdArray[i]->usage2) > 2)
			{
				printf("%s", gCmdArray[i]->usage2);
			}
		}
		i++;
	}
	printf("Where: <id> = Board level id = 0..1\n");
	printf("Type fan -h <command> for more help\n");
}

int doBoardInit(int stack)
{
	int dev = 0;
	int add = 0;
	uint8_t buff[8];

	if ( (stack < STACK_MIN) || (stack > STACK_MAX))
	{
		printf("Invalid stack level [0..1]!");
		return ERROR;
	}
	add = stack + SLAVE_OWN_ADDRESS_BASE;
	dev = i2cSetup(add);
	if (dev == -1)
	{
		return ERROR;
	}
	if (ERROR == i2cMem8Read(dev, I2C_MEM_REVISION_MAJOR_ADD, buff, 1))
	{
		printf("Smart Fan id %d not detected\n", stack);
		return ERROR;
	}
	return dev;
}

int boardCheck(int stack)
{
	int dev = 0;
	int add = 0;
	uint8_t buff[8];

	if ( (stack < STACK_MIN) || (stack > STACK_MAX))
	{
		printf("Invalid stack level [0..1]!");
		return ERROR;
	}
	add = stack + SLAVE_OWN_ADDRESS_BASE;
	dev = i2cSetup(add);
	if (dev == -1)
	{
		return ERROR;
	}
	if (ERROR == i2cMem8Read(dev, I2C_MEM_REVISION_MAJOR_ADD, buff, 1))
	{

		return ERROR;
	}
	return OK;
}
int doHelp(int argc, char *argv[]);
const CliCmdType CMD_HELP =
{
	"-h",
	1,
	&doHelp,
	"\t-h		Display the list of commands, or details about each command\n",
	"\tUsage:		fan -h    Display commands list\n",
	"\tUsage:		fan -h <param>   Display help for <param> command \n",
	"\tExample:	fan -h pwr    Display help for \"pwr\" command \n"};

int doHelp(int argc, char *argv[])
{
	int i = 0;
	if (argc == 3)
	{
		while (NULL != gCmdArray[i])
		{
			if (gCmdArray[i]->name != NULL)
			{
				if (strcasecmp(argv[2], gCmdArray[i]->name) == 0)
				{
					printf("%s%s%s%s", gCmdArray[i]->help, gCmdArray[i]->usage1,
						gCmdArray[i]->usage2, gCmdArray[i]->example);
					break;
				}
			}
			i++;
		}
		if (NULL == gCmdArray[i])
		{
			printf("Option \"%s\" not found\n", argv[2]);
			i = 0;
			while (NULL != gCmdArray[i])
			{
				if (gCmdArray[i]->name != NULL)
				{
					printf("%s", gCmdArray[i]->help);
					break;
				}
				i++;
			}
		}
	}
	else
	{
		i = 0;
		while (NULL != gCmdArray[i])
		{
			if (gCmdArray[i]->name != NULL)
			{
				printf("%s", gCmdArray[i]->help);
			}
			i++;
		}
	}
	return OK;
}

int doVersion(int argc, char *argv[]);
const CliCmdType CMD_VERSION =
{
	"-v",
	1,
	&doVersion,
	"\t-v		Display the fan command version number\n",
	"\tUsage:		fan -v\n",
	"",
	"\tExample:		fan -v  Display the version number\n"};

int doVersion(int argc, char *argv[])
{
	UNUSED(argc);
	UNUSED(argv);
	printf("fan v%d.%d.%d Copyright (c) 2016 - 2021 Sequent Microsystems\n",
	VERSION_BASE, VERSION_MAJOR, VERSION_MINOR);
	printf("\nThis is free software with ABSOLUTELY NO WARRANTY.\n");
	printf("For details type: fan -warranty\n");
	return OK;
}

int doWarranty(int argc, char *argv[]);
const CliCmdType CMD_WAR =
{
	"-warranty",
	1,
	&doWarranty,
	"\t-warranty	Display the warranty\n",
	"\tUsage:		fan -warranty\n",
	"",
	"\tExample:		fan -warranty  Display the warranty text\n"};

int doWarranty(int argc, char *argv[])
{
	UNUSED(argc);
	UNUSED(argv);
	printf("%s\n", warranty);
	return OK;
}

int doList(int argc, char *argv[]);
const CliCmdType CMD_LIST =
	{
		"-list",
		1,
		&doList,
		"\t-list:		List all fan boards connected, return the # of boards and stack level for every board\n",
		"\tUsage:		fan -list\n",
		"",
		"\tExample:		fan -list display: 1,0 \n"};

int doList(int argc, char *argv[])
{
	int ids[8];
	int i;
	int cnt = 0;

	UNUSED(argc);
	UNUSED(argv);

	for (i = 0; i < 2; i++)
	{
		if (boardCheck(i) == OK)
		{
			ids[cnt] = i;
			cnt++;
		}
	}
	printf("%d board(s) detected\n", cnt);
	if (cnt > 0)
	{
		printf("Id:");
	}
	while (cnt > 0)
	{
		cnt--;
		printf(" %d", ids[cnt]);
	}
	printf("\n");
	return OK;
}

int doBoard(int argc, char *argv[]);
const CliCmdType CMD_BOARD =
{
	"board",
	2,
	&doBoard,
	"\tboard		Display the board status and firmware version number\n",
	"\tUsage:		fan <id> board\n",
	"",
	"\tExample:		fan 0 board   Display firmware version \n"};

int doBoard(int argc, char *argv[])
{
	int dev = -1;
	u8 buff[4];
	int resp = 0;

	if (argc != 3)
	{
		printf("Invalid arguments number type \"fan -h\" for details\n");
		exit(1);
	}
	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	resp = i2cMem8Read(dev, I2C_MEM_REVISION_HW_MAJOR_ADD, buff, 4);
	if (FAIL == resp)
	{
		printf("Fail to read board info!\n");
		exit(1);
	}
	printf("Hardware %02d.%02d, Firmware %02d.%02d \n", (int)buff[0],
		(int)buff[1], (int)buff[2], (int)buff[3]);
	return OK;
}

int fanPwSet(int dev, int power)
{
	u8 buff = 0;
	int resp = 0;

	if ( (power < FAN_PWM_MIN) || (power > FAN_PWM_MAX))
	{
		printf("Invalid fan power value! [0..100]\n");
		return ERROR;
	}
	buff = (u8)power;
	resp = i2cMem8Write(dev, I2C_MEM_FAN_POWER, &buff, 1);
	return resp;
}

int fanPwGet(int dev, int* power)
{
	u8 buff = 0;
	int resp = OK;

	if (FAIL == i2cMem8Read(dev, I2C_MEM_FAN_POWER, &buff, 1))
	{
		return FAIL;
	}
	*power = buff;

	return resp;
}

int fanTempGet(int dev, int* temperature)
{
	u8 buff = 0;
	int resp = OK;

	if (FAIL == i2cMem8Read(dev, I2C_MEM_INT_TEMP, &buff, 1))
	{
		return FAIL;
	}
	*temperature = buff;

	return resp;
}

int fanSafeTempGet(int dev, int* temperature)
{
	u8 buff = 0;
	int resp = OK;

	if (FAIL == i2cMem8Read(dev, I2C_MEM_INT_TEMP_FS, &buff, 1))
	{
		return FAIL;
	}
	*temperature = buff;

	return resp;
}

int fanSafeTempSet(int dev, int temperature)
{
	u8 buff = 0;

	if ( (temperature < SAFETY_TEMP_MIN) || (temperature > SAFETY_TEMP_MAX))
	{
		printf("Invalid safety temperature! [30..80]");
		return ERROR;
	}
	buff = (u8)temperature;

	return i2cMem8Write(dev, I2C_MEM_INT_TEMP_FS, &buff, 1);
}

int fanBlinkSet(int dev, int blink)
{
	u8 buff = 0;

	if (blink == 0) //reverse logic
	{
		buff = 1;
	}
	return i2cMem8Write(dev, I2C_MEM_BLINK_OFF, &buff, 1);
}

int fanBlinkGet(int dev, int* blink)
{
	u8 buff = 0;
	int ret = OK;

	ret = i2cMem8Read(dev, I2C_MEM_BLINK_OFF, &buff, 1);
	if (OK == ret)
	{
		*blink = 1;
		if (buff != 0)
		{
			*blink = 0;
		}
	}
	return ret;
}




int doSetPower(int argc, char *argv[]);
const CliCmdType CMD_POWER_WRITE =
{
	"pwr",
	2,
	&doSetPower,
	"\tpwr:		Set fan power 0..100\% \n",
	"\tUsage:		fan <id> pwr <power>\n",
	"",
	"\tExample:	fan 0 pwr 20   Set fan id #0 to 20\% \n"};

int doSetPower(int argc, char *argv[])
{
	int dev = 0;
	int power = 0;

	if ( (argc != 4))
	{
		printf("%s", CMD_POWER_WRITE.usage1);
		exit(1);
	}
	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	power = atoi(argv[3]);
	if (OK != fanPwSet(dev, power))
	{
		printf("Fail to communicate with the card.\n");
		exit(1);
	}
	return OK;
}

int doGetPower(int argc, char *argv[]);
const CliCmdType CMD_POWER_READ =
{
	"prd",
	2,
	&doGetPower,
	"\tprd:		Get fan power\n",
	"\tUsage:		fan <id> prd\n",
	"",
	"\tExample:	fan 0 prd   Get fan id #0 power \n"};

int doGetPower(int argc, char *argv[])
{
	int dev = 0;
	int power = 0;

	if ( (argc != 3))
	{
		printf("%s", CMD_POWER_READ.usage1);
		exit(1);
	}
	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (OK != fanPwGet(dev, &power))
	{
		printf("Fail to communicate with the card.\n");
		exit(1);
	}
	printf("%d\n", power);
	return OK;
}

int doSetSafetyTemp(int argc, char *argv[]);
const CliCmdType CMD_SAFETY_TEMP_WRITE =
	{
		"stwr",
		2,
		&doSetSafetyTemp,
		"\tstwr:		Set fan safety temperature threshold. If fan processor temperature exceeds this value fan will run 100\% \n",
		"\tUsage:		fan <id> stwr <temperature>\n",
		"",
		"\tExample:	fan 0 stwr 60  Set fan id #0 safety temperature to 60 deg C\n"};

int doSetSafetyTemp(int argc, char *argv[])
{
	int dev = 0;
	int temp = 0;

	if ( (argc != 4))
	{
		printf("%s", CMD_SAFETY_TEMP_WRITE.usage1);
		exit(1);
	}
	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	temp = atoi(argv[3]);
	if (OK != fanSafeTempSet(dev, temp))
	{
		printf("Fail to communicate with the card.\n");
		exit(1);
	}
	return OK;
}

int doGetSafetyTemp(int argc, char *argv[]);
const CliCmdType CMD_SAFETY_TEMP_READ =
	{
		"strd",
		2,
		&doGetSafetyTemp,
		"\tstrd:		Get fan safety temperature in deg C. If fan processor temperature exceeds this value fan will run 100\% \n",
		"\tUsage:		fan <id> strd\n",
		"",
		"\tExample:	fan 0 strd  Get fan id #0 safety temperature \n"};

int doGetSafetyTemp(int argc, char *argv[])
{
	int dev = 0;
	int temp = 0;

	if ( (argc != 3))
	{
		printf("%s", CMD_SAFETY_TEMP_READ.usage1);
		exit(1);
	}
	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (OK != fanSafeTempGet(dev, &temp))
	{
		printf("Fail to communicate with the card.\n");
		exit(1);
	}
	printf("%d\n", temp);
	return OK;
}

int doGetTemp(int argc, char *argv[]);
const CliCmdType CMD_TEMP_READ =
{
	"trd",
	2,
	&doGetTemp,
	"\ttrd:		Get fan processor internal temperature in deg C\n",
	"\tUsage:		fan <id> trd\n",
	"",
	"\tExample:	fan 0 trd -> Get fan id #0 temperature \n"};

int doGetTemp(int argc, char *argv[])
{
	int dev = 0;
	int temp = 0;

	if ( (argc != 3))
	{
		printf("%s", CMD_TEMP_READ.usage1);
		exit(1);
	}
	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (OK != fanTempGet(dev, &temp))
	{
		printf("Fail to communicate with the card.\n");
		exit(1);
	}
	printf("%d\n", temp);
	return OK;
}

int doSetBlink(int argc, char *argv[]);
const CliCmdType CMD_BLINK_WRITE =
{
	"bwr",
	2,
	&doSetBlink,
	"\tbwr:		Turn fan led blinking on or off \n",
	"\tUsage:		fan <id> bwr <blink>\n",
	"",
	"\tExample:	fan 0 bwr 0  Turn fan #0 led blinking off \n"};

int doSetBlink(int argc, char *argv[])
{
	int dev = 0;
	int blink = 0;

	if ( (argc != 4))
	{
		printf("%s", CMD_BLINK_WRITE.usage1);
		exit(1);
	}
	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	blink = atoi(argv[3]);
	if (OK != fanBlinkSet(dev, blink))
	{
		printf("Fail to communicate with the card.\n");
		exit(1);
	}
	return OK;
}

int doGetBlink(int argc, char *argv[]);
const CliCmdType CMD_BLINK_READ =
{
	"brd",
	2,
	&doGetBlink,
	"\tbrd:		Get fan led blinking status (on/off)\n",
	"\tUsage:		fan <id> brd\n",
	"",
	"\tExample:	fan 0 brd  Get fan id #0 led blinking status  \n"};

int doGetBlink(int argc, char *argv[])
{
	int dev = 0;
	int blink = 0;

	if ( (argc != 3))
	{
		printf("%s", CMD_BLINK_READ.usage1);
		exit(1);
	}
	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (OK != fanBlinkGet(dev, &blink))
	{
		printf("Fail to communicate with the card.\n");
		exit(1);
	}
	printf("%d\n", blink);
	return OK;
}

int fanStopRemainingGet(int dev, int* seconds)
{
	u8 buff[2];
	u16 aux16 = 0;
	int resp = OK;

	if (FAIL == i2cMem8Read(dev, I2C_MEM_TIME_TO_STOP_REM, buff, 2))
	{
		return FAIL;
	}
	memcpy(&aux16, buff, 2);
	*seconds = aux16;

	return resp;
}

int fanStopTimeSet(int dev, int seconds)
{
	u8 buff[2];
	u16 aux16 = 0;

	if ( (seconds < STOP_INT_MIN) || (seconds > STOP_INT_MAX))
	{
		printf("Invalid stop interval! [1..65000]");
		return ERROR;
	}
	aux16 = (u16)seconds;
	memcpy(buff, &aux16, 2);

	return i2cMem8Write(dev, I2C_MEM_TIME_TO_STOP_SET, buff, 2);
}
int doSetStop(int argc, char *argv[]);
const CliCmdType CMD_STOP_WRITE =
	{
		"stopwr",
		2,
		&doSetStop,
		"\tstopwr:		Run for specified sec. 100%, then stop \n",
		"\tUsage:		fan <id> stopwr <time>\n",
		"",
		"\tExample:	fan 0 stopwr 60  Set fan to run for one minute full power, then stop \n"};

int doSetStop(int argc, char *argv[])
{
	int dev = 0;
	int time = 0;

	if ( (argc != 4))
	{
		printf("%s", CMD_STOP_WRITE.usage1);
		exit(1);
	}
	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	time = atoi(argv[3]);
	if (OK != fanStopTimeSet(dev, time))
	{
		printf("Fail to communicate with the card.\n");
		exit(1);
	}
	return OK;
}

int doGetStopRemaining(int argc, char *argv[]);
const CliCmdType CMD_STOP_REMAINING_READ =
	{
		"stoprd",
		2,
		&doGetStopRemaining,
		"\tstoprd:		Get the time in sec. until fan will stop \n",
		"\tUsage:		fan <id> stoprd\n",
		"",
		"\tExample:	fan 0 stoprd  Get fan id #0 time in sec. until stop \n"};

int doGetStopRemaining(int argc, char *argv[])
{
	int dev = 0;
	int temp = 0;

	if ( (argc != 3))
	{
		printf("%s", CMD_STOP_REMAINING_READ.usage1);
		exit(1);
	}
	dev = doBoardInit(atoi(argv[1]));
	if (dev <= 0)
	{
		exit(1);
	}

	if (OK != fanStopRemainingGet(dev, &temp))
	{
		printf("Fail to communicate with the card.\n");
		exit(1);
	}
	printf("%d\n", temp);
	return OK;
}
const CliCmdType *gCmdArray[] =
{
	&CMD_VERSION,
	&CMD_HELP,
	&CMD_WAR,
	&CMD_LIST,
	&CMD_BOARD,
	&CMD_POWER_READ,
	&CMD_POWER_WRITE,
	&CMD_SAFETY_TEMP_READ,
	&CMD_SAFETY_TEMP_WRITE,
	&CMD_TEMP_READ,
	&CMD_BLINK_WRITE,
	&CMD_BLINK_READ,
	&CMD_STOP_WRITE,
	&CMD_STOP_REMAINING_READ,

	NULL}; //null terminated array of cli structure pointers

int main(int argc, char *argv[])
{
	int i = 0;
	int ret = OK;

	if (argc == 1)
	{
		usage();
		return -1;
	}
	while (NULL != gCmdArray[i])
	{
		if ( (gCmdArray[i]->name != NULL) && (gCmdArray[i]->namePos < argc))
		{
			if (strcasecmp(argv[gCmdArray[i]->namePos], gCmdArray[i]->name) == 0)
			{
				ret = gCmdArray[i]->pFunc(argc, argv);
				if (ret == ARG_CNT_ERR)
				{
					printf("Invalid parameters number!\n");
					printf("%s", gCmdArray[i]->usage1);
					if (strlen(gCmdArray[i]->usage2) > 2)
					{
						printf("%s", gCmdArray[i]->usage2);
					}
				}
				return ret;
			}
		}
		i++;
	}
	printf("Invalid command option\n");
	usage();

	return -1;
}
