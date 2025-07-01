// 7x7 with Serial Communication
// === Pin Definitions ===
const int ledPins[7] = {2, 3, 4, 5, 6, 7, 8};
const int irSensorPin = 12;

// === Display Timing ===
const int columnDelay = 2; // milliseconds per column
const int LETTER_WIDTH = 7;
const int SPACE = 1;

// === Row-wise Font: Space + A–Z ===
const byte font[][7] = {
  // Space
  {B0000000, B0000000, B0000000, B0000000, B0000000, B0000000, B0000000},

  // A
  {B0011100, B0100010, B1000001, B1111111, B1000001, B1000001, B1000001},
  // B
  {B1111110, B1000001, B1000001, B1111110, B1000001, B1000001, B1111110},
  // C
  {B0011110, B0100001, B1000000, B1000000, B1000000, B0100001, B0011110},
  // D
  {B1111110, B1000001, B1000001, B1000001, B1000001, B1000001, B1111110},
  // E
  {B1111111, B1000000, B1000000, B1111100, B1000000, B1000000, B1111111},
  // F
  {B1111111, B1000000, B1000000, B1111100, B1000000, B1000000, B1000000},
  // G
  {B0011110, B0100001, B1000000, B1001111, B1000001, B0100001, B0011110},
  // H
  {B1000001, B1000001, B1000001, B1111111, B1000001, B1000001, B1000001},
  // I
  {B0111110, B0001000, B0001000, B0001000, B0001000, B0001000, B0111110},
  // J
  {B0001111, B0000010, B0000010, B0000010, B1000010, B1000010, B0111100},
  // K
  {B1000001, B1000010, B1000100, B1111000, B1000100, B1000010, B1000001},
  // L
  {B1000000, B1000000, B1000000, B1000000, B1000000, B1000000, B1111111},
  // M
  {B1000001, B1100011, B1010101, B1001001, B1000001, B1000001, B1000001},
  // N
  {B1000001, B1100001, B1010001, B1001001, B1000101, B1000011, B1000001},
  // O
  {B0011110, B0100001, B1000001, B1000001, B1000001, B0100001, B0011110},
  // P
  {B1111110, B1000001, B1000001, B1111110, B1000000, B1000000, B1000000},
  // Q
  {B0011110, B0100001, B1000001, B1000001, B1001001, B0100010, B0011101},
  // R
  {B1111110, B1000001, B1000001, B1111110, B1001000, B1000100, B1000010},
  // S
  {B0111110, B1000001, B1000000, B0111110, B0000001, B1000001, B0111110},
  // T
  {B1111111, B0001000, B0001000, B0001000, B0001000, B0001000, B0001000},
  // U
  {B1000001, B1000001, B1000001, B1000001, B1000001, B1000001, B0111110},
  // V
  {B1000001, B1000001, B1000001, B1000001, B0100010, B0010100, B0001000},
  // W
  {B1000001, B1000001, B1000001, B1001001, B1001001, B1001001, B0110110},
  // X
  {B1000001, B0100010, B0010100, B0001000, B0010100, B0100010, B1000001},
  // Y
  {B1000001, B0100010, B0010100, B0001000, B0001000, B0001000, B0001000},
  // Z
  {B1111111, B0000001, B0000010, B0000100, B0001000, B0010000, B1111111}
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
  Serial.println("POV display ready (row-wise font).");
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

    for (int col = 0; col < 7; col++) {
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
  for (int col = 0; col < 7; col++) {
    output[col] = 0;
    for (int row = 0; row < 7; row++) {
      bool bit = bitRead(input[row], 6 - col);   // read bit from row-wise input
      bitWrite(output[col], 6 - row, bit);       // write to transposed column
    }
  }
}
