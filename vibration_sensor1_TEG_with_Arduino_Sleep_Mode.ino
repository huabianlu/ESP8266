//vibration_sensor_TEG with Arduino Sleep Mode
#include <LowPower.h>

#define sensorPin0 A0
#define sensorPin1 A1
//#define ledPin 13        //连接到数字引脚13的LED 
void setup() {
// set the digital pin as output:
  pinMode(2, OUTPUT);//将数字引脚设置为输出
  pinMode(A2, OUTPUT);//将模拟引脚设置为输出
  
  Serial.begin(9600);//波特率要和RF模块保持一致DL-20是9600
//digitalWrite(LED_BUILTIN,LOW);       //设置LED熄灭
}

void loop() {

  //Arduino将进入8秒的idle空闲状态并自动唤醒。
  //LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART0_OFF, TWI_OFF);
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); 
  // Serial.println("Arduino:- Hey I just Woke up");
  
  //Serial.println("Vibration Level mg is now: "); // print it    
  //Serial.println("Battery Level % is now: "); // print it  
  for (int a = 0; a < 2; a++)//一次读2个数据 防止RF丢包 电容滤波后只取有效幅值
  {
  digitalWrite(2,HIGH); //将D2设置为高电平，接RF的VCC
  //digitalWrite(A2,HIGH); //将A2常为高电平
  //digitalWrite(2,LOW);  //将D2设置为低电平，接RF的GND
  delay(10); //0.01s
  int x=abs(analogRead(sensorPin1)-analogRead(sensorPin0));
  //float y=5*x/1023;
   if(x>200)Serial.println("overload");//幅值大于2V故障
   Serial.println(x);
 
  

  delay(10); //0.01s
  digitalWrite(2,LOW); //将D2设置为低电平，接RF的VCC
  }
//  if(x>500)digitalWrite(13,HIGH);  //将LED设置为开启
//  else digitalWrite(13,LOW);       //设置LED熄灭
  
}
