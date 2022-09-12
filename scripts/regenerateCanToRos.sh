#/bin/bash

echo "The current vin details are  " $(cat /etc/libpanda.d/vin_details.json)
echo "The current vin is " $(cat /etc/libpanda.d/vin)
echo "If something is incorrect, run libpanda/scripts/setVin.sh, or run vinParser.py to lookup details"

read -r -p "Does this information look correct? Proceed to generating CANtoROS from this VIN? [Y/N] " response
  case "$response" in
      [yY][eE][sS]|[yY])

	python3 /home/circles/catkin_ws/src/can_to_ros/scripts/makeSignalsJSON.py
	python3 /home/circles/catkin_ws/src/can_to_ros/scripts/generateDecode_Subs.py
  ./regen_vehicle_interface.sh

	cd /home/circles/catkin_ws
	catkin_make
esac
