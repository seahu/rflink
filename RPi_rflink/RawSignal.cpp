#include "RawSignal.h"
// Include libraries for RPi:
#include <string.h> 
#include <stdlib.h> 
#include <wiringPi.h>
// Thread
#include <pthread.h>
// My header files
#include "Base.h"
#include "Plug.h" // for PluginRXCall() and 


bool enableRawScan=true; // flag for end RawSignal thread

bool ReceiverInterrupt=false; // flag signalzet enabled or disabled interupt (exist because for Raspberry Pi (wiringPi) you can't unregister the ISR)
pthread_cond_t thread_flag_cv;
pthread_mutex_t thread_flag_mutex;
pthread_t threadScanEvent_id; // struct for id of scanevent theader
bool log_repeat=true; // pro ucely ladeni pozdeji smazat

// Oreginaly this function was run cycle with timeout so that can be serve serial line input
// Now run continualy with waiting for new data who get from function calling by interrupt.
// So that consume min. resources for wait is used conditional variable and whole function ScanEvent is execute in separate thread/ 
boolean ScanEvent() {                                         // Deze routine maakt deel uit van de hoofdloop en wordt iedere 125uSec. doorlopen
	enableRawScan=true;
	// init mutex and conditional varibale
	pthread_mutex_init(&thread_flag_mutex, NULL);
	pthread_cond_init(&thread_flag_cv, NULL);
	// enable Receive (interupt)
	enableReceive(); // use only for start iterupt receivre (can't use reparetly becouse for Raspberry Pi (wiringPi) you can't unregister the ISR)
	pthread_mutex_lock(&thread_flag_mutex);
	while (enableRawScan==true){
		log(LOG_HARD_DEBUG,"RawSignal: Wait for packet.");
		pthread_cond_wait(&thread_flag_cv, &thread_flag_mutex);		// wait for new packed
		ReceiverInterrupt=false;									// disable Receive
		if ( PluginRXCall(0,0) ) {									// Check all plugins to see which plugin can handle the received signal.
			RepeatingTimer=millis()+SIGNAL_REPEAT_TIME;
		}
		ReceiverInterrupt=true;										// anable Receivre
	}
	disableReceive();
	pthread_mutex_unlock(&thread_flag_mutex);
}

// prepare scanEvent function to be main function for new theader (must have right format and argument)
void* p_scanEvent(void* unused){
	ScanEvent();
}

// create theader with function &p_ScanEvent
int StartScanEventTheader(){
	pthread_create(&threadScanEvent_id, NULL, &p_scanEvent, NULL);
	log(LOG_STATUS,"RawSignal: Scan event theader started."); 
}

// Stop theader
int StopScanEventTheader(){
	enableRawScan=false;
	setRawSignal(0); // send signal to unblock conditional variable (for anable end ScanEvent cycle)
	pthread_join(threadScanEvent_id, NULL);
	log(LOG_STATUS,"RawSignal: Scan event theader stoped."); 
}

// Process sampling data.
// Because proces samples data is time consuming and function for serve interrupt must by very short.
// This function only fill in RawSignal struture and unlock condition varible used in function ScanEvent() who do main work for process data.
void setRawSignal(int RawCodeLength){
	sprintf(pbuffer,"Spracovani paketu delky:: %d",RawCodeLength);
	log(LOG_DEBUG,pbuffer);
	
	pthread_mutex_lock(&thread_flag_mutex);
	
	RawSignal.Repeats=0;                                                      // no repeats
	RawSignal.Multiply=RAWSIGNAL_SAMPLE_RATE;                                 // sample size.
	RawSignal.Number=RawCodeLength-1;                                         // Number of received pulse times (pulsen *2)
	RawSignal.Pulses[RawSignal.Number+1]=0;                                   // Last element contains the timeout. 
	RawSignal.Time=millis();                                                  // Time the RF packet was received (to keep track of retransmits
	//RawCodeLength=1;															// Start at 1 for legacy reasons. Element 0 can be used to pass special information like plugin number etc.
	RawCodeLength=0;															// Flag for my hook reduces occurrence of packet with noise on beginning.
	//place for threader conditions set
	pthread_cond_signal(&thread_flag_cv);

	pthread_mutex_unlock(&thread_flag_mutex);
	return;
}	

