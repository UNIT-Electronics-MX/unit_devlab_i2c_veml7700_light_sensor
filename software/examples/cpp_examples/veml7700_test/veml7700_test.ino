/** @file veml7700_test.ino 
2 *
3 * @brief Example code for test all the parameters included in the module sensor, with variations of gain, integration time and lux mode captation.
4 *
5 * @author Jonathan Mejorado Lopez
6 *
7 * @bug No known bugs.
8 */


/******************************************************************************
* INCLUDES
*****************************************************************************/
#include <Wire.h>
#include <DevLab_VEML7700.h>

/******************************************************************************
* MACROS AND DEFINES
*****************************************************************************/

//Construct veml object
DevLab_VEML7700 veml = DevLab_VEML7700();

#define SDA_PIN 6
#define SCL_PIN 7


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
const timings_cfg_t timings[] = {
  {VEML7700_IT_25MS,  25,  "25"},
  {VEML7700_IT_50MS,  50,  "50"},
  {VEML7700_IT_100MS, 100, "100"},
  {VEML7700_IT_200MS, 200, "200"},
  {VEML7700_IT_400MS, 400, "400"},
  {VEML7700_IT_800MS, 800, "800"}
};

/* Register | Label for Gains*/
const gains_cfg_t gains[] = {
  {VEML7700_GAIN_1_4, "1/4"},
  {VEML7700_GAIN_1_8,"1/8"},
  {VEML7700_GAIN_1, "1"},
  {VEML7700_GAIN_2, "2"}
};

/* Register | Label for Lux Modes*/
const lux_cfg_t luxs[] = {
  {VEML_LUX_NORMAL,"NORMAL"},
  {VEML_LUX_AUTO,"AUTO"},
  {VEML_LUX_NORMAL_NOWAIT, "NORMAL_NOWAIT"},
  {VEML_LUX_CORRECTED,"CORRECTED"},
  {VEML_LUX_CORRECTED_NOWAIT,"CORRECTED_NOWAIT"}
};



/******************************************************************************
* PUBLIC FUNCTIONS
*****************************************************************************/

/**
* @brief This function scan all devices in the I2C bus, and print if can find a device 
*/
void scanI2CDevices(){
  Serial.println("Escaneando bus I2C...");
  for (uint8_t addr = 1; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if (Wire.endTransmission() == 0) {
      Serial.print("✅ Dispositivo encontrado en: 0x");
      Serial.println(addr, HEX);
    }
  }
}



/**
* @brief This function contain 3 nested fors for sweep all available settings of the sensor: gains | integration time | lux
*/
void sweepMeasurements(){
  
  for(int i = 0; i < 4; i++){
     veml.setGain(gains[i].reg);
    for(int j = 0; j < 6; j++){
      veml.setIntegrationTime(timings[j].reg);
        
        delay(timings[j].time_ms + 20);
      for(int k = 0; k < 5; k++){
        Serial.print(gains[i].label);
        Serial.print(";");
        Serial.print(timings[j].label);
        Serial.print(";");
        Serial.print(luxs[k].label);
        Serial.print(";");
        printResults(luxs[k].mode);
      }
      
    }
  }

}


/**
* @brief Performs the selected operation with two integers
*
* @param[in] luxMethod. Register for setting the Lux comptation method to use 
*/
void printResults(luxMethod luxs){
  uint16_t raw = veml.readALS();
  uint16_t white = veml.readWhite();
  float lux = veml.readLux(luxs);
  

  Serial.print(millis());
  Serial.print(";");
  //Serial.print(tsl.getGain());
  //Serial.print(";");
  //Serial.print(tsl.getTiming());
  //Serial.print(";");
  Serial.print(raw);
  Serial.print(";");
  Serial.print(white);
  Serial.print(";");
  Serial.print(lux);
  Serial.println("");
}

void setup() {
  // put your setup code here, to run once:
  //Init serial terminal at 115200 bauds
  Serial.begin(115200);
  //Wait 1 sec, for stabilize the serial
  delay(1000);
  //Init the conection via I2C
  Wire.begin(SDA_PIN,SCL_PIN);
  
  Wire.setClock(400000);
  //Scan all the devices conected to the I2C bus for check the device working
  scanI2CDevices();

  //Init the sensor for measure values
  if (!veml.begin()) {
    Serial.println("Sensor not found");
    while (1);
  }
  Serial.println("Sensor found");
  //readDeviceID();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(veml.enabled()){
    //Sweepmeasurements
    sweepMeasurements();
  }else{
    Serial.println("No esta habilitado el sensor");
  }

}
