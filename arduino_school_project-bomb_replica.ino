#include "header.h"

void setup()
{
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
	lcd.begin();
	lcd.backlight();
  keypad.addEventListener(checkDefusing);
  updatePasswordLength();
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
    unsigned long current_time = millis();
    if(defusing) {
      unsigned long defusing_time_passed = current_time - defusing_started;
      lcd.setCursor(0, 0);
      lcd.print("Defusing...");
      lcd.setCursor(16, 0);
      lcd.print((defusing_time - defusing_time_passed) / 1000);
      int new_length = (defusing_time_passed / defusing_time) * password_length;
      if(new_length > input_length) {
        input_password += password[input_length];
        ++input_length;
        time_for_next_random = current_time + 300;
        printStars();
        lcd.setCursor(20 - password_length, 1);
        lcd.print(input_password); 
      }
      if(defusing_time_passed > defusing_time) {
        BombState = Disarmed;
        digitalWrite(led, LOW);
        lcd.setCursor(0,2);
        lcd.print(" ");
        delay(2000);
        lcd.clear();
      }
      else {
        if(current_time > time_for_next_random) {
          char random_symbol = random(33, 64);
          time_for_next_random = current_time + 300;
          lcd.setCursor(20 - (password_length - input_length), 1);
          lcd.print(random_symbol);
        }
      }
    }
    else {
      lcd.setCursor(0, 0); 
      lcd.print("Hold # to defuse");
    }
   
    unsigned long time_passed = current_time - time_of_bomb_activation;
    unsigned long time_left = init_timer - time_passed;
    beep(current_time, time_left);
    printTimeLeft(time_left);
    printExMarks((time_passed / init_timer) * 10);
    if(time_passed > init_timer) {
      BombState = Exploded;
      lcd.clear();
      lcd.setCursor(0, 3);
      lcd.print(exmarks);
      lcd.print(exmarks);
      finalBeep();
    }
    break;
  }

  case Disarmed:
    lcd.setCursor(1, 0);
    lcd.print("Counter-terrorists");
    lcd.setCursor(8, 1);
    lcd.print("win!");
    restartBomb(pressed_key);
    break;

  case Exploded:
    lcd.setCursor(5, 0);
    lcd.print("Terrorists");
    lcd.setCursor(8, 1);
    lcd.print("win!");
    restartBomb(pressed_key);
    break;

  case SettingsMenu:
    lcd.setCursor(0, 0); 
    lcd.print("Press [] to change:");
    lcd.setCursor(0, 1); 
    lcd.print("[1] password");
    lcd.setCursor(0, 2);
    lcd.print("[2] defuse time");
    lcd.setCursor(0, 3);
    lcd.print("[3] timer");  
    lcd.setCursor(13, 3); 
    lcd.print("#->Back");
    switch(pressed_key) {
      case '1':
        BombState = ChangePassword;
        stars = "";
        lcd.clear();
        break;
      case '3':
        BombState = ChangeTimer;
        lcd.clear();
        break;
      case '2':
        BombState = ChangeDefuse;
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
        updatePasswordLength();
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
    lcd.setCursor(3, 0); 
    lcd.print("Change timer:");
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
  
  case ChangeDefuse:
    lcd.setCursor(0, 0); 
    lcd.print("Defuse kit?");
    lcd.setCursor(0, 1); 
    lcd.print("[1] Yes -> 5 seconds");
    lcd.setCursor(0, 2);
    lcd.print("[2] No -> 10 seconds");  
    lcd.setCursor(13, 3); 
    lcd.print("#->Back");
    switch(pressed_key) {
      case '1':
        defusing_time = 5000;
        printDefuseTime(); 
        break;
      case '2':
        defusing_time = 10000;
        printDefuseTime();
        break;
      case '#':
        BombState = SettingsMenu;
        lcd.clear();
        break;
    }
    break;
  }
}