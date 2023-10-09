#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

#define WIFI_SSID "GuestWlan-UpperFloor"
#define WIFI_PASSWORD "RF-47475!"
#define API_KEY "AIzaSyAYH0ET5ZiIfHnPwKIp13afuINF4Uw4Hm4"
#define DATABASE_URL "https://esp32-rtdb-d5cb2-default-rtdb.europe-west1.firebasedatabase.app/"

#define LED1_PIN 32
#define LED2_PIN 33
#define LDR_PIN 35
#define PWMChannel 0

const int freq = 5000;
const int resolution = 8;



FirebaseData fbdo, fbdo_s1, fbdo_s2;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool SignupOK = false;
int ldrData = 0;
float voltage =0;
int pwmValue =0;
bool ledStatus = false;


void setup() {

  pinMode(LED2_PIN, OUTPUT);
  ledcSetup(PWMChannel, freq, resolution);
  ledcAttachPin(LED1_PIN, PWMChannel);
  Serial.begin(115200);
  // Wifi connection
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  Serial.print("Connecting to the WIFI...");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
     delay(300);
  }
  Serial.println();
  Serial.print("CONNECTED_WIFI");
  Serial.println(WiFi.localIP());
  Serial.println();
  //Database Configuration
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if(Firebase.signUp(&config, &auth,"","")){
    Serial.println("SignUp OK");
    SignupOK = true;
  }else{
    Serial.printf("&s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}


void loop() {
  if(Firebase.ready() && SignupOK && (millis()- sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    // ------------------ STORE sensor data to a RTDB
    ldrData = analogRead(LDR_PIN);
    voltage = (float)analogReadMilliVolts(LDR_PIN)/1000;
  if(Firebase.RTDB.setFloat(&fbdo,"sensor/voltage", voltage)){
    Serial.println();
    Serial.print(voltage);
    Serial.print("- succesfully saved to: " + fbdo.dataPath());
    Serial.println(" (" + fbdo.dataType() + ")");
  }else{
    Serial.println("FAILED: " + fbdo.errorReason());
    }
  }
// ----------------- READ data from a RTDB to control devices attached to the ESP32
if(Firebase.RTDB.getInt(&fbdo,"/LED/analog")){
    if(fbdo.dataType() == "int"){
      pwmValue = fbdo.intData();
      Serial.println("succesfully READ from " + fbdo.dataPath() + ": " + pwmValue +  " (" + fbdo.dataType() + ")");
      ledcWrite(PWMChannel, pwmValue);
    }else{
      Serial.println("FAILED: " + fbdo.errorReason());
    }
  
if(Firebase.RTDB.getBool(&fbdo,"/LED/digital")){
    if(fbdo.dataType() == "boolean"){
      ledStatus = fbdo.boolData();
      Serial.println("succesfully READ from " + fbdo.dataPath() + ": " + ledStatus +  " (" + fbdo.dataType() + ")");
      digitalWrite(LED2_PIN,ledStatus); 
    }else{
      Serial.println("FAILED" + fbdo.errorReason());
    }
  }
}
}

