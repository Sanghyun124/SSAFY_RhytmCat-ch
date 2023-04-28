#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <time.h>
#include "mpu6050_reg.h"
int main() {
    int fd;
    char data[14];
    int16_t ax, ay, az, gx, gy, gz;
    struct timespec sleep_time;

    fd = open("/dev/mpu6050_device", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open mpu6050_device");
        return 1;
    }

    sleep_time.tv_sec = 0;
    sleep_time.tv_nsec = 100000000; // 100 ms

    while (1) {
        if (read(fd, data, 14) != 14) {
            perror("Failed to read data");
            close(fd);
            return 1;
        }

        ax = (int16_t)((data[0] << 8) | data[1]);
        ay = (int16_t)((data[2] << 8) | data[3]);
        az = (int16_t)((data[4] << 8) | data[5]);
        gx = (int16_t)((data[8] << 8) | data[9]);
        gy = (int16_t)((data[10] << 8) | data[11]);
        gz = (int16_t)((data[12] << 8) | data[13]);

        printf("Acceleration: X = %d, Y = %d, Z = %d\n", ax, ay, az);
        printf("Gyroscope: X = %d, Y = %d, Z = %d\n", gx, gy, gz);

        nanosleep(&sleep_time, NULL);
    }

    close(fd);

    return 0;
}

