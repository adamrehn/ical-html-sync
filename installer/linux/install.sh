# Ensure the script is run as root
if [ `id -u` -ne 0 ]; then
	echo "Error: this script must be run as root!"
	exit
fi

# Copy the executables
cp ./icalsync /usr/local/bin/
cp ./ics2html /usr/local/bin/

# Set the permissions
chmod 755 /usr/local/bin/icalsync
chmod 755 /usr/local/bin/ics2html

# Add the startup item
sudo cp ./icalsync.init /etc/init.d/icalsync
export USER_ID=`logname`
cat >/etc/default/icalsync <<EOL
export USER_ID=${USER_ID}
export HOMEDIR="${HOME}"
EOL
chmod +x /etc/init.d/icalsync
update-rc.d icalsync defaults

# Make sure the config directory is created
/etc/init.d/icalsync start