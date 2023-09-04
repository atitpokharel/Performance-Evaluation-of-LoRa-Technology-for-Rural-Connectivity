#include <SPI.h>
#include <LoRa.h>

//define the pins used by the transceiver module
#define ss 10
#define rst 9
#define dio0 2

int trigPin = 3;
int echoPin = 4;
int duration;
String LoRaMessage;
int distance; 
int counter = 0;
int sf = 6; // Change SF accordingly
char bw = "125E6"; //Change Bandwidth accordingly
int cr = 5; //Change Coding Rate Accordingly

void setup() {
  //initialize Serial Monitor
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  while (!Serial);
  Serial.println("LoRa Sender");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  
 
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  
  }
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
 
  LoRa.setSpreadingFactor(7);
  
  LoRa.setSignalBandwidth(125E3);
  
  LoRa.setCodingRate4(5);
  LoRa.enableCrc();
  byte b = LoRa.random();
  
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2;
  Serial.println(distance);
  

  Serial.println(counter);
  LoRaMessage = String(distance);

  //Send LoRa packet to receiver
  
  LoRa.beginPacket();
  LoRa.print(LoRaMessage);
  Serial.print (String(distance+ " for counter " + counter));
  LoRa.endPacket();
  Serial.print("Freq is: ");
  Serial.print(sf);
  Serial.print(" ");

  Serial.print("BW is: ");
  Serial.print(String(bw));
  Serial.print(" ");
  Serial.print("CR is: ");
  Serial.print(cr);
  Serial.print(" ");
  Serial.print("Sending packet: ");

  counter++;

  delay(2000);
}
