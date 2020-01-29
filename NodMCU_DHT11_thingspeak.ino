// www.arduinesp.com
//
// Plot DTH11 data on thingspeak.com using an ESP8266 同时LED亮灯显示数据发送状态
// April 11 2015
// Author: Jeroen Beemster
// Website: www.arduinesp.com

#include <DHT.h>
#include <ESP8266WiFi.h>
// replace with your channel’s thingspeak API key,
String apiKey = "7NFNSDNS8OXMNT78";
const char* ssid = "WL-911";
const char* password = "123456789";
const char* server = "api.thingspeak.com";
#define LEDPIN 16 //数字引脚D0上LED灯 GPIO 16
#define DHTPIN 2 //数字引脚D4上的温湿度计 GPIO 02
DHT dht(DHTPIN, DHT11,15);
WiFiClient client;

void setup() {
Serial.begin(115200);//串口初始化
delay(10);
pinMode(LEDPIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
dht.begin();
//联网初始化
WiFi.begin(ssid, password);
Serial.println();
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);

WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
Serial.println("WiFi connected");
}
void loop() {

float h = dht.readHumidity();
float t = dht.readTemperature();
if (isnan(h) || isnan(t)) {
Serial.println("Failed to read from DHT sensor!");
return;
}

if (client.connect(server,80)) { // "184.106.153.149" or api.thingspeak.com
String postStr = apiKey;
postStr +="&field1=";
postStr += String(t);
postStr +="&field2=";
postStr += String(h);
postStr += "\r\n\r\n";

client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);

Serial.print("Temperature: ");
Serial.print(t);
Serial.print(" degrees Celcius Humidity: ");
Serial.print(h);
Serial.println("% send to Thingspeak");
}
client.stop();

Serial.println("Waiting…");
// thingspeak needs minimum 15 sec delay between updates
  digitalWrite(LEDPIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  Serial.println("我亮了");
  delay(1000);                       // wait for a second
  digitalWrite(LEDPIN, LOW);    // turn the LED off by making the voltage LOW
  Serial.println("我灭了");
  delay(2000);  //2秒测一次
}
