#ifndef __SER_BRIDGE_H__
#define __SER_BRIDGE_H__

#include <ip_addr.h>
#include <c_types.h>
#include <espconn.h>

#define MAX_CONN 4
#define SER_BRIDGE_TIMEOUT 300 // 300 seconds = 5 minutes

// Send buffer size
#define MAX_TXBUFFER (2*1460)

enum connModes {
  cmInit = 0,        // initialization mode: nothing received yet
  cmPGMInit,         // initialization mode for programming
  cmTransparent,     // transparent mode
  cmAVR1,            // Arduino/AVR programming mode: got 1st GET_SYNC
  cmAVR2,            // Arduino/AVR programming mode: got 2nd GET_SYNC
  cmAVR3,            // Arduino/AVR programming mode: normal transparent state
  cmARM,             // ARM/other programming mode
  cmTelnet,          // use telnet escape sequences for programming mode
};

typedef struct serbridgeConnData {
	struct espconn *conn;
	enum connModes conn_mode;     // connection mode
  uint8_t        telnet_state;
	uint16         txbufferlen;   // length of data in txbuffer
	char           *txbuffer;     // buffer for the data to send
  char           *sentbuffer;   // buffer sent, awaiting callback to get freed
  uint32_t       txoverflow_at; // when the transmitter started to overflow
	bool           readytosend;   // true, if txbuffer can be sent by espconn_sent
} serbridgeConnData;

// port1 is transparent&programming, second port is programming only
void ICACHE_FLASH_ATTR serbridgeInit(int port1, int port2);
void ICACHE_FLASH_ATTR serbridgeInitPins(void);
void ICACHE_FLASH_ATTR serbridgeUartCb(char *buf, short len);
void ICACHE_FLASH_ATTR serbridgeReset();

#endif /* __SER_BRIDGE_H__ */
