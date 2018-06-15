
#include <Password.h>
#include <Keypad.h>
#include <Servo.h>

Servo myservo;
Password password = Password( "0000" ); 

const byte ROWS = 4;
const byte COLS = 4; 
char keys[ROWS][COLS] = {
{'1','2','3'},
{'4','5','6'},
{'7','8','9'},
{'*','0','#'}
};
byte rowPins[ROWS] = { 9, 8, 7, 6 };
byte colPins[COLS] = { 5, 4, 3 };


Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial.begin(9600);
  Serial.write(254);
  Serial.write(0x01);
  delay(200); 
  myservo.attach(13); 
  keypad.addEventListener(keypadEvent);
  myservo.write(0);
  }

void loop(){
  
  keypad.getKey();
  
  }
  void keypadEvent(KeypadEvent eKey){
  switch (keypad.getState()){
  case PRESSED:
  
  Serial.print("Enter:");
  Serial.println(eKey);
  delay(10);
  
  Serial.write(254);
  
  switch (eKey){
    case '*': checkPassword(); delay(1); break;
    
    case '#': password.reset(); delay(1); break;
    
     default: password.append(eKey); delay(1);
}
}
}
void checkPassword(){
  
if (password.evaluate()){
    
    Serial.println("Accepted");
    Serial.write(254);delay(10);
    password.reset();
    myservo.write(90);
    delay(10000);
    myservo.write(0);
    
    
    
}else{
    Serial.println("Denied");
    password.reset();
    Serial.write(254);delay(10);
    myservo.write(0);
    delay(500);
    
}
}
/*
 * unsigned long x = milis();
 * if (x > 10 * 1000)
 *  myservo.write(0);
 */

