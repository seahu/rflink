#ifndef _RawSignal_h
#define _RawSignal_h

#include "./arduino/types.h"

// flag for end RawSignal thread
extern bool enableRawScan; 

//global use prototypes:
int StartScanEventTheader();
int StopScanEventTheader();
void RFLinkHW( void );
void RawSendRF(void);

//local use prototypes:
void enableReceive();
void disableReceive();
boolean ScanEvent();
void setRawSignal(int RawCodeLength);

#endif
