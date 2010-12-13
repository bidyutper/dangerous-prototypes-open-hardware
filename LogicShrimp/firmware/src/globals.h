#ifndef GLOBALS_H
#define GLOBALS_H


typedef unsigned char u8;
typedef unsigned int u16;

// built in headers
#include <p18cxxx.h>
//#include <spi.h>

// =================== GLOBAL HEADERS
#include "HardwareProfile.h"
#include "GenericTypeDefs.h"
#include "Compiler.h"

#include "usb_config.h"
//USB stack
#include "./USB/usb.h"
#include "./USB/usb_function_cdc.h"
#include "./USB/usb_device.h"


// user definer headers
#include "hal_sram.h"
#include "hal_logicshrimp.h"

#define OEON 1
#define OEOFF 0



#endif
