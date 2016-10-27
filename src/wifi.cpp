#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "WiFiManager.h"

#include <DNSServer.h>

DNSServer dnsServer;
#define DNS_PORT 53

extern void wifiConnect() {

	if (0) {

		WiFiManager wifiManager;
		wifiManager.autoConnect("esp-remote");

	} else {

	    WiFi.mode(WIFI_AP_STA);
	    IPAddress apIP(192, 168, 1, 1);
	    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
	    WiFi.softAP("esp-remote");

		delay(500);

		Serial.print("\nAP IP address: ");
		Serial.println(WiFi.softAPIP());

		dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
	  	dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

	}
	Serial.println("WIFI: Connected...");
 
}


extern void dnsListen() {
	dnsServer.processNextRequest();
}
