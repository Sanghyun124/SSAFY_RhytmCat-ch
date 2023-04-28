#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/i2c-dev.h>
#define DEVICE_NAME "mpu6050_device"
#define CLASS_NAME "mpu6050_class"
#define MPU6050_I2C_ADDRESS 0x68

static int major_number;
static struct class *mpu6050_class;
static struct device *mpu6050_device;
static struct i2c_client *mpu6050_client;

static int mpu6050_open(struct inode *inode, struct file *file)
{
    printk(KERN_INFO "mpu6050: device opened\n");
    return 0;
}

static int mpu6050_read_data(int reg)
{
    return i2c_smbus_read_byte_data(mpu6050_client, reg);
}

static ssize_t mpu6050_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    char data[14];
    int i, error;

    if (count < 14) 
    {
        return -EINVAL;
    }

    for (i = 0; i < 14; i++) {
        error = mpu6050_read_data(0x3B + i);
        int a = 0x3B + i;
        printk(KERN_INFO "llll %x llll %d : %d\n",a,i, error);
        if (error < 0) {
            printk(KERN_ERR "mpu6050: failed to read data\n");
            return error;
        }
        data[i] = error;
    }

    if (copy_to_user(buf, data, 14)) { // count?? 14?? ????
        return -EFAULT;
    }

    return count;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = mpu6050_open,
    .read = mpu6050_read,
};

static int __init mpu6050_init( )
{
    struct i2c_adapter *adapter;
    struct i2c_board_info board_info = {
        .type = "mpu6050",
        .addr = MPU6050_I2C_ADDRESS,
    };
    
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    printk(KERN_INFO "__init: mpu6050 driver loaded\n");
    if (major_number < 0) {
        printk(KERN_ALERT "mpu6050: failed to register a major number\n");
        return major_number;
    }

    mpu6050_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(mpu6050_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "mpu6050: failed to create a class\n");
        return PTR_ERR(mpu6050_class);
    }

    mpu6050_device = device_create(mpu6050_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
        if (IS_ERR(mpu6050_device)) {
        class_destroy(mpu6050_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ALERT "mpu6050: failed to create a device\n");
        return PTR_ERR(mpu6050_device);
    }

    adapter = i2c_get_adapter(1);
    if (!adapter) {
        printk(KERN_ERR "mpu6050: failed to get i2c adapter\n");
        return -ENODEV;
    }

    mpu6050_client = i2c_new_client_device(adapter, &board_info);
    if (!mpu6050_client) {
        printk(KERN_ERR "mpu6050: failed to create i2c client\n");
        return -ENODEV;
    }

    return 0;
}

static void __exit mpu6050_exit(void)
{
    device_destroy(mpu6050_class, MKDEV(major_number, 0));
    class_unregister(mpu6050_class);
    class_destroy(mpu6050_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    i2c_unregister_device(mpu6050_client);
    printk(KERN_INFO "mpu6050: driver unloaded\n");
}

module_init(mpu6050_init);
module_exit(mpu6050_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple Linux char driver for the MPU6050");
MODULE_VERSION("0.1");

