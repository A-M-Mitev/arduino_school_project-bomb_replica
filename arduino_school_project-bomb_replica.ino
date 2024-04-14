#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "armed.h"

enum Menus {
  Settings = 0;
  WaitingForInput = 1;
  Armed = 2;
  Disarmed = 3;
  Exploded = 4;
};

enum Menus Menu = WaitingForInput; 

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
	lcd.begin();
	lcd.backlight();
  Serial.begin(9600);
  input_password.reserve(20); // max input characters

  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
}

void loop()
{
  char pressed_key = keypad.getKey();
  unsigned long current_time = millis();

  switch(Menu) {
    case WaitingForInput:
      lcd.setCursor(0, 1); 
      lcd.print("Enter password:");
      if(input_password == "") {
        lcd.setCursor(6, 3); 
        lcd.print("* -> Settings");
        if(pressed_key && pressed_key == '*') {
          Menu = Settings;
          break;
        }
      } 
      else {
        lcd.setCursor(0, 3); 
        lcd.print("# ->Enter '*'->Clear");
        checkPressedKey(pressed_key);
      }
      break;

    case Armed:
      beep(current_time);
      time_left = init_timer - (current_time - time_of_bomb_activation);
      lcd.setCursor(0, 0); 
      lcd.print("Time left: ");
      lcd.print(time_left / 1000);
      if(time_left > init_timer) {
        Menu = Exploded;
      }
      lcd.setCursor(0, 1); 
      lcd.print("Enter password:");
      lcd.setCursor(0, 3); 
      lcd.print("# ->Enter '*'->Clear");
      checkPressedKey(pressed_key);
      break;
    
    case Disarmed:
      break;

    case Exploded:
      break;
    
    case Settings:
      break;

    default:
      Serial.print("Default\n");
      break;
  }
}

/* FUNCTION DEFINITIONS */

unsigned long findNextBeep(unsigned long current_time) {
  return (0.1 + 0.9 * (time_left / init_timer)) * 1000 + current_time;
}

void beep(unsigned long current_time) {
  if(current_time >= next_beep) {
    tone(buzzer, 2200, beep_length);
    //tone(buzzer, 2564, BEEP_LENGTH);
    //tone(buzzer, 4231, BEEP_LENGTH);
    digitalWrite(led, !digitalRead(led)); // toggle

    last_beep = current_time;
    next_beep = findNextBeep(current_time);

    lcd.clear();
  }
}

void checkPressedKey(char key) {
  if(key == '*') {
      lcd.clear();
      lcd.setCursor(0, 1);
      Serial.println("Clearing input...");
      lcd.setCursor(0, 2);
      lcd.print("--------------------");
      input_password = "";
      delay(300);
    } else if(key == '#') {
      Serial.println("____________________________");
        if(input_password == password) {
          switch(Menu) {
            case WaitingForInput:
              Serial.println("The bomb has been planted");
              Menu = Armed
              lcd.clear();
              lcd.setCursor(6, 0);
              lcd.print("THE BOMB");
              delay(200);
              lcd.setCursor(6, 1);
              lcd.print("HAS BEEN");
              delay(200);
              lcd.setCursor(7, 2);
              lcd.print("PLANTED");
              delay(600);
              lcd.clear();
              break;
            
            case Armed:
              Serial.println("The bomb has been defused");
              Menu = Defused
              lcd.clear();
              lcd.setCursor(6, 0);
              lcd.print("THE BOMB");
              delay(200);
              lcd.setCursor(6, 1);
              lcd.print("HAS BEEN");
              delay(200);
              lcd.setCursor(7, 2);
              lcd.print("DEFUSED");
              delay(200);
              break;
          }
        } else {
          Serial.println("Wrong password, try again");
          lcd.clear();
          lcd.setCursor(4, 1); 
          lcd.print("WRONG PASSWORD");
          delay(250);
          lcd.setCursor(6, 2); 
          lcd.print("TRY AGAIN");
          delay(250);
        }
        input_password = "";
        Serial.println("____________________________");
      }
      else {
        input_password += key;
        Serial.println(input_password); 
        lcd.setCursor(0, 2); 
        lcd.print(stars);
        lcd.setCursor(0, 2);
        lcd.print(input_password);
      }
}