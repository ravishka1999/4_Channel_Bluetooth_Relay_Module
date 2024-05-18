// Pin definitions for the relay module
const int relay1 = 2;
const int relay2 = 3;
const int relay3 = 4;
const int relay4 = 5;

// 74HC595 pins
#define DATA_PIN 8    // DS
#define CLOCK_PIN 9   // SH_CP
#define LATCH_PIN 10  // ST_CP

// Bit pattern for numbers on a seven-segment display (common cathode)
#define DIGIT_1 B00000110
#define DIGIT_2 B11001101
#define DIGIT_3 B11011001
#define DIGIT_4 B10010011

#define DIGIT_OFF B00000000 // All segments off

unsigned long lastUpdate = 0;  // Variable to store the last time the display was updated
const unsigned long interval = 1000; // Interval for updating the display (1 second)
byte activeRelays[4] = {0, 0, 0, 0}; // Array to keep track of active relays
int activeRelayCount = 0; // Count of active relays
int currentRelayIndex = 0; // Index of the current relay being displayed

void setup() {
  // Initialize serial communication for the Serial Monitor
  Serial.begin(9600);
  
  // Set pins as outputs
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  // Initialize relay pins as outputs
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  
  // Start with all relays off
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);

  // Display initial digit on the seven-segment display
  displayDigit(DIGIT_OFF);
}

void loop() {
  // Check if there is any data available on the serial port
  if (Serial.available()) {
    // Read the incoming byte
    char command = Serial.read();
    
    // Print the received command to the Serial Monitor
    Serial.print("Received: ");
    Serial.println(command);
    
    // Process the command
    switch (command) {
      case '1': // Turn on Relay 1
        digitalWrite(relay1, HIGH);
        break;
      case '2': // Turn off Relay 1
        digitalWrite(relay1, LOW);
        break;
      case '3': // Turn on Relay 2
        digitalWrite(relay2, HIGH);
        break;
      case '4': // Turn off Relay 2
        digitalWrite(relay2, LOW);
        break;
      case '5': // Turn on Relay 3
        digitalWrite(relay3, HIGH);
        break;
      case '6': // Turn off Relay 3
        digitalWrite(relay3, LOW);
        break;
      case '7': // Turn on Relay 4
        digitalWrite(relay4, HIGH);
        break;
      case '8': // Turn off Relay 4
        digitalWrite(relay4, LOW);
        break;
      default:
        // Unknown command
        break;
    }

    // Update the list of active relays
    updateActiveRelays();
  }

  // Update the display once every second
  if (millis() - lastUpdate >= interval) {
    lastUpdate = millis();
    displayNextActiveRelay();
  }
}

// Function to update the list of active relays
void updateActiveRelays() {
  activeRelayCount = 0;
  if (digitalRead(relay1) == HIGH) activeRelays[activeRelayCount++] = DIGIT_1;
  if (digitalRead(relay2) == HIGH) activeRelays[activeRelayCount++] = DIGIT_2;
  if (digitalRead(relay3) == HIGH) activeRelays[activeRelayCount++] = DIGIT_3;
  if (digitalRead(relay4) == HIGH) activeRelays[activeRelayCount++] = DIGIT_4;
}

// Function to display the next active relay
void displayNextActiveRelay() {
  if (activeRelayCount > 0) {
    displayDigit(activeRelays[currentRelayIndex]);
    currentRelayIndex = (currentRelayIndex + 1) % activeRelayCount;
  } else {
    displayDigit(DIGIT_OFF); // Turn off the display if no relays are on
  }
}

// Function to display a digit on the seven-segment display
void displayDigit(byte digitPattern) {
  // Send the bit pattern to the shift register
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, digitPattern);
  digitalWrite(LATCH_PIN, HIGH);
}
