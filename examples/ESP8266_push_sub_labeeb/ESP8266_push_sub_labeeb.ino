#include <ESP8266WiFi.h>        
#include <LabeebIoT.h>

const char* mqtt_server = "mea.labeeb-iot.com";
WiFiClient espClient;
LabeebIoT labeeb (espClient,mqtt_server,1883,"ENTERPRISE NAME");

// -- Wifi settings -->  
const char* ssid = "WIFI SSID";
const char* password = "WIFI PASS";

long lastMsg = 0;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


void reconnect() {
  // Loop until we're reconnected
  while (!labeeb.connected()) {
    Serial.print("Attempting MQTT connection to Labeeb...");
    // Attempt to connect
     if (labeeb.connectLabeeb("Client ID","TEMPLATE ID", "TEMPLATE PASS")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
       labeeb.subscribe("TOPIC");
   //   Serial.println("Subscribed");
    } else {
      Serial.print("failed, rc=");
      Serial.print(labeeb.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}




void setup() {
Serial.begin(9600);
Serial.println("Labeeb IoT Example ");
  setup_wifi();
labeeb.setCallback(callback);

}

void loop() {
  
  labeeb.loop();
if (!labeeb.connected()) {
         Serial.println("reconnecting");

   reconnect();

   
   }else {


long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    labeeb.publishLabeebValue("A","cylinder","gaslevel","12","0000000000");
  }
  }
}