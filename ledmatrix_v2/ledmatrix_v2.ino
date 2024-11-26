#include "DHT.h"

#define ClockRow 6
#define DataRow 5
#define Latch 7
#define ClockCol 9
#define DataCol 8
#define modeBtn 2
#define scriptBtn 3
#define DHTPIN 4

#define NUM_MATRICES 4
#define MATRIX_ROWS 8
#define MATRIX_COLS 8

#define scroll_time 75 //higher is scroll faster
#define shiftPixel 1 //lower for smoother scroll
#define spacingChar 1 //space between characters


const int DHTTYPE = DHT11;
DHT dht (DHTPIN, DHTTYPE);

byte letters[][8] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //  32
0x00, 0x10, 0x38, 0x38, 0x10, 0x10, 0x00, 0x10, //! 33
0x00, 0x6C, 0x6C, 0x48, 0x00, 0x00, 0x00, 0x00, //" 34
0x00, 0x00, 0x28, 0x7C, 0x28, 0x28, 0x7C, 0x28, //# 35
0x00, 0x20, 0x38, 0x40, 0x30, 0x08, 0x70, 0x10, //$ 36
0x00, 0x64, 0x64, 0x08, 0x10, 0x20, 0x4C, 0x4C, //% 37
0x00, 0x20, 0x50, 0x50, 0x20, 0x54, 0x48, 0x34, //& 38
0x00, 0x30, 0x30, 0x20, 0x00, 0x00, 0x00, 0x00, //' 39
0x00, 0x10, 0x20, 0x20, 0x20, 0x20, 0x20, 0x10, //( 40
0x00, 0x20, 0x10, 0x10, 0x10, 0x10, 0x10, 0x20, //) 41
0x00, 0x00, 0x28, 0x38, 0x7C, 0x38, 0x28, 0x00, //* 42
0x00, 0x00, 0x10, 0x10, 0x7C, 0x10, 0x10, 0x00, //+ 43
0x00, 0x00, 0x00, 0xcc, 0x78, 0x30, 0x78, 0xcc, //, 44
0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, //- 45
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, //. 46
0x00, 0x00, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, /// 47
  {0x00, 0x78, 0xcc, 0xdc, 0xec, 0xcc, 0xcc, 0x78},   // 0 48
  {0x00, 0x30, 0x30, 0x70, 0x30, 0x30, 0x30, 0xfc},   // 1 49
  {0x00, 0x78, 0xcc, 0x0c, 0x18, 0x60, 0xc0, 0xfc},   // 2 50
  {0x00, 0x78, 0xcc, 0x0c, 0x38, 0x0c, 0xcc, 0x78},   // 3 51
  {0x00, 0x18, 0x38, 0x58, 0x98, 0xfc, 0x18, 0x18},   // 4 52
  {0x00, 0xfc, 0xc0, 0xf8, 0x0c, 0x0c, 0xcc, 0x78},   // 5 53
  {0x00, 0x78, 0xcc, 0xc0, 0xf8, 0xcc, 0xcc, 0x78},   // 6 54
  {0x00, 0xfc, 0xcc, 0x18, 0x18, 0x30, 0x30, 0x30},   // 7 55
  {0x00, 0x78, 0xcc, 0xcc, 0x78, 0xcc, 0xcc, 0x78},   // 8 56
  {0x00, 0x78, 0xcc, 0xcc, 0x7c, 0x0c, 0xcc, 0x78},   // 9 57
