cmd_/home/pi/G547/Project/Module.symvers :=  sed 's/ko$$/o/'  /home/pi/G547/Project/modules.order | scripts/mod/modpost -m -a    -o /home/pi/G547/Project/Module.symvers -e -i Module.symvers -T - 
