#include <b64.h>
#include <HttpClient.h>
#include <SoftwareSerial.h>

SoftwareSerial esp8266(2,3);  // Pin 2 & 3 of Arduino as RX and TX. Connect TX and RX of ESP8266 respectively.

#define DEBUG true
#define led1_pin 11  // LED is connected to Pin 11 of Arduino
#define led2_pin 12
#define led3_pin 13

void setup() {
  pinMode(led1_pin, OUTPUT);
  digitalWrite(led1_pin, LOW);
  pinMode(led2_pin, OUTPUT);
  digitalWrite(led2_pin, LOW);
  pinMode(led3_pin, OUTPUT);
  digitalWrite(led3_pin, LOW);
  
  Serial.begin(9600);
  esp8266.begin(115200);  //Baud rate for communicating with ESP8266. Your's might be different.
  
  esp8266Serial("AT+RST\r\n", 5000, DEBUG); // Reset the ESP8266
  esp8266Serial("AT+CWMODE=1\r\n", 5000, DEBUG);  // Set station mode Operation
  esp8266Serial("AT+CWJAP=\"Interface\",\"arsalan415263\"\r\n", 5000, DEBUG); // Enter your WiFi network's SSID and Password.
                               
  while(!esp8266.find("OK")) {
    delay(500);
  }
  
  esp8266Serial("AT+CIFSR\r\n", 5000, DEBUG); // You will get the IP Address of the ESP8266 from this command. 
  esp8266Serial("AT+CIPMUX=1\r\n", 5000, DEBUG);
  esp8266Serial("AT+CIPSERVER=1,80\r\n", 5000, DEBUG);
}

void loop() {
  HttpClient client;
  
  if (esp8266.available()) {
    if (esp8266.find("+IPD,")) {
      
      String msg;
      esp8266.find("?");
      msg = esp8266.readStringUntil(' ');
      String command1 = msg.substring(0, 4);
      String command2 = msg.substring(5);
        
      if (DEBUG) {
        Serial.println(command1); // Must print "led"
        Serial.println(command2); // Must print "ON" or "OFF"
      }
      
      delay(100);
      
      if (command1 == "led1" && command2 == "ON") {
        digitalWrite(led1_pin, HIGH);
        
        client.begin("http://192.168.1.100/");
        client.addHeader("Content-Type", "application/x-www-form-urlencoded");
        client.POST("led1=ON");
        client.writeToStream(&Serial);
        client.end();
        
      } else if(command1 == "led2" && command2 == "ON") {
        digitalWrite(led2_pin, HIGH);
      } else if(command1 == "led3" && command2 == "ON") {
        digitalWrite(led3_pin, HIGH);
      }

      if (command1 == "led1" && command2 == "OFF") {
        digitalWrite(led1_pin, LOW);
      } else if(command1 == "led2" && command2 == "OFF") {
        digitalWrite(led2_pin, LOW);
      } else if(command1 == "led3" && command2 == "OFF") {
        digitalWrite(led3_pin, LOW);
      }
    
    }
  }
  delay(500);
}

String esp8266Serial(String command, const int timeout, boolean debug) {
  String response = "";
  esp8266.print(command);
  long int time = millis();
  
  while ((time + timeout) > millis()) {
    while (esp8266.available()) {
      char c = esp8266.read();
      response += c;
    }
  }
  
  if (debug) {
    Serial.print(response);
  }
  
  return response;
}
