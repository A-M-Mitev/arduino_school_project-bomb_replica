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
  /*
  // max input characters
  input_password.reserve(20); // one row on the display
  input_timer.reserve(3);  // up to 999
  */
  setStars();
  printStars();
}

void loop()
{
  char pressed_key = keypad.getKey();
  switch(BombState) {
    case WaitingForInput:
      lcd.setCursor(0, 0); 
      lcd.print("Enter password:");
      if(input_password == "") {
        lcd.setCursor(0, 3); 
        lcd.print("*->Settings");
        if(pressed_key && pressed_key == '*'){
          BombState = SettingsMenu;
          Serial.println("Going to settings");
          lcd.clear();
          break;
        }
      }
      else {
        lcd.setCursor(0, 3); 
        lcd.print("*->Clear    #->Enter");
      }
      inputPassword(pressed_key);
      break;

    case Armed: {
      lcd.setCursor(0, 0); 
      lcd.print("Enter password:");
      lcd.setCursor(0, 3); 
      lcd.print("*->Clear    #->Enter");
      inputPassword(pressed_key);
      
      unsigned long current_time = millis();
      unsigned int time_passed = current_time - time_of_bomb_activation;
      unsigned int time_left = init_timer - time_passed;
      beep(current_time, time_left);
      printTimeLeft(time_left);
      printExMarks((time_passed / init_timer) * 10);
      if(time_passed > init_timer) {
        BombState = Exploded;
        lcd.clear();
        lcd.setCursor(0, 2);
        lcd.print(exmarks);
        lcd.print(exmarks);
      }
      break;
    }

    case Disarmed:
      Serial.println("Counter-terrorists win!");
      restartBomb(pressed_key);
      break;

    case Exploded:
      Serial.println("Terrorists win!");
      restartBomb(pressed_key);
      break;

    case SettingsMenu:
      lcd.setCursor(0, 0); 
      lcd.print("Press [] to:");
      lcd.setCursor(0, 1); 
      lcd.print("[1] change password");
      lcd.setCursor(0, 2);
      lcd.print("[2] change timer"); 
      lcd.setCursor(13, 3); 
      lcd.print("#->Back");
      switch(pressed_key) {
        case '1':
          BombState = ChangePassword;
          stars = "";
          lcd.clear();
          break;
        case '2':
          BombState = ChangeTimer;
          lcd.clear();
          break;
        case '#':
          BombState = WaitingForInput;
          lcd.clear();
          printStars();
          break;
      }
      break;

    case ChangePassword:
      lcd.setCursor(0, 0); 
      lcd.print("Enter new password:");
      if(input_password == "") {
        lcd.setCursor(13, 3); 
        lcd.print("#->Back");
        if(pressed_key == '#') {
          BombState = SettingsMenu;
          setStars();
          lcd.clear();
          break;
        }
      }
      else {
        lcd.setCursor(0, 3); 
        lcd.print("*->Clear    #->Enter");
      }
      inputPassword(pressed_key);
      break;

    case ChangeTimer:
      lcd.setCursor(0, 0); 
      lcd.print("Enter new timer:");
      if(input_timer == "") {
        lcd.setCursor(13, 3);
        lcd.print("#->Back");
        if(pressed_key == '#') {
          BombState = SettingsMenu;
          lcd.clear();
          break;
        }
      }
      else {
        lcd.setCursor(0, 3); 
        lcd.print("*->Clear    #->Enter");
      }
      inputTimer(pressed_key);
      break;
  }
}