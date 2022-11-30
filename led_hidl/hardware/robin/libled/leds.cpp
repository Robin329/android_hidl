/*
 * Copyright (C) 2008 The Android Open Source Project
 * Copyright (C) 2014 The  Linux Foundation. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_NDEBUG 1
#define LOG_TAG "hw-leds"

#include <cutils/log.h>
#include <errno.h>
#include <fcntl.h>
#include <hardware/leds.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>

/******************************************************************************/

static pthread_once_t g_init = PTHREAD_ONCE_INIT;
static pthread_mutex_t g_lock = PTHREAD_MUTEX_INITIALIZER;
static struct led_state_t gUserLedStates;
static struct led_state_t gPowerLedStates;
char const* const USER_LED_FILE =
    "/sys/devices/platform/leds/leds/firefly:yellow:user/brightness";

char const* const POWER_LED_FILE =
    "/sys/devices/platform/leds/leds/firefly:blue:power/brightness";

/**
 * device methods
 */

void init_globals(void) {
  // init the mutex
  pthread_mutex_init(&g_lock, NULL);
}

static int write_int(char const* path, int value) {
  int fd;
  static int already_warned = 0;

  fd = open(path, O_RDWR);
  if (fd >= 0) {
    char buffer[20];
    int bytes = snprintf(buffer, sizeof(buffer), "%d\n", value);
    ssize_t amt = write(fd, buffer, (size_t)bytes);
    close(fd);
    return amt == -1 ? -errno : 0;
  } else {
    if (already_warned == 0) {
      ALOGE("write_int failed to open %s\n", path);
      already_warned = 1;
    }
    return -errno;
  }
}

static int set_led_power(struct led_device_t* dev,
                         struct led_state_t const* state) {
  int err = 0;
  if (!dev || !state) {
    return -1;
  }
  pthread_mutex_lock(&g_lock);
  err = write_int(POWER_LED_FILE, state->brightness);
  if (!err) gPowerLedStates.brightness = state->brightness;
  pthread_mutex_unlock(&g_lock);
  return err;
}

static unsigned int get_led_power(struct led_device_t* dev) {
  if (!dev) return -1;
  return gPowerLedStates.brightness;
}

static int set_led_user(struct led_device_t* dev,
                        struct led_state_t const* state) {
  int err = 0;
  if (!dev || !state) {
    return -1;
  }
  pthread_mutex_lock(&g_lock);
  err = write_int(USER_LED_FILE, state->brightness);
  if (!err) gUserLedStates.brightness = state->brightness;
  pthread_mutex_unlock(&g_lock);
  return err;
}
static unsigned int get_led_user(struct led_device_t* dev) {
  if (!dev) return -1;
  return gUserLedStates.brightness;
}

static int close_led(struct led_device_t* dev) {
  if (dev) free(dev);
  return 0;
}

static int open_leds(const struct hw_module_t* module, const char* name,
                     struct hw_device_t** device) {
  int (*setBrightnessValue)(struct led_device_t * dev,
                            struct led_state_t const* state);
  unsigned int (*getBrightnessRange)(struct led_device_t * dev);
  if (!strcmp(LED_ID_POWER, name)) {
    setBrightnessValue = set_led_power;
    getBrightnessRange = get_led_power;
    if ((access(POWER_LED_FILE, F_OK) < 0)) {
      ALOGE("access power led file failed(%d)!", errno);
      return -ENOSYS;
    }
  } else if (!strcmp(LED_ID_USER, name)) {
    setBrightnessValue = set_led_user;
    getBrightnessRange = get_led_user;
    if ((access(USER_LED_FILE, F_OK) < 0)) {
      ALOGE("access user led file failed!(%d)!", errno);
      return -ENOSYS;
    }
  }
  pthread_once(&g_init, init_globals);
  struct led_device_t* dev =
      (struct led_device_t*)malloc(sizeof(struct led_device_t));
  if (!dev) {
    return -ENOSYS;
  }
  memset(dev, 0, sizeof(*dev));
  dev->common.tag = HARDWARE_DEVICE_TAG;
  dev->common.version = 0;
  dev->common.module = (struct hw_module_t*)module;
  dev->common.close = (int (*)(struct hw_device_t*))close_led;
  dev->setBrightnessValue = setBrightnessValue;
  dev->getBrightnessRange = getBrightnessRange;

  *device = (struct hw_device_t*)dev;
  return 0;
}

static struct hw_module_methods_t leds_module_methods = {
    .open = open_leds,
};

/*
 * The leds Module
 */
struct hw_module_t HAL_MODULE_INFO_SYM = {
    .tag = HARDWARE_MODULE_TAG,
    .version_major = 1,
    .version_minor = 0,
    .id = LEDS_HARDWARE_MODULE_ID,
    .name = "Leds Module",
    .author = "Rockchip, Inc.",
    .methods = &leds_module_methods,
};
