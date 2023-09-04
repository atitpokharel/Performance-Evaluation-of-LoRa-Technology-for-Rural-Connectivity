#include <SPI.h> 
#include <LoRa.h> 
#include <WiFi.h> 
#include "ThingSpeak.h"
long lastConnectionTime = 0; 
#define ss 5
#define rst 14
#define dio0 2 
int buzz = 22; 
int c=1;
const int updateInterval = 16000;
const char *ssid = "SSID OF NETWORK ";
const char *pass = "PASSWORD OF NETWORK";
WiFiClient client;
unsigned long myChannelNumber = "THINGSPEAK CHANNEL NYMBER";
const char *myWriteAPIKey = "THINGSPEAK API KEY";
byte MasterNode = 0xFF; 
byte Node1 = 0xBB;
byte Node2 = 0xCC; 
String SenderNode = ""; 
String outgoing;
byte msgCount = 0; 
String incoming = "";
unsigned long previousMillis = 0; 
unsigned long int previoussecs = 0;
unsigned long int currentsecs = 0; 
unsigned long currentMillis = 0; 
int interval = 1 ;
int Secs = 0; 
int dist1;
int temp1; 
int dist2; 
int temp2;
void setup() { 
  pinMode(buzz,OUTPUT); 
  Serial.begin(115200); 
  while (!Serial);
  Serial.println("LoRa Starting"); 
  LoRa.setPins(ss, rst, dio0);
  if (!LoRa.begin(433E6)) { 
  Serial.println("LoRa failed,retry"); 
  while (true);
  }
  Serial.println("LoRa Started"); 
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client); 
  if(WiFi.status() != WL_CONNECTED){
  Serial.print("Attempting to connect"); 
  while(WiFi.status() != WL_CONNECTED){ 
  WiFi.begin(ssid, pass);
  delay(5000);
  }
  Serial.println("\nConnected.");
  }
  digitalWrite(buzz,LOW);
}
void loop() {
  currentMillis = millis(); 
  currentsecs = currentMillis / 1000;
  if ((unsigned long)(currentsecs - previoussecs) >= interval) { 
  Secs = Secs + 1;
  //Serial.println(Secs); 
  if ( Secs >= 11 )
  {
  Secs = 0;
  }
  if ( (Secs >= 1) && (Secs <= 5) )
  {
  String message = "10"; 
  sendMessage(message, MasterNode, Node1);
  }
  if ( (Secs >= 6 ) && (Secs <= 10))
  {
  String message = "20"; 
  sendMessage(message, MasterNode, Node2);
  }
  previoussecs = currentsecs;
  }
  onReceive(LoRa.parsePacket());
  if ((dist1<10) && (dist2<10)){
  Serial.print("WARNING!!! THREASHOLD REACHED");
  digitalWrite(buzz, HIGH);} 
  else{ digitalWrite(buzz,LOW);}
}
  void sendMessage(String outgoing, byte MasterNode, byte otherNode) { 
  LoRa.beginPacket();
  LoRa.write(otherNode); 
  LoRa.write(MasterNode); 
  LoRa.write(msgCount); 
  LoRa.write(outgoing.length()); 
  LoRa.print(outgoing);
  LoRa.endPacket(); 
  msgCount++;
}
void onReceive(int packetSize) { 
  if (packetSize == 0) return;
  // read packet header bytes: 
  Serial.print("a packet received"); 
  int recipient = LoRa.read();
  byte sender = LoRa.read(); 
  if ( sender == 0XBB ) 
  SenderNode = "Node1:"; 
  if ( sender == 0XCC ) 
  SenderNode = "Node2:";
  byte incomingMsgId = LoRa.read(); 
  byte incomingLength = LoRa.read();
  while (LoRa.available()) { 
  incoming += (char)LoRa.read();
  }
  if (incomingLength != incoming.length()) { 
  Serial.println("error: check conenctions");
  ;
  return;
  }
  if (recipient != Node1 && recipient != MasterNode) { 
  Serial.println("Other node is sending data");
  ;
  return;
  }
  if ( sender == 0XCC )
  {
  String d = getValue(incoming, ',', 0); 
  String t1 = getValue(incoming, ',', 1); 
  int dt1 = d.toInt();
  int tt1 = t1.toInt(); 
  dist1=dt1; 
  temp1=tt1;
  incoming = "";
  Serial.print(SenderNode);
  Serial.print("DATA FROM NODE 1 : "); 
  Serial.print(dist1);
  Serial.print(" "); 
  Serial.print("cm\n");
  }
  if ( sender == 0XBB ){
  String d1 = getValue(incoming, ',', 0); 
  String t2 = getValue(incoming, ',', 1); 
  int dt2 = d1.toInt();
  int tt2 = t2.toInt(); 
  dist2=dt2; 
  temp2=tt2;
  incoming = "";
  //clear display 
  Serial.print(SenderNode);
  Serial.print("DATA FROM NODE 2: ");
  Serial.print(dist2); 
  Serial.println(" cm");
  }
  int avg = ((dist1+dist2)/2); 
  ThingSpeak.setField(4, dist1); 
  ThingSpeak.setField(5, dist2); 
  ThingSpeak.setField(6, avg);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); 
  if (x=200){
  Serial.print("Ready to upload\n");
  }
}
String getValue(String data, char separator, int index)
{
int found = 0;
int strIndex[] = { 0, -1 };
int maxIndex = data.length() - 1;
for (int i = 0; i <= maxIndex && found <= index; i++) { 
if (data.charAt(i) == separator || i == maxIndex) { 
found++;
strIndex[0] = strIndex[1] + 1; 
strIndex[1] = (i == maxIndex) ? i + 1 : i;
}
}
return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
