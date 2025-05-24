#include <U8g2lib.h>
#include <Wire.h>
#include <EEPROM.h> // Include the EEPROM library

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define FLAP_BUTTON 12
#define SPRITE_HEIGHT 16
#define SPRITE_WIDTH 16

// Bird bitmap data
static const unsigned char PROGMEM wing_down_bmp[] = {
   0x00, 0x1c, 0x00, 0x21, 0x00, 0x40, 0x80, 0x48, 0x40, 0xed, 0xa0, 0x2a, 0x10, 0x28, 0x50, 0x29, 
	0xa8, 0x24, 0x54, 0x12, 0x4a, 0x00, 0x48, 0x04, 0x05, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00

};

static const unsigned char PROGMEM wing_up_bmp[] = {
  0x00, 0x1e, 0x00, 0x21, 0x00, 0x40, 0x80, 0x40, 0x40, 0xe4, 0x20, 0x22, 0x10, 0x21, 0x10, 0x21, 
	0x88, 0x20, 0x44, 0x10, 0x42, 0x08, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define GAME_SPEED 50

int game_state = 1;
int score = 0;
int high_score = 0;

int bird_x = u8g2.getDisplayWidth() / 4;
int bird_y;
int momentum = 0;
int wall_x[2];
int wall_y[2];
int wall_gap = 30;
int wall_width = 10;

void setup() {
  u8g2.begin();
  u8g2.clearBuffer();
  randomSeed(analogRead(0));
  pinMode(FLAP_BUTTON, INPUT_PULLUP);

  score = 0;
  high_score = 0;
}

void displayScore(int x, int y, int scr) {
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setCursor(x, y);
  //u8g2.print("Score: ");
  u8g2.print(scr);
  u8g2.setCursor(110, y);
  //u8g2.print("Score: ");
  u8g2.print(high_score);
}

void loop() {
  if (game_state == 0) {
    u8g2.clearBuffer();

    if (digitalRead(FLAP_BUTTON) == LOW) {
      momentum = -4;
    }

    momentum += 1;

    bird_y += momentum;

    if (bird_y < 0) {
      bird_y = 0;
    }

    if (bird_y > u8g2.getDisplayHeight() - SPRITE_HEIGHT) {
      bird_y = u8g2.getDisplayHeight() - SPRITE_HEIGHT;
      momentum = -2;
    }

    if (momentum < 0) {
      if (random(2) == 0) {
        u8g2.drawXBMP(bird_x, bird_y, SPRITE_WIDTH, SPRITE_HEIGHT, wing_down_bmp);
      } else {
        u8g2.drawXBMP(bird_x, bird_y, SPRITE_WIDTH, SPRITE_HEIGHT, wing_up_bmp);
      }
    } else {
      u8g2.drawXBMP(bird_x, bird_y, SPRITE_WIDTH, SPRITE_HEIGHT, wing_up_bmp);
    }

    for (int i = 0; i < 2; i++) {
      u8g2.drawBox(wall_x[i], 0, wall_width, wall_y[i]);
      u8g2.drawBox(wall_x[i], wall_y[i] + wall_gap, wall_width, u8g2.getDisplayHeight() - wall_y[i] + wall_gap);

      if (wall_x[i] < 0) {
        wall_y[i] = random(0, u8g2.getDisplayHeight() - wall_gap);
        wall_x[i] = u8g2.getDisplayWidth();
      }

      if (wall_x[i] == bird_x) {
        score = score+1;//max(score + 1, high_score);
      }

      if ((bird_x + SPRITE_WIDTH > wall_x[i] && bird_x < wall_x[i] + wall_width) &&
          (bird_y < wall_y[i] || bird_y + SPRITE_HEIGHT > wall_y[i] + wall_gap)) {
        u8g2.sendBuffer();
        delay(500);
        game_state = 1;
      }

      wall_x[i] -= 4;
    }
if (score > high_score) {
      high_score = score;
    }

    displayScore(0, 10, score);

    u8g2.sendBuffer();
    delay(GAME_SPEED);
  } else {
    u8g2.clearBuffer();

    outlineTextAtCenter(10, "NANO BIRD");
    textAtCenter(u8g2.getDisplayHeight() / 2 - 10, "GAME OVER");
    textAtCenter(u8g2.getDisplayHeight() / 2 + 1 , String(score));
    boldTextAtCenter(10, u8g2.getDisplayHeight() / 2 + 20, "HIGH SCORE: " + String(high_score));
    u8g2.setFont(u8g2_font_6x10_tf);
    u8g2.drawStr(50, 64, "Mastoras Inc");

    u8g2.sendBuffer();

    while (digitalRead(FLAP_BUTTON) == LOW);

    bird_y = u8g2.getDisplayHeight() / 2;
    momentum = -4;
    wall_x[0] = u8g2.getDisplayWidth();
    wall_y[0] = u8g2.getDisplayHeight() / 2 - wall_gap / 2;
    wall_x[1] = u8g2.getDisplayWidth() + u8g2.getDisplayWidth() / 2;
    wall_y[1] = u8g2.getDisplayHeight() / 2 - wall_gap / 1;
    score = 0;

    while (digitalRead(FLAP_BUTTON) == HIGH);

    game_state = 0;
  }
}

void textAt(int x, int y, String txt) {
  u8g2.setCursor(x, y);
  u8g2.print(txt);
}

void textAtCenter(int y, String txt) {
  textAt(u8g2.getDisplayWidth() / 2 - txt.length() * 4, y, txt);
}

void outlineTextAtCenter(int y, String txt) {
  int x = u8g2.getDisplayWidth() / 2 - txt.length() * 4;

  u8g2.setFont(u8g2_font_8x13_tf);
  u8g2.drawStr(x - 1, y, txt.c_str());
  u8g2.drawStr(x + 1, y, txt.c_str());
  u8g2.drawStr(x, y - 1, txt.c_str());
  u8g2.drawStr(x, y + 1, txt.c_str());

  u8g2.drawStr(x, y, txt.c_str());
}

void boldTextAtCenter(int x, int y, String txt) {
  textAt(x, y, txt);
  textAt(x + 1, y, txt);
}
