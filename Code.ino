//5x7 Bitmapping
// === Pin Definitions ===
const int ledPins[7] = {2, 3, 4, 5, 6, 7, 8};
const int irSensorPin = 12;

// === Display Timing ===
const float columnDelay =  1; // milliseconds per column
const int LETTER_WIDTH = 5;
const int SPACE = 4;

// === Row-wise Font: Space + A–Z (5x7 characters) ===
const byte font[][7] = {
  // SPACE
  {B00000, B00000, B00000, B00000, B00000, B00000, B00000},

  // A
  {B01110, B10001, B10001, B11111, B10001, B10001, B10001},
  // B
  {B11110, B10001, B10001, B11110, B10001, B10001, B11110},
  // C
  {B01111, B10000, B10000, B10000, B10000, B10000, B01111},
  // D
  {B11110, B10001, B10001, B10001, B10001, B10001, B11110},
  // E
  {B11111, B10000, B10000, B11110, B10000, B10000, B11111},
  // F
  {B11111, B10000, B10000, B11110, B10000, B10000, B10000},
  // G
  {B01111, B10000, B10000, B10111, B10001, B10001, B01111},
  // H
  {B10001, B10001, B10001, B11111, B10001, B10001, B10001},
  // I
  {B11111, B00100, B00100, B00100, B00100, B00100, B11111},
  // J
  {B11111, B00100, B00100, B00100, B10100, B10100, B11100},
  // K
  {B10001, B10010, B10100, B11000, B10100, B10010, B10001},
  // L
  {B10000, B10000, B10000, B10000, B10000, B10000, B11111},
  // M
  {B10001, B11011, B10101, B10001, B10001, B10001, B10001},
  // N
  {B10001, B10001, B11001, B10101, B10011, B10001, B10001},
  // O
  {B01110, B10001, B10001, B10001, B10001, B10001, B01110},
  // P
  {B11110, B10001, B10001, B11110, B10000, B10000, B10000},
  // Q
  {B01110, B10001, B10001, B10001, B10101, B10010, B01101},
  // R
  {B11110, B10001, B10001, B11110, B10100, B10010, B10001},
  // S
  {B01111, B10000, B10000, B01110, B00001, B00001, B11110},
  // T
  {B11111, B00100, B00100, B00100, B00100, B00100, B00100},
  // U
  {B10001, B10001, B10001, B10001, B10001, B10001, B01110},
  // V
  {B10001, B10001, B10001, B10001, B10001, B01010, B00100},
  // W
  {B10001, B10001, B10001, B10001, B10101, B11011, B10001},
  // X
  {B10001, B10001, B01010, B00100, B01010, B10001, B10001},
  // Y
  {B10001, B01010, B00100, B00100, B00100, B00100, B00100},
  // Z
  {B11111, B00001, B00010, B00100, B01000, B10000, B11111}
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

  pinMode(irSensorPin, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("POV display ready (5x7 font).");
}

void loop() {
  static bool waitingForIR = true;

  if (waitingForIR && digitalRead(irSensorPin) == LOW) {
    displayMessage("KIRAN");
    waitingForIR = false;
  }

  if (!waitingForIR && digitalRead(irSensorPin) == HIGH) {
    waitingForIR = true;
  }
}

void displayMessage(const char* msg) {
  for (int i = 0; msg[i] != '\0'; i++) {
    int index = charToIndex(msg[i]);
    byte transposed[LETTER_WIDTH];
    transposeChar(font[index], transposed);

    for (int col = 0; col < LETTER_WIDTH; col++) {
      displayColumn(transposed[col]);
      delay(columnDelay);
    }

    displayColumn(0b0000000); // spacing
    delay(columnDelay * SPACE);
  }
}

void displayColumn(byte rowData) {
  for (int i = 0; i < 7; i++) {
    digitalWrite(ledPins[i], bitRead(rowData, 6 - i)); // top to bottom
  }
}

void transposeChar(const byte input[7], byte output[LETTER_WIDTH]) {
  for (int col = 0; col < LETTER_WIDTH; col++) {
    output[col] = 0;
    for (int row = 0; row < 7; row++) {
      bool bit = bitRead(input[row], 4 - col); // Now 5 bits per row
      bitWrite(output[col], 6 - row, bit);
    }
  }
}
