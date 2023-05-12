cmd_/home/pi/111111/Module.symvers :=  sed 's/ko$$/o/'  /home/pi/111111/modules.order | scripts/mod/modpost -m -a    -o /home/pi/111111/Module.symvers -e -i Module.symvers -T - 
