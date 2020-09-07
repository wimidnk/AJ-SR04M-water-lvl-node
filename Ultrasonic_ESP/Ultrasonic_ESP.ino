#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>

#define TRUE 1

// Update these with values suitable for your network.
const char* set_hostname = "WATER-lvl-node";
const char* ssid = "********";
const char* password = "*********";
const char* mqtt_server = "********";
char Str1[15];
//void callback(char* topic, byte* payload, unsigned int length);

int incomingByte;  

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("node-arduino")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
void setup()
{
  Serial.begin(9600);
  setup_wifi(); 
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 1) {
    lastMsg = now;

    if (Serial.available() > 0) {
//      client.publish("/ha/NO", "Serial Data Receiving");
      client.publish("/ha/spub", String(Serial.readStringUntil('\n')).c_str(),TRUE);
    }
//    else{

//    }
    client.subscribe("/ha/sred");

    }
}

void callback(char* topic, byte* payload, unsigned int length) {
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println(); 
}
