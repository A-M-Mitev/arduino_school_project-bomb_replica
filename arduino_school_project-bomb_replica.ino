#include "header.h"

void setup()
{
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
	lcd.begin();
	lcd.backlight();
  
  Serial.begin(9600);
  Serial.println("______________________________________");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");

  input_password.reserve(20); // max input characters
  setStars();
  printStars();
}

void loop()
{
  char pressed_key = keypad.getKey();
  unsigned long current_time = millis();

  switch(BombState) {
    case WaitingForInput:
      lcd.setCursor(0, 0); 
      lcd.print("Enter password:");
      if(input_password == "") {
        lcd.setCursor(0, 3); 
        lcd.print("*->Settings");
        if(pressed_key == '*')
          BombState = Settings; 
        else
          checkPressedKey(pressed_key);
      }
      else {
        lcd.setCursor(0, 3); 
        lcd.print("*->Clear    #->Enter");
        checkPressedKey(pressed_key);
      }
      break;

    case Armed:
      lcd.setCursor(0, 0); 
      lcd.print("Enter password:");
      lcd.setCursor(0, 3); 
      lcd.print("*->Clear    #->Enter");
      checkPressedKey(pressed_key);
      
      beep(current_time);
      time_left = init_timer - (current_time - time_of_bomb_activation);
      lcd.setCursor(9, 2); 
      lcd.print(time_left / 1000);
      lcd.print(" ");
      if(time_left > init_timer) {
        BombState = Exploded;
        lcd.clear();
      }
      break;
    
    case Disarmed: case Exploded:
      Serial.println("Press # to restart");
      lcd.setCursor(10, 3); 
      lcd.print("#->Restart");
      checkPressedKey(pressed_key);
      break;
    
    case Settings:
      break;

    default:
      Serial.print("Default\n");
      break;
  }
}