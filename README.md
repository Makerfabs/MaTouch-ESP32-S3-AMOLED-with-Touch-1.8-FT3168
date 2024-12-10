# MaTouch_ESP32-S3 AMOLED with Touch 1.8 FT3168
```c++
/*
Version:	    V1.0
Author:		    Yuki
Create Date:	2024/12/10
Note:
*/
```

# Makerfabs

[Makerfabs home page](https://www.makerfabs.com/)

[Makerfabs Wiki](https://wiki.makerfabs.com/)

## Intruduce

Product Link: [MaTouch_ESP32-S3 AMOLED with Touch 1.8 FT3168](https://www.makerfabs.com/matouch-esp32-s3-amoled-with-touch-1-8-ft3168.html)

Wiki Link: [MaTouch_ESP32-S3 AMOLED with Touch 1.8 FT3168](https://wiki.makerfabs.com/MaTouch_ESP32_S3_AMOLED_with_Touch_1.8_FT3168.html)


## Features

Specs:
- Controller: ESP32-S3(R8)
- Wireless: WiFi& Bluetooth 5.0
- Resolution: 1.8”, 368*448
- Display Driver IC : SH8601
- Display Interface: SPI 3-wire / QSPI
- Driving Mode: Active Matrix.
- Color Mode: Full Color (16.7M color)
- Touch Driver IC : FT3168
- Touch Interface: IIC [ Slave Addr A[6:0]---0X38]
- Input: USB Type-C 5V(native USB)
- Input Voltage: 3.7V lion battery
- Output: 2X12 1.27mm Header
- Buttons: Reset, Boot and User button
- LED: WS2812
- Arduino support: Yes
- LVGL support: Yes
- High definition display


## Example

### GFX_FT3168_Image

This demo detects the display effect and touch response of the screen, demonstrating the high-definition display effect of the MAOLED screen.

### LVGL_change_background

This demo is based on the functionality of the LVGL buttons, which allows you to change the background color by pressing the buttons.

### LVGL_Animation

This demo is based on the LVGL animation feature. When you touch the ball, it will start bouncing and at the same time the touch coordinates will be displayed on the screen. There is also an adjustable slider for you to use.

### LVGL_T&H

This demo is a temperature and humidity detection system designed by combining Squareline Studio and Arduino. First, we use Squareline to design the UI interface, then output the code to Arduino for programming, then add the DHT11 sensor to display the detected temperature and humidity to the screen.

### Drawing_board 

This demo is a simple drawing board that allows you to draw your patterns in it.


