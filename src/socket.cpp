#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer   server(80);
AsyncWebSocket   ws("/ws");
AsyncEventSource events("/events");

extern void color(uint32 c);

#include <list>
using namespace std;
typedef list<AsyncWebSocketClient *> ClientList;
ClientList clients;

uint32 currentColor = 0xffffff;


extern void socketSend(const char * event) {

    for (ClientList::const_iterator client = clients.begin(); client != clients.end(); ++client) {
        (*client)->printf(event);
    }

}

void onWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len){

    if(type == WS_EVT_CONNECT){

        Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
        clients.push_back(client);
        client->printf("Hello Client %u :)", client->id());
        client->ping();
        color(0xff);


    } else if(type == WS_EVT_DISCONNECT){

        Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
        clients.remove(client);
        if (clients.size()==0) color(0xff00);

    } else if(type == WS_EVT_ERROR){

        Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);

    } else if(type == WS_EVT_PONG){

        Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");

    } else if(type == WS_EVT_DATA){

        AwsFrameInfo * info = (AwsFrameInfo*)arg;
        String msg = "";

        if(info->final && info->index == 0 && info->len == len) {

            Serial.printf("ws[%s][%u] %s-message: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary");

            if(info->opcode == WS_TEXT){
                for(size_t i=0; i < info->len; i++) {
                    msg += (char) data[i];
                }
                Serial.printf("%s",msg.c_str());

                if (msg.startsWith("diode")) {

                    client->printf("diode:%d", analogRead(0));

                 } else if (msg.startsWith("led:")) {

                    if (msg.startsWith("led:r:")) {

                        uint32 red = msg.substring(6).toInt();
                        currentColor = (currentColor&0x00ffff)|(red<<16);

                    } else if (msg.startsWith("led:g:")) {

                        uint32 green = msg.substring(6).toInt();
                        currentColor = (currentColor&0xff00ff)|(green<<8);

                    } else if (msg.startsWith("led:b:")) {

                        uint32 blue = msg.substring(6).toInt();
                        currentColor = (currentColor&0xffff00)|(blue<<0);
                    }

                    color(currentColor);
                    Serial.printf(" color:0x%06x ",currentColor);

                    char event[20];
                    sprintf(event,"led:%06x",currentColor);
                    socketSend(event);
                    // client->printf("led:%06x", currentColor);

                } else {

                    Serial.printf(" -- Unknown message");

                }

            }
            Serial.printf("\n");

        }

    }
}


String responseHTML = "<script type='text/javascript'>document.location.href = '/';</script>";

extern void startSocketSrv() {

	SPIFFS.begin();
	server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.htm");

    ws.onEvent(onWsEvent);
    server.addHandler(&ws);


    events.onConnect([](AsyncEventSourceClient *client){
        client->send("hello!",NULL,millis(),1000);
    });

    server.addHandler(&events);

    server.on("/heap", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", String(ESP.getFreeHeap()));
    });

    server.onNotFound([](AsyncWebServerRequest *request){
        request->send(200, "text/html", responseHTML);
    });

    server.begin();
}