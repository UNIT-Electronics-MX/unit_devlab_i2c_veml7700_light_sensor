/** @file veml7700_interrupt.ino 
2 *
3 * @brief Code for test the interruption register via I2C connector
4 *
5 * @author Jonathan Mejorado Lopez
6 *
7 * @bug No known bugs.
8 */



#include <Wire.h>
#include <DevLab_VEML7700.h>

#define SDA_PIN 6
#define SCL_PIN 7


DevLab_VEML7700 veml = DevLab_VEML7700();

void setup() {
   //Init serial terminal at 115200 bauds
  Serial.begin(115200);
  //Wait 1 sec, for stabilize the serial
  delay(1000);
  //Init the conection via I2C
  Wire.begin(SDA_PIN,SCL_PIN);
  
  Wire.setClock(400000);
  

  if (!veml.begin()) {
    Serial.println("Sensor not found");
    while (1);
  }
  Serial.println("Sensor found");


  veml.setGain(VEML7700_GAIN_1_8);
  veml.setIntegrationTime(VEML7700_IT_100MS);

  Serial.print(F("Gain: "));
  switch (veml.getGain()) {
    case VEML7700_GAIN_1: Serial.println("1"); break;
    case VEML7700_GAIN_2: Serial.println("2"); break;
    case VEML7700_GAIN_1_4: Serial.println("1/4"); break;
    case VEML7700_GAIN_1_8: Serial.println("1/8"); break;
  }

  Serial.print(F("Integration Time (ms): "));
  switch (veml.getIntegrationTime()) {
    case VEML7700_IT_25MS: Serial.println("25"); break;
    case VEML7700_IT_50MS: Serial.println("50"); break;
    case VEML7700_IT_100MS: Serial.println("100"); break;
    case VEML7700_IT_200MS: Serial.println("200"); break;
    case VEML7700_IT_400MS: Serial.println("400"); break;
    case VEML7700_IT_800MS: Serial.println("800"); break;
  }

  veml.setLowThreshold(5);
  veml.setHighThreshold(100);
  veml.interruptEnable(true);
}

void loop() {
  if(veml.enabled()){
    Serial.print("raw ALS: "); Serial.println(veml.readALS());
    Serial.print("raw white: "); Serial.println(veml.readWhite());
    Serial.print("lux: "); Serial.println(veml.readLux());

    uint16_t irq = veml.interruptStatus();
    if (irq & VEML7700_INTERRUPT_LOW) {
      Serial.println("** Low threshold");
    }
    if (irq & VEML7700_INTERRUPT_HIGH) {
    Serial.println("** High threshold");
    }
  }else{
    Serial.println("No esta habilitado el sensor");
  }

  delay(500);
}