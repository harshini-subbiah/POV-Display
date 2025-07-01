// 7x7 with IR sensor
// === Pin Definitions ===
const int ledPins[7] = {2, 3, 4, 5, 6, 7, 8};
const int irSensorPin = 12;

// === Display Timing ===
const int columnDelay = 2; // milliseconds per column (adjust to your RPM)
const int LETTER_WIDTH = 7;
const int SPACE = 1;

// === Font: Space + A–Z (uppercase) ===
const byte font[][7] = {
  {B0000000, B0000000, B0000000, B0000000, B0000000, B0000000, B0000000}, // space
  {B0111110, B1000001, B1000001, B1111111, B1000001, B1000001, B1000001}, // A
  {B1111110, B1000001, B1000001, B1111110, B1000001, B1000001, B1111110}, // B
  {B0111110, B1000001, B1000000, B1000000, B1000000, B1000001, B0111110}, // C
  {B1111110, B1000001, B1000001, B1000001, B1000001, B1000001, B1111110}, // D
  {B1111111, B1000000, B1000000, B1111100, B1000000, B1000000, B1111111}, // E
  {B1111111, B1000000, B1000000, B1111100, B1000000, B1000000, B1000000}, // F
  {B0111110, B1000001, B1000000, B1011111, B1000001, B1000001, B0111110}, // G
  {B1000001, B1000001, B1000001, B1111111, B1000001, B1000001, B1000001}, // H
  {B0111110, B0001000, B0001000, B0001000, B0001000, B0001000, B0111110}, // I
  {B0001111, B0000010, B0000010, B0000010, B1000010, B1000010, B0111100}, // J
  {B1000001, B1000010, B1000100, B1111000, B1000100, B1000010, B1000001}, // K
  {B1000000, B1000000, B1000000, B1000000, B1000000, B1000000, B1111111}, // L
  {B1000001, B1100011, B1010101, B1001001, B1000001, B1000001, B1000001}, // M
  {B1000001, B1100001, B1010001, B1001001, B1000101, B1000011, B1000001}, // N
  {B0111110, B1000001, B1000001, B1000001, B1000001, B1000001, B0111110}, // O
  {B1111110, B1000001, B1000001, B1111110, B1000000, B1000000, B1000000}, // P
  {B0111110, B1000001, B1000001, B1000001, B1001001, B1000101, B0111110}, // Q
  {B1111110, B1000001, B1000001, B1111110, B1000100, B1000010, B1000001}, // R
  {B0111110, B1000001, B1000000, B0111110, B0000001, B1000001, B0111110}, // S
  {B1111111, B0001000, B0001000, B0001000, B0001000, B0001000, B0001000}, // T
  {B1000001, B1000001, B1000001, B1000001, B1000001, B1000001, B0111110}, // U
  {B1000001, B1000001, B1000001, B1000001, B0100010, B0010100, B0001000}, // V
  {B1000001, B1000001, B1000001, B1001001, B1010101, B1100011, B1000001}, // W
  {B1000001, B0100010, B0010100, B0001000, B0010100, B0100010, B1000001}, // X
  {B1000001, B0100010, B0010100, B0001000, B0001000, B0001000, B0001000}, // Y
  {B1111111, B0000010, B0000100, B0001000, B0010000, B0100000, B1111111}  // Z
};
// === Convert char to font index ===
int charToIndex(char c) {
  if (c == ' ') return 0;
  if (c >= 'A' && c <= 'Z') return c - 'A' + 1;
  return 0;
}

void setup() {
  for (int i = 0; i < 7; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  pinMode(irSensorPin, INPUT_PULLUP); // IR sensor gives LOW when detected

  Serial.begin(9600);
  Serial.println("Ready to display SUCCESS on each rotation.");
}

void loop() {
  static bool waitingForIR = true;

  // Wait for IR trigger (falling edge)
  if (waitingForIR && digitalRead(irSensorPin) == LOW) {
    // IR detected (home position)
    displayMessage("SUCCESS");
    waitingForIR = false;
  }

  // Reset trigger when IR beam is broken again
  if (!waitingForIR && digitalRead(irSensorPin) == HIGH) {
    waitingForIR = true;
  }
}

void displayMessage(const char* msg) {
  for (int i = 0; msg[i] != '\0'; i++) {
    int index = charToIndex(msg[i]);
    for (int col = 0; col < LETTER_WIDTH; col++) {
      displayColumn(font[index][col]);
      delay(columnDelay);
    }
    // space between characters
    displayColumn(0b0000000);
    delay(columnDelay * SPACE);
  }
}

void displayColumn(byte colData) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(ledPins[i], bitRead(colData, 6 - i));
  }
}
