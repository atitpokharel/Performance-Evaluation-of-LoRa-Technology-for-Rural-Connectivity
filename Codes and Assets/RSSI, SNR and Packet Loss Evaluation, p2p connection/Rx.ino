#include <SPI.h>
#include <LoRa.h>

//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2
int buzz = 22;

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  pinMode(buzz,OUTPUT);
  while (!Serial);
  Serial.println("LoRa Receiver");

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
  LoRa.setSpreadingFactor(7); //Change accordingly, keep in mind that this value must be same in both Tx and Rx
  LoRa.setSignalBandwidth(500E3); //Change accordingly, keep in mind that this value must be same in both Tx and Rx
  LoRa.setCodingRate4(7); //Change accordingly, keep in mind that this value must be same in both Tx and Rx
  LoRa.enableCrc();
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    while (LoRa.available()) {
    // received a packet
    Serial.print("Received packet \n");
    digitalWrite(buzz,HIGH);
    delay(300);
    digitalWrite(buzz,LOW);
    delay(500);
    digitalWrite(buzz,HIGH);
    delay(500);
    digitalWrite(buzz,LOW);
    
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData); 
      Serial.print("\n");
      Serial.print("Packetsize is: ");
      Serial.print(packetSize);
      Serial.print("\n");
      Serial.print("RSSI: ");
      Serial.println(LoRa.packetRssi());
     
      Serial.print("Freq Error: ");
      long freqErr = LoRa.packetFrequencyError();
      Serial.print(freqErr);
      Serial.print("\n");
      float snr = LoRa.packetSnr();
      Serial.print("SNR: ");
      Serial.print(snr);
      Serial.print("\n");
      Serial.print("\n");
      
    }
    }


    
  }
