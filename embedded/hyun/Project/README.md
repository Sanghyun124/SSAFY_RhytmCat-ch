# MPU6050 SENSOR DRIVER
MPU6050 is a three-axis accelerometer and three-axis gyroscope and accelerometer Micro Electro-mechanical system (MEMS).It aids in the measurement of velocity, direction, acceleration, displacement, and other motion-related characteristics.

# Steps to build and insert MPU6050 Sensor driver in kernel and to run userspace 

Step-1 :  Change  path of the system to the directory where all the required driver files are stored using the following command   
`cd  path_address`  \
Step-2 : Now here Makefile consists of creating object files, kernel object file and  compiling userspace application. Following Command is used \
`sudo make all` \
Step-3 : In this step , we insert the driver in kernel using the following command \
`sudo insmod main.ko` \
Step-4 : To check whether the MPU6050 sensor is identified or not , we are reading WHO AM I register of MPU6050 whose value  bit6:bit1 is 110 100 which will be printed in kernel log. Use the following command to check \
`dmesg` \
**Note: Check Bit6:Bit1 in the value displayed in the kernel log equal to 110 100.**  \
Step-5:  To check the raw data readings of accelerometer and gyroscope in the kernel , we use the following command \
`sudo cat /dev/MPU6050`  \
Step-6: As userspace application program is compiled in Makefile so we directly see the output of userspace program using following command \
`sudo  ./userspace`  \
**Note: If we want to recompile the userspace application program, use the following command** \
`sudo gcc -o userspace userspace.c` \
Step-7 : To remove the driver from the kernel use the following command \
`sudo rmmod main.ko` \
Step-8 : To remove the object files use the following command \
`sudo make clean` 

 
 
