#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h> //Used in support of TFT Display
#include <string.h>  //used for some string handling and processing.

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

#define BACKGROUND TFT_BLACK
#define DEFAULT_FACE_COLOR TFT_WHITE

enum emotion {
  HAPPY, // ^_^
  SAD, // v_v
  SCARED, // >_<
  BLUSH, // >//<
  WEIRD, // o_O
  CUTE, //uwu
  BLANK, //owo
  INDIFFERENT, // ._.
  CRYING // ;~;
};

unsigned long last_face;
unsigned long animation_timer;

void setup() {
  Serial.begin(115200);

  Serial.println("initialize screen");
  tft.init();
  tft.setRotation(3);
  tft.setTextSize(1);
  tft.setTextColor(DEFAULT_FACE_COLOR);
  tft.fillScreen(BACKGROUND);
  last_face = millis();
  animation_timer = millis();

  /* chevron */
  /*tft.drawLine(0, 24, 12, 36, TFT_RED);
  tft.drawLine(1, 24, 13, 36, TFT_RED);
  tft.drawLine(2, 24, 14, 36, TFT_RED);
  tft.drawLine(3, 24, 15, 36, TFT_RED);
  tft.drawLine(4, 24, 16, 36, TFT_RED);
  tft.drawLine(0, 48, 12, 36, TFT_RED);
  tft.drawLine(1, 48, 13, 36, TFT_RED);
  tft.drawLine(2, 48, 14, 36, TFT_RED);
  tft.drawLine(3, 48, 15, 36, TFT_RED);
  tft.drawLine(4, 48, 16, 36, TFT_RED);*/
}

bool written = false;
int face = HAPPY;
int iter;

void loop() {
  if (millis() - last_face > 1000) {
    tft.fillScreen(BACKGROUND);
    face = (face+1)%9;
    written = false;
    last_face = millis();
  }

  /* for moving chevron */
  /*if(millis() - animation_timer > 100) {
    tft.drawLine(iter, 24, iter+12, 36, BACKGROUND);
    tft.drawLine(iter, 48, iter+12, 36, BACKGROUND);
    tft.drawLine(iter+5, 24, iter+5+12, 36, TFT_RED);
    tft.drawLine(iter+5, 48, iter+5+12, 36, TFT_RED);

    iter++;
    iter %= 160;
    animation_timer = millis();
  }*/

  /* shrinking chevron */
  /*if(millis() - animation_timer > 25) {
    tft.drawLine(0, 24+iter, 12, 36, BACKGROUND);
    tft.drawLine(1, 24+iter, 13, 36, BACKGROUND);
    tft.drawLine(2, 24+iter, 14, 36, BACKGROUND);
    tft.drawLine(3, 24+iter, 15, 36, BACKGROUND);
    tft.drawLine(4, 24+iter, 16, 36, BACKGROUND);
    tft.drawLine(0, 48-iter, 12, 36, BACKGROUND);
    tft.drawLine(1, 48-iter, 13, 36, BACKGROUND);
    tft.drawLine(2, 48-iter, 14, 36, BACKGROUND);
    tft.drawLine(3, 48-iter, 15, 36, BACKGROUND);
    tft.drawLine(4, 48-iter, 16, 36, BACKGROUND);

    iter++;
    iter %= 6;
    
    tft.drawLine(0, 24+iter, 12, 36, TFT_RED);
    tft.drawLine(1, 24+iter, 13, 36, TFT_RED);
    tft.drawLine(2, 24+iter, 14, 36, TFT_RED);
    tft.drawLine(3, 24+iter, 15, 36, TFT_RED);
    tft.drawLine(4, 24+iter, 16, 36, TFT_RED);
    tft.drawLine(0, 48-iter, 12, 36, TFT_RED);
    tft.drawLine(1, 48-iter, 13, 36, TFT_RED);
    tft.drawLine(2, 48-iter, 14, 36, TFT_RED);
    tft.drawLine(3, 48-iter, 15, 36, TFT_RED);
    tft.drawLine(4, 48-iter, 16, 36, TFT_RED);
    
    animation_timer = millis();
  }*/
  
  if (!written) {
    switch(face) {
      case HAPPY:
      tft.setCursor(20, 30, 1);
      tft.setTextSize(10);
      tft.setTextColor(TFT_ORANGE);
      tft.println("^_^");
      break;
      case SAD:
      tft.setCursor(20, 30, 1);
      tft.setTextSize(10);
      tft.setTextColor(TFT_SILVER);
      tft.println("V_V");
      break;
      case SCARED:
      tft.setCursor(20, 30, 1);
      tft.setTextSize(10);
      tft.setTextColor(TFT_YELLOW);
      tft.println(">_<");
      break;
      case BLUSH:
      tft.setCursor(7, 35, 1);
      tft.setTextSize(6);
      tft.setTextColor(0xFC9F);
      tft.println(">//<");
      break;
      case WEIRD:
      tft.setCursor(20, 30, 1);
      tft.setTextSize(10);
      tft.setTextColor(TFT_GREEN);
      tft.println("o_O");
      break;
      case CUTE:
      tft.setCursor(20, 30, 1);
      tft.setTextSize(10);
      tft.setTextColor(TFT_RED);
      tft.println("uwu");
      break;
      case BLANK:
      tft.setCursor(20, 30, 1);
      tft.setTextSize(10);
      tft.setTextColor(TFT_WHITE);
      tft.println("owo");
      break;
      case INDIFFERENT:
      tft.setCursor(20, 30, 1);
      tft.setTextSize(10);
      tft.setTextColor(0x797A7E);
      tft.println("._.");
      break;
      case CRYING: 
      tft.setCursor(20, 30, 1);
      tft.setTextSize(10);
      tft.setTextColor(TFT_BLUE);
      tft.println(";~;");
      break;
    }
    written = true;
  }
}
