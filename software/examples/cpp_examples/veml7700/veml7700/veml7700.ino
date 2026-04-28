/** @file veml7700.ino 
2 *
3 * @brief Code example for VEML7700 UNIT Module Light Sensor
4 *
5 * @author Jonathan Mejorado Lopez
6 *
7 * @bug No known bugs.
8 */

#include "Adafruit_VEML7700.h"


#include <Wire.h>
#include <Adafruit_VEML7700.h>


//Pin definitions for I2C Settings
#define SDA_PIN 6
#define SCL_PIN 7

//Object construct for VEML7700
Adafruit_VEML7700 veml = Adafruit_VEML7700();


/******************************************************************************
* TYPEDEFS
*****************************************************************************/

/* Config struct for gain settings of the sensor*/
typedef struct{
  uint8_t reg;
  const char* label;
}gains_cfg_t;

/* Config struct for integration time settings of the sensor*/
typedef struct{
  uint8_t reg;
  uint16_t time_ms;
  const char* label;

}timings_cfg_t;

/* Config struct for lux mode settings of the sensor*/
typedef struct{
  luxMethod mode;
  const char* label;
}lux_cfg_t;

/* Register | Delay | Label for integration timings*/
const timings_cfg_t timings[] = {                                                   //Integration Time Register, Time use for delay , Label for print
  {VEML7700_IT_25MS,  25,  "25"},                                                   //timings[0].reg,timings[0].time_ms,timings[0].label                                  
  {VEML7700_IT_50MS,  50,  "50"},                                                   //timings[1].reg,timings[1].time_ms,timings[1].label        
  {VEML7700_IT_100MS, 100, "100"},                                                  //timings[2].reg,timings[2].time_ms,timings[2].label        
  {VEML7700_IT_200MS, 200, "200"},                                                  //timings[3].reg,timings[3].time_ms,timings[3].label        
  {VEML7700_IT_400MS, 400, "400"},                                                  //timings[4].reg,timings[4].time_ms,timings[4].label        
  {VEML7700_IT_800MS, 800, "800"}                                                   //timings[5].reg,timings[5].time_ms,timings[5].label        
};

/* Register | Label for Gains*/
const gains_cfg_t gains[] = {                                                       //Gain Register, Label for print
  {VEML7700_GAIN_1_4, "1/4"},                                                       //gains[0].reg,luxs[0].label                                           
  {VEML7700_GAIN_1_8,"1/8"},                                                        //gains[1].reg,luxs[1].label    
  {VEML7700_GAIN_1, "1"},                                                           //gains[2].reg,luxs[2].label  
  {VEML7700_GAIN_2, "2"}                                                            //gains[3].reg,luxs[3].label  
};

/* Register | Label for Lux Modes*/
const lux_cfg_t luxs[] = {                                                          //Lux Mode Register , Label for print
  {VEML_LUX_NORMAL,"NORMAL"},                                                       //luxs[0].mode,luxs[0].label
  {VEML_LUX_AUTO,"AUTO"},                                                           //luxs[1].mode,luxs[1].label
  {VEML_LUX_NORMAL_NOWAIT, "NORMAL_NOWAIT"},                                        //luxs[2].mode,luxs[2].label
  {VEML_LUX_CORRECTED,"CORRECTED"},                                                 //luxs[3].mode,luxs[3].label
  {VEML_LUX_CORRECTED_NOWAIT,"CORRECTED_NOWAIT"}                                    //luxs[4].mode,luxs[4].label
};

/**
* @brief Setting up for the main characteristics of the sensor Gain and Integration Time
*
* @param[in] gain. Gain setting for the sensor
* @param[in] integrationTime Integration time for light captation of the sensor
*/

void configDevice(uint8_t gain,uint8_t integrationTime){
  veml.setGain(gain);
  veml.setIntegrationTime(integrationTime);


}

/**
* @brief Read Complete of the three variables availables for sensing with the IC
*
* @param[in] luxMethod Lux method comptation for captating the light
*/
void advancedRead(luxMethod luxs){
  uint16_t raw = veml.readALS();
  uint16_t white = veml.readWhite();
  float lux = veml.readLux(luxs);

  Serial.print("Raw Data: ");
  Serial.print(raw);
  Serial.print(" ; ");
  Serial.print("White Data: ");
  Serial.print(white);
  Serial.print(" ; ");
  Serial.print("Lux: ");
  Serial.print(lux);
  Serial.println("");

}

void setup() {

//Init serial terminal at 115200 bauds
  Serial.begin(115200);
  //Wait 1 sec, for stabilize the serial
  delay(1000);
  //Init the conection via I2C
  Wire.begin(SDA_PIN,SCL_PIN);
  
  Wire.setClock(400000);

  //Init the sensor for measure values
  if (!veml.begin()) {
    Serial.println("Sensor not found");
    while (1);
  }
  Serial.println("Sensor found");

  configDevice(gains[0].reg,timings[2].reg);

}

void loop() {
  
  if(veml.enabled()){
    //Read and print all the values that the module is capable of measure(Comment this line if you use the single options)
    advancedRead(luxs[0].mode);

    //Read and print raw data (Uncomment two after lines for single lecture)
    //uint16_t white = veml.readWhite();
    //Serial.printf("White Data measure:  %" PRIu16 "\n",white);

    //Read and print raw data (Uncomment two after lines for single lecture)
    //uint16_t raw = veml.readALS();
    //Serial.printf("Raw Data measure:  %" PRIu16 "\n",raw);

    //Read and print lux data (Uncomment two after lines for single lecture)
    //float lux = veml.readLux(luxs[0].mode);
    //Serial.printf("Lux Data measure:  %.2f \n",lux);

    //The delay must be the same index config in the function timings[].reg + 20 ms for stabilize the reading of the sensor
    delay(timings[2].time_ms + 20);

  }else{
    Serial.printf("The sensor is not ready or enabled");
  }
}
