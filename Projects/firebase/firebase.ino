#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <TimeLib.h>
#include <WiFi.h>

#define WIFI_SSID "iPhone de Maite"
#define WIFI_PASSWORD "maite2302"

#define API_KEY "AIzaSyDnzcrPDg26EvPpAc6wGhME3StJbx1qttg"
#define DATABASE_URL "https://iot-senai-68871-default-rtdb.firebaseio.com/" 

#define POT_PIN 25

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK = false;

void setup() {
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Ok");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){    
  if (Firebase.ready() && signupOK) {
    if (!Firebase.RTDB.setInt(&fbdo, String("LDR_Data/") + String(now()), analogRead(POT_PIN))) {
      Serial.println("ERRO: " + fbdo.errorReason());
    }

    delay(10 * 1000);
  }
}