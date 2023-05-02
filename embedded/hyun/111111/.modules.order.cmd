cmd_/home/pi/111111/modules.order := {   echo /home/pi/111111/mpu.ko; :; } | awk '!x[$$0]++' - > /home/pi/111111/modules.order
