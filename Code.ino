// 4X7 Bitmapping
// === Pin Definitions ===
const int ledPins[7] = {2, 3, 4, 5, 6, 7, 8};
const int irSensorPin = 12;

// === Display Timing ===
const int columnDelay = 2; // milliseconds per column
const int LETTER_WIDTH = 4;  // changed from 7 to 4
const int SPACE = 1;

// === Row-wise Font: Space + A–Z (4x7 characters) ===
const byte font[][7] = {
  // SPACE
  {B0000, B0000, B0000, B0000, B0000, B0000, B0000},

  // A
  {B0110, B1001, B1001, B1111, B1001, B1001, B1001},
  // B
  {B1110, B1001, B1001, B1110, B1001, B1001, B1110},
  // C
  {B0111, B1000, B1000, B1000, B1000, B1000, B0111},
  // D
  {B1110, B1001, B1001, B1001, B1001, B1001, B1110},
  // E
  {B1111, B1000, B1000, B1110, B1000, B1000, B1111},
  // F
  {B1111, B1000, B1000, B1110, B1000, B1000, B1000},
  // G
  {B0111, B1000, B1000, B1011, B1001, B1001, B0111},
  // H
  {B1001, B1001, B1001, B1111, B1001, B1001, B1001},
  // I
  {B0110, B0010, B0010, B0010, B0010, B0010, B0110},
  // J
  {B0001, B0001, B0001, B0001, B1001, B1001, B0110},
  // K
  {B1001, B1010, B1100, B1000, B1100, B1010, B1001},
  // L
  {B1000, B1000, B1000, B1000, B1000, B1000, B1111},
  // M
  {B1001, B1111, B1111, B1001, B1001, B1001, B1001},
  // N
  {B1001, B1001, B1101, B1101, B1011, B1011, B1001},
  // O
  {B0110, B1001, B1001, B1001, B1001, B1001, B0110},
  // P
  {B1110, B1001, B1001, B1110, B1000, B1000, B1000},
  // Q
  {B0110, B1001, B1001, B1001, B1011, B1001, B0111},
  // R
  {B1110, B1001, B1001, B1110, B1010, B1001, B1001},
  // S
  {B0111, B1000, B1000, B0110, B0001, B0001, B1110},
  // T
  {B1111, B0010, B0010, B0010, B0010, B0010, B0010},
  // U
  {B1001, B1001, B1001, B1001, B1001, B1001, B0110},
  // V
  {B1001, B1001, B1001, B1001, B1001, B0110, B0010},
  // W
  {B1001, B1001, B1001, B1001, B1111, B1111, B1001},
  // X
  {B1001, B1001, B0110, B0010, B0110, B1001, B1001},
  // Y
  {B1001, B1001, B1001, B0110, B0010, B0010, B0010},
  // Z
  {B1111, B0001, B0010, B0100, B1000, B1000, B1111}
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
  Serial.println("POV display ready (4x7 font).");
}

void loop() {
  static bool waitingForIR = true;

  if (waitingForIR && digitalRead(irSensorPin) == LOW) {
    displayMessage("SUCCESS");
    waitingForIR = false;
  }

  if (!waitingForIR && digitalRead(irSensorPin) == HIGH) {
    waitingForIR = true;
  }
}

void displayMessage(const char* msg) {
  for (int i = 0; msg[i] != '\0'; i++) {
    int index = charToIndex(msg[i]);
    byte transposed[7];
    transposeChar(font[index], transposed);   // Transpose row-wise to column-wise

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

void transposeChar(const byte input[7], byte output[7]) {
  for (int col = 0; col < LETTER_WIDTH; col++) {
    output[col] = 0;
    for (int row = 0; row < 7; row++) {
      bool bit = bitRead(input[row], 3 - col);   // now only 4 columns per row
      bitWrite(output[col], 6 - row, bit);
    }
  }
}
