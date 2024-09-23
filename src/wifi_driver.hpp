#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
int statusChecker(void);
void checkWifiStatus(void);
void initServerAndWebPage(void);
void initWifi(void);

extern bool ledOn;
extern WebSocketsServer webSocket;
extern ESP8266WebServer server;

extern const char *ssid;
extern const char *password;
