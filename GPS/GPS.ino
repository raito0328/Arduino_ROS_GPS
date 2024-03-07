#include <TinyGPS++.h>

#include <Arduino.h>
#include <ros.h>

#include <SoftwareSerial.h> //シリアル通信用のライブラリ

#include <std_msgs/Float32MultiArray.h>

ros::NodeHandle nh;
std_msgs::Float32MultiArray msg;

ros::Publisher pub("gnss", &msg);

void smartDelay(unsigned long ms);
SoftwareSerial gps(10, 11); //GPS のシリアル通信ポートの割り当て RX2 番ピン，TX3 番ピン
TinyGPSPlus tinyGPS;
void setup(){
  nh.initNode();
  msg.data_length = 3;
  msg.data = (float *)malloc((sizeof(float))*msg.data_length * 2);  
  nh.advertise(pub);

  Serial.begin(9600); //PC とのシリアル通信開始
  gps.begin(9600); //GPS とのシリアル通信開始
  delay(2000);
}

void loop(){
  Serial.print(tinyGPS.location.lat(), 6);
  Serial.print(", ");
  Serial.println(tinyGPS.location.lng(), 6);

  msg.data[0]=tinyGPS.location.lat();
  msg.data[1]=tinyGPS.location.lng();
  pub.publish(&msg);

  smartDelay(1000);

  nh.spinOnce();
}


void smartDelay(unsigned long ms){
 unsigned long start = millis();
 do{
 while(gps.available())
 tinyGPS.encode(gps.read());
 }while(millis()-start<ms);
}
