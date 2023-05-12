#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#define MPU6050_ADDRESS 0x68

int16_t read_word(int file, uint8_t reg) {
    int16_t data;
    uint8_t buf[2];
    buf[0] = reg;
    if (write(file, buf, 1) != 1) {
        perror("Failed to write to the i2c bus");
        exit(1);
    }
    if (read(file, buf, 2) != 2) {
        perror("Failed to read from the i2c bus");
        exit(1);
    }
    data = (buf[0] << 8) | buf[1];
    return data;
}

int main() {
    int file;
    const char *filename = "/dev/i2c-1";
    file = open(filename, O_RDWR);
    if (file < 0) {
        perror("Failed to open the i2c bus");
        exit(1);
    }

    if (ioctl(file, I2C_SLAVE, MPU6050_ADDRESS) < 0) {
        perror("Failed to set I2C_SLAVE address");
        exit(1);
    }

    int16_t ax_raw, ay_raw, az_raw, gx_raw, gy_raw, gz_raw;
    float ax, ay, az, gx, gy, gz;

    while (1) {
        ax_raw = read_word(file, 0x3B);
        ay_raw = read_word(file, 0x3D);
        az_raw = read_word(file, 0x3F);
        gx_raw = read_word(file, 0x43);
        gy_raw = read_word(file, 0x45);
        gz_raw = read_word(file, 0x47);

        // Convert raw accelerometer values to g
        float accelerometer_sensitivity = 16384.0; // For range +/- 2g
        ax = (float)ax_raw / accelerometer_sensitivity;
        ay = (float)ay_raw / accelerometer_sensitivity;
        az = (float)az_raw / accelerometer_sensitivity;

        // Convert raw gyroscope values to degrees/sec
        float gyroscope_sensitivity = 131.0; // For range +/- 250 degrees/sec
        gx = (float)gx_raw / gyroscope_sensitivity;
        gy = (float)gy_raw / gyroscope_sensitivity;
        gz = (float)gz_raw / gyroscope_sensitivity;

        printf("Acceleration: X = %.2f g, Y = %.2f g, Z = %.2f g\n", ax, ay, az);
        printf("Gyroscope: X = %.2f dps, Y = %.2f dps, Z = %.2f dps\n", gx, gy, gz);

        usleep(100000); // Sleep for 100ms
    }

    close(file);

    return 0;
}

