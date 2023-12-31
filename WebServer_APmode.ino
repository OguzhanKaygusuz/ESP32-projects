#include <WiFi.h>
#include <WebServer.h>

#define ssid "ESP32_AP"
#define password "5555"
#define blueLedPin 32
#define redLedPin 33
IPAddress local_ip(192,168,1,15);
IPAddress gateway(192,168,1,15);
IPAddress subnet(255,255,255,0);
WebServer server(80);

bool LED1status = LOW;
bool LED2status = LOW;

void setup() {
  Serial.begin(115200);
  pinMode(blueLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);

  WiFi.softAP(ssid,password);
  WiFi.softAPConfig(local_ip,gateway,subnet);
  delay(1000);

  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);  // Updated URL
  server.on("/led2off", handle_led2off);  // Updated URL
  server.onNotFound(handle_NotFound);
  server.begin();
  Serial.println("HTTP Server Started...");
}

void loop() {
  server.handleClient();
  if (LED1status) {
    digitalWrite(blueLedPin, HIGH);
  } else {
    digitalWrite(blueLedPin, LOW);
  }

  if (LED2status) {
    digitalWrite(redLedPin, HIGH);
  } else {
    digitalWrite(redLedPin, LOW);
  }
}

void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  Serial.println("GPIO32 Status: OFF | GPIO33 Status: OFF");
  server.send(200, "text/html", SendHTML()); 
}

void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO32 Status: ON");
  server.send(200, "text/html", SendHTML()); 
}

void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO32 Status: OFF");
  server.send(200, "text/html", SendHTML()); 
}

void handle_led2on() {
  LED2status = HIGH;
  Serial.println("GPIO33 Status: ON");
  server.send(200, "text/html", SendHTML()); 
}

void handle_led2off() {
  LED2status = LOW;
  Serial.println("GPIO33 Status: OFF");
  server.send(200, "text/html", SendHTML()); 
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML() {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 Web Server</h1>\n";
  ptr +="<h3>Using Access Point(AP) Mode</h3>\n";
  ptr +="<h4> Created by Oguzhan Kaygusuz </h4>\n";
  
  if (LED1status) {
    ptr +="<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";
  } else {
    ptr +="<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";
  }

  if (LED2status) {
    ptr +="<p>LED2 Status: ON</p><a class=\"button button-off\" href=\"/led2off\">OFF</a>\n";
  } else {
    ptr +="<p>LED2 Status: OFF</p><a class=\"button button-on\" href=\"/led2on\">ON</a>\n";
  }

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}