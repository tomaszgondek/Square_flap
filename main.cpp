#include <SPI.h>
#include <Wire.h>
#include <U8g2_for_Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 64, &Wire);

#define UP_BUTTON 2

const long int BIRD_SIZE = 10;

//initial position of the square
int bird_x = 10;
int bird_y = 20;
//some random shit
int score = 0;
int highest_score = 0;
int offset = 0;
int offset_total = 0;
int temptime = 0;
int rimlen = 55;
int gap_size = 0;
int gap_position = 0;
//Positions of things
int check_x = 0;
int y_upper = 0;
int y_lower = 0;
//flags
bool need_update = true;
bool check_offset = false;
bool flag_score = true;
bool startup = true;
bool up = false;
bool game_over = false;
//time
long int time_check = 0;
long int time = 0;

//bitmaps
const unsigned char down_bmp [] PROGMEM = 
{
  0x21, 0x00, 0x52, 0x80, 0x7f, 0x80, 0xff, 0xc0, 0xff, 0xc0, 0xee, 0xc0, 0xee, 0xc0, 0xfb, 0xc0, 
  0xf5, 0xc0, 0x7b, 0x80
};
const unsigned char up_bmp [] PROGMEM = 
{
  0x21, 0x00, 0x52, 0x80, 0x7f, 0x80, 0xff, 0xc0, 0xdf, 0x40, 0xee, 0xc0, 0xdb, 0x40, 0xf5, 0xc0, 
  0xff, 0xc0, 0x7f, 0x80
};

const unsigned char welcome_bmp [] PROGMEM = 
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x6b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd6, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 
0x00, 0x00, 0x00, 0x00, 0x03, 0x8c, 0x95, 0xf0, 0x3b, 0x8c, 0x64, 0x80, 0x00, 0x00, 0x00, 0x00, 
0x20, 0x00, 0x00, 0x00, 0x02, 0x52, 0x94, 0x40, 0x42, 0x52, 0x94, 0x80, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x02, 0x52, 0xd0, 0x40, 0x42, 0x52, 0x84, 0x80, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x02, 0x52, 0xb0, 0x40, 0x43, 0x9e, 0x67, 0x80, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x02, 0x52, 0x90, 0x40, 0x42, 0x52, 0x14, 0x80, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x02, 0x52, 0x90, 0x40, 0x42, 0x52, 0x94, 0x80, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x03, 0x8c, 0x90, 0x40, 0x3a, 0x52, 0x64, 0x80, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3a, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x80, 0x02, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x40, 0x04, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x40, 0x04, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x1f, 0xf0, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87, 0xc0, 0x0f, 0xe2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x20, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x20, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0xc1, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x07, 0xc0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x08, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x07, 0xd2, 0xe0, 0xe6, 0x45, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x12, 0x81, 0x09, 0x6d, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x1e, 0xe1, 0x69, 0x55, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x12, 0x81, 0x2f, 0x45, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x12, 0xe0, 0xe9, 0x45, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x6b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd6, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char end_bmp [] PROGMEM = 
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x6b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd6, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 
0x00, 0x00, 0x00, 0x00, 0x02, 0x26, 0x48, 0xee, 0x31, 0x92, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x20, 0x00, 0x00, 0x00, 0x02, 0x29, 0x49, 0x09, 0x4a, 0x52, 0x84, 0x80, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x01, 0x49, 0x49, 0x09, 0x4a, 0x12, 0x84, 0x80, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x89, 0x49, 0x0e, 0x79, 0x9e, 0xe4, 0x80, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x89, 0x49, 0x09, 0x48, 0x52, 0x84, 0x80, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x89, 0x49, 0x09, 0x4a, 0x52, 0x84, 0x80, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x86, 0x30, 0xe9, 0x49, 0x92, 0xf7, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3a, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x12, 0xb0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x01, 0x80, 0x09, 0x4e, 0x97, 0x9c, 0xf8, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x80, 0x02, 0x40, 0x09, 0x50, 0x94, 0x20, 0x20, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x40, 0x04, 0x20, 0x0f, 0x56, 0xf7, 0x9c, 0x20, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x40, 0x04, 0x20, 0x09, 0x52, 0x94, 0x02, 0x20, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x08, 0x10, 0x09, 0x4e, 0x97, 0x9c, 0x20, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x20, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x10, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x1f, 0xf0, 0x08, 0x07, 0x1c, 0xce, 0x78, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x08, 0x08, 0x21, 0x29, 0x41, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x08, 0x07, 0x21, 0x2e, 0x78, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x04, 0x00, 0xa1, 0x29, 0x41, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x04, 0x07, 0x1c, 0xc9, 0x78, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87, 0xc0, 0x0f, 0xe2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x88, 0x20, 0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x08, 0x20, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0xc1, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x07, 0xc0, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x08, 0x20, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x1e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x20, 0x0e, 0x73, 0xce, 0x70, 0xc7, 0x25, 0xf7, 0xcc, 0x93, 0xe6, 0x1c, 0xf9, 0x9c, 0xf8, 0x04, 
0x20, 0x09, 0x4a, 0x10, 0x81, 0x24, 0xa4, 0x41, 0x12, 0xd0, 0x89, 0x20, 0x22, 0x52, 0x20, 0x04, 
0x20, 0x0e, 0x73, 0xce, 0x71, 0xe7, 0x24, 0x41, 0x12, 0xb0, 0x89, 0x1c, 0x23, 0xdc, 0x20, 0x04, 
0x20, 0x08, 0x4a, 0x01, 0x09, 0x24, 0xa4, 0x41, 0x12, 0x90, 0x89, 0x02, 0x22, 0x52, 0x20, 0x04, 
0x20, 0x08, 0x4b, 0xce, 0x71, 0x27, 0x18, 0x41, 0x0c, 0x90, 0x86, 0x1c, 0x22, 0x52, 0x20, 0x04, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0x6b, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd6, 
0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//functions
void drawField()
{
  display.drawRect(0, 0, 128, rimlen, WHITE);
}

