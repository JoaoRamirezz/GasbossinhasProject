#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include <WiFi.h>

#define WIFI_SSID "iPhone de Maite"
#define WIFI_PASSWORD "maite2302"

#define DATABASE_URL "https://iot-senai-68871-default-rtdb.firebaseio.com/"
#define API_KEY "AIzaSyDnzcrPDg26EvPpAc6wGhME3StJbx1qttg"

#define CONN_PIN 35
#define POT_PIN 34

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

int value = 0;
bool signupOK = false;
bool isCriticalValue = false;

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

void loop() {
  int value = analogRead(POT_PIN);
  if (Firebase.ready() && signupOK) {
    isCriticalValue = value * 1.0 / 4095 > 0.65;
    Serial.println(isCriticalValue);

    digitalWrite(CONN_PIN, HIGH);

    if (!Firebase.RTDB.setInt(&fbdo, String("LDR_Data/") + String(millis() / 1000), value))
      Serial.println("ERRO: " + fbdo.errorReason());

    delay(5 * 1000);
  }
}
