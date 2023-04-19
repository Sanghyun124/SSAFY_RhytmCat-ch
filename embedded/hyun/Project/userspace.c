#include<stdio.h>
#include<stdlib.h>
#include<sys/ioctl.h>
#include<time.h>
#include<fcntl.h>
#include<signal.h>
#include<unistd.h>


#include "mpu6050.h"

int file_desc;
char str[20];

/* Functions for the ioctl calls */
int ioctl_accelerometer(int file_desc, struct accelData *msg)    //Function that reads the Values from Accelerometer
{
 int ret_val;
 
 ret_val = ioctl(file_desc, IOCTL_ACCEL,msg);    //ioctl function call with request code  IOCTL_ACCEL
 return ret_val;
}

int ioctl_gyroscope(int file_desc, struct gyroData *msg)    //Function that reads the Values from Gyroscope
{
 int ret_val;

 ret_val = ioctl(file_desc, IOCTL_GYRO,msg);     //ioctl function call with request code  IOCTL_GYRO 
 return 0;
}


/* Main - Call the ioctl functions */
int main(void)
{
 int ret_val,i;
 struct gyroData gyro_data;
 struct accelData accel_data;
 char buffer[100];
 float xaccel,yaccel,zaccel,xgyro,ygyro,zgyro;

 file_desc = open(DEVICE_FILE_NAME,0); // opening the device node and returning the value to file_desc
 if(file_desc<0)
 {
  printf(" Failed to open device %s\n",DEVICE_FILE_NAME);  //Display if permission is denied to open /dev/MPU6050
  exit(-1);
 }
 while(1)
 {
  ssize_t count = read(file_desc,buffer,sizeof(buffer));
  if(count > 0){
	  printf("%s",buffer);
  }
  sleep(1);

  ioctl_accelerometer(file_desc,&accel_data);   //Calling the accelerometer ioctl function call
  xaccel= ((float)(accel_data.x))/16384;       // Calibrating the obtained value as per register map datasheet
  yaccel= ((float)(accel_data.y))/16384;       // Calibrating the obtained value as per register map datasheet
  zaccel= ((float)(accel_data.z))/16384;       // Calibrating the obtained value as per register map datasheet
 // printf("Raw accelerometer readings: x:%d , y:%d, z:%d\n",accel_data.x,accel_data.y,accel_data.z);
 // printf("After calibrating accelerometer readings: x:%f , y:%f, z:%f\n",xaccel,yaccel,zaccel);
  ioctl_gyroscope(file_desc,&gyro_data);      //Calling the gyroscope ioctl function call
  xgyro= ((float)(gyro_data.x))/16.4;        // Calibrating the obtained value as per register map datasheet
  ygyro= ((float)(gyro_data.y))/16.4;        // Calibrating the obtained value as per register map datasheet
  zgyro= ((float)(gyro_data.z))/16.4;        // Calibrating the obtained value as per register map datasheet
 // printf("Raw gyroscope readings:x:%d , y:%d, z:%d\n",gyro_data.x,gyro_data.y,gyro_data.z);
 // printf("After calibrating gyroscope readings:x:%f , y:%f, z:%f\n",xgyro,ygyro,zgyro);
  sleep(1);
 }
 close(file_desc);
 return 0;
}
