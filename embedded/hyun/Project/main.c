#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>


#include "mpu6050.h"

#define DRIVER_NAME "MPU6050"
#define DRIVER_CLASS "MPU6050Class"

static struct i2c_adapter * MPU6050_i2c_adapter = NULL;
static struct i2c_client * MPU6050_i2c_client = NULL;

static struct gyroData gyro;
static struct accelData accel;

#define I2C_BUS_AVAILABLE       1               /* I2C Bus SDA at GPIO2 SCL at GPIO3 available on the raspberry */
#define SLAVE_DEVICE_NAME       "MPU6050"        /* Device and Driver Name */
#define MPU6050_SLAVE_ADDRESS    0x68            /* I2C address of MPU6050 */

static const struct i2c_device_id MPU6050_id[]={
{ SLAVE_DEVICE_NAME, 0},
{ }
};

static struct i2c_driver MPU6050_driver = {
 .driver = {
   .name = SLAVE_DEVICE_NAME,
   .owner = THIS_MODULE
  }
};

static struct i2c_board_info MPU6050_i2c_board_info = {
 I2C_BOARD_INFO(SLAVE_DEVICE_NAME, MPU6050_SLAVE_ADDRESS)
};                                  //Platform device


static dev_t dev_no;        //variable for device number
static struct cdev c_dev;     //variable for the character device structure
static struct class *dev_class;  //varible for the device class



static int open_driver(struct inode *i, struct file *f)   //open function of file
{
 printk("Device : Open() \n");
 return 0;
}

static int close_driver(struct inode *i, struct file *f)  //close function of file
{
        printk("Device : Close()\n");
        return 0;
}

static uint32_t read_accelerometer(void)                                 //reading accelerometer values from MPU_6050 sensor
{

    uint8_t lsb,msb,lsb1,msb1,lsb2,msb2;
    uint16_t x_accel,y_accel,z_accel;

    lsb = i2c_smbus_read_byte_data(MPU6050_i2c_client, 0x3C); //reading ACCEL_XOUT_LSB value of MPU6050 from reg60 located at 0x3C
    msb = i2c_smbus_read_byte_data(MPU6050_i2c_client, 0x3B); //reading ACCEL_XOUT_LSB value of MPU6050 from reg59 located at 0x3B
    x_accel = ((uint16_t)msb<<8) | ((uint16_t)lsb);           // concatinating the 8-bit values to form 16-bit value
    accel.x= x_accel;
	
    lsb1 = i2c_smbus_read_byte_data(MPU6050_i2c_client, 0x3E); //reading ACCEL_YOUT_LSB value of MPU6050 from reg62 located at 0x3E
    msb1 = i2c_smbus_read_byte_data(MPU6050_i2c_client, 0x3D); //reading ACCEL_YOUT_MSB value of MPU6050 from reg61 located at 0x3D
    y_accel = ((uint16_t)msb1<<8) | ((uint16_t)lsb1);          // concatinating the 8-bit values to form 16-bit value
    accel.y=  y_accel;
	
    lsb2 = i2c_smbus_read_byte_data(MPU6050_i2c_client, 0x40);  //reading ACCEL_ZOUT_LSB value of MPU6050 from reg64 located at 0x40
    msb2 = i2c_smbus_read_byte_data(MPU6050_i2c_client, 0x3F);	//reading ACCEL_ZOUT_MSB value of MPU6050 from reg63 located at 0x3F
    z_accel = ((uint16_t)msb2<<8) | ((uint16_t)lsb2);          // concatinating the 8-bit values to form 16-bit value
    accel.z=  z_accel;
    return 0;

}
static uint32_t read_gyroscope(void)                          //reading gyroscope values from MPU_6050 sensor
{
    uint8_t lsb,msb,lsb1,msb1,lsb2,msb2;
    uint16_t gyro_x,gyro_y,gyro_z;

    lsb = i2c_smbus_read_byte_data(MPU6050_i2c_client, 0x44);  //reading GYRO_XOUT_LSB value of MPU6050 from reg68 located at 0x44
    msb = i2c_smbus_read_byte_data(MPU6050_i2c_client, 0x43);  //reading GYRO_XOUT_MSB value of MPU6050 from reg67 located at 0x43
    gyro_x = ((uint16_t)msb<<8) | ((uint16_t)lsb);             // concatinating the 8-bit values to form 16-bit value
    gyro.x=  gyro_x;
	
    lsb1 = i2c_smbus_read_byte_data(MPU6050_i2c_client, 0x46); //reading GYRO_YOUT_LSB value of MPU6050 from reg70 located at 0x46
    msb1 = i2c_smbus_read_byte_data(MPU6050_i2c_client, 0x45); //reading GYRO_YOUT_MSB value of MPU6050 from reg69 located at 0x45	
    gyro_y = ((uint16_t)msb1<<8) | ((uint16_t)lsb1);           // concatinating the 8-bit values to form 16-bit value
    gyro.y=  gyro_y;
	
    lsb2 = i2c_smbus_read_byte_data(MPU6050_i2c_client, 0x48); //reading GYRO_YOUT_LSB value of MPU6050 from reg72 located at 0x48
    msb2 = i2c_smbus_read_byte_data(MPU6050_i2c_client, 0x47); //reading GYRO_YOUT_MSB value of MPU6050 from reg71 located at 0x47
    gyro_z= ((uint16_t)msb2<<8) | ((uint16_t)lsb2);            // concatinating the 8-bit values to form 16-bit value
    gyro.z=  gyro_z;
    return 0;
}