// handle interrupt
void handleInterrupt() {
	if (ReceiverInterrupt==false) return;										// if no enabled interrupt fast end
	static unsigned long lastTime = 0;
	static unsigned long PulseLength=0L;
	static int RawCodeLength=1;													// Start at 1 for legacy reasons. Element 0 can be used to pass special information like plugin number etc.

	const long time = micros();
	PulseLength = time - lastTime;
	lastTime = time;
	
	//Serial.print("Pulse lenght:");
	//Serial.println(PulseLength);
	//return;

	// reduce repeated signal
	// After a positive packet reception, the packet is assumed to be repeated.
	// Therefore, all subsequent communications into the long signal (significant termination of transmissions) is ignored.
	// In the case of long uninterrupted transmissions, where long signal not present, the SIGNAL_REPEAT_TIME terminates reduction.
	// Some modules do not set RawSignal.Repeats=true and use own solve.
	if (RawSignal.Repeats==true){
		if (log_repeat==true){
			log(LOG_DEBUG,"RawSignal: RawSignal.Repeats=true.");
			log_repeat=false;
		}
		//if ((RawSignal.Time+SIGNAL_REPEAT_TIME)>millis()) { 
		if (RepeatingTimer>millis()) { 
			if (PulseLength<SIGNAL_TIMEOUT*1000*4) { // much more than SIGNAL_TIMEOUT(=len_sync_pulse) detec end of transmit
				//log(LOG_STATUS,"return back from interput."); 
				return;
			}
			log(LOG_DEBUG,"RawSignal: end Repeats by log impuls"); 
			RawSignal.Repeats=false;
			RawCodeLength=1;
		}
		else {
			log(LOG_DEBUG,"RawSignal: end Repeats by end REPEAT_TIME"); 
			RawSignal.Repeats=false;
			RawCodeLength=1;
		}		
	}
	
	//Every devices send data in packets whith diferent format. This packets is consecutive repeates more times, for better 
	//probability sucessfully receive packet. But still never you have assurance 100%, that packet was sucessfully receive.
	//Problem is corect detect start point of packet, because when nobody transmit data, receiver is set to max sensitivy 
	//and receivere noise. After receive corect signal, recever reduce sensitivy and receivre only this signal. 
	//Standar packet uses start or stop synchronizatinou pulses who is more leng then data signals and can be used for detect packet.
	//Unfortunately most devices use zero level synchronization pulse, this is reason why can not be detec 
	//beginning first packet but alse secound.
	//This hook use RawCodeLength=0 as flag for wait into sync impuls. This reduces occurrence of packet with noise on beginning.
	if ( PulseLength < MIN_PULSE_LENGTH) { // puls is so short -> this is no packet or time out
		//Serial.println("S");
		RawCodeLength=0;
		return;
	}

	if ( RawCodeLength<RAW_BUFFER_SIZE && RawCodeLength!=0) {
		RawSignal.Pulses[RawCodeLength++]=4+PulseLength/(unsigned long)(RAWSIGNAL_SAMPLE_RATE); // store in RawSignal !!!!  (+4 is corection beatwen arduino cycle time calculate and real time from raspberryPi)
	}
	
	if ( RawCodeLength>=RAW_BUFFER_SIZE) { // Raw data reach max size (too mach long for corect data)
		//setRawSignal(RawCodeLength);
		RawCodeLength=0;
		return;
	}

	if ( PulseLength >= SIGNAL_TIMEOUT*1000 ) {
		//Serial.println("S");
		if (RawCodeLength>=MIN_RAW_PULSES) { // Raw data satisfy min. size
			setRawSignal(RawCodeLength);
			RawCodeLength=0;
		}
		else { // frist sync pulse
			RawCodeLength=1;		// Start at 1 for legacy reasons and indicate start new scanining
		}
		return;
	}
}

/**
 * Enable receiving data (use only for start interrupt receivre)
 */
void enableReceive() {
	ReceiverInterrupt=true;
	RawSignal.Number=0;
	RawSignal.Repeats=0;
	RawSignal.Delay=0;                                                                   // Delay in ms. after transmit of a single RF pulse packet
	RawSignal.Multiply=0;                                                                // Pulses[] * Multiply is the real pulse time in microseconds 
	RawSignal.Time=0;                                                           // Timestamp indicating when the signal was received (millis())
	RawSignal.Pulses[0]=0;
	log(LOG_HARD_DEBUG,"RawSignal: Enable interrupt.");
	wiringPiISR(PIN_RF_RX_DATA, INT_EDGE_BOTH, &handleInterrupt);
}

/**
 * Disable receiving data
 */
void disableReceive() {
	ReceiverInterrupt=false;
	// For Raspberry Pi (wiringPi) you can't unregister the ISR
}

/*********************************************************************************************/
// RFLink Board specific: Generate a short pulse to switch the Aurel Transceiver from TX to RX mode.
void RFLinkHW( void ) {
     delayMicroseconds(36);
     digitalWrite(PIN_BSF_0,LOW);
     delayMicroseconds(16);
     digitalWrite(PIN_BSF_0,HIGH);
     return;
}


/*********************************************************************************************\
 * Send rawsignal buffer to RF  * DEPRICATED * DO NOT USE *
\*********************************************************************************************/
void RawSendRF(void) {                                                    // * DEPRICATED * DO NOT USE *
	int x;
	disableReceive();
	RawSignal.Pulses[RawSignal.Number]=1;                                   // due to a bug in Arduino 1.0.1

	for(byte y=0; y<RawSignal.Repeats; y++) {                               // herhaal verzenden RF code
		x=1;
		//noInterrupts();
		while(x<RawSignal.Number) {
			digitalWrite(PIN_RF_TX_DATA,HIGH);
			delayMicroseconds(RawSignal.Pulses[x++]*RawSignal.Multiply-5);    // min een kleine correctie  
			digitalWrite(PIN_RF_TX_DATA,LOW);
			delayMicroseconds(RawSignal.Pulses[x++]*RawSignal.Multiply-7);    // min een kleine correctie
		}
		//interrupts();
		if (y+1 < RawSignal.Repeats) delay(RawSignal.Delay);                 // Delay buiten het gebied waar de interrupts zijn uitgeschakeld! Anders werkt deze funktie niet.
	}
	enableReceive();
}
/*********************************************************************************************/
