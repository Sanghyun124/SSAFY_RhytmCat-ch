#ifndef CHAR_CONFIG_H
#define CHAR_CONFIG_H

#include <linux/ioctl.h>

#define MAGIC_NUM 225


struct gyroData
{
 int16_t x;
 int16_t y;
 int16_t z;
};

struct accelData
{
 int16_t x;
 int16_t y;
 int16_t z;
};

//IOCTL interface prototypes

#define IOCTL_GYRO  _IOWR(MAGIC_NUM, 0, struct gyroData*)
#define IOCTL_ACCEL  _IOWR(MAGIC_NUM, 1, struct accelData*)



//Device file interface
#define DEVICE_FILE_NAME "/dev/MPU6050"

#endif
