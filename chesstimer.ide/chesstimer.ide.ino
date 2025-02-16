#include <LiquidCrystal.h>
#define SEC (60 +(millis() / 1000))
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

int contrast=75;
const int buttonWhite = 8;
const int buttonBlack = 9;
const int pinBuzzer = 10;
const int analogPin = A0;

int lcd_key = 0;
int adc_key_in = 0;
int countMenu = 0;
int blinkTime;
int initBlinkTime;
int blinkDelay = 300;
bool exitMenu = 1;
// bool sidePlayer = true;
bool blinkState = true;
//Variables for players
int cTemp = 0;
int cSecWhite = 0;
int cSecBlack = 0;
//Variables for white
int setSecWhite;
int horWhite;
int minWhite;
int secWhite;
//Variables for black
int setSecBlack;
int horBlack;
int minBlack;
int secBlack;
//Configuration for lcd
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
//Fix the skip of the keys
void debounceKey() {
  while (adc_key_in < 1000) {
    adc_key_in = analogRead(analogPin);
  }
}
//Read keyboard buttons
int read_LCD_buttons(){
  adc_key_in = analogRead(analogPin);      // read sensor value
  if (adc_key_in > 1000) return btnNONE; // We made this the first option for speed reasons, as it will be the most likely outcome.
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 195)  return btnUP;
  if (adc_key_in < 380)  return btnDOWN;
  if (adc_key_in < 555)  return btnLEFT;
  if (adc_key_in < 790)  return btnSELECT;
  return btnNONE;  // when all else fails, give this back...
}
//configuration menu
void menuSetUp() {
  lcd_key = read_LCD_buttons();  // read the buttons
  switch (lcd_key)  {
    case btnRIGHT:{
        countMenu++;
        if (countMenu > 3) countMenu = 0;
        debounceKey();
        break;
      }
    case btnLEFT:{
        countMenu--;
        if (countMenu < 0) countMenu = 3;
        debounceKey();
        break;
      }
    case btnUP:{
        if (countMenu == 0) { //increase hour
          horWhite++;
          if (horWhite > 9) horWhite = 0;
          debounceKey();
        }
        else if (countMenu == 1) { //increase mins
          minWhite++;
          if (minWhite > 59) minWhite = 0;
          debounceKey();
        }
        else if (countMenu == 2) { //increase secs
          secWhite++;
          if (secWhite > 59) secWhite = 0;
          debounceKey();
        }
        break;
      }
    case btnDOWN:{
        if (countMenu == 0) { //decrease the hours
          horWhite--;
          if (horWhite < 0) horWhite = 9;
          debounceKey();
        }
        else if (countMenu == 1) { //decrease the mins
          minWhite--;
          if (minWhite < 0) minWhite = 59;
          debounceKey();
        }
        else if (countMenu == 2) { //decrease the mins
          secWhite--;
          if (secWhite < 0) secWhite = 59;
          debounceKey();
        }
        break;
      }
    case btnSELECT:{ //exit the setup menu      
        exitMenu = 0;
        break;
      }
    case btnNONE:{
        break;
      }
  }
}
//printing white values for LCD and printing timer for setup menu
void printTimerWhite() {
  //used to flash the timer
  blinkTime = millis() - blinkDelay;
  //blinking print time 
  //used only in setup
  if (exitMenu == 1 && countMenu == 0) {
    if (blinkTime >= initBlinkTime && blinkState == 1) {
      lcd.setCursor(0, 1);
      lcd.print(" ");
      blinkState = 0;
      initBlinkTime = millis();
    }
    if (blinkTime >= initBlinkTime && blinkState == 0) {
      lcd.setCursor(0, 1);
      lcd.print(horWhite);
      blinkState = 1;
      initBlinkTime = millis();
    }
  }
  //print time without blinking
  else {
    lcd.setCursor(0, 1);
    lcd.print(horWhite);
  }
  lcd.setCursor(1, 1);
  lcd.print(":");
  //printing blinking minutes
  //used only in setup
  if (exitMenu == 1 && countMenu == 1) {
    if (blinkTime >= initBlinkTime && blinkState == 1) {
      lcd.setCursor(2, 1);
      lcd.print("  ");
      blinkState = 0;
      initBlinkTime = millis();
    }
    if (blinkTime >= initBlinkTime && blinkState == 0) {
      //this sets the print value below 10 by moving the cursor one notch and placing 0 to the left
      if ((minWhite < 10) && (minWhite >= 0)) {
        lcd.setCursor(2, 1);
        lcd.print(0);
        lcd.setCursor(3, 1);
        lcd.print(minWhite);
      }
      else {
        lcd.setCursor(2, 1);
        lcd.print(minWhite);
      }
      blinkState = 1;
      initBlinkTime = millis();
    }
  }
  //printing minutes without blinking
  else {
    //this sets the print value below 10 by moving the cursor one notch and placing 0 to the left
    if ((minWhite < 10) && (minWhite >= 0)) {
      lcd.setCursor(2, 1);
      lcd.print(0);
      lcd.setCursor(3, 1);
      lcd.print(minWhite);
    }
    else {
      lcd.setCursor(2, 1);
      lcd.print(minWhite);
    }
  }
  lcd.setCursor(4, 1);
  lcd.print(":");
  //print blinking seconds
  //used only in setup
  if (exitMenu == 1 && countMenu == 2) {
    if (blinkTime >= initBlinkTime && blinkState == 1) {
      lcd.setCursor(5, 1);
      lcd.print("  ");
      blinkState = 0;
      initBlinkTime = millis();
    }
    if (blinkTime >= initBlinkTime && blinkState == 0) {
      //this sets the print value below 10 by moving the cursor one point and placing 0 to the left
      if ((secWhite < 10) && (secWhite >= 0)) {
        lcd.setCursor(5, 1);
        lcd.print(0);
        lcd.setCursor(6, 1);
        lcd.print(secWhite);
      }
      else {
        lcd.setCursor(5, 1);
        lcd.print(secWhite);
      }
      blinkState = 1;
      initBlinkTime = millis();
    }
  }
  //print seconds without blinking
  else {
    //this sets the print value below 10 by moving the cursor one notch and placing 0 to the left
    if ((secWhite < 10) && (secWhite >= 0)) {
      lcd.setCursor(5, 1);
      lcd.print(0);
      lcd.setCursor(6, 1);
      lcd.print(secWhite);
    }
    else {
      lcd.setCursor(5, 1);
      lcd.print(secWhite);
    }
  }
}
//Black value printing on LCD
void printTimerBlack() {
  //printing time
  lcd.setCursor(9, 1);
  lcd.print(horBlack);
  lcd.setCursor(10, 1);
  lcd.print(":");
  //print minutes
  //this sets the print value below 10 by moving the cursor one point and placing 0 to the left
  if ((minBlack < 10) && (minBlack >= 0)) {
    lcd.setCursor(11, 1);
    lcd.print(0);
    lcd.setCursor(12, 1);
    lcd.print(minBlack);
  }
  else {
    lcd.setCursor(11, 1);
    lcd.print(minBlack);
  }
  lcd.setCursor(13, 1);
  lcd.print(":");
  //print seconds
  //this sets the print value below 10 by moving the cursor one point and placing 0 to the left
  if ((secBlack < 10) && (secBlack >= 0)) {
    lcd.setCursor(14, 1);
    lcd.print(0);
    lcd.setCursor(15, 1);
    lcd.print(secBlack);
  }
  else {
    lcd.setCursor(14, 1);
    lcd.print(secBlack);
  }
}
//print configure menu
void printMenu() {
  lcd.setCursor(4, 0);
  lcd.print("SETTINGS");
  //used to blink the timer
  blinkTime = millis() - blinkDelay;
}
//printing labels
void printLabels() {
  lcd.clear();
  // if (sidePlayer == 1) { //selected in menuSetUp
  lcd.setCursor(0, 0);
  lcd.print("WHITE");
  lcd.setCursor(11, 0);
  lcd.print("BLACK");
  lcd.setCursor(7, 1);
  lcd.print("||");
  tone(pinBuzzer, 500, 300);
  digitalWrite(LED_BUILTIN, LOW);
  //print timer values
  printTimerWhite();
  printTimerBlack();
}
void pauseGameWhite() {
  //with the game paused on the white timer, to exit you need to press the black button
  while (digitalRead(buttonBlack) == 0) {
    //used to blink the timer
    blinkTime = millis() - blinkDelay;
    //print < blinking character
    if (blinkTime >= initBlinkTime && blinkState == 1) {
      lcd.setCursor(5, 0);
      lcd.print(" ");
      blinkState = 0;
      initBlinkTime = millis();
    }
  }
  lcd.setCursor(5, 0);
  lcd.print(" ");
}
void pauseGameBlack() {
  //with the game paused on the white timer, to exit you need to press the black button
  while (digitalRead(buttonWhite) == 0) {
    //used to blink the timer
    blinkTime = millis() - blinkDelay;
    //print > blinking character
    if (blinkTime >= initBlinkTime && blinkState == 1) {
      lcd.setCursor(10, 0);
      lcd.print(" ");
      blinkState = 0;
      initBlinkTime = millis();
    }
  }
  lcd.setCursor(10, 0);
  lcd.print(" ");
}
//white race timer
void timerWhite() {
  secWhite = setSecWhite - cSecWhite;
  printTimerWhite(); //update values
//* making the stopwatch *//
//if the seconds timer is below 0 (-1) and the minutes >= 0 plus the seconds counter with 59 to
//setSecWhite updates the value and removes 1 in the minutes timer
  if ((secWhite == -1) && (minWhite >= 0)) {
    setSecWhite = cSecWhite + 59; //atualizar setSecWhite
    minWhite--;
  }
  //if the seconds and minutes timer is below 0 (-1) and the hour is > 0 plus 59 then
//setSecWhite updates the value, sets the minutes to 59 and removes 1 from the hour
  if ((secWhite == -1) && (minWhite == -1) && (horWhite > 0)) {
    setSecWhite = cSecWhite + 59;
    minWhite = 59;
    horWhite--;
  }
  //if the seconds, minutes and hours timers go to 0 loop forever and print the end game
  if ((secWhite == 0) && (minWhite == 0) && (horWhite == 0)) {
    tone(pinBuzzer, 500, 1000);
    while (1) {
      lcd.setCursor(10, 0);
      lcd.print("      ");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      lcd.setCursor(10, 0);
      lcd.print("WINNER");
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
    }
  }
}
//black race timer
void timerBlack() {
  secBlack = setSecBlack - cSecBlack;
  printTimerBlack();
  if ((secBlack == -1) && (minBlack >= 0)) {
    setSecBlack = cSecBlack + 59;
    minBlack--;
  }
  if ((secBlack == -1) && (minBlack == -1) && (horBlack > 0)) {
    setSecBlack = cSecBlack + 59;
    minBlack = 59;
    horBlack--;
  }
  //end game
  if ((secBlack == 0) && (minBlack == 0) && (horBlack == 0)) {
    tone(pinBuzzer, 500, 1000);
    while (1) {
      lcd.setCursor(0, 0);
      lcd.print("      ");
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1000);
      lcd.setCursor(0, 0);
      lcd.print("WINNER");
      digitalWrite(LED_BUILTIN, LOW);
      delay(1000);
    }
  }
}
void setup() {
  //configuration pins
  analogWrite(6,contrast);
  pinMode(buttonWhite, INPUT); //white button
  pinMode(buttonBlack, INPUT); //black button
  pinMode(analogPin, INPUT);   //used on keyboard
  pinMode(pinBuzzer, OUTPUT);  //bell
  pinMode(LED_BUILTIN, OUTPUT);//pin 13 led
  //initialize LCD
  lcd.begin(16, 2);
  lcd.clear();
  //starting messages
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("T.VENKATARAMANA");
  lcd.setCursor(1, 1);
  lcd.print("23A55A0414");
  delay(5000);
  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("ELECTRONICS AND");
  lcd.setCursor(3, 1);
  lcd.print("COMMUNICATION");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("CHESS TIMER");
  lcd.setCursor(1, 1);
  lcd.print("STARTING");
  delay(2000);
  lcd.clear();
  digitalWrite(LED_BUILTIN, HIGH);
  while (exitMenu) {    //start configuration menu
    menuSetUp();        //keyboard configured
    printMenu();        //print menu values
    printTimerWhite(); //print setup timer below left
  }
  //set values for white and black to start timers
  horBlack = horWhite;
  minBlack = minWhite;
  secBlack = secWhite;
  setSecBlack = secWhite;
  setSecWhite = secWhite;
  printLabels(); //printing labels
}
//Arduino loop
void loop() {
  adc_key_in = analogRead(analogPin); //read keyboard
  int black,white;
  if (digitalRead(buttonBlack)) black=1;
  if (digitalRead(buttonWhite)) white=1;
 //press the black button and run the white timer
  if (black==1){
    tone(pinBuzzer, 150, 100);
    cTemp = SEC - cSecWhite; //difference between SEC and previous seconds count
    while (digitalRead(buttonWhite) == 0 && adc_key_in > 1000) { //if white button or any keyboard is pressed loop output
      cSecWhite = SEC - cTemp; //count seconds for white stopwatch
      timerWhite(); //run white stopwatch
      adc_key_in = analogRead(analogPin); //read keyboard
    }
    if (adc_key_in < 1000) { //if any keyboard is pressed go to pause
      pauseGameWhite();
    }
  }
  //press the white button to run the black timer
  if (white==1){
    tone(pinBuzzer, 150, 100);
    cTemp = SEC - cSecBlack;
    while (digitalRead(buttonBlack) == 0 && adc_key_in > 1000) {
      cSecBlack = SEC - cTemp;
      timerBlack();
      adc_key_in = analogRead(analogPin);
    }
    if (adc_key_in < 1000) {
      pauseGameBlack();
    }
  }
}
