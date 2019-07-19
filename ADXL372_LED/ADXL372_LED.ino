
#include "Wire.h"
#include "adxl372.h"

#define BUFFER_SIZE   (160*6)

ADXL372 acc;
xyz_t xyz_acc;

uint8_t buffer[BUFFER_SIZE] = {0,};

void setup() {
  Serial.begin(115200);

  acc.begin();
  
  Serial.println(acc.id(), HEX);
  acc.timing_ctrl(RATE_400);
  acc.measurement_ctrl(BW_200, true);
  acc.fifo_ctrl(STREAMED, FIFO_XYZ);
  acc.power_ctrl(MEASUREMENT_MODE);

  Serial.println("unit 100 mg");
}

void loop() {
  uint16_t j;
  uint16_t samples = acc.samples_in_fifo();
  // To ensure that data is not overwritten and stored out of order, 
  // at least one sample set must be left in the FIFO after every read        
  if (samples > 12) {
    samples = (samples > BUFFER_SIZE)? BUFFER_SIZE : (samples / 6 - 1) * 6;

    acc.fifo_read(buffer, samples);
    for (j=0; j<samples; j+=6) {
      // convert raw data
      xyz_t *xyz = acc.format(buffer + j);
      Serial.print(samples);
      Serial.print('\t');
      Serial.print(xyz->x);
      xyz_acc.x = xyz->x + xyz_acc.x;
      Serial.print('\t');
      Serial.print(xyz->y);
      xyz_acc.y = xyz->y + xyz_acc.y;      
      Serial.print('\t');
      Serial.println(xyz->z);
      xyz_acc.z = xyz->z + xyz_acc.z;
    }
     xyz_acc.x = xyz_acc.x * 6.0 / j;
     Serial.println(xyz_acc.x);
     xyz_acc.y = xyz_acc.y * 6.0 / j;
     Serial.println(xyz_acc.y);
     xyz_acc.z = xyz_acc.z * 6.0 / j;
     Serial.println(xyz_acc.z);
  }

  delay(1000);
}
