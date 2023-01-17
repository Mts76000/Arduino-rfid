
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>
#include <Servo.h>
#include "LiquidCrystal.h"
 
#define RST_PIN 49
#define SS_PIN 53
#define chair 7
 
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
Servo myServo;
 
String card1 = "29 55 01 B9"; 
String card2 = "99 2E CA 55";
String currentTagId = "";
boolean state = false;

LiquidCrystal lcd(8, 9, 10, 11, 12,13);  //Pins où on va connecter l'écran (RS, E, D4, D5, D6, D7)

 
void setup() {
   lcd.begin(16, 2);   //on initialise l'écran
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  pinMode(chair, OUTPUT);
  myServo.attach(7); // dire que le servo et sur le pin 7 
  myServo.write(0);  // le moteur se met a 0°
   lcd.clear();
    lcd.print("scanner");
    lcd.setCursor(0, 1);  
    lcd.print("votre badge");
  
  Serial.println("Please Tagging the Reader");
  Serial.println("--------------------------");
  Serial.println();
 
 pinMode(2, OUTPUT);
 pinMode(3, OUTPUT);
}
 
void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    currentTagId += mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ";
    currentTagId += String(mfrc522.uid.uidByte[i], HEX);
  }

  currentTagId.trim();
  currentTagId.toUpperCase();
 
  Serial.print("Card 1: ");
  Serial.println(card1);
  Serial.print("Card 2: ");
  Serial.println(card2);
  Serial.print("Check : ");
  Serial.println(currentTagId);

  
  Serial.println();


  if (currentTagId == card1) {
    // Serial.println(state); 
    if (state == false) {
      lcd.setCursor(0, 0);   
    lcd.clear();
      lcd.print("porte ouvert");
      digitalWrite(2, LOW);  // Extinction de la LED : état bas = LOW
      digitalWrite(3, HIGH); // Allumage de la LED : état haut = HIGH
     myServo.write(90);
      Serial.println("Pragnent Card");
      state = true;
      currentTagId = "";
      delay(500);
    }
     else {
      myServo.write(0);
      lcd.setCursor(0, 0);
      lcd.clear();
      lcd.print("porte fermer");
       digitalWrite(3, LOW);  // Extinction de la LED : état bas = LOW
      digitalWrite(2, HIGH); // Allumage de la LED : état haut = HIGH
      delay(3000);
      lcd.clear();
      lcd.print("scanner");
    lcd.setCursor(0, 1);  
    lcd.print("votre badge");
      Serial.println("scan");
      state = false;
      currentTagId = "";
      delay(500);
    }
  }
  if (currentTagId == card2) {
    myServo.write(0);
    Serial.println("No Pragnent Card");
    state = false;
    currentTagId = "";
    delay(500);
  }
  Serial.println();
  delay(200);
 
}
 
