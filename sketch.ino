#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define DHTPIN 15
#define DHTTYPE DHT22

#define BUTTON_PIN 4
#define BUZZER_PIN 18
#define LED_PIN 19

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHT dht(DHTPIN, DHTTYPE);

float latitude = 18.5204;
float longitude = 73.8567;

unsigned long lastPressTime = 0;
int pressCount = 0;

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED Failed");
    while (true);
  }

  display.clearDisplay();
  display.display();

  Serial.println("=================================");
  Serial.println(" Alzheimer IoT Solution ");
  Serial.println("=================================");
}

void loop() {
  float temp = dht.readTemperature();

  showNormalScreen(temp);
  checkButton(temp);

  delay(200);
}

void showNormalScreen(float temp) {

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(0, 0);
  display.println("Alzheimer IoT");

  display.print("Temp:");
  display.print(temp);
  display.println(" C");

  display.print("Lat:");
  display.println(latitude, 4);

  display.print("Lon:");
  display.println(longitude, 4);

  display.println("Status:Normal");

  display.display();

  Serial.println("=================================");
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" C");

  Serial.print("Latitude: ");
  Serial.println(latitude, 4);

  Serial.print("Longitude: ");
  Serial.println(longitude, 4);

  Serial.println("Status: NORMAL");
  Serial.println("=================================");
}

void checkButton(float temp) {

  if (digitalRead(BUTTON_PIN) == LOW) {

    delay(50);

    if (digitalRead(BUTTON_PIN) == LOW) {

      pressCount++;
      lastPressTime = millis();

      while (digitalRead(BUTTON_PIN) == LOW) {
        delay(10);
      }
    }
  }

  if (pressCount > 0 && millis() - lastPressTime > 1500) {

    if (pressCount == 1) {
      emergencyAlert(temp);
    }

    else if (pressCount == 2) {
      medicineReminder();
    }

    pressCount = 0;
  }
}

void emergencyAlert(float temp) {

  digitalWrite(LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, HIGH);

  display.clearDisplay();

  display.setCursor(0, 0);
  display.println("EMERGENCY!");

  display.print("Temp:");
  display.print(temp);
  display.println(" C");

  display.print("Lat:");
  display.println(latitude, 4);

  display.print("Lon:");
  display.println(longitude, 4);

  display.println("Need Help!");

  display.display();

  Serial.println();
  Serial.println("******** EMERGENCY ALERT ********");

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" C");

  Serial.print("Latitude: ");
  Serial.println(latitude, 4);

  Serial.print("Longitude: ");
  Serial.println(longitude, 4);

  Serial.println("Patient requires assistance!");
  Serial.println("********************************");

  delay(5000);

  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZER_PIN, LOW);
}

void medicineReminder() {

  digitalWrite(LED_PIN, HIGH);

  for (int i = 0; i < 3; i++) {

    digitalWrite(BUZZER_PIN, HIGH);
    delay(300);

    digitalWrite(BUZZER_PIN, LOW);
    delay(300);
  }

  display.clearDisplay();

  display.setCursor(0, 0);
  display.println("MEDICINE TIME");
  display.println("");
  display.println("Eat Your");
  display.println("Medicine!");

  display.display();

  Serial.println();
  Serial.println("===== MEDICINE REMINDER =====");
  Serial.println("Eat Your Medicine!");
  Serial.println("Please take your tablet now.");
  Serial.println("=============================");

  delay(5000);

  digitalWrite(LED_PIN, LOW);
}
