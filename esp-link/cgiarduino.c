#include <esp8266.h>
#include "cgi.h"
#include "espfs.h"
#include "config.h"
#include "serled.h"
#include "status.h"
#include "serbridge.h"
#include "uart.h"
#include "slip.h"
#include "cgiarduino.h"

#ifdef REST_DBG
#define DBG_REST(format, ...) os_printf(format, ## __VA_ARGS__)
#else
#define DBG_REST(format, ...) do { } while(0)
#endif

int ICACHE_FLASH_ATTR cgiArduinoGet(HttpdConnData *connData) {
  if (connData->conn==NULL) return HTTPD_CGI_DONE; // Connection aborted

  char buff[128];
  int len = 0;
	
	UART_FORCE = 1;

	while(ready != 1){
	}

		len = os_sprintf(buff,"%s", buff_console);
		ready = 0;
  		httpdSend(connData, buff, len);
		UART_FORCE = 0;
  		return HTTPD_CGI_DONE;

}


int ICACHE_FLASH_ATTR cgiArduinoSet(HttpdConnData *connData) {
  if (connData->conn==NULL) return HTTPD_CGI_DONE; // Connection aborted
	DBG_REST("ARDUINO REST: cgiArduinoSet POST enter\n");
	uart0_write_char('B');
  return HTTPD_CGI_DONE;
}



int ICACHE_FLASH_ATTR cgiArduino(HttpdConnData *connData) {
  if (connData->conn==NULL) return HTTPD_CGI_DONE; // Connection aborted. Clean up.
  if (connData->requestType == HTTPD_METHOD_GET) {

    	DBG_REST("ARDUINO REST content: %s \n", connData->url);

	for(int i=0; i < strlen(connData->url); i++)
        uart0_write_char(connData->url[i]);
	uart0_write_char('\0');

    	return cgiArduinoGet(connData);


  } else if (connData->requestType == HTTPD_METHOD_POST) {
    return cgiArduinoSet(connData);
  } else {
    jsonHeader(connData, 404);
    return HTTPD_CGI_DONE;
  }
}
