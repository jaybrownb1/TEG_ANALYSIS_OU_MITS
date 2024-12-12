#include <Adafruit_MAX31865.h>
#include <Arduino.h>
#include <U8x8lib.h>
#include <Wire.h>

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display


// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31865 thermo = Adafruit_MAX31865(8, 10, 9, 6);

// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF       430.0
#define RNOMINAL   100.0

void setup() {
  Serial.begin(115200);
  Serial.println("Adafruit MAX31865 PT100 Sensor Test!");

  thermo.begin(MAX31865_3WIRE); // set to 2WIRE or 4WIRE as necessary

  u8x8.begin();
  u8x8.setFlipMode(1);  // set number from 1 to 3, the screen word will rotary 180
}

void loop() {
  uint16_t rtd = thermo.readRTD();
  float temperature = thermo.temperature(RNOMINAL, RREF);

// Send temperature reading over Serial
  Serial.println(temperature);

// Display temperature on the OLED
  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 32); // Adjust cursor position to center the text vertically
  u8x8.print("Temp: ");
  u8x8.print(temperature);
  u8x8.print(" C");

// Clear the OLED display for the next reading
  u8x8.clear();

// Delay for 1 second between readings
  delay(1000);
}