
/*
 * sender
 */
/*#include <SPI.h>
#include <LoRa.h>
#include <LiquidCrystal.h>

#define MY_ID 1
#define RECEIVER_ID 4
#define SYNC_WORD 0x12

LiquidCrystal lcd(13, 14, 27, 26, 25, 22);
int packetID = 0;

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("SENDER READY");

  LoRa.setPins(5, 21, 2);
  LoRa.begin(433E6);
  LoRa.setSyncWord(SYNC_WORD);
  LoRa.setTxPower(17);
}

void loop() {
  String msg = "Hello Receiver";

  // NEW PACKET FORMAT:
  // SRC | DEST | LAST_HOP | PACKET_ID | HOP | MSG
  String packet = String(MY_ID) + "|" +
                  String(RECEIVER_ID) + "|" +
                  String(MY_ID) + "|" +        //  LAST_HOP = Sender
                  String(packetID++) + "|" +
                  "0|" +
                  msg;

  LoRa.beginPacket();
  LoRa.print(packet);
  LoRa.endPacket();

  lcd.clear();
  lcd.print("Sent:");
  lcd.setCursor(0, 1);
  lcd.print(msg);

  Serial.println("TX: " + packet);
  delay(5000);
}

*/




/*
 *  sender final code
 */

 
#include <SPI.h>
#include <LoRa.h>
#include <LiquidCrystal.h>
#include <TinyGPS++.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define MY_ID 1
#define RECEIVER_ID 4
#define SYNC_WORD 0x12

#define PIR_PIN 35
#define SOUND_PIN 34
#define TEMP_PIN 32
#define RXD2 16
#define TXD2 17

LiquidCrystal lcd(13, 14, 27, 26, 25, 22);
TinyGPSPlus gps;
HardwareSerial gpsSerial(2);
OneWire oneWire(TEMP_PIN);
DallasTemperature sensors(&oneWire);

int packetID = 0;
bool soundActive = false;
unsigned long soundStartTime = 0;
const int soundThreshold = 3000;
const unsigned long SOUND_MIN_TIME = 300;

volatile bool motionDetected = false;
unsigned long lastTrigger = 0;
unsigned long lastGpsSend = 0;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR motionISR() {
  portENTER_CRITICAL_ISR(&mux);
  motionDetected = true;
  lastTrigger = millis();
  portEXIT_CRITICAL_ISR(&mux);
}

void sendLoRa(String msg) {
  String packet =
    String(MY_ID) + "|" +
    String(RECEIVER_ID) + "|" +
    String(MY_ID) + "|" +
    String(packetID++) + "|" +
    "0|" +
    msg;

  LoRa.beginPacket();
  LoRa.print(packet);
  LoRa.endPacket();
}

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT_PULLUP);
  pinMode(SOUND_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), motionISR, RISING);

  lcd.begin(16, 2);
  lcd.print("Sender Ready");

  gpsSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
  sensors.begin();

  LoRa.setPins(5, 21, 2);
  LoRa.begin(433E6);
  LoRa.setSyncWord(SYNC_WORD);
}

void loop() {
  while (gpsSerial.available()) gps.encode(gpsSerial.read());

  float lat = gps.location.isValid() ? gps.location.lat() : 12.345;
  float lon = gps.location.isValid() ? gps.location.lng() : 76.973;

  if (motionDetected) {
    sendLoRa("Motion Detected");
    motionDetected = false;
    
  }

  int soundValue = analogRead(SOUND_PIN);
  if (soundValue < soundThreshold && !soundActive) {
    soundActive = true;
    sendLoRa("Sound Detected");
    soundStartTime = millis();
  }
  if (soundValue >= soundThreshold && soundActive) {
    soundActive = false;
    if (millis() - soundStartTime > SOUND_MIN_TIME) {
      
    }
  }

  if (millis() - lastGpsSend > 3000) {
    lastGpsSend = millis();
    sensors.requestTemperatures();
    float t = sensors.getTempCByIndex(0);

    String msg = "LT:" + String(lat, 3) +
                 " LN:" + String(lon, 3) +
                 " T:" + String(t, 1);

    sendLoRa(msg);

    lcd.setCursor(0, 0);
    lcd.print("LT:");
    lcd.print(lat, 3);
    lcd.print(" T:");
    lcd.print(t, 1);

    lcd.setCursor(0, 1);
    lcd.print("LN:");
    lcd.print(lon, 3);
    lcd.print("   ");
  }
}
