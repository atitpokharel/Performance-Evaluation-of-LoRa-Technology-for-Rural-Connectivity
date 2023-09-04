#include <SPI.h> 
#include <LoRa.h>
#define ss 10
#define rst 9
#define dio0 2 
int trigPin = 3; 
int echoPin = 4; 
int duration; 
String outgoing;
byte msgCount = 0;
byte MasterNode = 0xFF; 
byte Node1 = 0xCC;
int distance = 0; 
int temp1 = 0;
String Mymessage = "";
void setup() { 
  Serial.begin(9600); 
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  while (!Serial); 
    Serial.println("LoRa Node2"); 
    LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(433E6)) { 
  Serial.println("Starting LoRa failed!"); 
  while (1);
}
Serial.print("LoRa initiated");
}
void loop() { 
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2); 
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); 
  distance= duration*0.034/2;
  temp1 = temp1 + 0;
  onReceive(LoRa.parsePacket());
}
void onReceive(int packetSize) { 
  if (packetSize == 0) return;
// read packet header bytes: 
  int recipient = LoRa.read(); 
  byte sender = LoRa.read();
  byte incomingMsgId = LoRa.read(); 
  byte incomingLength = LoRa.read();
  String incoming = ""; 
  while (LoRa.available()) {
    incoming += (char)LoRa.read();
}
  if (incomingLength != incoming.length()) { 
  Serial.println("Error!!");
  return;
}
  if (recipient != Node1 && recipient != MasterNode) { 
  Serial.println("Another node at use");
  return;
}
  Serial.println(distance); 
  int Val = incoming.toInt(); 
  if (Val == 20)
{
    Mymessage = Mymessage + distance + "," + temp1; 
    sendMessage(Mymessage, MasterNode, Node1); 
    Serial.print("Packet sent\n");
    delay(100); 
    Mymessage = "";
}
}
  void sendMessage(String outgoing, byte MasterNode, byte Node1) {
  LoRa.beginPacket(); 
  LoRa.write(MasterNode); 
  LoRa.write(Node1); 
  LoRa.write(msgCount); 
  LoRa.write(outgoing.length()); 
  LoRa.print(outgoing); 
  LoRa.endPacket(); 
  msgCount++;
}
