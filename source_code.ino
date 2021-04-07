/*

*/
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char *ssid     = "";//Fill here network name
const char *password = "";//Fil here password

const int speaker_pin=5;//Replace here with speaker pin
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
   
  }
  delay(5);
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(10800);//I'm located in GMT+3
}

void loop() {
  digitalWrite(2,LOW);
  timeClient.update();

  int hour = timeClient.getHours();
  Serial.print("Hour: ");
  Serial.println(hour);  

  int minute = timeClient.getMinutes();
  Serial.print("Minutes: ");
  Serial.println(minute); 
   
  int sec = timeClient.getSeconds();
  Serial.print("Seconds: ");
  Serial.println(sec);  
  if(hour>7){
    delay(5);//Extra time delays in order to avoid errors. NodeMCU has a bug which reboots the system as we perform new logic expression in another logic expression
    if(hour==8&&minute==0&&sec<7){
      //waking up
      tone(speaker_pin,180);
      delay(10000);
      noTone(speaker_pin);
    }
    else if(minute==0&&sec<7){//3 short beep hourly
      tone(speaker_pin,271);
      delay(500);
      noTone(speaker_pin);
      delay(500);
      tone(speaker_pin,271);
      delay(500);
      noTone(speaker_pin);
      delay(500);
      tone(speaker_pin,271);
      delay(500);
      noTone(speaker_pin);
      delay(1500);
    }
    else if(minute%15==0&&sec<7){//A beep for every 15 minutes
      tone(speaker_pin,271);
      delay(1000);
      noTone(speaker_pin);  
      delay(6000);    
    }
    else{
      delay(50);
      }
  }
  delay(5000);
}
