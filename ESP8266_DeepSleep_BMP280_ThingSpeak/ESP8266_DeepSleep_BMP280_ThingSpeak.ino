// ESP8266_DeepSleep_BMP280_ThingSpeak （温度，压力，高度）

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP280.h"
Adafruit_BMP280 bmp; // I2C
ADC_MODE(ADC_VCC);//ADC读取VCC系统电压

#include "ThingSpeak.h"
unsigned long myChannelNumber = 1005181;
const char * myWriteAPIKey = "M4WPCBCN7UAJH78S";

#include <ESP8266WiFi.h>
const char* ssid = "WL-911";   // your network SSID (name)
const char* password = "123456789";  // your network password
int keyIndex = 0;            // your network key index number (needed only for WEP)
WiFiClient  client;

void setup() {
  Serial.begin(9600);
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);    
  }
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
      delay(500);
    }
    Serial.println("\nWiFi Connected.");
  }
  
   printValues(); //从BMP280读取值并将结果打印在串行监视器中

  // Initialize our values
int number1 = bmp.readTemperature();//以摄氏度为单位读取温度；
int number2 = bmp.readPressure() ;//以hPa为单位读取压力（hectoPascal =毫巴）；
int number3 = bmp.readAltitude(1013.25) ;//根据海平面上的压力估算海拔高度（以米为单位）。
int number4 = ESP.getVcc();///NodeMCU模块VCC电压
  // set the fields with the values
  ThingSpeak.setField(1, number1);
  ThingSpeak.setField(2, number2);
  ThingSpeak.setField(3, number3);
  ThingSpeak.setField(4, number4);
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
    printValues(); //从BME280读取值并将结果打印在串行监视器中
    //delay(1000);
     }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
   
   Serial.println("deep sleep for 15 seconds");
   ESP.deepSleep(15e6); //将使模块在定义的时间间隔内休眠，该时间间隔以微秒为单位。
      
} 
void printValues() {
    Serial.println();
    Serial.print(F("Temperature = "));
    Serial.print(bmp.readTemperature());
    Serial.println(" *C");

    Serial.print(F("Pressure = "));
    Serial.print(bmp.readPressure());
    Serial.println(" Pa");

    Serial.print(F("Approx altitude = "));
    Serial.print(bmp.readAltitude(1013.25)); /* Adjusted to local forecast! */
    Serial.println(" m");
    
    Serial.print(F("System voltage = "));
    Serial.print(ESP.getVcc()); 
    Serial.println(" mV");
    
    Serial.println();
    //delay(2000);
}
