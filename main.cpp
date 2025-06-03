#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

#define DHT_PIN      15
#define TRIG_PIN     5
#define ECHO_PIN     18
#define LED_VERDE    22
#define LED_AMARELO  21
#define LED_VERMELHO 19
#define BUZZER_PIN   23

const char* ssid       = "Wokwi-GUEST";
const char* password   = "";
const char* mqtt_server = "test.mosquitto.org";
const char* mqtt_topic  = "leandro/1espb/enchente";

WiFiClient espClient;
PubSubClient client(espClient);
DHTesp dht;
LiquidCrystal_I2C lcd(0x27, 16, 2);


//conecta ao wifi
void connectToWiFi() {
  Serial.print("Conectando-se ao Wi-Fi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConectado ao Wi-Fi!");
}

//conecta ao mqtt
void connectToMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao broker MQTT...");
    if (client.connect("ESP32ClientLeonardo")) {
      Serial.println("Conectado!");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2;
  return distance;
}

void setup() {
  Serial.begin(115200);

  
  Wire.begin(13, 12);


  lcd.init();
  lcd.backlight();
  lcd.clear();


  WiFi.mode(WIFI_STA);
  connectToWiFi();
  client.setServer(mqtt_server, 1883);
  connectToMQTT();


  dht.setup(DHT_PIN, DHTesp::DHT22);


  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);


  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);


  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) connectToWiFi();
  if (!client.connected()) connectToMQTT();
  client.loop();


  TempAndHumidity data = dht.getTempAndHumidity();

  long distancia = readDistanceCM();
  

  String status = "Seguro";
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_AMARELO, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(BUZZER_PIN, LOW);


  if (distancia < 30) {
    status = "Perigo";
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, LOW);
    digitalWrite(LED_VERMELHO, HIGH);

    digitalWrite(BUZZER_PIN, HIGH);
  }

  else if (distancia < 70) {
    status = "Alerta";
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_AMARELO, HIGH);
    digitalWrite(LED_VERMELHO, LOW);
    digitalWrite(BUZZER_PIN, LOW);
  }

  //imprime no Serial
  Serial.print("Temperatura: ");
  Serial.print(data.temperature);
  Serial.print(" °C | Umidade: ");
  Serial.print(data.humidity);
  Serial.print(" % | Distância: ");
  Serial.print(distancia);
  Serial.print(" cm | LED: ");
  Serial.println(status);

  //atualiza LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(data.temperature, 0);
  lcd.print("C H:");
  lcd.print(data.humidity, 0);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lcd.print(status);
  lcd.print(" ");
  lcd.print(distancia);
  lcd.print("cm");

  //monta JSON e publica MQTT
  StaticJsonDocument<256> doc;
  doc["temperatura"] = data.temperature;
  doc["umidade"]     = data.humidity;
  doc["distancia"]   = distancia;
  doc["status"]         = status;

  char jsonBuffer[256];
  serializeJson(doc, jsonBuffer);
  client.publish(mqtt_topic, jsonBuffer);

  delay(5000);
}
