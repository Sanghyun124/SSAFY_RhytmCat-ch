#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/spi/spi.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include "glove_driver.h"

#define SEN10264_NUM_PINS 5
static struct gpio sen10264_gpio_pins[] = {
    { 5, GPIOF_IN, "GPIO 5" },
    { 6, GPIOF_IN, "GPIO 6" },
    { 13, GPIOF_IN, "GPIO 13" },
    { 19, GPIOF_IN, "GPIO 19" },
    { 26, GPIOF_IN, "GPIO 26" },
};

#define MCP3008_SPI_BUS_NUM 0
#define MCP3008_SPI_BITS_PER_WORD 8
#define MCP3008_SPI_MODE SPI_MODE_0
#define MCP3008_SPI_MAX_SPEED_HZ 1000000
static struct spi_device *mcp3008_spi_device;

static struct i2c_client *mpu6050_i2c_client;

static int device_major;

static long glove_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    // TODO: IOCTL 처리를 위한 코드를 작성하세요.
	glove_data_t glove_data;

    switch (cmd) {
        case IOCTL_READ_DATA:
            // TODO: 센서 데이터를 읽고 glove_data 구조체에 값을 저장하는 코드를 작성하세요.
            if (copy_to_user((glove_data_t *)arg, &glove_data, sizeof(glove_data))) {
                return -EFAULT;
            }
            break;
        default:
            return -EINVAL;
    }
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = glove_ioctl,
};

static int sen10264_gpio_init(void) {
    int result = gpio_request_array(sen10264_gpio_pins, SEN10264_NUM_PINS);
    if (result) {
        printk(KERN_ALERT "Failed to request GPIO pins\n");
        return result;
    }
    return 0;
}

static void sen10264_gpio_exit(void) {
    gpio_free_array(sen10264_gpio_pins, SEN10264_NUM_PINS);
}

static int mpu6050_i2c_init(void) {
    struct i2c_adapter *adapter;
    struct i2c_board_info i2c_device_info = {
        I2C_BOARD_INFO("mpu6050", 0x68)
    };

    adapter = i2c_get_adapter(MPU6050_I2C_ADAPTER_NUM);
    if (!adapter) {
        printk(KERN_ERR "Failed to get i2c adapter\n");
        return -ENODEV;
    }

    mpu6050_i2c_client = i2c_new_device(adapter, &i2c_device_info);
    if (!mpu6050_i2c_client) {
        printk(KERN_ERR "Failed to create i2c client\n");
        return -ENODEV;
    }
	return 0;
}

static void mpu6050_i2c_exit(void) {
    i2c_unregister_device(mpu6050_i2c_client);
}

static int mcp3008_spi_init(void) {
	struct spi_master *master;
    struct spi_device *spi_device;

    static struct spi_board_info spi_device_info = {
        .modalias = "mcp3008",
        .max_speed_hz = MCP3008_SPI_MAX_SPEED_HZ,
        .bus_num = MCP3008_SPI_BUS_NUM,
        .chip_select = MCP3008_SPI_CHIP_SELECT,
        .mode = SPI_MODE_0,
    };

    master = spi_busnum_to_master(spi_device_info.bus_num);
    if (!master) {
        printk(KERN_ERR "Failed to get spi master\n");
        return -ENODEV;
    }

    spi_device = spi_new_device(master, &spi_device_info);
    if (!spi_device) {
        printk(KERN_ERR "Failed to create spi device\n");
        return -ENODEV;
    }
    return 0;
}

static void mcp3008_spi_exit(void) {
    spi_unregister_device(mcp3008_spi_device);
}

static int __init glove_driver_init(void) {
    int result;

    result = sen10264_gpio_init();
    if (result) {
        return result;
    }

    result = mpu6050_i2c_init();
    if (result) {
        sen10264_gpio_exit();
        return result;
    }

    result = mcp3008_spi_init();
    if (result) {
        mpu6050_i2c_exit();
        sen10264_gpio_exit();
        return result;
    }

    device_major = register_chrdev(0, "glove_device", &fops);
    if (device_major < 0) {
        printk(KERN_ALERT "Failed to register character device\n");
        mcp3008_spi_exit();
        mpu6050_i2c_exit();
        sen10264_gpio_exit();
        return device_major;
    }

    printk(KERN_INFO "Glove driver loaded\n");
    return 0;
}

static void __exit glove_driver_exit(void) {
    unregister_chrdev(device_major, "glove_device");
    mcp3008_spi_exit();
    mpu6050_i2c_exit();
    sen10264_gpio_exit();

    printk(KERN_INFO "Glove driver unloaded\n");
}

module_init(glove_driver_init);
module_exit(glove_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("Glove driver");
MODULE_VERSION("0.1");
