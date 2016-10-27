#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>



extern void startSocketSrv();
extern void wifiConnect();
extern void dnsListen();
extern void socketSend(const char *);




#define RED 15
#define GREEN 12
#define BLUE 13

extern void color(uint32 c) {
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);

    analogWrite(RED, ((c&0xff0000)>>16)<<2);
    analogWrite(GREEN, ((c&0xff00)>>8)<<2);
    analogWrite(BLUE, ((c&0xff)<<2));

}

void MDNSConnect() {  
    if (!MDNS.begin("gleds")) {
        Serial.println("Error setting up MDNS responder!");
    }
    Serial.println("mDNS responder started");
    MDNS.addService("http", "tcp", 80);
}


void setup() {

    color(0xff);
    Serial.begin(115200);
    Serial.println("Starting");
    ArduinoOTA.setHostname("esp-captive-app");
    ArduinoOTA.setPassword((const char *)"set-your-passord");
    ArduinoOTA.begin();
    MDNSConnect();

    color(0xff0000);
    wifiConnect();
    color(0xff00);

    startSocketSrv();

}

unsigned long startPressTime = 0;
boolean isButtonPressed = false;
boolean pressEventSent = false;

unsigned long lastDiodeEvent = 0;

void loop() {
    ArduinoOTA.handle();
    dnsListen();

    int pressed = digitalRead(4);
    unsigned long t = millis();
    if (pressed && !startPressTime) {
        startPressTime = t;
        pressEventSent = false;
    } else if (pressed && startPressTime) {
        if (t-startPressTime>100) {
            if (!pressEventSent) {
                pressEventSent = true;
                socketSend("press");

            }
        }
    } else {
        startPressTime = 0;
    }




}
