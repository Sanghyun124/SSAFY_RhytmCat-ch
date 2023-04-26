cmd_/home/pi/glove2/Module.symvers := sed 's/\.ko$$/\.o/' /home/pi/glove2/modules.order | scripts/mod/modpost -m -a  -o /home/pi/glove2/Module.symvers -e -i Module.symvers   -T -
