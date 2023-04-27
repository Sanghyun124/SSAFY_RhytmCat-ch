#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/i2c.h>
#include <linux/uaccess.h>
#include <linux/delay.h>

#define DEVICE_NAME "mpu6050_device"
#define MPU6050_ADDR 0x68
#define MPU6050_PWR_MGMT_1 0x6B

static int major_number;
static struct class *mpu6050_class;
static struct device *mpu6050_device;
static struct i2c_client *mpu6050_client;

static int mpu6050_open(struct inode *inode, struct file *file)
{
    printk("Device : Open() \n");
    return 0;
}

static int mpu6050_release(struct inode *inode, struct file *file)
{
    printk("Device : Close()\n");
    return 0;
}

static ssize_t mpu6050_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
    u8 reg[2];
    s16 data;
    ssize_t len;

    reg[0] = file->f_path.dentry->d_name.name[0] == 'a' ? 0x3B : 0x43;
    i2c_smbus_read_i2c_block_data(mpu6050_client, reg[0], 2, reg);
    data = (reg[0] << 8) | reg[1];
    len = snprintf(NULL, 0, "%d\n", data);
    if (*ppos >= len)
        return 0;
    if (count < len)
        return -EINVAL;
    if (copy_to_user(buf, &data, len))
        return -EFAULT;
    *ppos += len;

    return len;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = mpu6050_open,
    .release = mpu6050_release,
    .read = mpu6050_read,
};

static int mpu6050_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    int ret;
    printk(KERN_INFO "mpu6050: probe function called\n");
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        return major_number;
    }

    mpu6050_class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(mpu6050_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        return PTR_ERR(mpu6050_class);
    }

    mpu6050_device = device_create(mpu6050_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(mpu6050_device)) {
        class_destroy(mpu6050_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        return PTR_ERR(mpu6050_device);
    }

    ret = i2c_smbus_write_byte_data(client, MPU6050_PWR_MGMT_1, 0);
    if (ret < 0) {
        device_destroy(mpu6050_class, MKDEV(major_number, 0));
        class_destroy(mpu6050_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        return ret;
    }

    mpu6050_client = client;

    return 0;
}

static int mpu6050_remove(struct i2c_client *client)
{
    device_destroy(mpu6050_class, MKDEV(major_number, 0));
    class_destroy(mpu6050_class);
    unregister_chrdev(major_number, DEVICE_NAME);
    return 0;
}

static const struct i2c_device_id mpu6050_id[] = {
    { "mpu6050", 0 },
    { }
};
MODULE_DEVICE_TABLE(i2c, mpu6050_id);

static struct i2c_driver mpu6050_driver = {
    .driver = {
        .name = "mpu6050",
    },
    .probe = mpu6050_probe,
    .remove = mpu6050_remove,
    .id_table = mpu6050_id,
};

module_i2c_driver(mpu6050_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("A simple driver for the MPU6050 sensor");
MODULE_VERSION("0.1");

