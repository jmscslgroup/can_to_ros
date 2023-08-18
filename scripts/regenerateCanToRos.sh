#/bin/bash

# if [ $EUID -ne 0 ]; then
#     echo "$0 not running as root. Please call using sudo."
#     exit 2
# fi

LIBPANDA_USER=$(cat /etc/libpanda.d/libpanda_usr)
FILE=/etc/libpanda.d/vin_details.json
if test -f "$FILE"; then
    echo "$FILE exists."
else
    echo "$FILE doesn't exist. Running vinParser"
    sudo python3 /home/circles/libpanda/scripts/vinParser.py
    # python3 /Users/mnice/Documents/GitHub/libpanda_real/scripts/vinParser.py
fi

echo "The current vin details are  " $(cat /etc/libpanda.d/vin_details.json)
echo "The current vin is " $(cat /etc/libpanda.d/vin)
echo "If something is incorrect, run libpanda/scripts/setVin.sh, or run vinParser.py to lookup details"

read -r -p "Does this information look correct? Proceed to generating CANtoROS from this VIN? [Y/N] " response
  case "$response" in
      [yY][eE][sS]|[yY])

	python3 ./makeSignalsJSON.py
	python3 ./generateDecode_Subs.py
  ./regen_vehicle_interface.sh

	cd /home/$LIBPANDA_USER/catkin_ws
	catkin_make
esac