0x00, 0x00, 0x00, 0x30, 0x30, 0x00, 0x30, 0x30, //: 58
0x00, 0x00, 0x00, 0xcc, 0xcc, 0x7c, 0x0c, 0x78, //; 59
0x00, 0x08, 0x10, 0x20, 0x40, 0x20, 0x10, 0x08, //< 60
0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x7C, 0x00, //= 61
0x00, 0x20, 0x10, 0x08, 0x04, 0x08, 0x10, 0x20, //> 62
//        {0x00, 0x10, 0x38, 0x7c, 0xfe, 0x38, 0x38, 0x38},   // ^
//        {0x00, 0x38, 0x38, 0x38, 0xfe, 0x7c, 0x38, 0x10},   // v
//        {0x00, 0x10, 0x18, 0xfc, 0xfe, 0xfc, 0x18, 0x10},   // >
//        {0x00, 0x10, 0x30, 0x7e, 0xfe, 0x7e, 0x30, 0x10},   // <
0x00, 0x38, 0x44, 0x04, 0x18, 0x10, 0x00, 0x10, //? 63
0x00, 0x38, 0x44, 0x5C, 0x54, 0x5C, 0x40, 0x38, //@ 64
  {0x00, 0x78, 0xcc, 0xcc, 0xfc, 0xcc, 0xcc, 0xcc},   // A 65
  {0x00, 0xf8, 0xcc, 0xcc, 0xf8, 0xcc, 0xcc, 0xf8},   // B 66
  {0x00, 0x78, 0xcc, 0xc0, 0xc0, 0xc0, 0xcc, 0x78},   // C 67
  {0x00, 0xf8, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xf8},   // D 68
  {0x00, 0xfc, 0xc0, 0xc0, 0xf8, 0xc0, 0xc0, 0xfc},   // E 69
  {0x00, 0xfc, 0xc0, 0xc0, 0xf8, 0xc0, 0xc0, 0xc0},   // F 70
  {0x00, 0x78, 0xcc, 0xc0, 0xc0, 0xdc, 0xcc, 0x78},   // G 71
  {0x00, 0xcc, 0xcc, 0xcc, 0xfc, 0xcc, 0xcc, 0xcc},   // H 72
  {0x00, 0xf0, 0x60, 0x60, 0x60, 0x60, 0x60, 0xf0},   // I 73
  {0x00, 0x3c, 0x18, 0x18, 0x18, 0xd8, 0xd8, 0x70},   // J 74
  {0x00, 0xcc, 0xd8, 0xf0, 0xe0, 0xf0, 0xd8, 0xcc},   // K 75
  {0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfc},   // L 76
  {0x00, 0xc6, 0xee, 0xfe, 0xd6, 0xc6, 0xc6, 0xc6},   // M 77
  {0x00, 0xc6, 0xe6, 0xf6, 0xde, 0xce, 0xc6, 0xc6},   // N 78
  {0x00, 0x78, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x78},   // O 79
  {0x00, 0xf8, 0xcc, 0xcc, 0xcc, 0xf8, 0xc0, 0xc0},   // P 80
  {0x00, 0x78, 0xcc, 0xcc, 0xcc, 0xdc, 0x78, 0x0c},   // Q 81
  {0x00, 0xf8, 0xcc, 0xcc, 0xf8, 0xf0, 0xd8, 0xcc},   // R 82
  {0x00, 0x78, 0xcc, 0xc0, 0x78, 0x0c, 0xcc, 0x78},   // S 83
  {0x00, 0xfc, 0xb4, 0x30, 0x30, 0x30, 0x30, 0x30},   // T 84
  {0x00, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x7c},   // U 85
  {0x00, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x78, 0x30},   // V 86
  {0x00, 0xc6, 0xc6, 0xc6, 0xd6, 0xfe, 0xee, 0xc6},   // W 87
  {0x00, 0xc6, 0xc6, 0x6c, 0x38, 0x6c, 0xc6, 0xc6},   // X 88
  {0x00, 0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x30, 0x30},   // Y 89
  {0x00, 0xfc, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0xfc},   // Z 90
