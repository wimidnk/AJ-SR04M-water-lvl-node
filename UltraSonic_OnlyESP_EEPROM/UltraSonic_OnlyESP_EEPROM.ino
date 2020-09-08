#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <EEPROM.h>
#define TRUE 1
// Define Trig and Echo pin:
#define trigPin D1
#define echoPin D2
#define limitLed D3

// Define variables:
long duration;
int distance;
long lastMsg = 0;
char msg[20];
char Limit[20];
int val;
int valnew;
int eeAddress = 0; 

// Update these with values suitable for your network.
const char* set_hostname = "WATER-lvl";
const char* ssid = "AXIOM-Ground-Floor";
const char* password = "bismillah";
const char* mqtt_server = "192.168.100.101";
WiFiClient espClient;
PubSubClient client(espClient);


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
    if (client.connect("node-lvl")) {
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
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(limitLed, OUTPUT);
  Serial.begin(9600);
  EEPROM.begin(512);
  while (!Serial) {
    ; 
  }
  EEPROM.get(eeAddress, val);
 
  Serial.print(val);
  Serial.println("Loaded From EEPROM");
  
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
  if (now - lastMsg > 1000) {
    lastMsg = now;
    if (valnew > 15){
      if (valnew == val){}
      else
      {
        val = valnew;
      }
    }
    client.subscribe("/ha/sred");
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = duration*0.0343/2;
    Serial.println(distance);
    delay(100);

    
    if(distance < val) {
    digitalWrite(limitLed, HIGH);
    delay(500);
    }
    else {
      digitalWrite(limitLed, LOW);
    }
    delay(1);

    sprintf(msg, "%ld", distance);
    client.publish("/ha/spub", msg);
    sprintf(Limit, "%ld", val);
    client.publish("/ha/limit", Limit);
    }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  char buff_p[length];
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    buff_p[i] = (char)payload[i];
  }
  buff_p[length] = '\0';
  String msg_p = String(buff_p);
  valnew = msg_p.toInt(); // to Int
//  Serial.print(valnew);
  Serial.println(" Value Received");
  client.publish("/ha/Status", "Value Received");

  if (valnew > 15)
  {
    if (valnew == val)
    {
      Serial.println("Already Written this value");
      client.publish("/ha/Status", "Already Written");
    }
    else 
    {
      int eeAddress = 0;
      EEPROM.put(eeAddress, valnew);
      eeAddress += sizeof(int);
      EEPROM.put(eeAddress, valnew);
      EEPROM.commit();
      Serial.println("Written");
      client.publish("/ha/Status", "New Value Written in EEPROM");
    }
  }
  else
  {
    Serial.println("Enter Value > 15");
    client.publish("/ha/Status", "Enter Value > 15");
    delay(500);
  }
  
}
