#ifndef LabeebIoT_h
#define LabeebIoT_h

#include <Arduino.h>
#include "IPAddress.h"
#include "Client.h"
#include "Stream.h"

#define MQTT_VERSION_3_1      3
#define MQTT_VERSION_3_1_1    4

// MQTT_VERSION : Pick the version
//#define MQTT_VERSION MQTT_VERSION_3_1
#ifndef MQTT_VERSION
#define MQTT_VERSION MQTT_VERSION_3_1_1
#endif

// MQTT_MAX_PACKET_SIZE : Maximum packet size
#ifndef MQTT_MAX_PACKET_SIZE
#define MQTT_MAX_PACKET_SIZE 1024
#endif

// MQTT_KEEPALIVE : keepAlive interval in Seconds
#ifndef MQTT_KEEPALIVE
#define MQTT_KEEPALIVE 15
#endif

// MQTT_SOCKET_TIMEOUT: socket timeout interval in Seconds
#ifndef MQTT_SOCKET_TIMEOUT
#define MQTT_SOCKET_TIMEOUT 15
#endif

// MQTT_MAX_TRANSFER_SIZE : limit how much data is passed to the network client
//  in each write call. Needed for the Arduino Wifi Shield. Leave undefined to
//  pass the entire MQTT packet in each write call.
//#define MQTT_MAX_TRANSFER_SIZE 80

// Possible values for client.state()
#define MQTT_CONNECTION_TIMEOUT     -4
#define MQTT_CONNECTION_LOST        -3
#define MQTT_CONNECT_FAILED         -2
#define MQTT_DISCONNECTED           -1
#define MQTT_CONNECTED               0
#define MQTT_CONNECT_BAD_PROTOCOL    1
#define MQTT_CONNECT_BAD_CLIENT_ID   2
#define MQTT_CONNECT_UNAVAILABLE     3
#define MQTT_CONNECT_BAD_CREDENTIALS 4
#define MQTT_CONNECT_UNAUTHORIZED    5

#define MQTTCONNECT     1 << 4  // Client request to connect to Server
#define MQTTCONNACK     2 << 4  // Connect Acknowledgment
#define MQTTPUBLISH     3 << 4  // Publish message
#define MQTTPUBACK      4 << 4  // Publish Acknowledgment
#define MQTTPUBREC      5 << 4  // Publish Received (assured delivery part 1)
#define MQTTPUBREL      6 << 4  // Publish Release (assured delivery part 2)
#define MQTTPUBCOMP     7 << 4  // Publish Complete (assured delivery part 3)
#define MQTTSUBSCRIBE   8 << 4  // Client Subscribe request
#define MQTTSUBACK      9 << 4  // Subscribe Acknowledgment
#define MQTTUNSUBSCRIBE 10 << 4 // Client Unsubscribe request
#define MQTTUNSUBACK    11 << 4 // Unsubscribe Acknowledgment
#define MQTTPINGREQ     12 << 4 // PING Request
#define MQTTPINGRESP    13 << 4 // PING Response
#define MQTTDISCONNECT  14 << 4 // Client is Disconnecting
#define MQTTReserved    15 << 4 // Reserved

#define MQTTQOS0        (0 << 1)
#define MQTTQOS1        (1 << 1)
#define MQTTQOS2        (2 << 1)

#ifdef ESP8266
#include <functional>
#define MQTT_CALLBACK_SIGNATURE std::function<void(char*, uint8_t*, unsigned int)> callback
#else
#define MQTT_CALLBACK_SIGNATURE void (*callback)(char*, uint8_t*, unsigned int)
#endif

class LabeebIoT {
private:
   Client* _client;
   uint8_t buffer[MQTT_MAX_PACKET_SIZE];
   uint16_t nextMsgId;
   unsigned long lastOutActivity;
   unsigned long lastInActivity;
   bool pingOutstanding;
   MQTT_CALLBACK_SIGNATURE;
   uint16_t readPacket(uint8_t*);
   boolean readByte(uint8_t * result);
   boolean readByte(uint8_t * result, uint16_t * index);
   boolean write(uint8_t header, uint8_t* buf, uint16_t length);
   uint16_t writeString(const char* string, uint8_t* buf, uint16_t pos);
   IPAddress ip;
   const char* domain;
   const char* enterprise;
   const char* templateID;
   uint16_t port;
   Stream* stream;
   int _state;
public:
   LabeebIoT();
   LabeebIoT(Client& client);
   LabeebIoT(IPAddress, uint16_t, Client& client);
   LabeebIoT(IPAddress, uint16_t, Client& client, Stream&);
   LabeebIoT(IPAddress, uint16_t, MQTT_CALLBACK_SIGNATURE,Client& client);
   LabeebIoT(IPAddress, uint16_t, MQTT_CALLBACK_SIGNATURE,Client& client, Stream&);
   LabeebIoT(uint8_t *, uint16_t, Client& client);
   LabeebIoT(uint8_t *, uint16_t, Client& client, Stream&);
   LabeebIoT(uint8_t *, uint16_t, MQTT_CALLBACK_SIGNATURE,Client& client);
   LabeebIoT(uint8_t *, uint16_t, MQTT_CALLBACK_SIGNATURE,Client& client, Stream&);
   LabeebIoT(const char*, uint16_t, Client& client);
   LabeebIoT(const char*, uint16_t, Client& client, Stream&);
   LabeebIoT(const char*, uint16_t, MQTT_CALLBACK_SIGNATURE,Client& client);
   LabeebIoT(const char*, uint16_t, MQTT_CALLBACK_SIGNATURE,Client& client, Stream&);
   LabeebIoT(Client& client,const char*, uint16_t,const char*);

   LabeebIoT& setServer(IPAddress ip, uint16_t port);
   LabeebIoT& setServer(uint8_t * ip, uint16_t port);
   LabeebIoT& setServer(const char * domain, uint16_t port);
   LabeebIoT& setCallback(MQTT_CALLBACK_SIGNATURE);
   LabeebIoT& setClient(Client& client);
   LabeebIoT& setStream(Stream& stream);
   LabeebIoT& setEnterprise(const char * enterprise); 

   boolean connect(const char* id);
   boolean connectLabeeb(const char *id, const char *templateID, const char *pass);
   
   boolean connect(const char* id, const char* user, const char* pass);
   boolean connect(const char* id, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage);
   boolean connect(const char* id, const char* user, const char* pass, const char* willTopic, uint8_t willQos, boolean willRetain, const char* willMessage);
   void disconnect();
   boolean publish(const char* topic, const char* payload);
   boolean publishLabeeb(const char* deviceID,const char* DataModel, const char* payload,const char* timestamp);
   boolean publishLabeeb(const char* deviceID,const char* DataModel, const char* payload,const char* timestamp,const char* lon,const char* lat,const char* altt);
   boolean publishLabeebValue(const char* deviceID,const char* DataModel, const char* key,const char* value,const char* timestamp,const char* lon,const char* lat,const char* altt);
   boolean publishLabeebValue(const char* deviceID,const char* DataModel, const char* key,const char* value,const char* timestamp);
   boolean publish(const char* topic, const char* payload, boolean retained);
   boolean publish(const char* topic, const uint8_t * payload, unsigned int plength);
   boolean publish(const char* topic, const uint8_t * payload, unsigned int plength, boolean retained);
   boolean publish_P(const char* topic, const uint8_t * payload, unsigned int plength, boolean retained);
   boolean subscribe(const char* topic);
   boolean subscribe(const char* topic, uint8_t qos);
   boolean unsubscribe(const char* topic);
   boolean loop();
   boolean connected();
   int state();
};


#endif
