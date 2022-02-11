#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

WiFiClient wifiClient;
const int httpsPort = 443;
const String host = "www.smartswitch.ga";
const char* key = "957eb0aba638ea9b0bc64fa9004237d3d63d50e1";

int b = 4;
int f = 0;
int s = 2;

String id = "5576";

String state = "";

void setup() {

  pinMode(f, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(s, OUTPUT);

  analogWrite(s, 255);

  Serial.begin(9600);
  WiFi.begin("Free Wifi", "1223334444@"); 
  
  while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion
 
    delay(500);
    Serial.println("Waiting for connection");

  }

  Serial.println("connected to wifey");
}

int _i = 0;

void loop() { 

    String s = GetData(id, "get");

    Serial.println(s);

    if (_i == 0){
      state = s;
      _i++;
    }

    if (state != s) {
      if (s == "0"){  // 0 stands for off and 1 stands for on
        Off();
      } else if (s == "1"){
        On();
      }

      state = s;
    }

}

void On(){
  front();
  delay(500);
  Stop();
}

void Off(){
  back();
  delay(500);
  Stop();
}

void front(){
  digitalWrite(f, HIGH);
  digitalWrite(b, LOW);
}

void back(){
  digitalWrite(f, LOW);
  digitalWrite(b, HIGH);}

void Stop(){
  digitalWrite(f, LOW);
  digitalWrite(b, LOW);
}

String GetData(String id, String task){

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
  
  String Link = "/switch/" + id + "/" + task + "/x";

//  Serial.print("requesting URL: ");
 // Serial.println(host+Link);

  httpsClient.print(String("GET ") + Link + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +               
               "Connection: close\r\n\r\n");


  while (httpsClient.connected()) {
    String line = httpsClient.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }

  
  String line;
   
  while(httpsClient.available()){        
    line = httpsClient.readStringUntil('\n');  //Read Line by Line
  }

  return line;


}
