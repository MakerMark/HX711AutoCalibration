/*
  Example using the SparkFun HX711 breakout board with a scale
  By: Nathan Seidle
  SparkFun Electronics
  Date: November 19th, 2014
  License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).

  This is the calibration sketch. Use it to determine the calibration_factor that the main example uses. It also
  outputs the zero_factor useful for projects that have a permanent mass on the scale in between power cycles.

  Setup your scale and start the sketch WITHOUT a weight on the scale
  Once readings are displayed place the weight on the scale
  Press +/- or a/z to adjust the calibration_factor until the output readings match the known weight
  Use this calibration_factor on the example sketch

  This example assumes pounds (lbs). If you prefer kilograms, change the Serial.print(" lbs"); line to kg. The
  calibration factor will be significantly different but it will be linearly related to lbs (1 lbs = 0.453592 kg).

  Your calibration factor may be very positive or very negative. It all depends on the setup of your scale system
  and the direction the sensors deflect from zero state
  This example code uses bogde's excellent library: https://github.com/bogde/HX711
  bogde's library is released under a GNU GENERAL PUBLIC LICENSE
  Arduino pin 2 -> HX711 CLK
  3 -> DOUT
  5V -> VCC
  GND -> GND

  Most any pin on the Arduino Uno will be compatible with DOUT/CLK.

  The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.

*/

#include "HX711.h"

#define DOUT  A2
#define CLK  A3

HX711 scale(DOUT, CLK);

float calibration_factor = -7050; //-7050 worked for my 440lb max scale setup

void setup() {
  Serial.begin(9600);

  scale.set_scale();
  scale.tare(); //Reset the scale to 0

  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);
}

void loop() {
ricalibra:
  boolean esci = false;
  Serial.println("Metti il peso");
  delay(5000);
  for (int i = 0; i < 25000; i = i + 10) {
    scale.set_scale(i); //Adjust to this calibration factor
    delay(10);
    float compareValueFloat = scale.get_units(20) * 100;
    int compareValue = round(compareValueFloat);
    Serial.println(compareValue);
    if (compareValue < 0) {
      Serial.println("ERROE");
      esci = true;
      break;
    }
    if ((compareValue - 500) < 30 && (compareValue - 500) >= 5) {
      Serial.println("+100");
      i += 100;
    }
    if ((compareValue - 500) >= 30 && (compareValue - 500) < 100) {
      Serial.println("+400");
      i += 400;
    }
    if ((compareValue - 500) >= 100 && (compareValue - 500) < 500) {
      Serial.println("+1000");
      i += 1000;
    }
    if ((compareValue - 500) >= 500) {
      Serial.println("+2000");
      i += 2000;
    }

    if (compareValue == 500) {
      calibration_factor = i;
      Serial.print(" calibration_factor: ");
      Serial.print(calibration_factor);
      break;
    }
  }
  if (esci) {
    goto ricalibra;
  }
  Serial.println("Togli il peso");
  delay(5000);
  Serial.println("Tara");
  scale.tare();
  while (true) {
    Serial.print(scale.get_units());
  }
}
