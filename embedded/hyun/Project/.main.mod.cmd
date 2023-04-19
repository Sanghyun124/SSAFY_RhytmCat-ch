cmd_/home/pi/G547/Project/main.mod := printf '%s\n'   main.o | awk '!x[$$0]++ { print("/home/pi/G547/Project/"$$0) }' > /home/pi/G547/Project/main.mod
