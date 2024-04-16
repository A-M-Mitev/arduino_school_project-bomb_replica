#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include "armed.h"
#include "keypad-4x3.h"

enum BombStates {
  Settings = 0,
  WaitingForInput = 1,
  Armed = 2,
  Disarmed = 3,
  Exploded = 4
};

enum BombStates BombState = WaitingForInput; 

LiquidCrystal_I2C lcd(0x27, 20, 4);

char keys[4][3] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte pin_rows[4] = {4, 5, 6, 7};
byte pin_columns[3] = {8, 9, 10};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_columns, 4, 3);

String  password = "123";
String  input_password = "";
String  stars = "***";

void setup()
{
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
	lcd.begin();
	lcd.backlight();
  Serial.begin(9600);
  input_password.reserve(20); // max input characters

  Serial.println("______________________________________");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  lcd.setCursor(0, 1); 
  lcd.print(stars);
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
        if(pressed_key) {
          if(pressed_key == '*') {
            BombState = Settings;
          } 
          else {
            checkPasswordInput(pressed_key);
          }
        }
      }
      else {
        lcd.setCursor(0, 3); 
        lcd.print("*->Clear    #->Enter");
        if(pressed_key)
          checkPasswordInput(pressed_key);
      }
      break;

    case Armed:
      lcd.setCursor(0, 0); 
      lcd.print("Enter password:");
      lcd.setCursor(0, 3); 
      lcd.print("*->Clear    #->Enter");
      if(pressed_key)
        checkPasswordInput(pressed_key);
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
      if(pressed_key && pressed_key == '#') {
        lcd.clear();
        lcd.setCursor(0, 1); 
        lcd.print("Restarting...");
        Serial.println("Restarting...");
        BombState = WaitingForInput;
        input_password = "";
        pressed_key = 0; // ?
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 1); 
        lcd.print(stars);
      }
      break;
    
    case Settings:
      break;

    default:
      Serial.print("Default\n");
      break;
  }
}



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

void checkPasswordInput(char key) {
  if(key == '*') {
      Serial.println("Clearing input...");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Clearing input...");
      lcd.setCursor(0, 1);
      lcd.print("--------------------");
      input_password = "";
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 1); 
      lcd.print(stars);
    } else if(key == '#') {
      Serial.println("____________________________");
        if(input_password == password) {
          switch(BombState) {
            case WaitingForInput:
              Serial.println("The bomb has been planted");
              BombState = Armed;
              time_of_bomb_activation = millis() + 1000;
              next_beep = time_of_bomb_activation;
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
              lcd.setCursor(0, 1); 
              lcd.print(stars);
              break;
            
            case Armed:
              Serial.println("The bomb has been defused");
              BombState = Disarmed;
              lcd.clear();
              lcd.setCursor(6, 0);
              lcd.print("THE BOMB");
              delay(200);
              lcd.setCursor(6, 1);
              lcd.print("HAS BEEN");
              delay(200);
              lcd.setCursor(7, 2);
              lcd.print("DEFUSED");
              delay(600);
              lcd.clear();
              break;
          }
        } else {
          Serial.println("Wrong password, try again");
          lcd.clear();
          lcd.setCursor(4, 1); 
          lcd.print("WRONG PASSWORD");
          delay(330);
          lcd.setCursor(6, 2); 
          lcd.print("TRY AGAIN");
          delay(670);
          lcd.clear();
          lcd.setCursor(0, 1); 
          lcd.print(stars);
        }
        input_password = "";
        Serial.println("____________________________");
      }
      else {
        Serial.println(password);
        input_password += key;
        lcd.setCursor(0, 1); 
        lcd.print(stars);
        lcd.setCursor(0, 1);
        lcd.print(input_password);
      }
}