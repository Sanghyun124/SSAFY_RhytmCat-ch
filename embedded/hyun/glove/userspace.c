#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "glove_driver.h"


int main() {
	int fd;
    glove_data_t glove_data;

    fd = open("/dev/glove_device", O_RDWR);
    if (fd < 0) {
        perror("Failed to open glove device");
        return 1;
    }

    if (ioctl(fd, IOCTL_READ_DATA, &glove_data) < 0) {
        perror("Failed to read data from glove device");
        close(fd);
        return 1;
    }

    printf("Data received from glove device:\n");
    printf("MCP3008 values: ");
    for (int i = 0; i < 8; i++) {
        printf("%d ", glove_data.mcp3008_values[i]);
    }
    printf("\nSEN10264 values: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", glove_data.sen10264_values[i]);
    }
    printf("\nMPU6050 acceleration: ");
    for (int i = 0; i < 3; i++) {
        printf("%d ", glove_data.mpu6050_accel[i]);
    }
    printf("\nMPU6050 gyroscope: ");
    for (int i = 0; i < 3; i++) {
        printf("%d ", glove_data.mpu6050_gyro[i]);
    }
    printf("\n");

    close(fd);
    return 0;
}
