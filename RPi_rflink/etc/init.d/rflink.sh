#!/bin/sh
### BEGIN INIT INFO
# Provides:          Seahu
# Required-Start:    $remote_fs $syslog
# Required-Stop:     $remote_fs $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Control and Sniffer devices on 433 MHz for Home Automation System
# Description:       This daemon will start 433 MHz control and sniffer and specialy used for Domoticz Home Automation System
### END INIT INFO


DAEMON="/opt/rflink/RFlink"
config_file="/etc/rflink.conf"

# default config
user=""
TCP_port=5050
PIN_TX=28
PIN_RX=29
log_level=3
log_file=""

#include config file  if exist (may redefine TCP_port, log_level, log_file)
if [ -f "$config_file" ]; then
    . /etc/rflink.conf
fi

CONFIG="$TCP_port $PIN_TX $PIN_RX $log_level"

#. /lib/lsb/init-functions

name=`basename $0`
PIDFILE="/var/run/$name.pid"

case "$1" in
    start)
        echo "Starting $name"
        if [ -z "$user" ]; then
	    #start-stop-daemon --start --verbose --background --pidfile $PIDFILE --make-pidfile --exec $DAEMON
	    if [ -z "$log_file" ]; then
		echo "start-stop-daemon -S -x $DAEMON -b -C -v -m -p $PIDFILE -- $CONFIG | logger -t $name &"
		start-stop-daemon -S -x $DAEMON -b -C -v -m -p $PIDFILE -- $CONFIG | logger -t $name &
	    else
		start-stop-daemon -S -x $DAEMON -b -C -v -m -p $PIDFILE -- $CONFIG >> $log_file
	    fi
	else
	    if [ -z "$log_file" ]; then
		start-stop-daemon -S -c $user -x $DAEMON -b -C -v -m -p $PIDFILE -- $CONFIG | logger -t $name &
	    else
		start-stop-daemon -S -c $user -x $DAEMON -b -C -v -m -p $PIDFILE -- $CONFIG >> $log_file
	    fi
	fi
	;;
    stop)
    	echo -n "Stopping $name."
	#pkill -P `cat $PIDFILE`
	start-stop-daemon -K -x $DAEMON -p $PIDFILE --remove-pidfile
	;;
    restart)
	echo -n "Restarting $name."
	$0 stop
	$0 start
	;;
    status)
	start-stop-daemon -T -x $DAEMON
	;;
    *)
    echo "Usage: $0 {start|stop|restart|status}"
    exit 1
    ;;
esac

exit 0