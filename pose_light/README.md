# posture lamp

最近的睡眠不是很好，发现主要问题在于每次困了，一起身去关灯困意就完全没有了。于是，我去互联网上浏览了一下，最终得到了一个非常棒的创意。我准备制作一个可以定时的姿势灯,我就可以不需做任何动作就能关闭灯。为了避免局限性，所以还需要设计一个不依赖定时就能马上关闭灯的功能。但是，我还是感觉功能还是不够，最后我准备设计出一个类似于手机摇一摇的控制，当然我设想的功能只是摇一摇灯就能改变灯的颜色。为了使外型简单我准备不用按键去控制灯的定时以及开关功能，直接采用传感的方式来控制，比如我想定时5分钟，那么我将灯以桌子和灯的接触点为原点顺时针旋转90°。假如我想关闭姿势灯,那么我将以桌子和正常摆放的姿势灯的接触点为原点顺时针旋转180°。当然，变换颜色就很简单只需要摇一摇姿势灯就能变换颜色了。

recently, My sleep is not very good, When I wake up and turn off the lamp, it will relieve my fatigue.of course, it make me exciting and can not sleep. therefore,I need some way to resolve this problem. I went to the Internet to browse and finally got a great idea.I am going to make a posture lamp that can be turned off regularly, because then I can turn off the lamp without any action.

In order to avoid limitations，So you also need to design a function that turns off the lamp immediately without relying on timing. But, I don't think the function is enough. I am going to design a control that is similar to a mobile phone shake. Of course, the function I envisioned is just to change the color of the lamp by shaking the lamp. In order to make the appearance simple, I am ready to control the timing and switching function of the lamp without using the button, and directly use the sensing method to control. For example, I want to time 5 minutes.
Then I rotate the lamp 90° clockwise with the contact point of the table and the lamp as the origin. If I want to turn off the lamp,then I rotate the lamp 180° clockwise with the contact point of the table and the lamp as the origin. Of course, It is very easy to change color. You just need to shake lamp.

## 硬件构成

### Seeeduino Nano

