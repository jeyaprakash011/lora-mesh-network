

/*
 *  Node-1 (Device-ID-2)
 */

#include <SPI.h>
#include <LoRa.h>
#include <LiquidCrystal.h>

#define MY_ID 2
#define SYNC_WORD 0x12
#define MAX_HOPS 3

LiquidCrystal lcd(13, 14, 27, 26, 25, 33);

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.print("NODE 1 READY");

  LoRa.setPins(5, 21, 2);
  LoRa.begin(433E6);
  LoRa.setSyncWord(SYNC_WORD);
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (!packetSize) return;

  String data = "";
  while (LoRa.available()) data += (char)LoRa.read();

  int src, dest, lastHop, pid, hop;
  sscanf(data.c_str(), "%d|%d|%d|%d|%d|", &src, &dest, &lastHop, &pid, &hop);
  String msg = data.substring(data.lastIndexOf('|') + 1);

  lcd.clear();
  lcd.print("From:");
  lcd.print(src);
  lcd.setCursor(0, 1);
  lcd.print(msg);

  if (hop < MAX_HOPS) {
    hop++;
    String fwd =
      String(src) + "|" +
      String(dest) + "|" +
      String(MY_ID) + "|" +
      String(pid) + "|" +
      String(hop) + "|" +
      msg;

    LoRa.beginPacket();
    LoRa.print(fwd);
    LoRa.endPacket();
  }
}
