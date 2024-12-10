#include <DHT.h>  
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>


#define FIREBASE_HOST "heating-system-54d88-default-rtdb.europe-west1.firebasedatabase.app"
#define FIREBASE_AUTH "jIA3FD1hrqAKnSsqFmv9ScJ0WpIKlCpjLmzZUdNM"

// Wi-Fi credentials
#define WIFI_SSID "TP-Link_9EC6"
#define WIFI_PASSWORD "10179260"

// Pins
#define DHT11_PIN D3
#define DHTTYPE DHT11 
#define LED_PIN D7

float temperature;
float humidity;

int refreshTime = 3000;
int lastRefreshTime = 0;
bool isOn;
float wantedTemp = 0;

// DHT11 sensor setup
DHT dht(DHT11_PIN, DHTTYPE);

void setup() {
  dht.begin();
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while(WiFi.status() != WL_CONNECTED){
    Serial.print("..");
    delay(200);
  }
  pinMode(D4, OUTPUT); 
  pinMode(LED_PIN, OUTPUT);
  
  Serial.println();
  Serial.println("NodeMCU is Connected!");
  Serial.println(WiFi.localIP());
    
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  if(millis() - lastRefreshTime >= refreshTime) {
    lastRefreshTime = millis();

    Firebase.setFloat("temperature", temperature);
    Firebase.setFloat("humidity", humidity);

    wantedTemp = Firebase.getFloat("wantedTemp");
    if(wantedTemp >= temperature) {
      digitalWrite(LED_PIN, HIGH);

    } else {
      digitalWrite(LED_PIN, LOW);
    }

    Serial.println("humidity"); 
    Serial.println(humidity);
    Serial.println("temperature");
    Serial.println(temperature);
  }

  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
}
