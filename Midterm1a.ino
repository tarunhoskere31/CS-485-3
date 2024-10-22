
// pin mappings for the rows and columns of the matrix
// Define the row pin numbers:
const int rowPins[8] = {2, 7, 19, 5, 13, 18, 12, 16};

// Define the column pin numbers:
const int colPins[8] = {6, 11, 10, 3, 17, 4, 8, 9};

// Pattern 1
byte pattern1[8] = {
  0x7E,  // Row 1: 01111110
  0xE7,  // Row 2: 10000001
  0xC3,  // Row 3: 10000001
  0xC3,  // Row 4: 10000001
  0xC3,  // Row 5: 10000001
  0xC3,  // Row 6: 10000001
  0xE7,  // Row 7: 10000001
  0x7E   // Row 8: 01111110
};

// Pattern 2
byte pattern2[8] = {
  0x63,  // Row 1: 01111110
  0x66,  // Row 2: 10000001
  0x6C,  // Row 3: 10000001
  0x78,  // Row 4: 10000001
  0x6C,  // Row 5: 10000001
  0x66,  // Row 6: 10000001
  0x63,  // Row 7: 10000001
  0x61   // Row 8: 01111110
};

// Bit patters for the letters in Tarun
byte name[] = {
  // T
  0x7E, //   01111110
  0x18, //   00011000
  0x18, //   00011000
  0x18, //   00011000 
  0x18, //   00011000  
  0x18, //   00011000
  0x18, //   00011000
  0x00, //   00000000

  // A
  0x18, //   00011000
  0x24, //   00100100
  0x42, //   01000010
  0x7E, //   01111110 
  0x42, //   01000010  
  0x42, //   01000010
  0x42, //   01000010
  0x00, //   00000000

  // R
  0x7C, //   01111100
  0x42, //   01000010
  0x42, //   01000010
  0x7C, //   01111100 
  0x50, //   01010000  
  0x48, //   01001000
  0x46, //   01000110
  0x00, //   00000000

  // U
  0x81, //   01000010
  0x81, //   01000010
  0x81, //   01000010
  0x81, //   01000010 
  0x81, //   01000010  
  0x81, //   01000010
  0x7E, //   00111100
  0x00, //   00000000

  // N
  0x42, //   01000001
  0x62, //   01100001
  0x52, //   01010001
  0x4A, //   01001001
  0x46, //   01000101  
  0x42, //   01000011
  0x42, //   01000001
  0x00, //   00000000
};

byte invertedPattern1[8];
byte invertedPattern2[8];

// To set columns based on rows pattern
void setColumns(byte pattern) {
  for (int col = 0; col < 8; col++) {
    // set column low if corresponding is 1
    digitalWrite(colPins[col], (pattern >> col) & 0x01 ? LOW : HIGH);
  }
} 


// Function to draw on LED Matrix
void displayPattern(const byte buffer[]) {
  for (byte i = 0; i < 8; i++) {
    // set the columns for current row
    setColumns(buffer[i]);

    // activate current row
    digitalWrite(rowPins[i], HIGH);
    delay(1); // short delay to display row
    digitalWrite(rowPins[i], LOW); // Turn off row

  }
}

// Function to invert the pattern
void invertPattern(const byte original[], byte inverted[]) {
  for (byte i = 0; i < 8; i++) {
    inverted[i] = ~original[i];
  }
}

// Function to scroll mesage across 
void scroll(byte message[], int messageLength) {
  for (int i = 0; i < messageLength + 8; i++) {   // loop over length of message
    for (int col = 0; col < 8; col++) {   // display 8 columns at a time
      for (int row = 0; row < 8; row++) {
        // check if within bounds of message array
        if (i + col < messageLength) {
          digitalWrite(rowPins[row], (message[i + col] & (1 << row)) ? HIGH : LOW); // display one column
        } else {
          digitalWrite(rowPins[row], LOW);   // clear row if out
        }
      }
      digitalWrite(colPins[col], LOW);
      delay(5);   // small delay to avoid flicker
      digitalWrite(colPins[col], HIGH);
    }
  }
}

void setup() {
  // Initialize all row and columns pins as outputs
  for (int i = 0; i < 8; i++) {
    pinMode(rowPins[i], OUTPUT);
    pinMode(colPins[i], OUTPUT);
  }
// set up inverted arrays
invertPattern(pattern1, invertedPattern1);
invertPattern(pattern2, invertedPattern2);
}

void loop() {
  // invert the patterns first
  invertPattern(pattern1, invertedPattern1);
  invertPattern(pattern2, invertedPattern2);
  // Display pattern1 for 1 sec
  // refresh every 1 ms, loop 125 times for 1 sec
  for (int i = 0; i < 1000 / 8; i++) {
    displayPattern(pattern1);
  }

  // Display pattern2 for 1 sec
   for (int i = 0; i < 1000 / 8; i++) {
    displayPattern(pattern2);
  }

  // Display invertedPattern1 for 1 sec
   for (int i = 0; i < 1000 / 8; i++) {
    displayPattern(invertedPattern1);
  }

  // Display invertedPattern2 for 1 sec
   for (int i = 0; i < 1000 / 8; i++) {
    displayPattern(invertedPattern2);
  }
  
  // Scroll patterns
  scroll(pattern1, sizeof(pattern1));
  delay(250);
  scroll(pattern2, sizeof(pattern2));

  // scroll name
  scroll(name, sizeof(name));
  delay(1000);
  
}
