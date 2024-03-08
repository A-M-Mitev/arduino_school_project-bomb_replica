#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include "armed.h"

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup()
{
  pinMode(buzzer, OUTPUT);
	// initialize the LCD & turn on the backlight
	lcd.begin();
	lcd.backlight();
  Serial.begin(9600);
  
  delay(1000);
}

void loop()
{
    int next_beep = findNextBeepInterval(); 
    Serial.print("  Next beep will be in: ");
    Serial.print(next_beep);
    Serial.print("\n");
    delay(200);
    time_left_on_bomb = init_bomb_timer - (millis() - time_of_bomb_activation);
    Serial.print("  Time_left_on_bomb: ");
    Serial.print(time_left_on_bomb);
    Serial.print("\n");

  /*
  if(bomb_is_armed) { 
    int next_beep = findNextBeepInterval(); 
    Serial.print("Next beep will be in: ");
    Serial.print(next_beep);
    Serial.print("\n");
    delay(next_beep);
    beep();
    time_left_on_bomb = init_bomb_timer - (millis() - time_of_bomb_activation);
    Serial.print("Time left on bomb: ");
    Serial.print(time_left_on_bomb);
    Serial.print("\n");
  }
  else {
    lcd.setCursor(9, 0);
    lcd.print("BOMB");
    delay(100);
    lcd.setCursor(9, 1);
    lcd.print("HAS");
    delay(100);
    lcd.setCursor(9, 2);
    lcd.print("BEEN");
    delay(100);
    lcd.setCursor(7, 3);
    lcd.print("PLANTED");
    delay(1000);
    bomb_is_armed = true;
    /* 
      Code for keypad input...
      Condition to arm the bomb
        bomb_is_armed = true;
        time_of_bomb_activation = milis();
    
    * /
    
  }
  /*
    if(timer >= 0) {
      lcd.clear();  
      lcd.setCursor(6, 0);
      lcd.print("TIMER: ");
      lcd.print(timer);
      tone(buzzer, 2200, 125);
      delay(timeBetweenBeeps);
      
      //delay(1000);
      timer -= 1;
    }
    else {
      lcd.setCursor(7, 2);
      lcd.print("[BOOM]");
    }
    */
}
