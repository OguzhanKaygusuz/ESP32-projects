#include "WiFi.h"
#define blueLedPin 32
#define redLedPin 33

void setup() {
  pinMode(redLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);
}

void loop() {
  Serial.println("Scanning available networks...");
  digitalWrite(redLedPin, HIGH);
  digitalWrite(blueLedPin, LOW);
  int n = WiFi.scanNetworks();
  if (n != 0) {
    digitalWrite(redLedPin, LOW);
    digitalWrite(blueLedPin, HIGH);
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i) {
      Serial.print("Network ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(") ");
      Serial.print((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "open" : "*** ");
      Serial.println(WiFi.BSSIDstr(i));
      delay(50);
    }
  } else {
    Serial.println("No available networks found");
    digitalWrite(redLedPin, LOW);
    digitalWrite(blueLedPin, LOW);
  }
  Serial.println("\n--------------------------------------\n");
  delay(30000);
}