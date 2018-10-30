#include <MicroView.h>
// 7, 4, 17
// 7 -> pieces
// 4 -> variations (up right down left)
// 17 -> able to rotate in that way + 16 pixels (4x4)
boolean pieces[7][4][17] = {{
  {true,
  false, false, false, false,
  false, false, false, false,
  false, false, true, true,
  false, false, true, true
  },
  {false},
  {false},
  {false}
  }, // square
  {
    {true,
    false, false, false, true,
    false, false, false, true,
    false, false, false, true,
    false, false, false, true
    },
    {true,
    false, false, false, false,
    false, false, false, false,
    false, false, false, false,
    true, true, true, true
    },
    {false},
    {false}
  }, // line
  {
    {true,
    false, false, false, false,
    false, false, false, false,
    false, false, true, true,
    false, true, true, false
    },
    {true,
    false, false, false, false,
    false, false, true, false,
    false, false, true, true,
    false, false, false, true
    },
    {false},
    {false}
  }, // s
  {
    {true,
    false, false, false, false,
    false, false, false, false,
    false, true, true, false,
    false, false, true, true
    },
    {true,
    false, false, false, false,
    false, false, false, true,
    false, false, true, true,
    false, false, true, false
    },
    {false},
    {false}
  }, // z
  {
    {true,
    false, false, false, false,
    false, false, true, false,
    false, false, true, false,
    false, false, true, true, 
    },
    {true,
    false, false, false, false,
    false, false, false, false,
    true, true, true, false,
    true, false, false, false
    },
    {true,
    false, false, false, false,
    false, false, true, true,
    false, false, false, true,
    false, false, false, true
    },
    {true,
    false, false, false, false,
    false, false, false, false,
    false, false, false, true,
    false, true, true, true
    }
  }, // L
  {
    {true,
    false, false, false, false,
    false, false, false, true,
    false, false, false, true,
    false, false, true, true
    },
    {true,
    false, false, false, false,
    false, false, false, false,
    false, true, true, true,
    false, false, false, true
    },
    {true,
    false, false, false, false,
    false, false, true, true,
    false, false, true, false,
    false, false, true, false
    },
    {true,
    false, false, false, false,
    false, false, false, false,
    false, true, false, false,
    false, true, true, true
    }
  }, // J
  {
    {true,
      false, false, false, false,
      false, false, false, false,
      false, true, true, true,
      false, false, true, false
    },
    {true,
      false, false, false, false,
      false, false, false, true,
      false, false, true, true,
      false, false, false, true
    },
    {true,
      false, false, false, false,
      false, false, false, false,
      false, false, true, false,
      false, true, true, true
    },
    {true,
      false, false, false, false,
      false, false, true, false,
      false, false, true, true,
      false, false, true, false
    }
  }}; // T

// false for no pixel, true for pixel
// 20 x 10 = 200
boolean board[200];
boolean pieceBoard[200];

byte gameSpeedCount = 5;
byte gameSpeedSet = 5;

int currentScore = 0;

byte nextPiece = random(7);

byte currentPiece = 7;
byte currentVariation;
byte currentX;
byte currentY;

byte sw_pin = 2;
byte x_pin = 0;
byte y_pin = 1;

void setup() {
  uView.begin();
  for (short s = 0; s < 200; s++) board[s] = false;
  pinMode(sw_pin, INPUT);
  digitalWrite(sw_pin, HIGH);
}

void loop() {
  if (currentPiece == 7) {
    currentPiece = nextPiece;
    nextPiece = random(7);
    currentVariation = 0;
    currentX = 3; // 1 - 7
    currentY = 1; // 1 - 17
  }
  // Add way to detect joystick pressed and call methods accordingly
  checkPiece();
  drawBoard();
  delay(200);
  if (gameSpeedCount == 0) {
    down();
    gameSpeedCount = gameSpeedSet;
  } else gameSpeedCount--;
  if (analogRead(x_pin) < 200) right();
  if (analogRead(x_pin) > 800) left();
  if (analogRead(y_pin) < 200) down();
  if (analogRead(y_pin) > 800) rotate();
}

