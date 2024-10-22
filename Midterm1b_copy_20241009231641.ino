#include <Arduino.h>
// pin mappings for the rows and columns of the matrix
// Define the row pin numbers:
const int rowPins[8] = {2, 7, 19, 5, 13, 18, 12, 16};

// Define the column pin numbers:
const int colPins[8] = {6, 11, 10, 3, 17, 4, 8, 9};

// Pin mappings for the button
const int button = A6;

// Matrix to store LED states
bool matrix[8][8];
unsigned int startTime;
unsigned int reactionTime;
bool reactionActive = false;
// Count number of tests
int testCount = 0;
// Calculate number of tests
unsigned int totalReactionTime = 0;

// helper functions

// to start the game
void startGame() {
  // choose random LED to light up
  int randomRow = random(0,8);
  int randomCol = random(0,8);

  // light up random LED
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      // clear matrix
      matrix[i][j] = false;
    }
  }
  // set the chosen led
  matrix[randomRow][randomCol] = true;

  // display matrix;
  displayMatrix();

  // Start timer
  startTime = millis();
  reactionActive = true;
}

// function to display the matrix
void displayMatrix() {
  // Loop through the rows
  for (int i = 0; i < 8; i++) {
    // set the row state
    digitalWrite(rowPins[i], LOW);   // Activate the row
    // Loop throughthe columns
    for (int j = 0; j < 8; j++) {
      //set the column state
      // light up the LED if true
      digitalWrite(colPins[j], matrix[i][j] ? LOW : HIGH);
    }
    // Deactivre the row
    digitalWrite(rowPins[i], HIGH);
  }
}

// function to determine the individual reaction time
void displayReactionTime(unsigned long time) {
  // clear matrix
  clearMatrix();

  // calculate the score based on the reaction time
  int score = calculateScore(time);

  // Dim the single LED in row corresponding to the score
  matrix[score - 1][0] = true;
  displayMatrix();
}

int calculateScore(unsigned long time) {
  if (time < 15000) return 1;   // Excellentreaction time
  if (time < 17500) return 2;   // Goodreaction time
  if (time < 22500) return 3;   // AVG reaction time
  if (time < 30000) return 4;   // Below AVG. reaction time
  return 5;
}

// displaying averageReactionTime
void displayAverageReactionTime() {
  unsigned long averageTime = totalReactionTime / testCount;

  // clear matrix
  clearMatrix();

  // calculate the score based on the average reaction time
  int score = calculateScore(averageTime);

  // Light up a single LED for the average score
  matrix[score - 1][0] = true;
  displayMatrix();
}

// function to clear matrix
void clearMatrix() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      matrix[i][j] = false;
    }
  }
}


void setup() {
  // Initialize row pins as outputs
  for (int i = 0; i < 8; i++) {
    pinMode(rowPins[i], OUTPUT);
    // Turn off all rows initally
    digitalWrite(rowPins[i], HIGH);
  }

  // Initialize col pins as outputs
  for (int i = 0; i < 8; i++) {
    pinMode(colPins[i], OUTPUT);
    // Turn off all cols initally
    digitalWrite(colPins[i], HIGH);
  }

  // Initialize button pin
  pinMode(button, INPUT_PULLUP);

  // Start game
  delay(1000);   // Wait two seconds before starting
  startGame();
}


void loop() {
  if (reactionActive) {
    //check if the button is pressed
    if (digitalRead(button) == LOW) {
      // calculate reaction time
      reactionTime = millis() - startTime;
      totalReactionTime += reactionTime;
      testCount++;

      // check if 10 tests have been completed
      if (testCount < 10) {
        displayReactionTime(reactionTime);
        // Deactivate reaction
        reactionActive = false;
        // wait before restarting game
        delay(2000);
        startGame();
      } else {
        // calculate and siplay average reaction time
        displayAverageReactionTime();
      }
    }
  }
}












