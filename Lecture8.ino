//here, i've created arrays in order to simplify the variable process. This way, all of the LEDs and buttons can be in one variable instead of four
int buttonPins [4] = { 11, 25, 29, 12 };
int buttonPin2 = 33;
int buttonPin3 = 34;
int ledPins [4] = { 23, 22, 21, 20 };
int ledPins2 [3] = { 35, 36, 37 };
int tempo = 0;
int currentSequence = 0;
int currentStep = 0;
int sequences = 3;
int leds = 4;
unsigned long lastStepTime = 0;
boolean lastButtonState [4] = { LOW, HIGH, LOW, HIGH };
boolean buttonState [4] = { LOW, HIGH, LOW, HIGH };
boolean lastButtonState2 = LOW;
boolean buttonState2 = LOW;
boolean lastButtonState3 = LOW;
boolean buttonState3 = LOW;
boolean stepState [3][4] = {
  {LOW, HIGH, LOW, HIGH },
  {HIGH, HIGH, LOW, LOW },
  {HIGH, LOW, HIGH, LOW }
};

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  //here, I use a 'for' loop in order to simplify the setup and get started with using the arrays ive created above
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins [i], OUTPUT);
  }

  for (int i = 0; i < 4; i++) {
    pinMode(buttonPins [i], INPUT);
  }

  for (int i = 0; i < 3; i++) {
    pinMode(ledPins2 [i], OUTPUT);
  }
}

void loop() {
  //here, i call three functions, which will be defined below
  checkButtons();
  sequence();
  setLeds();
}

//this is the function that is used to make the buttons work
void checkButtons() {
  lastButtonState2 = buttonState2;
  lastButtonState3 = buttonState3;
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  digitalWrite(ledPins2[currentSequence], LOW);
  if (buttonState2 == HIGH && lastButtonState2 == LOW) {
    currentSequence = currentSequence - 1;
  }
  if (currentSequence < 0) {
    currentSequence = 2;
  }
  if (buttonState3 == HIGH && lastButtonState3 == LOW) {
    currentSequence++;
  }
  if (currentSequence == sequences) {
    currentSequence = 0;
  }
  digitalWrite(ledPins2[currentSequence], HIGH);

  if (currentSequence == 0) {
  }
  
  for (int i = 0; i < leds; i++) {
    digitalWrite(ledPins[i], stepState[currentSequence][i]);
  }

  if (currentSequence == 1) {
  }
  
  for (int i = 0; i < leds; i++) {
    digitalWrite(ledPins[i], stepState[currentSequence][i]);
  }

  if (currentSequence == 2) {
  }
  
  for (int i = 0; i < leds; i++) {
    digitalWrite(ledPins[i], stepState[currentSequence][i]);
  }


  //this for loop will make it so that the function will check the four buttons without me having to write out the same code three more times
  for (int i = 0; i < 4; i++) {
    lastButtonState [i] = buttonState [i];
    buttonState [i] = digitalRead(buttonPins [i]);
    if (buttonState [i] == HIGH && lastButtonState [i] == LOW) {
      if (stepState [currentSequence][i] == false) {
        stepState [currentSequence][i] = true;
      } else if (stepState [currentSequence][i] == true) {
        stepState [currentSequence][i] = false;
      }
    }
  }
}


//this is the function that will control the LEDs
void setLeds() {
  //this is another for loop that will, again, increment the steps without me having to repeat the code
  for (int i = 0; i < 4; i++) {
    //this sets the LEDs with their steps, so that LED 1 is with step 1, LED 2 is with step 2 and so on
    if (currentStep == i) {
      //this just makes the brightness as high as possible so that the user can see that the sequencer is on no matter whether or not each step is on
      analogWrite(ledPins[i], 255);
    } else if (stepState[currentSequence][i] == true) {
      //this sets the ON LEDs at a much lower brightness than above so that it's obvious when they're ON, but still showing where the sequencer is stepping
      analogWrite(ledPins[i], 50);
    } else {
      //and this just means that, if the button isn't pressed, than the LED isn't on
      analogWrite(ledPins[i], 0);
    }
  }
}


//here, i define my sequencer function
void sequence() {
  //this line of code programs the potentiometer to control the tempo integer, which ive defined above
  tempo = analogRead(A3);
  if (millis() > lastStepTime + tempo) {   //if its time to go to the next step...
    //digitalWrite(ledPins[currentSequence], LOW);  //turn off the current led

    currentStep = currentStep + 1;         //increment to the next step
    if (currentStep > 3) {
      currentStep = 0;
    }

    if (stepState[0][currentStep] == HIGH) {
      usbMIDI.sendNoteOff(72, 0, 1);
      usbMIDI.sendNoteOn(72, 127, 1);
    }

    if (stepState[1][currentStep] == HIGH) {
      usbMIDI.sendNoteOff(76, 0, 1);
      usbMIDI.sendNoteOn(76, 127, 1);
    }

    if (stepState[2][currentStep] == HIGH) {
      usbMIDI.sendNoteOff(77, 0, 1);
      usbMIDI.sendNoteOn(77, 127, 1);
    }

    //digitalWrite(ledPins[currentSequence], HIGH); //turn on the new led
    lastStepTime = millis();               //set lastStepTime to the current time
  }
}
