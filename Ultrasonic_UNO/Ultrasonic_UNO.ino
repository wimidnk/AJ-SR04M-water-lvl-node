/* Arduino example sketch to control a JSN-SR04T ultrasonic distance sensor with Arduino. No library needed. More info: https://www.makerguides.com */

// Define Trig and Echo pin:
#define trigPin 3
#define echoPin 4
#define limitLed 5

// Define variables:
long duration;
int distance;


long lastMsg = 0;
String str;
int limit;

void setup() {
  // Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Begin Serial communication at a baudrate of 9600:
  Serial.begin(9600);

}

void loop() {
  long now = millis();
  if (now - lastMsg > 3000) {
   lastMsg = now;
//  int str = 30;
  if (Serial.available() > 0) {
      str = Serial.readStringUntil('\n');
      limit = str.toInt();
      Serial.println(Serial.readStringUntil('\n'));
////      lcd.print(Serial.readStringUntil('\n'));
      delay(100);
      
  }
  
  else{



  
  // Clear the trigPin by setting it LOW:
  digitalWrite(trigPin, LOW);
  
  delayMicroseconds(5);

 // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the echoPin. pulseIn() returns the duration (length of the pulse) in microseconds:
  duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance:
  distance = duration*0.0343/2;
    Serial.println(limit);
  // Print the distance on the Serial Monitor (Ctrl+Shift+M):
//  Serial.print("Distance = ");
  Serial.println(distance);
//  Serial.println(" cm");

  if(distance <= limit) {
  digitalWrite(limitLed, HIGH);
  delay(500);
  }
  else {
    digitalWrite(limitLed, LOW);
  }
  delay(1);
  }
  }
}
