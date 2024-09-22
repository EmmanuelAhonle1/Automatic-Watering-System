#include <WebSocketsServer.h>
#include <ESP8266WebServer.h>
int statusChecker(void);
void checkWifiStatus(void);
void initWiFiChecker(void);
void initServerAndWebPage(void);

extern bool ledOn;
extern WebSocketsServer webSocket;
extern ESP8266WebServer server;

extern const char *ssid;
extern const char *password;