0x00, 0x38, 0x20, 0x20, 0x20, 0x20, 0x20, 0x38, //[ 91
0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x00, //\ 92
0x00, 0x38, 0x08, 0x08, 0x08, 0x08, 0x08, 0x38, //] 93
0x00, 0x10, 0x28, 0x44, 0x00, 0x00, 0x00, 0x00, //^ 94
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, //_ 95
0x00, 0x30, 0x30, 0x10, 0x00, 0x00, 0x00, 0x00, //` 96
  {0x00, 0x00, 0x00, 0x78, 0x0c, 0x7c, 0xcc, 0x7c},   // a 97
  {0x00, 0xc0, 0xc0, 0xc0, 0xf8, 0xcc, 0xcc, 0xf8},   // b 98
  {0x00, 0x00, 0x00, 0x78, 0xcc, 0xc0, 0xcc, 0x78},   // c 99
  {0x00, 0x0c, 0x0c, 0x0c, 0x7c, 0xcc, 0xcc, 0x7c},   // d 100
  {0x00, 0x00, 0x00, 0x78, 0xcc, 0xfc, 0xc0, 0x78},   // e 101
  {0x00, 0x38, 0x6c, 0x60, 0x60, 0xf8, 0x60, 0x60},   // f 102
  {0x00, 0x00, 0x7c, 0xcc, 0xcc, 0x7c, 0x0c, 0x78},   // g 103
  {0x00, 0xc0, 0xc0, 0xc0, 0xf8, 0xcc, 0xcc, 0xcc},   // h 104
  {0x00, 0x00, 0x60, 0x00, 0x60, 0x60, 0x60, 0xf0},   // i 105
  {0x00, 0x18, 0x00, 0x18, 0x18, 0xd8, 0xd8, 0x70},   // j 106
  {0x00, 0xc0, 0xc0, 0xcc, 0xd8, 0xf0, 0xd8, 0xcc},   // k 107
  {0x00, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0},   // l 108
  {0x00, 0x00, 0x00, 0xc6, 0xee, 0xfe, 0xd6, 0xd6},   // m 109
  {0x00, 0x00, 0x00, 0xf8, 0xfc, 0xcc, 0xcc, 0xcc},   // n 110
  {0x00, 0x00, 0x00, 0x78, 0xcc, 0xcc, 0xcc, 0x78},   // o 111
  {0x00, 0x00, 0xf8, 0xcc, 0xcc, 0xf8, 0xc0, 0xc0},   // p 112
  {0x00, 0x00, 0x78, 0xd8, 0xd8, 0x78, 0x1a, 0x1e},   // q 113
  {0x00, 0x00, 0x00, 0xf8, 0xcc, 0xcc, 0xc0, 0xc0},   // r 114
  {0x00, 0x00, 0x00, 0x7c, 0x80, 0x78, 0x04, 0xf8},   // t 115
  {0x00, 0x00, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x30},   // s 116
  {0x00, 0x00, 0x00, 0xcc, 0xcc, 0xcc, 0xcc, 0x7c},   // u 117
  {0x00, 0x00, 0x00, 0x00, 0xcc, 0xcc, 0x78, 0x30},   // v 118
  {0x00, 0x00, 0x00, 0xc6, 0xd6, 0xd6, 0xd6, 0x7c},   // w 119
  {0x00, 0x00, 0x00, 0xcc, 0x78, 0x30, 0x78, 0xcc},   // x 120
  {0x00, 0x00, 0x00, 0xcc, 0xcc, 0x7c, 0x0c, 0x78},   // y 121
  {0x00, 0x00, 0x00, 0xf0, 0x30, 0x60, 0xc0, 0xf0},   // z 122
0x00, 0x18, 0x20, 0x20, 0x60, 0x20, 0x20, 0x18, //{ 123
0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, //| 124
0x00, 0x30, 0x08, 0x08, 0x0C, 0x08, 0x08, 0x30, //} 125
0x00, 0x66, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x18, // ❤   126  ~ 0x00, 0x00, 0x00, 0x28, 0x50, 0x00, 0x00, 0x00
};
int matrix_X, matrix_Y;
unsigned char Buffer_display[NUM_MATRICES][MATRIX_ROWS]{
//{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,},
//{0xff, 0xff, 0xff, 0xee, 0x3e, 0xff, 0xff, 0xff,},
//{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,},
//{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,},
};

