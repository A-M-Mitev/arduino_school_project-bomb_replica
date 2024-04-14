#ifndef KEYPAD_H
#define KEYPAD_H

const int ROWS = 4;
const int COLUMNS = 3;
char keys[ROWS][COLUMNS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte pin_rows[ROWS] = {3, 4, 5, 6};
byte pin_columns[COLUMNS] = {7, 8, 9};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_columns, ROWS, COLUMNS);

const String password = "123";
String stars = "***";
String input_password = "";

#endif