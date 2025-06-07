# Sistema de Alerta de Enchente com ESP32

Este projeto utiliza um ESP32, um sensor DHT22, um buzzer e dois LEDs para detectar possíveis enchentes com base na umidade do ambiente e em dados de um servidor (simulado).

---

## Objetivo

Verificar continuamente a umidade e simular uma consulta a um servidor para identificar risco de enchente.  
Se o risco for detectado (por umidade extrema ou servidor), o sistema ativa um alarme (buzzer) e acende o LED vermelho. Caso contrário, o LED verde permanece aceso.

---

## Componentes

- ESP32  
- Sensor DHT22  
- Buzzer Piezo  
- LED Verde  
- LED Vermelho  
- Resistores  
- Protoboard e jumpers  

---

## Pinos

| Função        | Pino |
|---------------|------|
| DHT22         | 15   |
| Buzzer        | 2    |
| LED Verde     | 5    |
| LED Vermelho  | 4    |

---

## Observações

- A requisição ao servidor é simulada, pois o Wokwi não permite acesso à internet.
- O código usa `millis()` para controle de tempo, sem `delay()`.

---

# Link Do Simulador:

https://wokwi.com/projects/433027740443594753

---

## Código

```cpp
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
  Serial.println("ESP32 + Buzzer + DHT22 + LEDs");
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
    simulatedServerResponse = 1; // Simulação
    serverUpdated = true;
  }

  if (humidityUpdated || serverUpdated) {
    bool risco = (simulatedServerResponse == 1) || (currentHumidity > 90.0);
    if (risco) {
      Serial.println("Risco detectado: alarme ativado.");
      if (!buzzerOn) {
        tone(PIEZO_PIN, 1000);
        buzzerEndTime = currentMillis + 10000;
        buzzerOn = true;
      }
      digitalWrite(LED_VERMELHO, HIGH);
      digitalWrite(LED_VERDE, LOW);
    } else {
      Serial.println("Sem risco.");
      digitalWrite(LED_VERMELHO, LOW);
      digitalWrite(LED_VERDE, HIGH);
    }
    humidityUpdated = false;
    serverUpdated = false;
  }
}
```
