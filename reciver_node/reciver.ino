


/*
 *  Final code Reciver (Device ID-4)
 */


#include <SPI.h>
#include <LoRa.h>
#include <LiquidCrystal.h>

#define MY_ID 4
#define SYNC_WORD 0x12

LiquidCrystal lcd(13, 14, 27, 26, 25, 33);

String lastLat  = "0.000";
String lastLon  = "0.000";
String lastTemp = "0.0";
int lastFromID  = 0;

bool alertActive = false;
unsigned long alertStart = 0;
const unsigned long ALERT_TIMEOUT = 5000;

void setup() {
  Serial.begin(115200);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Receiver Ready");

  LoRa.setPins(5, 21, 2);
  if (!LoRa.begin(433E6)) {
    lcd.clear();
    lcd.print("LoRa Failed");
    while (1);
  }

  LoRa.setSyncWord(SYNC_WORD);
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (!packetSize) return;

  String data = "";
  while (LoRa.available()) {
    data += (char)LoRa.read();
  }

  Serial.println("RX: " + data);

  int src = 0, dest = 0, lastHop = 0, pid = 0, hop = 0;

  int parsed = sscanf(
    data.c_str(),
    "%d|%d|%d|%d|%d|",
    &src, &dest, &lastHop, &pid, &hop
  );

  if (parsed != 5) return;
  if (dest != MY_ID) return;

  String msg = data.substring(data.lastIndexOf('|') + 1);

  
  lastFromID = lastHop;

  
  if (msg == "Motion Detected" || msg == "Sound Detected") {
    alertActive = true;
    alertStart = millis();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ALERT!");
    lcd.setCursor(0, 1);
    lcd.print(msg);
    return;
  }

 
  if (msg.startsWith("LT:")) {
    int lnIndex = msg.indexOf(" LN:");
    int tIndex  = msg.indexOf(" T:");

    if (lnIndex > 0 && tIndex > 0) {
      lastLat  = msg.substring(3, lnIndex);
      lastLon  = msg.substring(lnIndex + 4, tIndex);
      lastTemp = msg.substring(tIndex + 3);

      lastLat.trim();
      lastLon.trim();
      lastTemp.trim();
    }
  }

  
  if (alertActive && millis() - alertStart > ALERT_TIMEOUT) {
    alertActive = false;
    lcd.clear();
  }

  
  if (!alertActive) {
    lcd.setCursor(0, 0);
    lcd.print("LT:");
    lcd.print(lastLat);
    lcd.print(" T:");
    lcd.print(lastTemp);
    lcd.print(" ");

    lcd.setCursor(0, 1);
    lcd.print("LN:");
    lcd.print(lastLon);
    lcd.print(" F:");
    lcd.print(lastFromID);
    lcd.print(" ");
  }
}
