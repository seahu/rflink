#ifndef _Plugin_h
#define _Plugin_h

#include "Base.h"
/*
#include "./Plugins/Plugin_001.h"
#include "./Plugins/Plugin_002.h"
#include "./Plugins/Plugin_003.h"
#include "./Plugins/Plugin_004.h"
#include "./Plugins/Plugin_005.h"
#include "./Plugins/Plugin_006.h"
#include "./Plugins/Plugin_007.h"
#include "./Plugins/Plugin_008.h"
#include "./Plugins/Plugin_009.h"
#include "./Plugins/Plugin_010.h"
#include "./Plugins/Plugin_011.h"
#include "./Plugins/Plugin_012.h"
#include "./Plugins/Plugin_013.h"
#include "./Plugins/Plugin_014.h"
#include "./Plugins/Plugin_015.h"
#include "./Plugins/Plugin_030.h"
#include "./Plugins/Plugin_031.h"
#include "./Plugins/Plugin_032.h"
#include "./Plugins/Plugin_033.h"
#include "./Plugins/Plugin_034.h"
#include "./Plugins/Plugin_035.h"
#include "./Plugins/Plugin_040.h"
#include "./Plugins/Plugin_041.h"
#include "./Plugins/Plugin_042.h"
#include "./Plugins/Plugin_043.h"
#include "./Plugins/Plugin_044.h"
#include "./Plugins/Plugin_045.h"
#include "./Plugins/Plugin_046.h"
#include "./Plugins/Plugin_048.h"
#include "./Plugins/Plugin_060.h"
#include "./Plugins/Plugin_061.h"
#include "./Plugins/Plugin_062.h"
#include "./Plugins/Plugin_063.h"
#include "./Plugins/Plugin_070.h"
#include "./Plugins/Plugin_071.h"
#include "./Plugins/Plugin_072.h"
#include "./Plugins/Plugin_073.h"
#include "./Plugins/Plugin_074.h"
#include "./Plugins/Plugin_075.h"
#include "./Plugins/Plugin_080.h"
//#include "./Plugins/Plugin_081.h"
#include "./Plugins/Plugin_082.h"
#include "./Plugins/Plugin_090.h"
#include "./Plugins/Plugin_100.h"
#include "./Plugins/Plugin_254.h"
*/

#include "./Plugins/Make_Generated_all_h.h"

//#define PLUGIN_001   

//prototypes
void PluginInit(void);
void PluginTXInit(void);
byte PluginInitCall(byte Function, char *str);
byte PluginTXInitCall(byte Function, char *str);
byte PluginRXCall(byte Function, char *str);
byte PluginTXCall(byte Function, char *str);

// Van alle devices die worden mee gecompileerd, worden in een tabel de adressen opgeslagen zodat hier naar toe gesprongen kan worden
extern boolean (*Plugin_ptr[PLUGIN_MAX])(byte, char*);                                 // Receive plugins
extern byte Plugin_id[PLUGIN_MAX];
extern boolean (*PluginTX_ptr[PLUGIN_TX_MAX])(byte, char*);                            // Transmit plugins
extern byte PluginTX_id[PLUGIN_TX_MAX];

//example Plugin_001.cpp :
//
//#include "Plugin_001.h"
//#ifdef PLUGIN_001
//#include "../../Plugins/Plugin_001.c"
//#endif


#endif
