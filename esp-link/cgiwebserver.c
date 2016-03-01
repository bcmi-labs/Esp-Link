#include <esp8266.h>
#include "cgi.h"
#include "espfs.h"
#include "config.h"
#include "serled.h"
#include "status.h"
#include "serbridge.h"
#include "uart.h"
#include "slip.h"
#include "cgiwebserver.h"

#ifdef REST_DBG
#define DBG_REST(format, ...) os_printf(format, ## __VA_ARGS__)
#else
#define DBG_REST(format, ...) do { } while(0)
#endif

int ICACHE_FLASH_ATTR cgiArduinoWebGet(HttpdConnData *connData) { // IMP: 480 chars max for reply (6*80) and min 160 chars

  if (connData->conn==NULL) return HTTPD_CGI_DONE; // Connection aborted

  char buff[1024];
  int len = 0;
  char tmp[81];
	
	UART_FORCE = 1;

	while(ready != 1){
	}
		len = os_sprintf(tmp,"%s", buff_console);
		os_strcpy(buff, tmp);
		ready = 0;
		for(int i=0; i<80; i++) buff_console[i] = '\0' ;

for(int j=0; j < 5; j++){
	while(ready != 1){
	}
		len = len + os_sprintf(tmp,"%s", buff_console);
		os_strcat(buff, tmp);
		ready = 0;
		for(int i=0; i<80; i++){ if(buff_console[i] == '~') j=5;  buff_console[i] = '\0' ; }
}
/*
	while(ready != 1){
	}
		len = len + os_sprintf(tmp,"%s", buff_console);
		os_strcat(buff, tmp);
		ready = 0;
		for(int i=0; i<80; i++) buff_console[i] = '\0' ;

	while(ready != 1){
	}
		len = len + os_sprintf(tmp,"%s", buff_console);
		os_strcat(buff, tmp);
		ready = 0;
		for(int i=0; i<80; i++) buff_console[i] = '\0' ;

*/
		//DBG_REST("ARDUINO REST len: %d \n", len);

  		httpdSend(connData, buff , len);
		UART_FORCE = 0;
  		return HTTPD_CGI_DONE;

}


int ICACHE_FLASH_ATTR cgiArduinoWebSet(HttpdConnData *connData) {
  if (connData->conn==NULL) return HTTPD_CGI_DONE; // Connection aborted
	DBG_REST("ARDUINO REST: cgiArduinoSet POST enter\n");
	uart0_write_char('B');
  return HTTPD_CGI_DONE;
}



int ICACHE_FLASH_ATTR cgiWebserver(HttpdConnData *connData) {
  if (connData->conn==NULL) return HTTPD_CGI_DONE; // Connection aborted. Clean up.
  if (connData->requestType == HTTPD_METHOD_GET) {

    	DBG_REST("ARDUINO REST content: %s \n", connData->url);

	for(int i=0; i < strlen(connData->url); i++)
        uart0_write_char(connData->url[i]);
	uart0_write_char('\0');

    	return cgiArduinoWebGet(connData);


  } else if (connData->requestType == HTTPD_METHOD_POST) {
    return cgiArduinoWebSet(connData);
  } else {
    jsonHeader(connData, 404);
    return HTTPD_CGI_DONE;
  }
}
