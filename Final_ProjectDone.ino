// needed libraries for oled
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1327.h>
#include <Wire.h>

// OLED Setup
#define SCREEN_WIDTH 128    // Width of screen
#define SCREEN_HEIGHT 128   // height of screen
Adafruit_SSD1327 display = Adafruit_SSD1327(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

// Joystick Pins
#define JOYSTICK_X A0   // Joystick X-axis
#define JOYSTICK_Y A1   // Joystick Y-axis

// Enter Button Pin
#define ENTER_BUTTON 7  // Button to enter, when on star

// Speaker Pin
#define SPEAKER_Pin 6   // Pin for speaker to produce sound

// Global Variables
bool gameActive = false;    // Indicates if game is active
long reactionTimes[10];     // Array to store reaction times for each round in ms
int numShapes = 5;          // Initial number of moving shapes on the screen
int shapePositions[10][2];  // Array to store positions of up to 10 moving shapes
int posX, posY;             // Joystick-conttrolled cursor position (X, Y)
int targetX, targetY;       // Position of the target star
int cursorSpeed = 5;        // Speed of cursor movement

// Function Prototypes
void startRound(int round);     // Displayes start message for round
void playRound(int round);      // Handles gameplay
void endRound(int round);       // Displayes end message for the round
void initializeShapes();        // Randomly initializes positions of moving shapes
void drawMovingShapes();        // Draws moving shapes on screen
void drawStar(int x, int y, int radius, int brightness);  // Draws a star shape
void displayResults();          // Displayes the final game results


void setup() {
  // Initialize3 Serial Monitor
  Serial.begin(9600);

  // Initialize OLED
  if (!display.begin(SSD1327_I2C_ADDRESS, 0x3D)) {
    Serial.println("SSD1327 allocation failed");
    while (1);  // Halt if fails
  }

  display.clearDisplay();
  display.display();

  // configure button and speaker pins
  pinMode(ENTER_BUTTON, INPUT_PULLUP);  // configure button as input w/ pullup resistor
  pinMode(SPEAKER_Pin, OUTPUT);       // speaker as output
}

void loop() {
  // Main game loop for 10 rounds
  for (int currentRound = 1; currentRound <= 10; currentRound++) {
    startRound(currentRound);   // Display start of round
    playRound(currentRound);    // Play round
    endRound(currentRound);     // Display end of the round
  
    // Increase difficulty after each round
    numShapes = min(numShapes + 2, 10);     // Increase number of shapes (max 10)
    cursorSpeed = min(cursorSpeed + 1, 10); // Increase cursor speed (max 10)
  }

  // Display final results after game ends
  displayResults();
  while (1); // Halt after display results
}

void startRound (int round) {
  // Display start message for the round
  display.clearDisplay();
  display.setCursor (10, 10);
  display.setTextSize(2);
  display.setTextColor(15);
  display.print("Round ");
  display.print(round);
  display.setCursor(10, 40);
  display.println("Start");
  display.display();
  delay(2000);  // wait 2 seconds before round starts
}

void playRound (int round) {
  // Play a single round of the game
  display.clearDisplay();
  unsigned long randomDelay = random(1000, 5000); // Random delay before round starts
  delay(randomDelay);

  // Initialize positions of moving shape
  initializeShapes();

  // Random Star position
  targetX = random(20, SCREEN_WIDTH - 20);
  targetY = random(20, SCREEN_HEIGHT - 20);

  // Random starting position of joystick cursor
  posX = random(0, SCREEN_WIDTH);
  posY = random(0, SCREEN_HEIGHT);

  // Record start time of round
  unsigned long startTime = millis();
  gameActive = true;

  // Gameplay loop
  while (gameActive) {
    int xValue = analogRead(JOYSTICK_X);  // read X-axis joystick value
    int yValue = analogRead(JOYSTICK_Y);  // read Y-axis joystick value
    bool buttonPressed = digitalRead(ENTER_BUTTON) == LOW; // check if button is pressed

    // Adjust cursor position based on joystick input
    if (xValue > 600) posX = constrain(posX + cursorSpeed, 0, SCREEN_WIDTH - 1);
    if (xValue < 400) posX = constrain(posX - cursorSpeed, 0, SCREEN_WIDTH - 1);
    if (yValue > 600) posY = constrain(posY - cursorSpeed, 0, SCREEN_HEIGHT - 1);
    if (yValue < 400) posY = constrain(posY + cursorSpeed, 0, SCREEN_HEIGHT - 1);
  
    // Clear display and redraw elements
    display.clearDisplay();
    drawMovingShapes();   // draw dynamically moving shapes
    drawStar(targetX, targetY, 8, 15);  // draw target star
    display.fillCircle(posX, posY, 2, 15);  // draw cursor
    display.display();

    // Check if cursor overlaps with target and button is pressed
    if (buttonPressed && abs(posX - targetX) <= 8 && abs(posY - targetY) <= 8) {
      gameActive = false;   // End round
      tone(SPEAKER_Pin, 1000, 200); // Play sound on success
    }

    delay(30); // delay for smooth motion
  }
  unsigned long endTime = millis(); // record end time of round
  reactionTimes[round - 1] = endTime - startTime; // calculate reaction time
}

void endRound(int round) {
  // Display end message for the round
  display.clearDisplay();
  display.setCursor (10, 10);
  display.setTextSize(2);
  display.setTextColor(15);
  display.print("Round ");
  display.print(round);
  display.setCursor(10, 40);
  display.println("End");
  display.display();
  delay(2000);  // wait 2 seconds before round start
}

void initializeShapes() {
  // Randomly initialize positions of shapes
  for (int i = 0; i < numShapes; i++) {
    shapePositions[i][0] = random(10, SCREEN_WIDTH - 10);
    shapePositions[i][1] = random(10, SCREEN_HEIGHT - 10);
  }
}


void drawMovingShapes() {
  // Draw and move shapes dynamically on screen
  for (int i = 0; i < numShapes; i++) {
    int &x = shapePositions[i][0];
    int &y = shapePositions[i][1];
    int dx = random (-3, 4);  // random X-axis movement
    int dy = random (-3, 4);  // random Y-axis movement

    // Constrain movement to screen size
    x = constrain(x +dx, 10, SCREEN_WIDTH - 10);
    y = constrain(y +dy, 10, SCREEN_HEIGHT - 10);

    // Randomly draw circles or rectangles for shapes
    if (random(0, 2) == 0) {
      display.fillCircle(x, y, random(5, 10), random(5, 15));
    } else {
      display.fillRect(x, y, random(5, 10), random(5, 15), random (5, 15));
    }
  }
}

void drawStar(int x, int y, int radius, int brightness) {
  // Draw a star shape at a given position
  for (int i = 0; i < 5; i++) {
    float angle1 = i * (2 * PI / 5) - PI / 2; // angles for star points
    float angle2 = (i + 2) * (2 * PI / 5) - PI / 2;

    int x1 = x + cos(angle1) * radius;
    int y1 = y + sin(angle1) * radius;
    int x2 = x + cos(angle2) * radius;
    int y2 = y + sin(angle2) * radius;

    // Draw lines connecting star
    display.drawLine(x1, y1, x2, y2, brightness);
  }
}

void displayResults() {
  // Display the final results of the game
  display.clearDisplay();
  display.setCursor (10, 10);
  display.setTextSize(2);
  display.setTextColor(15);
  display.println("Game Over");
  long totalReactionTime = 0;

  // Calculate total reaction time
  for (int i = 0; i < 10; i++) {
    totalReactionTime += reactionTimes[i];
  }
  long avgReactionTime = totalReactionTime / 10;  // calculate average reaction time

  display.setCursor (10, 30);
  display.setTextSize(2);
  display.setTextColor(15);
  display.print("AVG TIME: ");
  display.print(avgReactionTime);
  display.println("ms");
  display.display();
}