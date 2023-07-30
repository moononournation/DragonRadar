/*******************************************************************************
 * Start of Arduino_GFX setting
 ******************************************************************************/
#include <Arduino_GFX_Library.h>
#define GFX_BL 38
Arduino_DataBus *bus = new Arduino_SWSPI(
    GFX_NOT_DEFINED /* DC */, 39 /* CS */,
    48 /* SCK */, 47 /* MOSI */, GFX_NOT_DEFINED /* MISO */);
Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    18 /* DE */, 17 /* VSYNC */, 16 /* HSYNC */, 21 /* PCLK */,
    11 /* R0 */, 12 /* R1 */, 13 /* R2 */, 45 /* R3 */, 0 /* R4 */,
    8 /* G0 */, 19 /* G1 */, 20 /* G2 */, 46 /* G3 */, 9 /* G4 */, 10 /* G5 */,
    4 /* B0 */, 5 /* B1 */, 6 /* B2 */, 7 /* B3 */, 15 /* B4 */,
    1 /* hsync_polarity */, 50 /* hsync_front_porch */, 1 /* hsync_pulse_width */, 30 /* hsync_back_porch */,
    1 /* vsync_polarity */, 20 /* vsync_front_porch */, 1 /* vsync_pulse_width */, 30 /* vsync_back_porch */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    480 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */,
    bus, GFX_NOT_DEFINED /* RST */, st7701_type6_init_operations, sizeof(st7701_type6_init_operations));
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

bool btn_pressed = false;
uint8_t map_scale = 1;
void IRAM_ATTR ISR() {
    btn_pressed = true;
    map_scale++;
    if (map_scale > 3)
    {
      map_scale = 1;
    }
}

void draw_map() {
  gfx->fillScreen(RGB565(0, 112, 0));

  for (int i = (map_scale * 40); i < 480; i += (map_scale * 40))
  {
    gfx->fillRect(i, 0, 2, 480, BLACK);
    gfx->fillRect(0, i, 480, 2, BLACK);
  }

  gfx->fillTriangle(240, 230, 230, 247, 250, 247, RED);
  gfx->fillCircle(240 - (map_scale * 25), 240 - (map_scale * 55), 8, RGB565(248, 176, 0));
  gfx->fillCircle(240 + (map_scale * 95), 240 + (map_scale * 75), 8, RGB565(248, 176, 0));
  gfx->fillCircle(240 + (map_scale * 75), 240 - (map_scale * 125), 8, RGB565(248, 176, 0));
}

void setup() {
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("Dragon Radar");

  pinMode(14, INPUT_PULLUP);

  gfx->begin();
  draw_map();

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

  attachInterrupt(14, ISR, FALLING);
}

void loop() {
  if (btn_pressed)
  {
    tone(42, 1600, 100);
    tone(42, 4000, 100);

    draw_map();

    btn_pressed = false;
  }
}
