#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

int sensorValue = 0;
enum
{
  BufSize = 4
};
char buf[BufSize];

void setup()
{
  Serial.begin(9600);
  Serial.print("Starting");
  u8g2.begin();
  u8g2.setFont(u8g2_font_logisoso32_tf);
}

void loop()
{
  sensorValue = analogRead(A1);
  snprintf(buf, BufSize, "%d", sensorValue);
  u8g2.firstPage();
  do
  {
    u8g2.drawStr(0, 32, buf);
  } while (u8g2.nextPage());
  delay(1);
}