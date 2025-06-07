#include <DHT.h>

#define DHTPIN 15
#define DHTTYPE DHT22
#define PIEZO_PIN 2
#define LED_VERDE 5
#define LED_VERMELHO 4

DHT dht(DHTPIN, DHTTYPE);

unsigned long lastHumidityCheck = 0;
const unsigned long humidityInterval = 5000;

unsigned long lastServerCheck = 0;
const unsigned long serverInterval = 5000;

unsigned long buzzerEndTime = 0;
bool buzzerOn = false;

int simulatedServerResponse = 1;

float currentHumidity = 0;
bool humidityUpdated = false;
bool serverUpdated = false;

void setup() {
  Serial.begin(115200);
  pinMode(PIEZO_PIN, OUTPUT);
  digitalWrite(PIEZO_PIN, LOW);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  dht.begin();
  Serial.println("ESP32 + Buzzer + DHT22 + LEDs Verde/Vermelho - Sem delay");
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_VERMELHO, LOW);
}

void loop() {
  unsigned long currentMillis = millis();

  if (buzzerOn && currentMillis >= buzzerEndTime) {
    noTone(PIEZO_PIN);
    buzzerOn = false;
    Serial.println("Buzzer desligado");
  }

  if (currentMillis - lastHumidityCheck >= humidityInterval) {
    lastHumidityCheck = currentMillis;
    float humidity = dht.readHumidity();
    if (!isnan(humidity)) {
      currentHumidity = humidity;
      humidityUpdated = true;
      Serial.print("Umidade: ");
      Serial.print(humidity);
      Serial.println(" %");
    } else {
      Serial.println("Erro ao ler umidade!");
    }
  }

  if (currentMillis - lastServerCheck >= serverInterval) {
    lastServerCheck = currentMillis;
    simulatedServerResponse = 1; // Simulação normal, ajuste conforme sua lógica
    serverUpdated = true;
  }

  if (humidityUpdated || serverUpdated) {
    bool perturbacao = (simulatedServerResponse == 1) || (currentHumidity > 90.0);
    if (perturbacao) {
      Serial.println("Perturbação detectada: Ativando piezo e LED vermelho.");
      if (!buzzerOn) {
        tone(PIEZO_PIN, 1000);
        buzzerEndTime = currentMillis + 10000;
        buzzerOn = true;
      }
      digitalWrite(LED_VERMELHO, HIGH);
      digitalWrite(LED_VERDE, LOW);
    } else {
      Serial.println("Tudo OK - LED verde ligado.");
      digitalWrite(LED_VERMELHO, LOW);
      digitalWrite(LED_VERDE, HIGH);
    }
    humidityUpdated = false;
    serverUpdated = false;
  }
}