主控制选择的是[Seeeduino Nano](https://www.seeedstudio.com/Seeeduino-Nano-p-4111.html) 之所以选择它就是因为它小而且便宜。当然，为了更方便接线，直接购买[Grove Shield for Arduino Nano](https://www.seeedstudio.com/Grove-Shield-for-Arduino-Nano-p-4112.html)也是一个很好的选择。

controller select  [Seeeduino Nano](https://www.seeedstudio.com/Seeeduino-Nano-p-4111.html). Because it is very cheap and small. Of Course, For easy connection [Grove Shield for Arduino Nano](https://www.seeedstudio.com/Grove-Shield-for-Arduino-Nano-p-4112.html) is also a good choice.

![](https://github.com/hansonCc/Arduino/raw/master/pose_light/DOC/Grove%20Shield%20for%20Arduino%20Nano.png)

### Grove - 3-Axis Digital Accelerometer ±200g (ADXL372)

[Grove - 3-Axis Digital Accelerometer ±200g (ADXL372)](https://www.seeedstudio.com/Grove-3-Axis-Digital-Accelerometer-200g-ADXL372-p-4003.html)is a ultra low power digital output MEMS Accelerometer, it can provide a 12-bit output at 100 mg/LSB scale factor. The most notable feature of this sensor is its ultra-low power consumption(only 22μA in measurement mode) and large measurement range(±200g). All the data output via the Grove I2C port, the I2C address is changeable. In order to meet a wider range of measurement needs, the sampling rate can be selected from 400Hz/800Hz/1600Hz/3200Hz/6400Hz, and the bandwidth can be selected from 200Hz/400Hz/800Hz/1600Hz/3200Hz. In addition to being used as an acceleration measurement, you can also use this module to do impact and shock detection。So,It is suitable for this project.

![](https://github.com/hansonCc/Arduino/raw/master/pose_light/DOC/3-Axis%20Digital%20Accelerometer.png)

### Grove - RGB LED Ring (20 - WS2813 Mini)

为了达到良好的灯效，我选择了[The Grove - RGB LED Ring (20 - WS2813 Mini)](http://wiki.seeedstudio.com/Grove-LED_ring)，因为每个LED都有一个恒定电流驱动器，因此即使电压发生变化，颜色也会非常一致。这样灯效就会变的很合适。

In order to achieve good lighting efficiency, [The Grove - RGB LED Ring (20 - WS2813 Mini)](http://wiki.seeedstudio.com/Grove-LED_ring) is good choice, Because each LED has a constant current driver, the color will be very consistent even if the voltage changes. This will make the lighting effect very suitable.

![](https://github.com/hansonCc/Arduino/raw/master/pose_light/DOC/Grove%20-%20RGB%20LED%20Ring.jpg)

### Li-po Rider

Seeeduino Nano 支持5V供电 ，但是为了使姿势灯方便携带显然一直使用电脑USB供电是一个不太合适的选择，这里我们选用[Li-po Rider](https://www.seeedstudio.com/Li-po-Rider-p-710.html)来解决这个问题。

Seeeduino Nano support 5v, However, in order to make the lamp easy to carry, it is obviously not a suitable choice to always use the computer USB power supply.we use [Li-po Rider](https://www.seeedstudio.com/Li-po-Rider-p-710.html) to solve this problem.

![](https://github.com/hansonCc/Arduino/raw/master/pose_light/DOC/Li-po%20Rider.jpg)

## 软件设计

### Grove - 3-Axis Digital Accelerometer ±200g (ADXL372)

Grove - 3-Axis Digital Accelerometer ±200g 有两种模式一种是普通读写模式，还以一种是FIFO读写模式。非常感谢seeed提供的[wiki](http://wiki.seeedstudio.com/Grove-3-Axis_Digital_Accelerometer_200g-ADXL372/)，因为里面很完美的库文件，使我开发这个小设计节省不少时间。最终我选择fifo模式输出一组数据然后对其求平均数。

Grove - 3-Axis Digital Accelerometer ±200g have two reads and write mode,the one is normal reads and write mode ,the other one is FIFO reads and write mode. Thank for Seeed's [wiki](http://wiki.seeedstudio.com/Grove-3-Axis_Digital_Accelerometer_200g-ADXL372/), Because it is a perfect library file inside, it saves me a lot of time to develop this small design. Finally I chose the fifo mode to output a set of data and then average it.

```c
  uint16_t samples = acc.samples_in_fifo();
  // To ensure that data is not overwritten and stored out of order,
  // at least one sample set must be left in the FIFO after every read
  if (samples > 12) {
    samples = (samples > BUFFER_SIZE)? BUFFER_SIZE : (samples / 6 - 1) * 6;
    acc.fifo_read(buffer, samples);
    for (j=0; j<samples; j+=6) {
      // convert raw data
      xyz_t *xyz = acc.format(buffer + j);
      xyz_acc.x = xyz->x + xyz_acc.x;
      xyz_acc.y = xyz->y + xyz_acc.y;
      xyz_acc.z = xyz->z + xyz_acc.z;
    }
     xyz_acc.x = xyz_acc.x * 6.0 / j;
     xyz_acc.y = xyz_acc.y * 6.0 / j;
     xyz_acc.z = xyz_acc.z * 6.0 / j;
```

### Grove - RGB LED Ring (20 - WS2813 Mini)

关于Grove - RGB LED Ring (20 - WS2813 Mini)我只是简单写了一个控制函数，控制函数里面调用的代码我都是参考[wiki](http://wiki.seeedstudio.com/Grove-LED_ring)上面内容，这是我写的控制函数的代码

About Grove - RGB LED Ring (20 - WS2813 Mini) I just wrote a control function, the code called in the control function I refer to [wiki](http://wiki.seeedstudio.com/Grove-LED_ring) Content, this is the code of the control function I wrote

```c
void Led_Control(uint32_t Led_num,uint32_t Color,bool Switch)
{
    for(int i=0; i<Led_num; i++)
    {
      if(Switch)
      {
        strip.setPixelColor(i, Color);
        strip.show();
      }  
      else
      {
        strip.setPixelColor(i, Off_All);
        strip.show();
      }
    }
}
```

## 产品整合

最终经过半天的时间我搭建好整体硬件连接，硬件的整体连接如下图：

In the end, after half a day, I built the overall hardware connection. The overall connection of the hardware is as follows:

![](https://github.com/hansonCc/Arduino/raw/master/pose_light/DOC/Overall%20connection%20diagram.jpg)

这是一个非常需要想象力的事情，显然我对3D外壳设计就显得更加的理性化一点，所以还请为各位对于3D外壳的设计少做一些评价。


## 效果展示

