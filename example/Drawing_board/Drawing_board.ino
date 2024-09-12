/*
 * @Description: GFX屏幕显示示例
 * @version: V1.0.0
 * @Author: LILYGO_L
 * @Date: 2023-09-06 10:58:19
 * @LastEditors: LILYGO_L
 * @LastEditTime: 2024-05-29 10:43:59
 * @License: GPL 3.0
 */
#include <Arduino.h>
#include "Arduino_GFX_Library.h"
#include "Arduino_DriveBus_Library.h"
#include "pin_config.h"

Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    LCD_CS /* CS */, LCD_SCLK /* SCK */, LCD_SDIO0 /* SDIO0 */, LCD_SDIO1 /* SDIO1 */,
    LCD_SDIO2 /* SDIO2 */, LCD_SDIO3 /* SDIO3 */);

Arduino_GFX *gfx = new Arduino_SH8601(bus, LCD_RST /* RST */,
                                      0 /* rotation */, false /* IPS */, LCD_WIDTH, LCD_HEIGHT);

std::shared_ptr<Arduino_IIC_DriveBus> IIC_Bus =
    std::make_shared<Arduino_HWIIC>(IIC_SDA, IIC_SCL, &Wire);

void Arduino_IIC_Touch_Interrupt(void);

std::unique_ptr<Arduino_IIC> FT3168(new Arduino_FT3x68(IIC_Bus, FT3168_DEVICE_ADDRESS,
                                                       DRIVEBUS_DEFAULT_VALUE, TP_INT, Arduino_IIC_Touch_Interrupt));

void Arduino_IIC_Touch_Interrupt(void)
{
    FT3168->IIC_Interrupt_Flag = true;
}

void setup()
{
    USBSerial.begin(115200);
    USBSerial.println("1.8 test");

    pinMode(LCD_EN, OUTPUT);
    digitalWrite(LCD_EN, HIGH);

    while (FT3168->begin() == false)
    {
        USBSerial.println("FT3168 initialization fail");
        delay(2000);
    }
    USBSerial.println("FT3168 initialization successfully");

    FT3168->IIC_Write_Device_State(FT3168->Arduino_IIC_Touch::Device::TOUCH_POWER_MODE,
                                   FT3168->Arduino_IIC_Touch::Device_Mode::TOUCH_POWER_MONITOR);

    gfx->begin();
    gfx->fillScreen(WHITE);

    for (int i = 0; i <= 100; i++)  //0-255
    {
        gfx->Display_Brightness(i);
        gfx->setCursor(30, 150);
        gfx->setTextColor(BLUE);
        gfx->setTextSize(4);
        gfx->println("Drawing board");
        delay(3);
    }
    delay(500);
    gfx->fillScreen(WHITE);
    
}

void loop()
{
  int32_t touchX = FT3168->IIC_Read_Device_Value(FT3168->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_X);
  int32_t touchY = FT3168->IIC_Read_Device_Value(FT3168->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_Y);

  if (FT3168->IIC_Interrupt_Flag == true)
  {
      FT3168->IIC_Interrupt_Flag = false;
      USBSerial.printf("Touch X:%d Y:%d\n",touchX,touchY);
      if(touchX>20 && touchY>20){gfx->fillCircle(touchX,touchY,5,BLUE);}
  }
}
