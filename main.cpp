#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Servo lockServo;

#define SOC_PIN       1
#define CURRENT_PIN   2
#define TEMP_PIN      3
#define BUTTON_PIN   4

#define GREEN_LED     5
#define YELLOW_LED    6
#define RED_LED       7
#define BUZZER_PIN   10
#define RELAY_PIN    11
#define SERVO_PIN    12

bool vehicleConnected = false;
bool lastButtonState = HIGH;

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  lockServo.attach(SERVO_PIN);

  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  lockServo.write(0);

  Wire.begin(8, 9);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found!");
  } else {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("EV SMART CHARGER");
    display.println("----------------");
    display.println("System Ready");
    display.display();
  }
  delay(1500);
}

void loop() {
  int socRaw = analogRead(SOC_PIN);
  int currentRaw = analogRead(CURRENT_PIN);
  int tempRaw = analogRead(TEMP_PIN);

  float soc = (socRaw / 4095.0) * 100.0;
  float current = (currentRaw / 4095.0) * 10.0;
  float voltage = (tempRaw / 4095.0) * 3.3;
  float temperature = voltage * 100.0;

  bool buttonState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && buttonState == LOW) {
    vehicleConnected = !vehicleConnected;
    delay(200);
  }
  lastButtonState = buttonState;

  bool overTemperature = temperature >= 60.0;
  bool overCurrent = current >= 7.5;
  bool criticalTemperature = temperature >= 80.0;
  bool fault = overTemperature || overCurrent || criticalTemperature;

  if (!vehicleConnected) {
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    lockServo.write(0);
    showDisplay(soc, current, temperature, "READY");
  } else if (fault) {
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    lockServo.write(180);
    showDisplay(soc, current, temperature, "FAULT");
  } else {
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(YELLOW_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    lockServo.write(90);

    digitalWrite(BUZZER_PIN, temperature >= 50.0 ? HIGH : LOW);
    showDisplay(soc, current, temperature, "CHARGING");
  }

  Serial.print("SOC: ");
  Serial.print(soc, 1);
  Serial.print("% | Current: ");
  Serial.print(current, 1);
  Serial.print("A | Temp: ");
  Serial.print(temperature, 1);
  Serial.print("C | Vehicle: ");
  Serial.println(vehicleConnected ? "YES" : "NO");

  delay(500);
}

void showDisplay(float soc, float current, float temperature, const char *status) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("EV SMART CHARGER");
  display.println("----------------");

  display.print("VEHICLE : ");
  display.println(vehicleConnected ? "YES" : "NO");

  display.print("SOC     : ");
  display.print(soc, 0);
  display.println("%");

  display.print("TEMP    : ");
  display.print(temperature, 1);
  display.println(" C");

  display.print("CURRENT : ");
  display.print(current, 1);
  display.println(" A");

  display.print("STATUS  : ");
  display.println(status);

  display.display();
}
