Reuse that old arduino uno tft shield 8 bit parallel

uses a lot more io pins

is faster?

wil try this in the future

modifeid version of the thermostat will be here in the future




<h1>ReUse that old Arduino UNO / Mega Touch TFT Shield</h1>

it is possible to re use that old parallel tft shield

ILI9341 + ESP32 (Parallel) at blazing speeds


<a href="https://www.pangodream.es/ili9341-esp32-parallel" target="spain">https://www.pangodream.es/ili9341-esp32-parallel</a>
#
## looks like in 21 22 are free for i2c ds3231 rtc an bme280

    <a href="https://github.com/Bodmer/TFT_eSPI/issues/448" target="bodmer">8-bit parallel 3.5 TFT LCD Shield + ESP32 -TouchScreen issue #448                        https://github.com/Bodmer/TFT_eSPI/issues/448</a>


sebbe2407 commented 3 days ago

Hi everyone...

i had the same issues with this 3,5" TFT LCD and wiring it to an ESP32 and making the TouchScreen work. 

However i managed to find a solution to the problem. Lets start with the wiring:

#define TFT_CS 33

#define TFT_DC 15

#define TFT_RST 32

#define TFT_WR 4

#define TFT_RD 2


#define TFT_D0 12

#define TFT_D1 13

#define TFT_D2 26

#define TFT_D3 25

#define TFT_D4 17

#define TFT_D5 16

#define TFT_D6 27

#define TFT_D7 14


Next lets focus on the software side. 

In bodmers awesome library you have to comment/uncomment the right sections. 

The User_Setup.h file is pretty straight forward. 

My display used the ILI9488 processor and is run in 8 Bit parallel mode:


#define ILI9488_DRIVER

#define TFT_PARALLEL_8_BIT


When it came to the touchscreen i faced some difficulties. 

Since the display with 8 bit there is no dedicated "Touch Pin" like other displays use.

I decided to use a different library than @Bodmer (no front, still love you and the library <3). 

I used the ADAFRUIT Touchscreen library in extend:


#include <stdint.h>

#include "TouchScreen.h"

The Touchscreen sends us four different values if wired correctly. 

The X-Position, Y-Postition aswell as the pressure applied (XM, YM). 

These values are received via the following pins:


XP = TFT_D0

YP = TFT_CS

XM = TFT_DC

YM = TFT_D1


This did the trick for me and made the touchscreendemo example work for me.

Maybe this helps some people ....

Have a good one ! :) @gvfo5



    
    <a href="https://github.com/Bodmer/TFT_eSPI/issues/539" target="bodmer">8-bit parallel 3.5" TFT LCD ILI9486 does not work #539                                   https://github.com/Bodmer/TFT_eSPI/issues/539</a>

    
<img src="https://www.pangodream.es/wp-content/uploads/2019/04/20190417_181247-768x576.jpg">

<a href="https://www.pangodream.es/ili9341-esp32-parallel" target="spain">https://www.pangodream.es/ili9341-esp32-parallel</a>

<iframe width="640" height="480" src="https://www.youtube.com/embed/dhQjDKtNi58?rel=0" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>


<a href="https://www.pangodream.es/ili9341-esp32-parallel" target="spain">https://www.pangodream.es/ili9341-esp32-parallel</a>


Previet, Spassiba  Андрей Ушаков
<iframe width="640" height="480" src="https://www.youtube.com/embed/56BZ-v4ings?rel=0" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
