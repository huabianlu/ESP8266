//锂电池充电电量监测NodeMCU+MAX17043+ThingSpeak
/*
MAX17043  I2C接口
Arduino:  SDA=A4  SCL=A5  GND=GND //VCC=Vin 
NodeMCU:  SDA=D2  SCL=D1  GND=GND //VCC=Vin 
*/
#include "MAX17043.h"
#include "Wire.h"
MAX17043 batteryMonitor;

#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
// replace with your channel’s thingspeak API key,
const char * myWriteAPIKey = "RSJTKD0RL0CROKU6"; // replace XYZ with your channel write API Key
unsigned long myChannelNumber = 987681; // replace 0000000 with your channel number
const char* ssid = "WL-911";
const char* password = "123456789";
//const char* server = "api.thingspeak.com";
//int keyIndex = 0;            // your network key index number (needed only for WEP)
WiFiClient client;

void setup(){
  Wire.begin(); 
  Serial.begin(115200);//串口初始化
  //联网初始化
WiFi.mode(WIFI_STA); 
ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

// Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
     WiFi.begin(ssid, password); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
     Serial.println();
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
       while (WiFi.status() != WL_CONNECTED) {
            Serial.print(".");
      delay(1000);
    }
    Serial.println("\nWiFi Connected.");
  }
  // Initialize our values
float number1 = batteryMonitor.getVCell();//Cell voltage
float number2 = batteryMonitor.getSoC();//State of charge
 
  // set the fields with the values
  ThingSpeak.setField(1, number1);
  ThingSpeak.setField(2, number2);

if (isnan(number1) || isnan(number2)) {
Serial.println("Failed to read from MAX17043 sensor!");
return;
 }
// write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Thingspeak channel update successful.");

      //串口输出锂电池电压和充电状态
  Serial.print("Voltage:\t\t");
  Serial.print(number1,4);//Setting Alert Threshold to 4
  Serial.println("V");

  Serial.print("State of charge:\t");
  Serial.print(number2);
  Serial.println("%");
     }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
    // Wait 15 seconds to update the channel again
  delay(15000);
}