byte cols[8] = {0b10000000, 0b01000000, 0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000001};
byte rows[8] = {0b10000000, 0b01000000, 0b00100000, 0b00010000, 0b00001000, 0b00000100, 0b00000010, 0b00000001};

byte  mode = 0;
byte  currentMode = 0;


volatile unsigned long lastChangeModeTime = 0;
volatile unsigned long lastChangeScriptTime = 0;
const unsigned long debounceDelay = 100;

unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 2000;

float temperature = 0.0;
float humidity = 0.0;

char sensorText[16]; 
char textTemp[8];
char textHum[8];
char message[16];

bool displayTemp = true;

void setup() {
  // put your setup code here, to run once:
  dht.begin();
  pinMode(ClockRow, OUTPUT);
  pinMode(DataRow, OUTPUT);
  pinMode(Latch, OUTPUT);
  pinMode(ClockCol, OUTPUT);
  pinMode(DataCol, OUTPUT);

  pinMode(modeBtn, INPUT_PULLUP);
  pinMode(scriptBtn, INPUT_PULLUP);

  attachInterrupt(0, changeMode, FALLING);
  attachInterrupt(1, changeScript, FALLING);

  Serial.begin(9600);
}

void loop() {
  
//  sendToBuffer(0, '9');
//  displayBuffer();
//  scrollText_ToLeft("huong tit", NUM_MATRICES * MATRIX_COLS, 0, scroll_time);
//  setPosition(0,0);
//  displayString("NIKE");
//  displayBuffer();       
//  displayMatrix();

  currentMode = mode;

  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 2000) {
        previousMillis = currentMillis;
        
        temperature = dht.readTemperature();
        humidity = dht.readHumidity();
  
        Serial.print("Temperature: ");
        Serial.println(temperature);
        Serial.print("Humidity: ");
        Serial.println(humidity);
    }
    if (isnan(temperature) || isnan(humidity)) {
      //Serial.println("Failed to read from DHT sensor!");
      const char* error = "Error reading DHT sensor!";
      scrollText_ToLeft(error, NUM_MATRICES * MATRIX_COLS, 0, scroll_time);
    }
    else{

    itoa((int)temperature, textTemp, 10);
    itoa((int)humidity, textHum, 10);  

    strcat(textTemp, "`C"); 
    strcat(textHum, "%");    
    strcpy(sensorText, textTemp); 
    strcat(sensorText, "|"); 
    strcat(sensorText, textHum);  

    strcpy(message, sensorText); 
    size_t textLength = strlen(message);
      switch(mode){
        case 0:
          if (displayTemp) {
              clearBuffer();
              setPosition(0,0);
              displayString(textTemp);
              displayBuffer();
            }
          else{
              clearBuffer();
              setPosition(0,0);
              displayString(textHum);
              displayBuffer();
          }
          currentMillis = millis();
          static unsigned long lastTypeData = 0;
          if (currentMillis - lastTypeData >= 5000) {
            lastTypeData = currentMillis;
            displayTemp = !displayTemp;        
          }
          break;
        case 1:
          scrollText_ToLeft(message, NUM_MATRICES * MATRIX_COLS, 0, scroll_time);
          break;
        case 2:
          scrollText_ToRight(message, (-textLength*7), 0, scroll_time);
          break;
        case 3:
          scrollText_ToTop(message, 0, MATRIX_ROWS, scroll_time);
          break;
        case 4:
          scrollText_ToBottom(message, 0, -MATRIX_ROWS, scroll_time);
          break;
        default:
          scrollText_ToLeft("ERROR", NUM_MATRICES * MATRIX_COLS, 0, scroll_time);
          break;
      }
    }


  
}

