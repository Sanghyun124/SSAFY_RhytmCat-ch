cmd_/home/pi/111111/smartglove.mod := printf '%s\n'   smartglove.o | awk '!x[$$0]++ { print("/home/pi/111111/"$$0) }' > /home/pi/111111/smartglove.mod
