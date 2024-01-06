#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


 
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_VEML7700.h"

#define SEALEVELPRESSURE_HPA (1013.25)

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_BME280 bme; // I2C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_VEML7700 veml = Adafruit_VEML7700();
int buzzerPin = 6;
int color = true;


void setup() {
    Serial.begin(9600);
    // while(!Serial);    // time to get serial running

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println(F("SSD1306 allocation failed"));
    }
    unsigned status;
    
    // default settings
    // status = bme.begin();  
    // You can also pass in a Wire library object like &Wire2
    Wire.begin();
    status = bme.begin(0x76, &Wire);
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }

    Serial.println();

    Serial.println("starting VEML");
    if (!veml.begin()) {
      Serial.println("VEML not found");
      while (1);
    }
    Serial.println("VEML found");
    veml.setLowThreshold(10000);
    veml.setHighThreshold(20000);
    veml.interruptEnable(true);

    // buzzer
    pinMode(buzzerPin, OUTPUT);
}


void printValues() {
    display.invertDisplay(color);
    // color = !color;
    // print temp

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);

    display.print("Temp = ");
    display.print(bme.readTemperature());
    display.println(" C");

    display.display(); 
    delay(2000);

    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);

    display.print("Pressure = ");
    display.print(bme.readPressure() / 100.0F);
    display.println(" hPa");

    display.display(); 
    delay(2000);


    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);

    display.print("Humidity = ");
    display.print(bme.readHumidity());
    display.println(" percent");
    display.println();

    display.display(); 
    delay(2000);


    // print light sensor
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);

    display.print("lux: "); 
    display.println(veml.readLux());
    if (veml.readLux() > 300) {
      color = true;
    }
    else color = false;

    display.display(); 
    delay(2000);

    // display
    // display.clearDisplay();
    // display.setTextSize(2);
    // display.setTextColor(WHITE);
    // display.setCursor(0, 10);
    // // Display static text
    // display.println("Hello, world, too!");
    // display.display(); 


    // delay(1000);

    // buzzer 
    tone(buzzerPin, 1000);
    delay(2000);
    noTone(buzzerPin);
    Serial.println("One cycle finished!");

}


void loop() { 
    printValues();

    delay(1000);
    // Serial.println(veml.readLux());
    // delay(100);
}