static ssize_t read_driver(struct file *File, char __user *user_buffer, size_t count, loff_t *offs)  //read function of file
{
 int to_copy, not_copied, delta;
 char out_string[100];                // declaring no of characters to get printed
 int16_t accel_x,accel_y,accel_z;
 int16_t x_gyro,y_gyro,z_gyro;
 int32_t k,k1;

 to_copy = min(sizeof(out_string), count);

 k=read_accelerometer();             // Reading accelerometer values
 accel_x = accel.x;
 accel_y = accel.y;
 accel_z = accel.z;
 
 k1=read_gyroscope();              // Reading gyroscope values
 x_gyro = gyro.x;
 y_gyro = gyro.y;
 z_gyro = gyro.z;

 snprintf(out_string, sizeof(out_string), "accel_readings:x:%d,y:%d,z:%d,Gyro_readings:x:%d,y:%d,z:%d,\n",accel_x,accel_y,accel_z,x_gyro,y_gyro,z_gyro);
 not_copied = copy_to_user(user_buffer, out_string, to_copy);

 delta = to_copy - not_copied;

 return delta;
}

///////////////// IOCTL FUNCTION /////////////////////////////////////
long ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)      // number and param for ioctl 
{
 
 switch(ioctl_num)
 {
  case IOCTL_GYRO:
  read_gyroscope();
  if (copy_to_user((struct gyroData *)ioctl_param,&gyro,sizeof(struct gyroData)) != 0) {
     return -EFAULT;
  }
  break;
 
  case IOCTL_ACCEL:
  read_accelerometer();
  if (copy_to_user((struct accelData *)ioctl_param,&accel,sizeof(struct accelData)) != 0) {
   return -EFAULT;
  }
  break;

 }
return 0;
}



static struct file_operations fops  = {
 .owner = THIS_MODULE,
 .open = open_driver,
 .release = close_driver,
 .read = read_driver,
};

static int __init mydriver_init(void)
{
 int ret =-1;
 u8 check;
 printk("Driver for MPU6050 sensor registered\n");
 //////////////////////reserve <major,minor>/////////////////////////
 if ( alloc_chrdev_region(&dev_no, 0, 1, DRIVER_NAME) < 0) {
                printk("Failed to assign Device Number!\n");
        }
        printk("Driver with device number %d for MPU6050 sensor registered\n", dev_no);

   ////////////////dynamically create device node in /dev directory     ////////////////////////
        if ((dev_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) 
		{
                printk("Failed to create Device Class!\n");
				unregister_chrdev(dev_no, DRIVER_NAME);  //unregistering the character device with major and minor number
                return (-1);
        }

 //////////// creating device node ////////////////////////
        if (device_create(dev_class, NULL, dev_no, NULL, DRIVER_NAME) == NULL)
		{
                printk("Failed to create device file!\n");
				class_destroy(dev_class);                    //destroying the device class
				unregister_chrdev(dev_no, DRIVER_NAME);      //unregistering the character device with major and minor number
                return (-1);
        }

/////////// Link file_operations and Cdev to device node///////////////////
        cdev_init(&c_dev, &fops);

        /* register device to kernel */
        if (cdev_add(&c_dev, dev_no, 1) == -1) 
		{
                printk("Failed to register device to kernel!\n");
				device_destroy(dev_class, dev_no);              //destroy device node
				class_destroy(dev_class);                       //destroying the device class   
				unregister_chrdev(dev_no, DRIVER_NAME);         //unregistering the character device with major and minor number      
                return (-1);
        }

        MPU6050_i2c_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);

        if(MPU6050_i2c_adapter != NULL) {
                MPU6050_i2c_client = i2c_new_client_device(MPU6050_i2c_adapter, &MPU6050_i2c_board_info);
                if(MPU6050_i2c_client != NULL) {
                        if(i2c_add_driver(&MPU6050_driver) != -1) {
                                ret = 0;
                        }
                        else
                                printk("Can't add driver...\n");
                }
                i2c_put_adapter(MPU6050_i2c_adapter);
        }

        printk("MPU6050 Driver Init\n");

	check = i2c_smbus_read_byte_data(MPU6050_i2c_client, 0x75);     // Reading value from WHO AM I stored at reg117 located at 0x75 which returns bit6:bit1 110 100 whether connection is established or not
    printk("Checking whether communication is established or not: 0x%x\n",check);

     i2c_smbus_write_byte_data(MPU6050_i2c_client, 0x6B, 0x01);  //Configuring power management-1 located at 0x6B
     i2c_smbus_write_byte_data(MPU6050_i2c_client, 0x1B, 0x18);  //Configuring Gyroscope at reg27 located at 0x1B with full range of +/- 2000 degrees /s
     i2c_smbus_write_byte_data(MPU6050_i2c_client, 0x1C, 0x00);  //Configuring accelerometer at reg28 located at 0x1C with full range of +/- 2g

        return ret;

}

static void __exit mydriver_exit(void) {
        i2c_unregister_device(MPU6050_i2c_client);   // unregistering the i2c client device
        i2c_del_driver(&MPU6050_driver);          // unregister I2C driver
        cdev_del(&c_dev);                      //deleting the link between cdev and file operations
        device_destroy(dev_class, dev_no);     //destroy device node
        class_destroy(dev_class);             //destroying the device class 
        unregister_chrdev_region(dev_no, 1);  //unregistering the character device with major and minor number
		printk("Bye:Driver for MPU6050 sensor unregistered!\n");
}

module_init(mydriver_init);
module_exit(mydriver_exit);
MODULE_AUTHOR("SAINADH");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MPU6050 Sensor Driver");
