/*
Arduino IDE V2.3.4
esp32 v2.0.16
lvgl v8.3.11
Driver Bus Library v1.0.1

Tools:
Flash size: 16MB(128Mb)
Partition Schrme: 16M Flash(3MB APP/9.9MB FATFS)
PSRAM: OPI PSRAM
*/
#include <lvgl.h>
#include "Arduino_GFX_Library.h"
#include "Arduino_DriveBus_Library.h"
#include "pin_config.h"
#include <ui.h>
#include <WiFi.h>

#define SSID "Makerfabs"
#define PWD "20160704"

static const uint16_t screenWidth  = 368;
static const uint16_t screenHeight = 448;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[ screenWidth * screenHeight / 10 ];

int i=0,j=0,b=200,brightness_flag=0;

Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    LCD_CS /* CS */, LCD_SCLK /* SCK */, LCD_SDIO0 /* SDIO0 */, LCD_SDIO1 /* SDIO1 */,
    LCD_SDIO2 /* SDIO2 */, LCD_SDIO3 /* SDIO3 */);

Arduino_GFX *gfx = new Arduino_SH8601(bus, LCD_RST /* RST */,
                                      0 /* rotation */, false /* IPS */, LCD_WIDTH, LCD_HEIGHT);

// NTP
const char *ntpServer = "120.25.108.11";
int net_flag = 0;

std::shared_ptr<Arduino_IIC_DriveBus> IIC_Bus =
    std::make_shared<Arduino_HWIIC>(IIC_SDA, IIC_SCL, &Wire);

void Arduino_IIC_Touch_Interrupt(void);

std::unique_ptr<Arduino_IIC> FT3168(new Arduino_FT3x68(IIC_Bus, FT3168_DEVICE_ADDRESS,
                                                       DRIVEBUS_DEFAULT_VALUE, TP_INT, Arduino_IIC_Touch_Interrupt));

void Arduino_IIC_Touch_Interrupt(void)
{
    FT3168->IIC_Interrupt_Flag = true;
}

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char * buf)
{
    Serial.printf(buf);
    Serial.flush();
}
#endif

/* Display flushing */
void my_disp_flush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
    uint32_t w = ( area->x2 - area->x1 + 1 );
    uint32_t h = ( area->y2 - area->y1 + 1 );

    #if (LV_COLOR_16_SWAP != 0)
    gfx->draw16bitBeRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
    #else
    gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)&color_p->full, w, h);
    #endif

    lv_disp_flush_ready( disp );
}

/*Read the touchpad*/
void my_touchpad_read( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    int32_t touchX = FT3168->IIC_Read_Device_Value(FT3168->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_X);
    int32_t touchY = FT3168->IIC_Read_Device_Value(FT3168->Arduino_IIC_Touch::Value_Information::TOUCH_COORDINATE_Y);
    
    if( FT3168->IIC_Interrupt_Flag == true)
    {
      FT3168->IIC_Interrupt_Flag = false;
      data->state = LV_INDEV_STATE_PR;
      
      /*Set the coordinates*/
      data->point.x = touchX;
      data->point.y = touchY;

      USBSerial.print( "Data x " );
      USBSerial.print( touchX );

      USBSerial.print( "Data y " );
      USBSerial.println( touchY );

      if(touchY<=90 && touchY>=20){brightness_flag=1;}
    }
    else
    {   
      data->state = LV_INDEV_STATE_REL;
    }
}

void wifi_init()
{
    WiFi.begin(SSID, PWD);

    int connect_count = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        vTaskDelay(500);
        USBSerial.print(".");
        connect_count++;
    }

    USBSerial.println("Wifi connect");
    configTime((const long)(8 * 3600), 0, ntpServer);

    net_flag = 1;
}

long task_runtime_1 = 0;
void Task_my(void *pvParameters)
{
    while (1)
    {

        if (net_flag == 1)
            if ((millis() - task_runtime_1) > 1000)
            {
                display_time();

                task_runtime_1 = millis();
            }

        vTaskDelay(100);
    }
}

void display_time()
{
    struct tm timeinfo;

    if (!getLocalTime(&timeinfo))
    {
        USBSerial.println("Failed to obtain time");
        return;
    }
    else
    {
        int year = timeinfo.tm_year + 1900;
        int month = timeinfo.tm_mon + 1;
        int day = timeinfo.tm_mday;
        int hour = timeinfo.tm_hour;
        int min = timeinfo.tm_min;
        int sec = timeinfo.tm_sec;

        if(min<10){lv_label_set_text_fmt(ui_Label10, "%d:0%d", hour,min);}
        else {lv_label_set_text_fmt(ui_Label10, "%d:%d", hour,min);}
        lv_label_set_text_fmt(ui_Label30, "%d", sec);

    }
}

void Task_TFT(void *pvParameters)
{
    while (1)
    {
        lv_timer_handler(); /* let the GUI do its work */
        delay(5);
        if(brightness_flag==1 && FT3168->IIC_Interrupt_Flag == true)
        {
          delay(100);
          if(brightness_flag==1 && FT3168->IIC_Interrupt_Flag == true)
          {
            brightness_flag=0;
            b=b-50;
            if(b==0){b=200;}
            gfx->Display_Brightness(b);
            lv_label_set_text_fmt(ui_Label29, "%d", b);
          }
        }
        
        i=i+1;
        lv_arc_set_value(ui_Arc1, i);
        if(i>100){i=0;}
    }
}

void setup()
{
    USBSerial.begin( 115200 ); /* prepare for possible serial debug */

    wifi_init();

    pinMode(LCD_EN, OUTPUT);
    digitalWrite(LCD_EN, HIGH);

    while (FT3168->begin() == false)
    {
        USBSerial.println("FT3168 initialization fail");
        delay(2000);
    }
    USBSerial.println("FT3168 initialization successfully");

    gfx->begin();
    gfx->Display_Brightness(b);

    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    USBSerial.println( LVGL_Arduino );
    USBSerial.println( "I am LVGL_Arduino" );

    lv_init();

#if LV_USE_LOG != 0
    lv_log_register_print_cb( my_print ); /* register print function for debugging */
#endif

    FT3168->IIC_Write_Device_State(FT3168->Arduino_IIC_Touch::Device::TOUCH_POWER_MODE,
                                FT3168->Arduino_IIC_Touch::Device_Mode::TOUCH_POWER_MONITOR);


    lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * screenHeight / 10 );

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init( &disp_drv );
    /*Change the following line to your display resolution*/
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.sw_rotate = 1;   // add for rotation
    disp_drv.rotated = LV_DISP_ROT_90;   // add for rotation
    lv_disp_drv_register( &disp_drv );

    /*Initialize the (dummy) input device driver*/
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init( &indev_drv );
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register( &indev_drv );


    ui_init();

    USBSerial.println( "Setup done" );

    xTaskCreatePinnedToCore(Task_TFT, "Task_TFT", 40960, NULL, 3, NULL, 0);
    xTaskCreatePinnedToCore(Task_my, "Task_my", 20000, NULL, 1, NULL, 1);
}

void loop()
{
}
