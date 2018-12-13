//Test code is based on https://www.instructables.com/id/Use-a-DIY-Pressure-Plate-Switch-to-Automate-Your-H/
//Except I am doing a pull down
//Here is an example of code that you could use with this switch.

//numbers are according to the GPIO layout: https://wiki.wemos.cc/products:d1:d1_mini
const int buttonPin=0; //D3
const int ledPin =2; //D4 -- this is just the light on the arduino.

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);     
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);    
}

void loop(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is LOW:
  if (buttonState == LOW) {    
    // turn LED on:
    digitalWrite(ledPin,LOW);

  }
  else {
    // turn LED off:
    
    digitalWrite(ledPin,HIGH);
  }
}