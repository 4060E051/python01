#include "DHT.h"
#include <MQ2.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SPI.h>
#include <MFRC522.h>
//change this with the pin that you use
int pin = A0;
int smoke, lpg;
MQ2 mq2(pin);
float smokeData;
float lpgData;

const char* ssid = "XZ1";// 
const char* password = "00000000";
//WiFiClient client;
char server[] = "120.114.134.170";   //eg: 192.168.0.222

WiFiClient client;    

void setup(){
 Serial.begin(115200);
  delay(10);
  mq2.begin();
    // Connect to WiFi network
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
 
  // Start the server
  //  server.begin();
  Serial.println("Server started");
  Serial.print(WiFi.localIP());
  delay(1000);
  Serial.println("connecting...");

}

void loop()
{ 
  smokeData = mq2.readSmoke();
  lpgData = mq2.readLPG(); 
  Sending_To_phpmyadmindatabase(smokeData, lpgData); 
  delay(10000); // every 10 secs
 }
 void Sending_To_phpmyadmindatabase(float smokeData, float lpgData)   //CONNECTING WITH MYSQL, pass these parameters as well 
   {
   if (client.connect(server, 80)) {
    Serial.println("connected to local server");
    // Make a HTTP request:
    
    Serial.print("GET /mq2.php?lpg=");
    client.print("GET /mq2.php?lpg=");     //YOUR URL

    
    Serial.println(smokeData);
    client.print(smokeData);
    client.print("&smoke=");
    
    Serial.println("&smoke=");
    client.print(lpgData);
    
    Serial.println(lpgData);
    client.print(" ");      //SPACE BEFORE HTTP/1.1
    client.print("HTTP/1.1");
    client.println();
    client.println("Host: 120.114.134.170"); //dont forget to add your server IP address here. otherwise if you see apache_access_logs, this show HTTP1.1 400 error meaning malformed request
    client.println("Connection: close");
    client.println(); 
    //Serial.print("GET /test.txt");
    //client.println("GET /test.txt HTTP/1.0"); //download text
    //client.println(); //end of get request
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }






}
