! ============================================================================
! Only for educational purposes, this source is not be synchronized with the
! latest release! 
! For normal operation, use the RFLink Loader that includes the latest release
! This source code tree is made available so that you can develop your own plugins
! ============================================================================

Please note that the RFLink Gateway is a freeware project.   
Stuntteam is not making money in any way.   
This means that there are no unlimited funds to purchase test devices,   
it also means the project has to rely on you, the user, to send debug data.  
  
If you want to contribute to this project, you can send a donation which is more than welcome (see www.nemcon.nl/blog2 donation button),   
or help with sending debug data of unsupported devices (you can even write and contribute plugins and/or code fixes),  
or donate the device that you would like to see supported.  
   
Right now we are looking for some older remotes and/or switches.  
Like for example: Impuls, Ikea Koppla, Powerfix, Blyss, Home Confort, Conrad, Kambrook, Everflourish 
For the implementation of the planned 2.4Ghz support we could use some simple MySensor devices.   
For the implementation of the planned 868Mhz support we could use some devices as well.   
If you have anything that you do not use, send a mail to frankzirrone@gmail.com   
Thanks in advance!  

------------------------
Synology NAS:  
If you want to use RFLink with a Synology NAS you can use:  
- an Arduino Mega clone based on CH340 USB/serial chip  
In all other cases:  
- connect a 10 uF capacitor between reset and ground on the Arduino.   
  Simply stick the pins of the capacitor in the power connector socket.  
  When you want to update the firmware of the Arduino, remove the capacitor and reconnect it when done.   
  For details about the Domoticz Synology package check out: http://www.jadahl.com  
------------------------
RFlink via Network Connection:   
It is possible to use RFlink via a network connection using ser2net.   
------------------------   
You can now use the RFLink Gateway with the following home automation software:   
Domoticz   
Jeedom   
------------------------   
