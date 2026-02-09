#include <Keyboard.h>
#include <Arduino_LED_Matrix.h>

ArduinoLEDMatrix matrix;

// 建立一個畫布 (8列 x 12行)
uint8_t frame[8][12];

// ⏱️ 計時器變數
unsigned long lastKeyPressTime = 0;
bool isHeartMode = false; // 紀錄現在是不是已經在顯示愛心

void setup() {
  Serial.begin(115200);
  Keyboard.begin();
  matrix.begin();
  
  // 開機先顯示愛心
  drawHeart();
  matrix.renderBitmap(frame, 8, 12);
  isHeartMode = true;
}

// --- 字庫定義 ---

void drawC(int x) {
  uint8_t bmp[7][5] = {{0,1,1,1,0},{1,0,0,0,1},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,1},{0,1,1,1,0}};
  drawBitmap(bmp, x);
}
void drawD(int x) {
  uint8_t bmp[7][5] = {{1,1,1,0,0},{1,0,0,1,0},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,0,1},{1,0,0,1,0},{1,1,1,0,0}};
  drawBitmap(bmp, x);
}
void drawE(int x) {
  uint8_t bmp[7][5] = {{1,1,1,1,1},{1,0,0,0,0},{1,0,0,0,0},{1,1,1,1,0},{1,0,0,0,0},{1,0,0,0,0},{1,1,1,1,1}};
  drawBitmap(bmp, x);
}
void drawF(int x) {
  uint8_t bmp[7][5] = {{1,1,1,1,1},{1,0,0,0,0},{1,0,0,0,0},{1,1,1,1,0},{1,0,0,0,0},{1,0,0,0,0},{1,0,0,0,0}};
  drawBitmap(bmp, x);
}
void drawG(int x) {
  uint8_t bmp[7][5] = {{0,1,1,1,0},{1,0,0,0,1},{1,0,0,0,0},{1,0,1,1,1},{1,0,0,0,1},{1,0,0,0,1},{0,1,1,1,0}};
  drawBitmap(bmp, x);
}
void drawA(int x) {
  uint8_t bmp[7][5] = {{0,0,1,0,0},{0,1,0,1,0},{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,1},{1,0,0,0,1},{1,0,0,0,1}};
  drawBitmap(bmp, x);
}
void drawB(int x) {
  uint8_t bmp[7][5] = {{1,1,1,1,0},{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,0},{1,0,0,0,1},{1,0,0,0,1},{1,1,1,1,0}};
  drawBitmap(bmp, x);
}

// 畫愛心函數
void drawHeart() {
  clearFrame();
  // 手繪愛心像素
  frame[1][2]=1; frame[1][3]=1; frame[1][8]=1; frame[1][9]=1;
  frame[2][1]=1; frame[2][4]=1; frame[2][7]=1; frame[2][10]=1;
  frame[3][1]=1; frame[3][10]=1;
  frame[4][1]=1; frame[4][10]=1;
  frame[5][2]=1; frame[5][9]=1;
  frame[6][3]=1; frame[6][8]=1;
  frame[7][4]=1; frame[7][7]=1; // 尖端
}

// 通用繪圖函數
void drawBitmap(uint8_t bmp[7][5], int xOffset) {
  for (int y = 0; y < 7; y++) {
    for (int x = 0; x < 5; x++) {
      if (bmp[y][x] == 1) {
        if (x + xOffset < 12) frame[y+1][x + xOffset] = 1; 
      }
    }
  }
}

void drawMod(byte mod) {
  if (mod == 1) { // #
    frame[2][7]=1; frame[3][7]=1; frame[4][7]=1; frame[5][7]=1; frame[6][7]=1;
    frame[2][9]=1; frame[3][9]=1; frame[4][9]=1; frame[5][9]=1; frame[6][9]=1;
    frame[3][6]=1; frame[3][8]=1; frame[3][10]=1;
    frame[5][6]=1; frame[5][8]=1; frame[5][10]=1;
  }
  else if (mod == 2) { // b
    frame[1][7]=1; frame[2][7]=1; frame[3][7]=1; frame[4][7]=1; frame[5][7]=1; frame[6][7]=1;
    frame[4][8]=1; frame[4][9]=1; frame[5][10]=1; frame[6][8]=1; frame[6][9]=1;
  }
}

void clearFrame() {
  for(int i=0; i<8; i++) for(int j=0; j<12; j++) frame[i][j] = 0;
}

void showNoteOnScreen(char key, byte mod) {
  clearFrame();
  int xPos = (mod > 0) ? 1 : 4;
  
  if      (key == 'z' || key == 'a' || key == 'q') drawC(xPos);
  else if (key == 'x' || key == 's' || key == 'w') drawD(xPos);
  else if (key == 'c' || key == 'd' || key == 'e') drawE(xPos);
  else if (key == 'v' || key == 'f' || key == 'r') drawF(xPos);
  else if (key == 'b' || key == 'g' || key == 't') drawG(xPos);
  else if (key == 'n' || key == 'h' || key == 'y') drawA(xPos);
  else if (key == 'm' || key == 'j' || key == 'u') drawB(xPos);
  
  drawMod(mod);
  matrix.renderBitmap(frame, 8, 12);
}

void loop() {
  // 1. 如果有收到電腦訊號 (正在彈琴)
  if (Serial.available() >= 2) {
    byte modState = Serial.read();
    char keyChar = Serial.read();
    
    // 顯示音名
    showNoteOnScreen(keyChar, modState);
    
    // 🔔 更新計時器 (重置倒數)
    lastKeyPressTime = millis();
    isHeartMode = false; // 退出愛心模式

    // 執行按鍵
    if (modState == 1) Keyboard.press(KEY_LEFT_SHIFT);
    else if (modState == 2) Keyboard.press(KEY_LEFT_CTRL);
    
    Keyboard.press(keyChar);
    delay(10);
    Keyboard.releaseAll();
  }

  // 2. 檢查是否閒置超過 3 秒 (3000 ms)
  // millis() 是 Arduino 開機後經過的總時間
  if (!isHeartMode && (millis() - lastKeyPressTime > 3000)) {
    drawHeart();
    matrix.renderBitmap(frame, 8, 12);
    isHeartMode = true; // 標記為愛心模式，避免重複刷新
  }
}