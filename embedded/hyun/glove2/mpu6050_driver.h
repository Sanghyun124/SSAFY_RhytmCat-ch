#ifndef _MPU6050_DRIVER_H_
#define _MPU6050_DRIVER_H_

//#include <linux/miscdevice.h>
//#include </usr/src/linux-headers-6.1.21-v7l+/include/linux/miscdevice.h>
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_PWR_MGMT_1 0x6B

struct mpu6050_data {
    int16_t accel_x;
    int16_t accel_y;
    int16_t accel_z;
};

#endif // _MPU6050_DRIVER_H_

