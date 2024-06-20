/* gpio.c
 * Based on
 * Raspberry Pi GPIO example using sysfs interface.
 * Guillermo A. Amaral B. <g@maral.me>
 *
 * This file provide functions to control Raspberry GPIO pins.
 */

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <gpiod.h>

#ifndef	CONSUMER
#define	CONSUMER	"Consumer"
#endif

int GPIOWrite(int pin, int value)
{
	#ifdef RPI5
	char *chipname = "gpiochip4";
	#else
	char *chipname = "gpiochip0";
	#endif
	struct gpiod_chip *chip;
	struct gpiod_line *line;
	int ret;
	chip = gpiod_chip_open_by_name(chipname);
	if (!chip)
	{
		perror("Open chip failed\n");
		return -1;
	}

	line = gpiod_chip_get_line(chip, pin);
	if (!line)
	{
		perror("Get line failed\n");
		gpiod_chip_close(chip);
		return -1;
	}

	ret = gpiod_line_request_output(line, CONSUMER, 0);
	if (ret < 0)
	{
		perror("Request line as output failed\n");
		gpiod_line_release(line);
		gpiod_chip_close(chip);
		return -1;
	}
	ret = gpiod_line_set_value(line, value);
	if (ret < 0)
	{
		perror("Set line output failed\n");
		gpiod_line_release(line);
		gpiod_chip_close(chip);
		return -1;
	}

	gpiod_line_release(line);
	gpiod_chip_close(chip);
	return (0);
}
