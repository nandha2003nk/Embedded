#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Create ESP32 Wi-Fi AP
const char* ssid = "ESP32_Display";
const char* password = "12345678";

WebServer server(80);
String receivedText = "Waiting...";

void handleRoot() {
  String html = "<html><body>"
                "<h1>Send Text to ESP32</h1>"
                "<form action='/send' method='GET'>"
                "<input type='text' name='msg' style='width:200px;height:25px;'>"
                "<input type='submit' value='Send'>"
                "</form>"
                "</body></html>";

  server.send(200, "text/html", html);
}

void handleSend() {
  if (server.hasArg("msg")) {
    receivedText = server.arg("msg");

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println(receivedText);
    display.display();

    server.send(200, "text/html", "<h1>Message Sent!</h1><a href='/'>Back</a>");
  } else {
    server.send(400, "text/plain", "No message received");
  }
}

void setup() {
  Serial.begin(115200);

  // Start OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("ESP32 Display Ready");
  display.display();

  // Start Wi-Fi AP
  WiFi.softAP(ssid, password);
  Serial.println("Access Point started");
  Serial.println("Connect to WiFi: ESP32_Display");
  Serial.println("Password: 12345678");
  Serial.print("Open browser: http://");
  Serial.println(WiFi.softAPIP());

  // Web server routes
  server.on("/", handleRoot);
  server.on("/send", handleSend);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
