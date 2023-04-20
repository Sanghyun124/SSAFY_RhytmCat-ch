#ifndef _GLOVE_DRIVER_H_
#define _GLOVE_DRIVER_H_
#include <linux/ioctl.h>
// TODO: 필요한 구조체, 상수 및 함수 선언을 작성하세요.
typedef struct
{
    int mcp3008_values[8];
    int sen10264_values[5];
    int mpu6050_accel[3];
    int mpu6050_gyro[3];
} glove_data_t;

#define IOCTL_READ_DATA _IOR('g', 1, glove_data_t *)
#endif
