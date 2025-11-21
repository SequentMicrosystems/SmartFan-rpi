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
#include <errno.h>
#include <string.h>

#ifndef	CONSUMER
#define	CONSUMER	"Consumer"
#endif


int GPIOWrite(int pin, int value)
{
	int ret = -1;

#if defined(GPIOD_V2_API)  /* compile this only when you detect libgpiod v2 headers */
#ifdef RPI5
    const char *chip_device = "/dev/gpiochip4";
#else
    const char *chip_device = "/dev/gpiochip0";
#endif

    struct gpiod_chip *chip = NULL;
      struct gpiod_line_settings *settings = NULL;
      struct gpiod_line_config *line_cfg = NULL;
      struct gpiod_request_config *req_cfg = NULL;
      struct gpiod_line_request *request = NULL;

      unsigned int offsets[1] = { (unsigned int)pin };
      enum gpiod_line_value lv = value ? GPIOD_LINE_VALUE_ACTIVE : GPIOD_LINE_VALUE_INACTIVE;

      /* Open chip by device path (gpiod_chip_open_by_name is not present in your header) */
      chip = gpiod_chip_open(chip_device);
      if (!chip) {
          fprintf(stderr, "Open chip %s failed: %s\n", chip_device, strerror(errno));
          goto cleanup;
      }

      settings = gpiod_line_settings_new();
      if (!settings) {
          fprintf(stderr, "Failed to create line settings: %s\n", strerror(errno));
          goto cleanup;
      }

      if (gpiod_line_settings_set_direction(settings, GPIOD_LINE_DIRECTION_OUTPUT) < 0) {
          fprintf(stderr, "Failed to set line direction: %s\n", strerror(errno));
          goto cleanup;
      }

      if (gpiod_line_settings_set_output_value(settings, lv) < 0) {
          fprintf(stderr, "Failed to set initial output value in settings: %s\n", strerror(errno));
          goto cleanup;
      }

      line_cfg = gpiod_line_config_new();
      if (!line_cfg) {
          fprintf(stderr, "Failed to create line config: %s\n", strerror(errno));
          goto cleanup;
      }

      if (gpiod_line_config_add_line_settings(line_cfg, offsets, 1, settings) < 0) {
          fprintf(stderr, "Failed to add line settings for offset %u: %s\n", offsets[0], strerror(errno));
          goto cleanup;
      }

      req_cfg = gpiod_request_config_new();
      if (!req_cfg) {
          fprintf(stderr, "Failed to create request config: %s\n", strerror(errno));
          goto cleanup;
      }
      gpiod_request_config_set_consumer(req_cfg, "myconsumer");

      request = gpiod_chip_request_lines(chip, req_cfg, line_cfg);
      if (!request) {
          fprintf(stderr, "Request lines failed for offset %u on %s: %s\n", offsets[0], chip_device, strerror(errno));
          goto cleanup;
      }

      /* Ensure the requested line has the expected value; set it explicitly */
      if (gpiod_line_request_set_value(request, offsets[0], lv) < 0) {
          fprintf(stderr, "Set value failed for offset %u: %s\n", offsets[0], strerror(errno));
          goto cleanup;
      }

      ret = 0; /* success */

  cleanup:
      if (request) gpiod_line_request_release(request);
      if (req_cfg) gpiod_request_config_free(req_cfg);
      if (line_cfg) gpiod_line_config_free(line_cfg);
      if (settings) gpiod_line_settings_free(settings);
      if (chip) gpiod_chip_close(chip);

#else /* GPIOD v1 */
#ifdef RPI5
    const char *chipname = "gpiochip4";
#else
	const char *chipname = "gpiochip0";
#endif

	struct gpiod_chip *chip_v1 = NULL;
	struct gpiod_line *line = NULL;

	chip_v1 = gpiod_chip_open_by_name(chipname);
	if (!chip_v1)
	{
		perror("V1: Open chip failed");
		return -1;
	}

	line = gpiod_chip_get_line(chip_v1, pin);
	if (!line)
	{
		perror("V1: Get line failed");
		gpiod_chip_close(chip_v1);
		return -1;
	}

	ret = gpiod_line_request_output(line, "myconsumer", value ? 1 : 0);
	if (ret < 0)
	{
		perror("V1: Request line as output failed");
		gpiod_line_release(line);
		gpiod_chip_close(chip_v1);
		return -1;
	}

	ret = gpiod_line_set_value(line, value ? 1 : 0);
	if (ret < 0)
	{
		perror("V1: Set line output failed");
	}
	else
	{
		ret = 0;
	}

	gpiod_line_release(line);
	gpiod_chip_close(chip_v1);

#endif

	return ret;
}
