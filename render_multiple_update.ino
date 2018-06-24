//* is to validate password   
//# is to reset password attempt

/////////////////////////////////////////////////////////////////

#include <Password.h> //http://www.arduino.cc/playground/uploads/Code/Password.zip
#include <Keypad.h> //http://www.arduino.cc/playground/uploads/Code/Keypad.zip
#include <Servo.h>
// Create a Password Array to store multiple Passwords

const byte MAXPASSWD = 3;
Password passwordArray[MAXPASSWD] = { Password( "1515"), Password( "5151"), Password("4747")};

Servo myservo;

unsigned long int previousInterval = 0;
int count1 = 0;

const byte ROWS = 4; // Four rows
const byte COLS = 3; //  columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[ROWS] = { 9,8,7,6 };
byte colPins[COLS] = { 5,4,0 };
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
      }//cycle lock code and message
      else{
        myservo.write(90);
        Serial.print("Right!");
        count = 1;
      }
      
      resetAllPasswords();
      break;
    }
    
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
  //auto locking timer initialized
  if(myservo.read() == 90){
    unsigned long currentTime = millis();
    if (count1 != 1){
      previousInterval = currentTime;
      count1 = 1;
    }
    //timeer is set to 15 seconds
    if (currentTime - previousInterval > 15000){
     myservo.write(0);
     Serial.print(currentTime);
     count1 = 0;
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


