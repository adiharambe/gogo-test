//* is to validate password   
//# is to reset password attempt

/////////////////////////////////////////////////////////////////

#include <Password.h> //http://www.arduino.cc/playground/uploads/Code/Password.zip
#include <Keypad.h> //http://www.arduino.cc/playground/uploads/Code/Keypad.zip
#include <Servo.h>
// Create a Password Array to store multiple Passwords

bool justLocked = false;
const byte MAXPASSWD = 3;
bool check = false;
int count1 = 0;
Password passwordArray[MAXPASSWD] = { Password( "1515"), Password( "5151"), Password("4747")};
int passwordCheck[MAXPASSWD] = {0, 0, 0};
int index = 0;
int checkTime = 0;
int reuseStopTime = 0;
Servo myservo;

unsigned long int previousInterval = 0;

const byte ROWS = 4; // Four rows
const byte COLS = 3; //  columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = { 9,8,7,6 };// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte colPins[COLS] = { 5,4,0 };// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
int led = 13;
int relay = 11;



// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){

  Serial.begin(9600);
  pinMode(led, OUTPUT);     
  pinMode(relay, OUTPUT);     
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad
  myservo.attach(10);
  myservo.write(0);
}

void checkAllPasswords(){
  int i;
  int count = 0;
  for (i = 0; i < MAXPASSWD; i++) {
    if (passwordArray[i].evaluate()){
        if(i == MAXPASSWD - 1){
          myservo.write(0);
          Serial.print("Thank you for using our service");
          count = 1;
          count1 = 0;
          justLocked = true;
        }
        if (passwordCheck[i] == 0 && i != MAXPASSWD - 1){
          myservo.write(90);
          Serial.print("Right!");
          passwordCheck[i] = 1;
          index = i;
          count = 1;
          count1 = 1;
          justLocked = false;
        }
    }
      resetAllPasswords();
      break;
  }
  
  if (count != 1){
      Serial.println("Wrong");
      resetAllPasswords();
    }
}

void resetAllPasswords() {
  int i;
  for (i = 0; i < MAXPASSWD; i++) {
     passwordArray[i].reset(); 
  }
}

void appendAllPasswords(KeypadEvent eKey) {
  int i;
  for (i = 0; i < MAXPASSWD; i++) {
     passwordArray[i].append(eKey); 
  }
}

void loop(){
  
  keypad.getKey();
  if(myservo.read() == 90){
    int currentTime = millis() % 30000;
    if (count1 == 1){
      if (currentTime < 15000)
        checkTime = currentTime + 15000;
      else
        checkTime = currentTime - 15000;
      count1 = 0;
    }
    if (currentTime == checkTime){
      myservo.write(0);
      Serial.print(currentTime);
      Serial.print("Thank you for using our service");
      justLocked = true;
    }
  }
  if(myservo.read() == 0 && justLocked){
    int lockedTime = millis() % 60000;
    if (!check){
      if (lockedTime < 30000)
        reuseStopTime = lockedTime + 30000;
      else
        reuseStopTime = lockedTime - 30000;
      check = true;
    }
    if (lockedTime == reuseStopTime){
      justLocked = false;
      passwordCheck[index] = 0;
      check = false;
    }
  }
}



//take care of some special events
void keypadEvent(KeypadEvent eKey){
  switch (keypad.getState()){
    case PRESSED:
  
  Serial.print("Enter:");
  Serial.println(eKey);
  delay(10);
  
  Serial.write(254);
  
  switch (eKey){
    case '*': checkAllPasswords(); delay(1); break;
    
    case '#': resetAllPasswords(); delay(1); break;
    
    default: appendAllPasswords(eKey); delay(1);
  }
}
}
    //case '*': checkAllPasswords(); break;  // check guessed pswd against passwordArray
    //case '#': resetAllPasswords(); break;  // clean all buffers on passwordArray
    //default: appendAllPasswords(eKey);     // update all buffers on passwordArray


