// inspired by https://youtu.be/wY1SRehZ9hM

#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

// note buttons
const int firstPin = 2;
const int numNotePins = 8;  // 8 face buttons of arcade stick
const int numTotalOctaves = 7;
const int numActiveOctaves = 3;  // middle octave when joystick is untouched, increase and decrease octave when UP or DOWN are held
int myButtons[numNotePins];
int myButtonVals[numNotePins];
int myButtonStates[numNotePins];

// UP and DOWN buttons for octave control
const int upPin = 12;
const int downPin = 13;
int upVal = 0;
int downVal = 0;
int upState = 0;
int downState = 0;

// START and BACK buttons for octave range control
const int startPin = 11;
const int backPin = 10;
int startVal = 0;
int backVal = 0;
int startState = 0;
int backState = 0;

int octave = 0;
int octaveRangeCursor = 2;
const int minOctaveRangeCursor = 0;
const int maxOctaveRangeCursor = 4;

const int whiteKeys[numTotalOctaves][numNotePins] = {
  { 12, 14, 16, 17, 19, 21, 23, 24 },
  { 24, 26, 28, 29, 31, 33, 35, 36 },
  { 36, 38, 40, 41, 43, 45, 47, 48 },  // default octave
  { 48, 50, 52, 53, 55, 57, 59, 60 },  // default octave
  { 60, 62, 64, 65, 67, 69, 71, 72 },  // default octave
  { 72, 74, 76, 77, 79, 81, 83, 84 },
  { 84, 86, 88, 89, 91, 93, 95, 96 },
};

void setup() {
  MIDI.begin();
  // comment the line above and uncomment the line below if you need to console log with Serial.println();
  // Serial.begin(9600);

  pinMode(upPin, INPUT_PULLUP);
  pinMode(downPin, INPUT_PULLUP);
  pinMode(startPin, INPUT_PULLUP);
  pinMode(backPin, INPUT_PULLUP);

  for (int i = 0; i < numNotePins; i++) {
    myButtons[i] = firstPin + i;
    myButtonVals[i] = 0;
    myButtonStates[i] = 0;phot
    pinMode(myButtons[i], INPUT_PULLUP);
  }
}

void loop() {
  upVal = digitalRead(upPin);
  downVal = digitalRead(downPin);
  startVal = digitalRead(startPin);
  backVal = digitalRead(backPin);

  // increase octave range cursor when START pressed
  if ((startVal == LOW) && (startState == 0)) {
    startState = 1;
  } else if ((startVal == HIGH) && (startState == 1)) {
    startState = 0;
    octaveRangeCursor++;
    if (octaveRangeCursor > maxOctaveRangeCursor) {
      octaveRangeCursor = maxOctaveRangeCursor;
    }
  }
  // decrease octave range cursor when BACK pressed
  if ((backVal == LOW) && (backState == 0)) {
    backState = 1;
  } else if ((backVal == HIGH) && (backState == 1)) {
    backState = 0;
    octaveRangeCursor--;
    if (octaveRangeCursor < minOctaveRangeCursor) {
      octaveRangeCursor = minOctaveRangeCursor;
    }
  }

  // increase octave when UP held
  if ((upVal == LOW) && (upState == 0)) {
    octave = 1;
    upState = 1;
  } else if ((upVal == HIGH) && (upState == 1)) {
    octave = 0;
    upState = 0;
  }
  // decrease octave when DOWN held
  if ((downVal == LOW) && (downState == 0)) {
    octave = -1;
    downState = 1;
  } else if ((downVal == HIGH) && (downState == 1)) {
    octave = 0;
    downState = 0;
  }

  // play notes when note buttons are held
  for (int i = 0; i < numNotePins; i++) {
    myButtonVals[i] = digitalRead(myButtons[i]);

    if ((myButtonVals[i] == LOW) && (myButtonStates[i] == 0)) {
      MIDI.sendNoteOn(whiteKeys[octaveRangeCursor + (octave + 1)][i], 127, 1);
      myButtonStates[i] = 1;
    } else if ((myButtonVals[i] == HIGH) && (myButtonStates[i] == 1)) {
      MIDI.sendNoteOff(whiteKeys[octaveRangeCursor + (octave + 1)][i], 0, 1);
      myButtonStates[i] = 0;
    }
  }
}
