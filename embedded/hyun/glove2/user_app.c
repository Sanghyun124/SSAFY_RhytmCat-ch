#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include "mpu6050_driver.h"

#define DEVICE_FILE "/dev/mpu6050_device"

int main()
{
    int fd;
    struct mpu6050_data data;

    fd = open(DEVICE_FILE, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open device file");
        return 1;
    }

    if (read(fd, &data, sizeof(data)) != sizeof(data)) {
        perror("Failed to read data from device file");
        close(fd);
        return 1;
    }

    printf("Accel_X: %d, Accel_Y: %d, Accel_Z: %d\n", data.accel_x, data.accel_y, data.accel_z);

    close(fd);
    return 0;
}

