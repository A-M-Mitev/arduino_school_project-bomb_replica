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
      break;

    case Armed:
      beep(current_time);
      time_left = init_timer - (current_time - time_of_bomb_activation);
      lcd.setCursor(0, 1); 
      lcd.print("Time left: ");
      lcd.print(time_left / 1000);
      if(time_left > init_timer) {
        Menu = Exploded;
      }
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