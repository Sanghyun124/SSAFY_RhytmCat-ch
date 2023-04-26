cmd_/home/pi/glove2/modules.order := {   echo /home/pi/glove2/mpu6050.ko; :; } | awk '!x[$$0]++' - > /home/pi/glove2/modules.order