void hc595out(unsigned char byte){
  int i;
  for(i=0; i<8; i++){
    digitalWrite(ClockRow, LOW);
    digitalWrite(DataRow, (byte & (0x80 >> i)) ? HIGH : LOW);
    digitalWrite(ClockRow, HIGH);
  }
}

void displayMatrix(){
  for(int i=0; i<8; i++){
    digitalWrite(Latch, LOW);       
    shiftOut(DataCol, ClockCol, MSBFIRST, ~cols[i]); // 0x00
    shiftOut(DataRow, ClockRow, MSBFIRST, rows[i] ); //0xff    letters[65-32][i]
    digitalWrite(Latch, HIGH);
    delay(10000);                                                                      
  }
}

void clearBuffer(){
    memset(Buffer_display, 0, sizeof(Buffer_display));
}

void sendToBuffer(int position, char character) {
  if (position >= 0 && position < NUM_MATRICES) {
    // Update the corresponding row of the buffer with the character pattern
    for (int i = 0; i < MATRIX_ROWS; i++) {
      Buffer_display[position][i] = letters[character - 32][i]; // Assuming character is uppercase alphabet
    }
  }
}

void displayBuffer(){
  // Iterate over each column of the LED matrices
   for (int col = 0; col < MATRIX_COLS; col++) {
    // Send data for each LED matrix in the current column
    for (int matrix = 0; matrix < NUM_MATRICES; matrix++) {
      hc595out(Buffer_display[matrix][col]); // Send data for current LED matrix
    }
      digitalWrite(Latch, LOW);
      shiftOut(DataCol, ClockCol, LSBFIRST, ~cols[col]);
      digitalWrite(Latch, HIGH);
  }
}

void setPosition(int x, int y){
  matrix_X = x;
  matrix_Y = y;
}
void setPosMatrix(int x, int y, bool light){
  if (x < 0 || y < 0 || x >= NUM_MATRICES * MATRIX_COLS || y >= MATRIX_ROWS) return;
  
  if (light) {
    Buffer_display[y / MATRIX_COLS][x] |= (0x80 >> (y % MATRIX_COLS));
  } else {
    Buffer_display[y / MATRIX_COLS][x] &= ~(0x80 >> (y % MATRIX_COLS));
  }
}

int getCharWidth(char txt) {
  int charWidth;
  
  switch (txt) {
    case 'l':
      charWidth = 2;
      break;
    case 'I':
    case 'i':
    case 'z':
    case ' ':
      charWidth = 4;
      break;
    case 'W':
    case 'M':
    case 'N':
    case 'X':
    case 'm':
    case 'w':
    case 'q':
      charWidth = 7;
      break;
    case '~':
      charWidth = 8;
      break; 
    default:
      // Default case if none of the specific characters match
      charWidth = 6; // Assuming default width
      break;
  }
  return charWidth;
}

void displayChar(unsigned char txt) {
  int charWidth = getCharWidth(txt); // Default character width

  for (int i = 0; i < 8; i++) {
    byte pattern = letters[txt - 32][i];
    for (int j = 0; j < charWidth; j++) { // Use adjusted width here
      int x = matrix_X + j;
      int y = matrix_Y + i;
      if (x >= 0 && x < NUM_MATRICES * MATRIX_COLS && y >= 0 && y < MATRIX_ROWS) {
        setPosMatrix(x, y, pattern & (1 << (7 - j)));
      }
    }
  }
}


void displayString(const char *str) {
  int positionX = matrix_X; // Initial position
  int positionY = matrix_Y;
  
  // Iterate over each character in the string
  for (int i = 0; str[i] != '\0'; i++) {
    int charWidth = getCharWidth(str[i]); // Get character width
    
    // Display the character
    setPosition(positionX, positionY);
    displayChar(str[i]);
    
    // Increment position for the next character
    positionX += charWidth + spacingChar; // Add 1 pixel spacing between characters
  }
}

