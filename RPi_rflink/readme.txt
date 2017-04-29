# About

Modification RFlink for run on raspberryPI. ( http://www.nemcon.nl/blog2/ )
This modification enable connect RF tranceiver and receiver directly into free raspberryPI pins.
Instead comunication with serial port use TCP conecction.
This modification use interrupt and theaders for reduce cpu load.

## Compile and install
#### Compile
First you have to install the [wiringpi](https://projects.drogon.net/raspberry-pi/wiringpi/download-and-install/) library (also you can install by: apt-get install wiringpi).
After that you can compile on this directory by executing *make* . This generate one execute file RFlink who you can run.

#### Install
After taht you can copy RFlink into /opt/rflink and install initialization stript as standart linux daemon by *make install*.
If you can automaticaly start this daemon after start system than type *make autostart_on*.
Addions plugins may by add into standart diractory *../Plugins* and enableb in *../Config/Config_02.c* (you can change config file into *./Makefile* by edit varibale CONFIG_PLUGINS).

## Run from comandline
Use:
 *sudo /opt/rflink/RFlink* 
 *sudo /opt/rflink/RFlink TCP_port_number* 
 *sudo /opt/rflink/RFlink TCP_port_number log_level_number* 
 *sudo /opt/rflink/RFlink TCP_port_number TX_PIN RX_PIN* 
 *sudo /opt/rflink/RFlink TCP_port_number TX_PIN RX_PIN log_level_number*
 or *sudo /opt/rflink/RFlink -h*  for this help

 TCP_port_number: 1-65535
 log_level number: 0-nothing, 1-error log, 2-warning, 3-running status, 4-debug
 TX_PIN - transmitter pin (by wiringpi numbering)
 TR_PIN - receiver pin (by wiringpi numbering)

For view wiringpi numbering pins run command *gpio readall* or see bellow for numbering raspberryPI v3:
 +-----+-----+---------+------+---+---Pi 3---+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 |     |     |    3.3v |      |   |  1 || 2  |   |      | 5v      |     |     |
 |   2 |   8 |   SDA.1 | ALT0 | 1 |  3 || 4  |   |      | 5V      |     |     |
 |   3 |   9 |   SCL.1 | ALT0 | 1 |  5 || 6  |   |      | 0v      |     |     |
 |   4 |   7 | GPIO. 7 |   IN | 1 |  7 || 8  | 1 | ALT5 | TxD     | 15  | 14  |
 |     |     |      0v |      |   |  9 || 10 | 1 | ALT5 | RxD     | 16  | 15  |
 |  17 |   0 | GPIO. 0 |   IN | 1 | 11 || 12 | 0 | IN   | GPIO. 1 | 1   | 18  |
 |  27 |   2 | GPIO. 2 |   IN | 0 | 13 || 14 |   |      | 0v      |     |     |
 |  22 |   3 | GPIO. 3 |   IN | 0 | 15 || 16 | 0 | IN   | GPIO. 4 | 4   | 23  |
 |     |     |    3.3v |      |   | 17 || 18 | 0 | IN   | GPIO. 5 | 5   | 24  |
 |  10 |  12 |    MOSI |   IN | 0 | 19 || 20 |   |      | 0v      |     |     |
 |   9 |  13 |    MISO |   IN | 0 | 21 || 22 | 0 | IN   | GPIO. 6 | 6   | 25  |
 |  11 |  14 |    SCLK |   IN | 0 | 23 || 24 | 1 | IN   | CE0     | 10  | 8   |
 |     |     |      0v |      |   | 25 || 26 | 1 | IN   | CE1     | 11  | 7   |
 |   0 |  30 |   SDA.0 |  OUT | 0 | 27 || 28 | 1 | IN   | SCL.0   | 31  | 1   |
 |   5 |  21 | GPIO.21 |   IN | 1 | 29 || 30 |   |      | 0v      |     |     |
 |   6 |  22 | GPIO.22 |   IN | 1 | 31 || 32 | 0 | IN   | GPIO.26 | 26  | 12  |
 |  13 |  23 | GPIO.23 |   IN | 0 | 33 || 34 |   |      | 0v      |     |     |
 |  19 |  24 | GPIO.24 |   IN | 0 | 35 || 36 | 1 | OUT  | GPIO.27 | 27  | 16  |
 |  26 |  25 | GPIO.25 |   IN | 0 | 37 || 38 | 0 | OUT  | GPIO.28 | 28  | 20  |
 |     |     |      0v |      |   | 39 || 40 | 0 | IN   | GPIO.29 | 29  | 21  |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+---Pi 3---+---+------+---------+-----+-----+

PS: rigt columb is wPi

## run as daemon (service)
#### Start
*/etc/init.d/rflink start*

#### Stop
*/etc/init.d/rflink stop*

### Configure
*edit file /etc/rflink.conf*

#### defult values:
port: 5050
log level: 4
PIN_TX: 28
PIN_RX: 29

## Test
1. run with default values */opt/rflink/RFlink* or own config values etc. */opt/rflink/RFlink 5050 28 29 4*
2. run *telnet localhost 5050*

you can see:
> pi@raspberrypi:~ $ telnet localhost 5050
> Trying ::1...
> Trying 127.0.0.1...
> Connected to localhost.
> Escape character is '^]'.
> 20;00;Nodo RadioFrequencyLink - RFLink Gateway V1.1 - R33;

try type:
> 10;PING;

you may get:
> 20;10;PONG;

if RFlink detect RF activity you can get something like that:
> 20;14;FA500;ID=0a00;SWITCH=0a00;CMD=OFF;

for end connection type:
> 10;REBOOT;


## Note
This tested only witch 433MHz receiver, not test transceiver. And tested only with free aviable modules, full version contain much more no tested plugins.
Raspberry pi generate much more eletrict noise than arduino. This is not problem for load CPU, but some time noise random generate corect data for some plugins.


### Autor
Ing. Ondrej Lycka  (ondrej.lycka(at)seznam.cz)
www.seahu.cz

### Licence
GNU
