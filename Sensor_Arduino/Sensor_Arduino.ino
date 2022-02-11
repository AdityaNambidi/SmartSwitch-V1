#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

WiFiClient wifiClient;
const int httpsPort = 443;
const char* host = "www.smartswitch.ga";
const char* key = "957eb0aba638ea9b0bc64fa9004237d3d63d50e1";

String id = "5576";

int pirSensor = 2;

void setup() {
  pinMode(pirSensor, INPUT);
  
  Serial.begin(9600);


  WiFi.begin("Free Wifi", "1223334444@"); 
  
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");

  }

  Serial.println("connected to wifey");
  
  delay(500);
}

void loop() {

  float sensorValue = digitalRead(pirSensor);

  if (sensorValue == HIGH) {
      
    delay(12000);
  } else {
    notify(id ); 
  }

}


void notify(String id) {

  WiFiClientSecure httpsClient;    //Declare object of class WiFiClient


  //Serial.printf("Using fingerprint '%s'\n", key);
  httpsClient.setFingerprint(key);
  
  //Serial.print("HTTPS Connecting");
  int r=0; //retry counter
  while((!httpsClient.connect(host, httpsPort)) && (r < 30)){
      delay(100);
      //Serial.print(".");
      r++;
  }
  
  String Link = "/switch/" + id + "/statusUpdate/x";

//  Serial.print("requesting URL: ");
 // Serial.println(host+Link);

  httpsClient.print(String("GET ") + Link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +               
               "Connection: close\r\n\r\n");
}
