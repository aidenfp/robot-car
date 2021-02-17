emotion get_random_emotion() {
  int rand_int = random(0, 9);
  return (emotion) rand_int;
}

void update_face(emotion new_emotion) {
  tft.fillScreen(BACKGROUND);
  switch(new_emotion) {
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
}
