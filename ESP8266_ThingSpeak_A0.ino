/*
 * A0+RSSI_WL - ThingSpeak IoT
https://thingspeak.com/channels/987924/private_show
 ESP8266 --> ThingSpeak Channel
 This sketch sends the value of Analog Input (A0) to a ThingSpeak channel 1
 This sketch sends the Wi-Fi Signal Strength (RSSI) of an ESP8266 to a ThingSpeak channel 2
 */

#include "ThingSpeak.h"

unsigned long myChannelNumber = 987924;
const char * myWriteAPIKey = "BPB5AKJFO1ZLXD1F";

#include <ESP8266WiFi.h>

const char* ssid = "WL-911";   // your network SSID (name)
const char* password = "123456789";  // your network password
int keyIndex = 0;            // your network key index number (needed only for WEP)
WiFiClient  client;

void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
}

void loop() {

  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println();
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
  
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(1000);
    }
    Serial.println("\nWiFi Connected.");
  }


  // Initialize our values
int number1 = analogRead(A0);//
int number2 = WiFi.RSSI();//Received Signal Strength Indicator
  // set the fields with the values
  ThingSpeak.setField(1, number1);
  ThingSpeak.setField(2, number2);

  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
    Serial.println(number1);
    Serial.println(number2);//dBm
     }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
    // Wait 15 seconds to update the channel again
  delay(15000);
}
