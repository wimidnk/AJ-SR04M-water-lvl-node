#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define TRUE 1
#define led D3

// Update these with values suitable for your network.
const char* ssid = "AXIOM-Ground-Floor";
const char* password = "bismillah";
const char* mqtt_server = "192.168.100.101";
//void callback(char* topic, byte* payload, unsigned int length);


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char Limit[20];
int stat=0;

int on_level=0;
int off_level=0;




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
    if (client.connect("arduinoClient")) {
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
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  setup_wifi(); 
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  client.subscribe("/ha/switch");
  digitalWrite(led, LOW);
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    
  
    
    client.subscribe("/ha/switch");
    sprintf(Limit, "%ld", stat);
    client.publish("/ha/Status2", Limit);
    Serial.println(on_level);
      if(stat == 1){
        on_level++;
        if(on_level>8){
          client.publish("/ha/switchstatus", "ON");
          Serial.println(on_level);
          digitalWrite(led, HIGH);
        }
      }
      else{
        client.publish("/ha/switchstatus", "OFF");
        on_level = 0;
        off_level = 0;
        digitalWrite(led, LOW);
      }
    
   

    
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
  stat = msg_p.toInt(); // to Int
  Serial.print(stat);
  Serial.println(" Value Received");
}
