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

dir="/opt/rflink"
cmd="./RFlink"
user=""
TCP_port=5050
PIN_TX=28
PIN_RX=29
log_level=1
log_file=""

#include config file (may redefine TCP_port, log_level, log_file)
. /etc/rflink.conf

cmd="$cmd $TCP_port $PIN_TX $PIN_RX $log_level"
cmd_to_syslog="$cmd_to_sysylog $TCP_port $PIN_TX $PIN_RX $log_level"

name=`basename $0`
pid_file="/var/run/$name.pid"

get_pid() {
    cat "$pid_file"
}

is_running() {
    [ -f "$pid_file" ] && ps `get_pid` > /dev/null 2>&1
}


killtree() {
    local parent=$1 child
    for child in $(ps -o ppid= -o pid= | awk "\$1==$parent {print \$2}"); do
        killtree $child
    done
    kill $parent 2> /dev/null
}

start_with_logger() {
    if [ -z "$user" ]; then
	sudo $cmd | logger
    else
	sudo -u "$user" $cmd | logger
    fi
}

start_with_log_file() {
    if [ -z "$user" ]; then
	sudo $cmd >> "$log_file"
    else
	sudo -u "$user" $cmd >> "$log_file"
    fi
}

case "$1" in
    start)
    if is_running; then
        echo "Already started"
    else
        echo "Starting $name"
        cd "$dir"
	if [ -z "$log_file" ]; then
	    start_with_logger &
	else
	    start_with_log_file &
	fi
        echo $! > "$pid_file"
        if ! is_running; then
            echo "Unable to start, see $stdout_log and $stderr_log"
            exit 1
        fi
    fi
    ;;
    stop)
    if is_running; then
        echo -n "Stopping $name.."
        killtree `get_pid`
        for i in {1..10}
        do
            if ! is_running; then
                break
            fi

            echo -n "."
            sleep 1
        done
        echo

        if is_running; then
            echo "Not stopped; may still be shutting down or shutdown may have failed"
            exit 1
        else
            echo "Stopped"
            if [ -f "$pid_file" ]; then
                rm "$pid_file"
            fi
        fi
    else
        echo "Not running"
    fi
    ;;
    restart)
    $0 stop
    if is_running; then
        echo "Unable to stop, will not attempt to start"
        exit 1
    fi
    $0 start
    ;;
    status)
    if is_running; then
        echo "Running"
    else
        echo "Stopped"
        exit 1
    fi
    ;;
    *)
    echo "Usage: $0 {start|stop|restart|status}"
    exit 1
    ;;
esac

exit 0