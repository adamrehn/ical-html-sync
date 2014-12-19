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

# Make sure the config directory is created
icalsync > /dev/null 2>&1

# Add the startup item
cp ./com.adamrehn.icalsync.plist ~/Library/LaunchAgents/