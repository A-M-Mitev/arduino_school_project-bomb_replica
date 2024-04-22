#include "header.h"

void checkDefusing(char key) {
  if(BombState == Armed && key == '#') {
    switch(keypad.getState()) {
      case PRESSED:
        defusing = true;
        defusing_started = millis();
        lcd.setCursor(0, 0);
        lcd.print("                    ");
        break;

      case RELEASED:
        defusing = false;
        input_password = "";
        input_length = 0;
        lcd.setCursor(0, 0);
        lcd.print("                    ");
        break;
    }
  }
}

unsigned long findNextBeep(unsigned long current_time, unsigned long time_left) {
  return (0.1 + 0.9 * (time_left / init_timer)) * 2000 + current_time;
}

void beep(unsigned long current_time, unsigned long time_left) {
  if(current_time >= next_beep) {
    tone(buzzer, 2200, beep_length);
    //tone(buzzer, 2564, BEEP_LENGTH);
    //tone(buzzer, 4231, BEEP_LENGTH);
    digitalWrite(led, !digitalRead(led));    // toggle
    next_beep = findNextBeep(current_time, time_left);
  }
}

void finalBeep() {
  //tone(buzzer, 2200, 1000);
  //tone(buzzer, 2564, 1000);
  tone(buzzer, 1500, 2000);
  digitalWrite(led, HIGH);
  delay(2000);
  digitalWrite(led, LOW);
}

void clearInput() {
  input_password = "";
  input_timer = "";
  input_length = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Clearing input...");
  lcd.setCursor(0, 1);
  lcd.print("--------------------");
  delay(1000);
  lcd.clear();
  if(BombState != ChangeTimer)
    printStars();
}

void enterPassword() {
  switch(BombState) {
  case WaitingForInput:
    if(input_password == password) {
      BombState = Armed;
      defusing = 0;
      time_of_bomb_activation = millis() + 1000;
      next_beep = time_of_bomb_activation;
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("THE BOMB");
      lcd.setCursor(6, 1);
      lcd.print("HAS BEEN");
      lcd.setCursor(7, 2);
      lcd.print("PLANTED");
      delay(1000);
      lcd.clear();
      printStars();
    }
    else 
      wrongPassword();
    break;

  case ChangePassword:
    BombState = SettingsMenu;
    password = input_password;
    updatePasswordLength();
    lcd.clear();
    lcd.setCursor(2, 1);
    lcd.print("NEW PASSWORD SET!");
    lcd.setCursor((20 - input_length) / 2, 2);
    lcd.print(password);
    delay(1000);
    lcd.clear();
    break;
  }
  input_password = "";
  input_length = 0;
}

void wrongPassword() {
  lcd.clear();
  lcd.setCursor(4, 1); 
  lcd.print("WRONG PASSWORD");
  lcd.setCursor(6, 2); 
  lcd.print("TRY AGAIN");
  delay(1000);
  lcd.clear();
  printStars();
}

void inputPassword(char key) {
  switch(key) {
  case NO_KEY:
    break;
  case '*':
    clearInput();
    break;
  case '#':
    enterPassword();
    break;
  default:
    if(input_length < 20) {
      input_password += key;
      ++input_length;
      printStars();
      lcd.setCursor(20 - input_length, 1);
      lcd.print(input_password);
    }
    else {
      lcd.setCursor(0, 2);
      lcd.print("Password is too long");
    }
    break;
  }
}

void inputTimer(char key) {
  switch(key) {
  case NO_KEY:
    break;
  case '*':
    clearInput();
    break;
  case '#': {
    double new_timer = convertInputTimer();
    if(new_timer > 10) {
      init_timer = new_timer * 1000;
      BombState = SettingsMenu;
      lcd.clear();
      lcd.setCursor(3, 1);
      lcd.print("NEW TIMER SET!");
      lcd.setCursor(5, 2);
      lcd.print(input_timer);
      lcd.print(" seconds");
      delay(1500);
      lcd.clear();
    }
    else {
      lcd.clear();
      lcd.setCursor(4, 2);
      lcd.print("                    ");
      lcd.setCursor(4, 2);
      lcd.print("[timer > 10]");
    }
    input_timer = "";
    input_length = 0;
    break;
  }
  default:
    if(input_length < 3) {
      input_timer += key;
      ++input_length;
      lcd.setCursor(9, 1);
      lcd.print(input_timer);
    }
    else {
      lcd.setCursor(3, 2);
      lcd.print("                    ");
      lcd.setCursor(3, 2);
      lcd.print("[timer < 1000]");
    }
    break;
  }
}

double convertInputTimer() {
  double new_timer = 0;
  for(int i = 0; input_timer[i] != '\0'; ++i) {
    new_timer = new_timer * 10 + (input_timer[i] - '0');
  }
  return new_timer;
}

void restartBomb(char key) {
  if(key == '#') {
    BombState = WaitingForInput;
    exmarks = "";
    number_of_exmarks = 0;
    input_password = "";
    input_length = 0;
    defusing = 0;
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Restarting...");
    delay(1000);
    lcd.clear();
    printStars();
  }
}

void printTimeLeft(unsigned long time_left) {
  lcd.setCursor(9, 3);
  if(time_left < 10000)
    lcd.print(" ");
  lcd.print(time_left / 1000);
}

void printDefuseTime() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("DEFUSE TIME CHANGED!");
  lcd.setCursor(5, 2);
  lcd.print(defusing_time / 1000);
  if(defusing_time == 10000)
    lcd.setCursor(7, 2);
  else 
    lcd.setCursor(6, 2);
  lcd.print(" seconds");
  delay(1500);
  lcd.clear();
  BombState = SettingsMenu;
}

void updatePasswordLength() {
  password_length = 0;
  for(int i = 0; password[i] != '\0'; i++) {
    ++password_length;
    stars += '*';
  }
}

void printStars() {
  lcd.setCursor(20 - password_length, 1); 
  lcd.print(stars);
}

void printExMarks(unsigned int new_number_of_exmarks) {
  if(new_number_of_exmarks > number_of_exmarks) {
    number_of_exmarks = new_number_of_exmarks;
    exmarks += '!';
  }
  lcd.setCursor(0, 3);
  lcd.print(exmarks);
  lcd.setCursor(20 - number_of_exmarks, 3);
  lcd.print(exmarks);
}