#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

// raw sensor value
float rawValue = 0.0;

// smoothing the value 0 - 1 range
// larger number, faster change but more noise
const float smoothFilter = 0.5;
float smoothedValue = 0;

// mapped value
// arduino 10bit adc has a step of 0.0049
// the sensor gives values between 1v and 4.2v
// which maps to -1200 and 1200
const long minValue = 1/0.0049;
const long maxValue = 4.2/0.0049;
const long minMapped = -1200;
const long maxMapped = 1200;
float mappedValue = 0.0;

// buffer for printing the values out
const int BufSize = 6;
char buf[BufSize];

void setup()
{
  u8g2.begin();
  // initial reading for getting the startup smoothness out
  smoothedValue = analogRead(A1);
}

void loop()
{
  // read the value
  rawValue = analogRead(A1);
  smoothedValue += (rawValue - smoothedValue) * smoothFilter;
  mappedValue = map(smoothedValue, minValue, maxValue, minMapped, maxMapped);

  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_logisoso32_tf);
    snprintf(buf, BufSize, "%d", (int)mappedValue);
    u8g2.drawStr(0, 32, buf);

    // raw value
    u8g2.setFont(u8g2_font_5x8_mn);
    snprintf(buf, BufSize, "%d", (int)rawValue);
    u8g2.drawStr(110, 8, buf);

    // North / South guess
    if (mappedValue > 150)
    {
      u8g2.setFont(u8g2_font_profont29_mf);
      u8g2.drawStr(110, 32, "S");
    }
    if (mappedValue < -150)
    {
      u8g2.setFont(u8g2_font_profont29_mf);
      u8g2.drawStr(110, 32, "N");
    }
  } while (u8g2.nextPage());

}