void scrollText_ToLeft(const char *text, int x, int y, int scrollTime) {
  static unsigned long previousMillis = 0;
  static int positionX = x; 
  static int positionY = y;
  unsigned long currentMillis = millis();
  displayBuffer();

  if (currentMillis - previousMillis >= scrollTime) {
    previousMillis = currentMillis;

    clearBuffer(); // Clear the buffer for the next frame
    
    // Display the text at the current position
    setPosition(positionX, positionY);
    displayString(text);
    
    displayBuffer(); // Display the buffer
    positionX-=shiftPixel; // Move the position for the next frame
    
    int resetPoint = -(strlen(text)*7);
    if (positionX < resetPoint) {
      positionX = x; // Reset position once the text goes off-screen
    }
  }
}

void scrollText_ToRight(const char *text, int x, int y, int scrollTime) {
  static unsigned long previousMillis = 0;
  static int positionX = x; 
  static int positionY = y;
  unsigned long currentMillis = millis();
  displayBuffer();

  if (currentMillis - previousMillis >= scrollTime) {
    previousMillis = currentMillis;

    clearBuffer(); // Clear the buffer for the next frame
    
    // Display the text at the current position
    setPosition(positionX, positionY);
    displayString(text);
    
    displayBuffer(); // Display the buffer
    positionX+=shiftPixel; // Move the position for the next frame
    
    if (positionX > NUM_MATRICES * MATRIX_COLS) {
      positionX = x; // Reset position once the text goes off-screen
    }
  }
}
void scrollText_ToTop(const char *text, int x, int y, int scrollTime) {
  static unsigned long previousMillis = 0;
  static int positionX = x; 
  static int positionY = y;
  unsigned long currentMillis = millis();
  displayBuffer();

  if (currentMillis - previousMillis >= scrollTime) {
    previousMillis = currentMillis;

    clearBuffer(); // Clear the buffer for the next frame
    
    // Display the text at the current position
    setPosition(positionX, positionY);
    displayString(text);
    
    displayBuffer(); // Display the buffer
    positionY-=shiftPixel; // Move the position for the next frame
    
    if (positionY < -MATRIX_ROWS) {
      positionY = y; // Reset position once the text goes off-screen
    }
  }
}

void scrollText_ToBottom(const char *text, int x, int y, int scrollTime) {
  static unsigned long previousMillis = 0;
  static int positionX = x; 
  static int positionY = y;
  unsigned long currentMillis = millis();
  displayBuffer();

  if (currentMillis - previousMillis >= scrollTime) {
    previousMillis = currentMillis;

    clearBuffer(); // Clear the buffer for the next frame
    
    // Display the text at the current position
    setPosition(positionX, positionY);
    displayString(text);
    
    displayBuffer(); // Display the buffer
    positionY+=shiftPixel; // Move the position for the next frame
    
    if (positionY > MATRIX_ROWS) {
      positionY = y; // Reset position once the text goes off-screen
    }
  }
}

void changeMode(){
  unsigned long currentTime = millis();
  if (currentTime - lastChangeModeTime > debounceDelay) {
    lastChangeModeTime = currentTime;
    
    clearBuffer();
    if(mode == 0){
      Serial.println("AUTO -> MANUAL");
      mode = 1;
      Serial.println("Right to left");
      return;
    }
    if(mode != 0){
      mode = 0;
      Serial.println("MANUAL -> AUTO");
      return;
    }
  }
}

void changeScript(){
  unsigned long currentTime = millis();
  if (currentTime - lastChangeScriptTime > debounceDelay) {
    lastChangeScriptTime = currentTime;
    clearBuffer();
    if(mode == 0){
      Serial.println("Can not change script in auto mode!");
      return;
    }
    mode++;
    switch(mode){
      case 1:
        Serial.println("Right to left");
        break;
      case 2: 
        Serial.println("Left to right");
        break;
      case 3: 
        Serial.println("Bottom to top");
        break;
      case 4: 
        Serial.println("Top to bottom");
        break;
      default:
        mode = 1;
        break;
    }
  }
}
