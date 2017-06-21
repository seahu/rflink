#ifndef _base_h
#define _base_h

// C style headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
// C++ style headers
#include <iostream>     // std::cout, std::ios
#include <string>
// My headers
#include "./arduino/types.h"
#include "./arduino/binary.h"
#include "./arduino/EmulateArduino.h"
#include "Misc.h"
#include "RawSignal.h"

#define WELCOME							"20;00;Nodo RadioFrequencyLink - RFLink Gateway V1.1 - "
#define DEFAULT_TCP_PORT				5050									// defaul TCP port for TCP server
#define BUILDNR                         0x07                                    // shown in version
#define REVNR                           0x33                                    // shown in version and startup string
#define MIN_RAW_PULSES                    20                                    // =8 bits. Minimal number of bits*2 that need to have been received before we spend CPU time on decoding the signal.
#define RAWSIGNAL_SAMPLE_RATE             30                                    // Sample width / resolution in uSec for raw RF pulses.
#define MIN_PULSE_LENGTH                  25                                    // Pulses shorter than this value in uSec. will be seen as garbage and not taken as actual pulses.
#define SIGNAL_TIMEOUT                    4                             		// Timeout, after this time in mSec. the RF signal will be considered to have stopped.
#define SIGNAL_REPEAT_TIME              1000                                    // Time in mSec. in which the same RF signal should not be accepted again. Filters out retransmits.
#define BAUD                           57600                                    // Baudrate for serial communication.
#define TRANSMITTER_STABLE_DELAY         500                                    // delay to let the transmitter become stable (Note: Aurel RTX MID needs 500µS/0,5ms).
#define RAW_BUFFER_SIZE                  512                                    // Maximum number of pulses that is received in one go.
#define PLUGIN_MAX                        55                                    // Maximum number of Receive plugins
#define PLUGIN_TX_MAX                     26                                    // Maximum number of Transmit plugins
#define SCAN_HIGH_TIME                    50                                    // tijdsinterval in ms. voor achtergrondtaken snelle verwerking
#define FOCUS_TIME                        50                                    // Duration in mSec. that, after receiving serial data from USB only the serial port is checked. 
#define INPUT_COMMAND_SIZE                60                                    // Maximum number of characters that a command via serial can be.
#define PRINT_BUFFER_SIZE                 60                                    // Maximum number of characters that a command should print in one go via the print buffer.
#define BUFSIZE 						1000									// Max size for InputBuffer_Serial and max size incoming packet from TCP server

// to include Config_xx.c files:
#define stringify(x) #x
#define CONFIGFILE2(a, b) stringify(a/Config/b)
#define CONFIGFILE(a, b) CONFIGFILE2(a, b)
#define CONFIG_FILE Config_02.c
#define SKETCH_PATH ../
#include CONFIGFILE(SKETCH_PATH,CONFIG_FILE)

#define VALUE_PAIR                      44
#define VALUE_ALLOFF                    55
#define VALUE_OFF                       74
#define VALUE_ON                        75
#define VALUE_DIM                       76
#define VALUE_BRIGHT                    77
#define VALUE_UP                        78
#define VALUE_DOWN                      79
#define VALUE_STOP                      80
#define VALUE_CONFIRM                   81
#define VALUE_LIMIT                     82
#define VALUE_ALLON                     141

// PIN Definition 
// This pin is not the first pin on the RPi GPIO header!
// Consult https://projects.drogon.net/raspberry-pi/wiringpi/pins/
// for more information.
#define PIN_BSF_0                   27                                          // Board Specific Function lijn-0
#define PIN_BSF_1                   27                                          // Board Specific Function lijn-1
#define PIN_BSF_2                   27                                          // Board Specific Function lijn-2
#define PIN_BSF_3                   27                                          // Board Specific Function lijn-3
#define PIN_RF_TX_VCC               27                                          // +5 volt / Vcc power to the transmitter on this pin
#define DEFAUL_PIN_RF_TX_DATA       28                                   		// Data to the 433Mhz transmitter on this pin (wiringpi numbering)
#define PIN_RF_RX_VCC               27                                          // Power to the receiver on this pin
#define DEFAULT_PIN_RF_RX_DATA      29                                  		// On this input, the 433Mhz-RF signal is received. LOW when no signal. (wiringpi numbering)

#define LOG_NOTHING						0
#define LOG_ERROR						1
#define LOG_WARNING						2
#define LOG_STATUS						3
#define LOG_DEBUG						4
#define LOG_HARD_DEBUG					5

struct RawSignalStruct                                                          // Raw signal variabelen places in a struct
  {
  int  Number;                                                                  // Number of pulses, times two as every pulse has a mark and a space.
  byte Repeats;                                                                 // Number of re-transmits on transmit actions.
  byte Delay;                                                                   // Delay in ms. after transmit of a single RF pulse packet
  byte Multiply;                                                                // Pulses[] * Multiply is the real pulse time in microseconds 
  unsigned long Time;                                                           // Timestamp indicating when the signal was received (millis())
  byte Pulses[RAW_BUFFER_SIZE+2];                                               // Table with the measured pulses in microseconds divided by RawSignal.Multiply. (halves RAM usage)                                                                              // First pulse is located in element 1. Element 0 is used for special purposes, like signalling the use of a specific plugin
};

extern struct RawSignalStruct RawSignal;

extern byte PIN_RF_TX_DATA;														// Data to the 433Mhz transmitter on this pin (wiringpi numbering)
extern byte PIN_RF_RX_DATA;														// On this input, the 433Mhz-RF signal is received. LOW when no signal. (wiringpi numbering)

extern unsigned long RepeatingTimer;
extern unsigned long SignalCRC;                                                     // holds the bitstream value for some plugins to identify RF repeats
extern unsigned long SignalHash;                                                    // holds the processed plugin number
extern unsigned long SignalHashPrevious;                                            // holds the last processed plugin number

//****************************************************************************************************************************************
extern byte PKSequenceNumber;                                                        // 1 byte packet counter
extern boolean RFDebug;                                                          // debug RF signals with plugin 001 
extern boolean RFUDebug;                                                         // debug RF signals with plugin 254 
extern boolean QRFDebug;                                                         // debug RF signals with plugin 254 but no multiplication
extern byte logLevel;															// log level (0-nothing, 1-error log, 2-warning, 3-running status, 4-debug)

extern char pbuffer[PRINT_BUFFER_SIZE];											// Buffer for printing data
extern char InputBuffer_Serial[BUFSIZE];                              // Buffer for input data this name is uset in PluginTX_XXX function therefore this is alias to inBuf from server.cpp


// serial port emulation for print and write functions
extern Ser Serial;

// protoype log function
void log(int level, std::string str, boolean endLine);							// log withou endline
void log(int level, std::string str);											// log with endline

//functions prototype for serve incoming packet
int serve_input(char* InputBuffer_Serial);

#endif
