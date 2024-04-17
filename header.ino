#include "header.h"

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
  }
}

void clearPassword() {
  Serial.println("Clearing input...");
  input_password = "";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Clearing input...");
  lcd.setCursor(0, 1);
  lcd.print("--------------------");
  delay(1000);
  lcd.clear();
  printStars();
}

void enterPassword() {
  switch(BombState) {
    case WaitingForInput:
      if(input_password == password) {
        Serial.println("The bomb has been planted");
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
        delay(1000);
        lcd.clear();
        printStars();
      }
      else {
        Serial.println("Wrong password, try again");
        lcd.clear();
        lcd.setCursor(4, 1); 
        lcd.print("WRONG PASSWORD");
        lcd.setCursor(6, 2); 
        lcd.print("TRY AGAIN");
        delay(1000);
        lcd.clear();
        printStars();
      }
      break;

    case Armed:
      if(input_password == password) {
        Serial.println("The bomb has been defused");
        BombState = Disarmed;
        lcd.clear();
        lcd.setCursor(6, 0);
        lcd.print("THE BOMB");
        lcd.setCursor(6, 1);
        lcd.print("HAS BEEN");
        lcd.setCursor(7, 2);
        lcd.print("DEFUSED");
        delay(1000);
        lcd.clear();
      }
      else {
        Serial.println("Wrong password, try again");
        lcd.clear();
        lcd.setCursor(4, 1); 
        lcd.print("WRONG PASSWORD");
        lcd.setCursor(6, 2); 
        lcd.print("TRY AGAIN");
        delay(1000);
        lcd.clear();
      }
      break;

    case Settings:
      break;

    case Disarmed: case Exploded:
      Serial.println("Restarting...");
      BombState = WaitingForInput;
      lcd.clear();
      lcd.setCursor(0, 1); 
      lcd.print("Restarting...");  
      delay(1000);
      lcd.clear();
      printStars();
      break;
  }
  input_password = "";
}

void checkPressedKey(char key) {
  if(!key) return ;
  if(key == '*')
    clearPassword();
  else if(key == '#') 
    enterPassword();
  else {
    Serial.println(password);
    input_password += key;
    printStars();
    lcd.setCursor(0, 1);
    lcd.print(input_password);
  }
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