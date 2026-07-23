#include <DHT11.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// -------------------- DHT11 SETUP --------------------
#define DHTPIN 35        // D5 on T-Display
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

// -------------------- OLED SETUP ---------------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Init DHT11
  dht.begin();
  Serial.println("DHT11 + OLED Test");

  // Init OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed!");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("DHT11 Ready...");
  display.display();
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Validate readings
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT11");

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("DHT11 ERROR!");
    display.display();

    delay(2000);
    return;
  }

  // Print to serial
  Serial.print("Temp: ");
  Serial.print(t);
  Serial.print(" C | Humidity: ");
  Serial.print(h);
  Serial.println(" %");

  // Print to OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  
  display.setTextSize(2);
  display.print(t);
  display.println(" C");

  display.setTextSize(2);
  display.print(h);
  display.println(" %");

  display.display();

  delay(2000);   // DHT11 needs 2 sec delay
}