void drawBird(int y_coord, const int x_coord, const unsigned long size)
{
  display.fillRect(y_coord, x_coord, size, size, BLACK);
  if(up == true)
  {
    display.drawBitmap(y_coord, x_coord, up_bmp, size, size, WHITE);
  }
  else
  {
    display.drawBitmap(y_coord, x_coord, down_bmp, size, size, WHITE);
  }
}

void printCrap()
{
  
  display.setTextColor(WHITE, BLACK);
  display.setCursor(110, rimlen + 1);
  display.print(highest_score);
  display.setCursor(10, rimlen + 1);
  display.print(score);
}

int drawObstacle(int x, int y, int offset, int gapsize, int cutoff)
{
  x = x - offset;
  int y1 = y + cutoff;
  int y2 = y1 + gapsize;
  display.drawLine(x, y, x, y1, WHITE);
  display.drawLine(x, y2, x, rimlen - 1, WHITE);
  display.drawLine(x+1, y, x+1, y1, WHITE);
  display.drawLine(x+1, y2, x+1, rimlen - 1, WHITE);
  y_upper = y1;
  y_lower = y2;
  return x;
}

void checkScore(int x)
{ 
  if(x < 10 && x > 8)
  {
    score = score + 1;
  }
}

bool checkColisions(int bx, int by, int xo, int yu, int yl)
{
  if(bx + 9 >= xo && bx < xo)
  {
    if(by > yu && by < yl && by + 9 < yl)
    {
      return false;
    }
    else
    {
      return true;
    }
  }
  return false;
}


void setup() 
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  pinMode(UP_BUTTON, INPUT_PULLUP);
  delay(500);
  display.clearDisplay();
}

void loop() 
{
  static bool up_state = false;
  bool button = false;
  time_check = millis();
  up_state |= (digitalRead(UP_BUTTON) == LOW);

  {
  if(startup == true)
  {
    display.clearDisplay();
    while(startup == true)
    {
      button = digitalRead(UP_BUTTON);
      display.drawBitmap(0, 0, welcome_bmp, 128, 64, WHITE);
      display.display();
      offset = 0;
      if(button == false)
      {
        startup = false;
        break;
      }
    }
  }
  if(game_over == true)
  {
    display.clearDisplay();
    while(game_over == true)
    {
      button = digitalRead(UP_BUTTON);
      display.drawBitmap(0, 0, end_bmp, 128, 64, WHITE);
      display.setCursor(93, 35);
      display.setTextColor(WHITE, BLACK);
      display.setTextSize(2);
      display.print(highest_score);
      display.display();
      offset = 0;
      if(button == false)
      {
        game_over = false;
        break;
      }
    }
  }
  }
  display.setTextSize(1);
  if(check_offset == true)
  {
    temptime = millis();
    check_offset = false;
    time = 0;
  }
  if(check_offset == false)
  {
    offset = millis() - temptime;
    time = offset / 1000;
    offset = offset / 25;
  }

  if(up_state) 
  {
    bird_y -= 1;
    up = true;
  }
  else
  {
    bird_y += 1;
    up = false;
  }
  up_state = false;

  if(bird_y < 1) 
  {
    bird_y = 1;
  }
  if(bird_y + BIRD_SIZE > rimlen) 
  {
    bird_y = rimlen - BIRD_SIZE;
  }

  if(need_update == true)
  {
    gap_size = 20 + random(10);
    gap_position = random(25);
    need_update = false;
    flag_score = true;
  }
  
  drawField();
  check_x = drawObstacle(128, 0, offset, gap_size, gap_position);
  drawBird(bird_x, bird_y, BIRD_SIZE);
  if(checkColisions(bird_x, bird_y, check_x, y_upper, y_lower) == true)
  {
    display.clearDisplay();
    display.setCursor(35, 35);
    display.print("GAME OVER!");
    display.display();
    delay(1000);
    display.clearDisplay();
    check_offset = true;
    need_update = true;
    bird_x = 10;
    bird_y = 20;
    if(score > highest_score)
    {
      highest_score = score;
    }
    score = 0;
    game_over = true;
  }
  display.display();
  display.clearDisplay();
  
  if(check_x <= 0)
  {
    check_offset = true;
    need_update = true;
  }
  checkScore(check_x);
  printCrap();
}

