#include "header.h"

unsigned long findNextBeep(unsigned long current_time, unsigned int time_left) {
  return (0.1 + 0.9 * (time_left / init_timer)) * 1000 + current_time;
}

void beep(unsigned long current_time, unsigned int time_left) {
  if(current_time >= next_beep) {
    tone(buzzer, 2200, beep_length);
    //tone(buzzer, 2564, BEEP_LENGTH);
    //tone(buzzer, 4231, BEEP_LENGTH);
    digitalWrite(led, !digitalRead(led)); // toggle

    last_beep = current_time;
    next_beep = findNextBeep(current_time, time_left);
  }
}

void clearInput() {
  input_password = "";
  input_timer = "";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Clearing input...");
  lcd.setCursor(0, 1);
  lcd.print("--------------------");
  Serial.println("Clearing input...");
  delay(1000);
  lcd.clear();
  if(BombState != ChangeTimer) {
    printStars();
  }
}

void enterPassword() {
  switch(BombState) {
    case WaitingForInput:
      if(input_password == password) {
        BombState = Armed;
        time_of_bomb_activation = millis() + 1000;
        next_beep = time_of_bomb_activation;
        lcd.clear();
        lcd.setCursor(6, 0);
        lcd.print("THE BOMB");
        lcd.setCursor(6, 1);
        lcd.print("HAS BEEN");
        lcd.setCursor(7, 2);
        lcd.print("PLANTED");
        Serial.println("The bomb has been planted");
        delay(1000);
        lcd.clear();
        printStars();
      }
      else 
        wrongPassword();
      break;

    case Armed:
      if(input_password == password) {
        BombState = Disarmed;
        lcd.clear();
        lcd.setCursor(6, 0);
        lcd.print("THE BOMB");
        lcd.setCursor(6, 1);
        lcd.print("HAS BEEN");
        lcd.setCursor(7, 2);
        lcd.print("DEFUSED");
        Serial.println("The bomb has been defused");
        delay(1000);
        lcd.clear();
      }
      else
        wrongPassword();
      break;

    case ChangePassword:
      BombState = SettingsMenu;
      password = input_password;
      setStars();
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print("NEW PASSWORD SET!");
      lcd.setCursor(0, 2);
      lcd.print(password);
      delay(1000);
      lcd.clear();
      break;
  }
  input_password = "";
}

void wrongPassword() {
  lcd.clear();
  lcd.setCursor(4, 1); 
  lcd.print("WRONG PASSWORD");
  lcd.setCursor(6, 2); 
  lcd.print("TRY AGAIN");
  Serial.println("Wrong password, try again");
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
      input_password += key;
      printStars();
      lcd.setCursor(0, 1);
      lcd.print(input_password);
      Serial.println(password);
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
      float new_timer = convertInputTimer();
      if(new_timer >= 5 && new_timer < 1000) {
        init_timer = new_timer;
        BombState = SettingsMenu;
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("NEW TIMER SET!");
        lcd.setCursor(0, 2);
        lcd.print(input_timer);
        lcd.print(" seconds");
        delay(1000);
        lcd.clear();
      }
      else {
        lcd.clear();
        lcd.setCursor(1, 2);
        lcd.print("(4 < timer < 1000)");
      }
      input_timer = "";
      break;
    }
    default:
      input_timer += key;
      lcd.setCursor(0, 1);
      lcd.print(input_timer);
      break;
  }
}

float convertInputTimer() {
  float new_timer = 0;
  for(int i = 0; input_timer[i] != '\0'; ++i) {
    new_timer = new_timer * 10 + (input_timer[i] - '0');
  }
  return new_timer;
}

void restartBomb(char key) {
  lcd.setCursor(10, 3); 
  lcd.print("#->Restart");
  Serial.println("Press # to restart");
  if(key == '#') {
    BombState = WaitingForInput;
    input_password = "";
    lcd.clear();
    lcd.setCursor(0, 1); 
    lcd.print("Restarting...");
    Serial.println("Restarting...");  
    delay(1000);
    lcd.clear();
    printStars();
  }
}

void printTimeLeft(unsigned int time_left) {
  lcd.setCursor(9, 2);
  if(time_left < 10000)
    lcd.print(" ");
  lcd.print(time_left / 1000);
}

void setStars() {
  for(int i = 0; password[i] != '\0'; i++) {
    stars += '*';
  }
}

void printStars() {
  lcd.setCursor(0, 1); 
  lcd.print(stars);
}

void printExMarks(unsigned int new_number_of_exmarks) {
  if(new_number_of_exmarks > number_of_exmarks) {
    number_of_exmarks = new_number_of_exmarks;
    exmarks += '!';
  }
  lcd.setCursor(0, 2);
  lcd.print(exmarks);
  lcd.setCursor(20 - number_of_exmarks, 2);
  lcd.print(exmarks);
}