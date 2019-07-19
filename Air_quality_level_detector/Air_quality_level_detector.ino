/*
 * Air_quality_level_detector.ino
 * 
 * 
 * Copyright (c) 2019 Seeed Technology Co., Ltd.
 * Website    : www.seeed.cc
 * Author     : HansonCc
 * Create Time: February 2019
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "Seeed_HM330X.h"
#include "Adafruit_NeoPixel.h"

#ifdef  ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define SERIAL SerialUSB
#else
  #define SERIAL Serial
#endif
#define PIN            6
#define NUMPIXELS      20
#define NOMAL_LEVEL    8
#define WARNNING_LEVEL 12
#define DANGER_LEVEL   15
char Numpixels = 0;
char Now_Numpixels = 0;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

HM330X sensor;
u8 buf[30];
u8 PM_Base_Date;

int delayval = 500; // delay for half a second

const char *str[]={"sensor num: ","PM1.0 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
                    "PM2.5 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
                    "PM10 concentration(CF=1,Standard particulate matter,unit:ug/m3): ",
                    "PM1.0 concentration(Atmospheric environment,unit:ug/m3): ",
                    "PM2.5 concentration(Atmospheric environment,unit:ug/m3): ",
                    "PM10 concentration(Atmospheric environment,unit:ug/m3): ",
                    };

err_t print_result(const char* str,u16 value)
{
    if(NULL==str)
        return ERROR_PARAM;
    SERIAL.print(str);
    SERIAL.println(value);
    return NO_ERROR;
}

/*parse buf with 29 u8-data*/
err_t parse_result(u8 *data)
{
    u16 value=0;
    err_t NO_ERROR;
    if(NULL==data)
        return ERROR_PARAM;
    for(int i=1;i<8;i++)
    {
         value = (u16)data[i*2]<<8|data[i*2+1];
         if(value > 200) value = 200; 
         print_result(str[i-1],value);
    }
}

err_t parse_result_value(u8 *data)
{
    if(NULL==data)
        return ERROR_PARAM;
    for(int i=0;i<28;i++)
    {
        SERIAL.print(data[i],HEX);
        SERIAL.print("  ");
        if((0==(i)%5)||(0==i))
        {
            SERIAL.println(" ");
        }
    }
    u8 sum=0;
    for(int i=0;i<28;i++)
    {
        sum+=data[i];
    }
    if(sum!=data[28])
    {
        SERIAL.println("wrong checkSum!!!!");
    }
    SERIAL.println(" ");
    SERIAL.println(" ");
    return NO_ERROR;
}


/*30s*/
void setup()
{
    SERIAL.begin(115200);
    delay(100);
    SERIAL.println("Serial start");
    if(sensor.init())
    {
        SERIAL.println("HM330X init failed!!!");
        while(1);
    }
    delay(1000);
    if(sensor.read_sensor_value(buf,29))
    {
        SERIAL.println("HM330X read result failed!!!");
    }
    PM_Base_Date = buf[13];   //PM2.5 environment value
    SERIAL.println("PM_Base_Date:");
    SERIAL.println(PM_Base_Date);
    pixels.setBrightness(150);
    pixels.begin(); // This initializes the NeoPixel library.
    for(int i=0;i<20;i++)
    {
      pixels.Color(0,0,0);
      pixels.show(); // This sends the updated pixel color to the hardware.
      delay(50); // Delay for a period of time (in milliseconds).
  }    
}


void loop()
{
  
  if(sensor.read_sensor_value(buf,29))
  {
      SERIAL.println("HM330X read result failed!!!");
  }

  if(SERIAL.available())
  {
    Now_Numpixels = SERIAL.read()-48;
    //Now_Numpixels = SERIAL.read()-48;
    SERIAL.print(Now_Numpixels);
    SERIAL.print(" ");
  }
  if(buf[13] > 70) buf[13] = 70;
  Now_Numpixels = buf[13]-PM_Base_Date;  
  parse_result_value(buf);
  parse_result(buf);
  SERIAL.println(" ");
  SERIAL.println(" ");
  SERIAL.println(" ");  
  SERIAL.println("PM_Base_Date:");
  SERIAL.println(PM_Base_Date);
    
  if(Now_Numpixels < 0) Now_Numpixels = 0;
  else if(Now_Numpixels > DANGER_LEVEL) Now_Numpixels = DANGER_LEVEL;
  
  if((float)(Now_Numpixels - Numpixels) > 0)
  {
        for(int i=0;i<Now_Numpixels;i++)
        {
        
        // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
        if(i < NOMAL_LEVEL) pixels.setPixelColor(i, pixels.Color(0,150,0)); // Moderately bright green color.
        else if(i < WARNNING_LEVEL) pixels.setPixelColor(i, pixels.Color(150,150,0));
        else if(i < DANGER_LEVEL)pixels.setPixelColor(i, pixels.Color(150,0,0));
        pixels.show(); // This sends the updated pixel color to the hardware.
        delay(50); // Delay for a period of time (in milliseconds).
        
      }
  }
  else if ((float)(Now_Numpixels - Numpixels) < 0)
  {
      for(int i=Numpixels;i>=Now_Numpixels;i--)
      {        
          pixels.setPixelColor(i, pixels.Color(0,0,0)); // Moderately bright green color.
          pixels.show(); // This sends the updated pixel color to the hardware.
          delay(50); // Delay for a period of time (in milliseconds). 
      }
  }
  Numpixels = Now_Numpixels;
  delay(3000);  
   
}
