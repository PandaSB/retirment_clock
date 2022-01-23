#include <Arduino.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <config.h>

#define SerialTFMini Serial2
#define SerialDataBits 115200

#define retirment_date 2232662400 

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;
unsigned long epoch ; 

void setup() {

  SerialTFMini.begin(  SerialDataBits, SERIAL_8N1);
  Serial.begin (115200) ; 

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

// Initialize a NTPClient to get time
  timeClient.begin();
  timeClient.setTimeOffset(3600) ; 



  SerialTFMini.print("CLS(7);");

  SerialTFMini.print("DS16(10,2,'");  // DS16 line
  SerialTFMini.print("Temps avant la retraite :");
  SerialTFMini.print("',6);");

  SerialTFMini.println("");



}

void loop() {
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // The formattedDate comes with the following format:
  // 2018-05-28T16:00:13Z
  // We need to extract date and time
  //formattedDate = timeClient.getFormattedDate();
  formattedDate = timeClient.getFormattedDate() ;
  epoch  = timeClient.getEpochTime() ; 
  Serial.println(formattedDate);

  // Extract date
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
  Serial.print("DATE: ");
  Serial.println(dayStamp);
  // Extract time
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
  Serial.print("HOUR: ");
  Serial.println(timeStamp);

  SerialTFMini.print("DS16(20,30,'");  // DS16 line
  SerialTFMini.print(dayStamp);
  SerialTFMini.print("   ");
  SerialTFMini.print(timeStamp);
  SerialTFMini.print("',6);");

  SerialTFMini.print("DS16(30,60,'");  // DS16 line
  SerialTFMini.print(retirment_date - epoch );
  SerialTFMini.print(" secondes" );
  SerialTFMini.print("',3);");

  SerialTFMini.print("DS16(30,80,'");  // DS16 line
  SerialTFMini.print("ou " );
  SerialTFMini.print((retirment_date - epoch) / 60 );
  SerialTFMini.print(" minutes" );
  SerialTFMini.print("',3);");

  SerialTFMini.print("DS16(30,100,'");  // DS16 line
  SerialTFMini.print(" ou " );
  SerialTFMini.print((retirment_date - epoch) / 3600 );
  SerialTFMini.print(" heures" );
  SerialTFMini.print("',3);");

  SerialTFMini.print("DS16(50,120,'");  // DS16 line
  SerialTFMini.print("ou " );
  SerialTFMini.print((retirment_date - epoch) / 86400 );
  SerialTFMini.print(" jours" );
  SerialTFMini.print("',3);");


  SerialTFMini.println("");




  delay(1000);

}