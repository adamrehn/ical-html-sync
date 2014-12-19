# Ensure the script is run as root
if [ `id -u` -ne 0 ]; then
	echo "Error: this script must be run as root!"
	exit
fi

# Kill the icalsync process
echo Killing icalsync process...
killall -v icalsync

# Delete the executables
echo Deleting executables...
rm /usr/local/bin/icalsync
rm /usr/local/bin/ics2html

# Delete the config files
echo Deleting config files...
rm -r $HOME/.config/ical-html-sync

# Remove the startup item
rm ~/Library/LaunchAgents/com.adamrehn.icalsync.plist