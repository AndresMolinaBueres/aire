#include <ESP8266WiFi.h>

#include "DFRobot_RP2040_SCI.h"
DFRobot_RP2040_SCI_IIC sci(/*addr=*/RP2040_SCI_ADDR_0X21, &Wire);

String apiKey = "VGSPGMRUMIXZBPRB";     //  Enter your Write API key from ThingSpeak
 
const char *ssid = "iPhone de Andres";
const char *pass = "paquis2010";
const char* server = "api.thingspeak.com";
 
WiFiClient client;

void setup() 
{
  Serial.begin(115200);
  delay(10);

  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) 
  {
        delay(500);
        Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

}
 
void loop() 
{
  String TimeStamp = sci.getTimeStamp(); 
  Serial.print(" TimeStamp: "); Serial.print(TimeStamp);


  /* Sensor PM */
  float PM1_0_val  = sci.getValue(sci.eALL,"PM1.0").toFloat();
  String PM1_0_unit = sci.getUnit(sci.eALL,"PM1.0");
  float PM2_5_val  = sci.getValue(sci.eALL,"PM2.5").toFloat();
  String PM2_5_unit = sci.getUnit(sci.eALL,"PM2.5");
  float PM10_val = sci.getValue(sci.eALL,"PM10").toFloat();
  String PM10_unit = sci.getUnit(sci.eALL,"PM10");
  Serial.print(" PM1.0: "); Serial.print(PM1_0_val); Serial.print(" ");Serial.print(PM1_0_unit);
  Serial.print(" PM2.5: "); Serial.print(PM2_5_val); Serial.print(" ");Serial.println(PM2_5_unit);
  Serial.print(" PM10: "); Serial.print(PM10_val); Serial.print(" ");Serial.println(PM10_unit);
  
  /* Sensor ENS160 */
  float AQI_val  = sci.getValue(sci.eALL,"AQI").toFloat();
  String AQI_unit = sci.getUnit(sci.eALL,"AQI");
  float TVOC_val  = sci.getValue(sci.eALL,"TVOC").toFloat();
  String TVOC_unit = sci.getUnit(sci.eALL,"TVOC");
  float ECO2_val = sci.getValue(sci.eALL,"ECO2").toFloat();
  String ECO2_unit = sci.getUnit(sci.eALL,"ECO2");
  Serial.print(" AQI: "); Serial.print(AQI_val); Serial.print(" ");Serial.print(AQI_unit);
  Serial.print(" TVOC: "); Serial.print(TVOC_val); Serial.print(" ");Serial.println(TVOC_unit);
  Serial.print(" ECO2: "); Serial.print(ECO2_val); Serial.print(" ");Serial.println(ECO2_unit);
  
  delay(500);

  if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
  {  
    String postStr = apiKey;
    postStr +="&field1=";
    postStr += String(AQI_val);
    postStr +="&field2=";
    postStr += String(TVOC_val);
    postStr +="&field3=";
    postStr += String(ECO2_val);
    postStr +="&field5=";
    postStr += String(PM1_0_val);
    postStr +="&field6=";
    postStr += String(PM2_5_val);
    postStr +="&field7=";
    postStr += String(PM10_val);
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
    Serial.println("%. Send to Thingspeak.");
  }
  client.stop();
  Serial.println("Waiting...");
    // thingspeak needs minimum 15 sec delay between updates
  delay(15000);
  

}