void checkLine() {
  for (byte b = 0; b < 20; b++) {
    for (byte c = 0; c < 10; c++) {
      if (!board[b * 10 + c]) break;
      if (c == 9) {
        boolean newBoard[200];
        for (byte d = 0; d < 9; d++) board[b * 10 + d] = false;
        addScore(1);
        for (short s = 0; s < 200; s++) { 
          newBoard[s] = false;
          if (board[s]) newBoard[s + 10] = true;
        }
        for (short s = 0; s < 200; s++) board[s] = newBoard[s];
        checkLine();
      }
    }
  }
}

// 2 TIMES SCALE xy (40-20)
// pixel(x*2,y*2)
// pixel (x*2+1,y*2)
// pixel (x*2, y*2+1)
// pixel (x*2+1, y*2+1)

// 3 TIMES SCALE xy (60-30)
// pixel (x*3, y*3)
// pixel (x*3+1, y*3)
// pixel (x*3+2, y*3)
// pixel (x*3, y*3+1)
// pixel (x*3+1, y*3+1)
// pixel (x*3+2, y*3+1)
// pixel (x*3, y*3+2)
// pixel (x*3+1, y*3+2)
// pixel (x*3+2, y*3+2)

// 4 TIMES SCALE xy (80-40)
// pixel(x*4,y*4)
// pixel(x*4+1, y*4)
// pixel(x*4+2, y*4)
// pixel(x*4+3, y*4)
// pixel(x*4,y*4+1)
// pixel(x*4+1, y*4+1)
// pixel(x*4+2, y*4+1)
// pixel(x*4+3, y*4+1)
// pixel(x*4,y*4+2)
// pixel(x*4+1, y*4+2)
// pixel(x*4+2, y*4+2)
// pixel(x*4+3, y*4+2)
// pixel(x*4,y*4+3)
// pixel(x*4+1, y*4+3)
// pixel(x*4+2, y*4+3)
// pixel(x*4+3, y*4+3)

void drawBoard() {
  uView.clear(PAGE);
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      if (pieceBoard[(i * 10) + j] == true) {
        uView.pixel(j, i);
      }
    }
  }
  uView.display();
}

void drawPieceBoard() {
  for (int i = 0; i < 200; i++) {
    pieceBoard[i] = board[i];
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (pieces[currentPiece][currentVariation][i * 4 + j + 1]) {
        pieceBoard[(currentY * 10) + currentX + (i * 10) + j] = true;
      }
    }
  }
}

short pieceIndexes[4] = {-1,-1,-1,-1};

void setPieceIndexes() {
  for (byte b = 0; b < 4; b++) pieceIndexes[b] = -1;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (pieces[currentPiece][currentVariation][i * 4 + j + 1]) {
        for (byte t = 0; t < 4; t++) {
          if (pieceIndexes[t] == -1) { pieceIndexes[t] = (currentY * 10) + currentX + (i * 10) + j; break; }
          else continue;
        }
      }
    }
  }
}

void checkPiece() {
  setPieceIndexes();
  for (byte b = 0; b < 4; b++) {
    if (board[pieceIndexes[b] + 10]) {
      stopPiece(); 
      for (int i = 0; i < 200; i++) {
        pieceBoard[i] = board[i];
      }
      return;
    }
  }
  drawPieceBoard();
}

void stopPiece() {
  for (byte b = 0; b < 4; b++) { board[pieceIndexes[b]] = true; }
  currentPiece = 7;
  checkLine();
}

void rotate() {
  byte localVariation = currentVariation;
  for (int i = 0; i < 4; i++) {
    if (localVariation == 3) localVariation = 0;
    else localVariation++;
    if (!pieces[currentPiece][localVariation][0]) continue;
    else { 
      if (canRotate(localVariation)) {
        currentVariation = localVariation;
        return;
      } else continue;
    }
  }
}

boolean canRotate(int variation) {
  boolean localBoard[200];
  for (int i = 0; i < 200; i++) {
    localBoard[i] = board[i];
  }
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (pieces[currentPiece][variation][i * 4 + j + 1]) {
        short index = (currentY * 10) + currentX + (i * 10) + j;
        if (board[index]) return false;
      }
    }
  }
  return true;
}

void down() {
  if (currentY == 17) { stopPiece(); return; }
  currentY++;
}

void left() {
  if (currentX == 1) return;
  currentX--;
}

void right() {
  if (currentX == 6) return;
  currentX++;
}

void addScore(int score) {
  currentScore += score;  
}
