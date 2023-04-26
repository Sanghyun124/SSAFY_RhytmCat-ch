#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include "mpu6050_driver.h"

#define DEVICE_NAME "mpu6050"
#define I2C_BUS 1
#define MPU6050_ADDR 0x68

static struct i2c_client *client;
static int major_number;

static int device_open(struct inode *inode, struct file *file)
{
    return 0;
}

static int device_release(struct inode *inode, struct file *file)
{
    return 0;
}

static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
    uint8_t reg_data[6];
    struct mpu6050_data data;
    int ret;

    ret = i2c_smbus_read_i2c_block_data(client, MPU6050_ACCEL_XOUT_H, 6, reg_data);
    if (ret < 0) {
        return ret;
    }

    data.accel_x = (reg_data[0] << 8) | reg_data[1];
    data.accel_y = (reg_data[2] << 8) | reg_data[3];
    data.accel_z = (reg_data[4] << 8) | reg_data[5];

    ret = copy_to_user(buffer, &data, sizeof(data));
    if (ret != 0) {
        return -EFAULT;
    }

    return sizeof(data);
}

static struct file_operations fops = {
    .open = device_open,
    .release = device_release,
    .read = device_read,
};

static int mpu6050_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    int ret;

    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        return major_number;
    }

    ret = i2c_smbus_write_byte_data(client, MPU6050_PWR_MGMT_1, 0);
    if (ret < 0) {
        unregister_chrdev(major_number, DEVICE_NAME);
        return ret;
    }

    return 0;
}

static int mpu6050_remove(struct i2c_client *client)
{
    unregister_chrdev(major_number, DEVICE_NAME);
    return 0;
}

static const struct i2c_device_id mpu6050_id[] = {
    {"mpu6050", 0},
    {}
};

MODULE_DEVICE_TABLE(i2c, mpu6050_id);

static struct i2c_driver mpu6050_driver = {
    .driver = {
        .name = "mpu6050",
        .owner = THIS_MODULE,
    },
    .probe = mpu6050_probe,
    .remove = mpu6050_remove,
    .id_table = mpu6050_id,
};

static struct i2c_board_info mpu6050_i2c_board_info = {
    I2C_BOARD_INFO("mpu6050", MPU6050_ADDR),
};

static struct i2c_client *mpu6050_i2c_client;

static int __init mpu6050_init(void)
{
    struct i2c_adapter *adapter;
    int ret;

    adapter = i2c_get_adapter(I2C_BUS);
    if (!adapter) {
        return -ENODEV;
    }
    mpu6050_i2c_client = i2c_new_client_device(adapter, &mpu6050_i2c_board_info);
    if (!mpu6050_i2c_client) {
        i2c_put_adapter(adapter);
        return -ENODEV;
    }

    ret = i2c_add_driver(&mpu6050_driver);
    if (ret < 0) {
        i2c_unregister_device(mpu6050_i2c_client);
        i2c_put_adapter(adapter);
    }

    return ret;
}

static void __exit mpu6050_exit(void)
{
    i2c_unregister_device(mpu6050_i2c_client);
    i2c_del_driver(&mpu6050_driver);
}

module_init(mpu6050_init);
module_exit(mpu6050_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MPU6050 device driver");
MODULE_AUTHOR("Your Name");

