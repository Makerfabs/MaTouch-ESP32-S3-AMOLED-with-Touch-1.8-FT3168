#include <esp_log.h>


#include "lcd_display.h"
#include "esp_lvgl_port.h"

static const char *TAG = "lcd_display";

lv_display_t *spi_lcd_display(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_handle_t panel,
                           int width, int height, int offset_x, int offset_y,
                           bool mirror_x, bool mirror_y, bool swap_xy)
{
    uint16_t *buffer = malloc(width * sizeof(uint16_t));
    if(buffer == NULL) {
        ESP_LOGE(TAG, "No memory for LCD buffer");
        return NULL;
    }
    for(int y = 0; y < height; y++) {
        esp_lcd_panel_draw_bitmap(panel, 0, y, width, y + 1, buffer);
    }
    free(buffer);

    ESP_LOGI(TAG, "Initialize LVGL library");
    lv_init();

    ESP_LOGI(TAG, "Initialize LVGL port");
    lvgl_port_cfg_t port_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    port_cfg.task_priority = 1;
#if CONFIG_SOC_CPU_CORES_NUM > 1
    port_cfg.task_affinity = 1;
#endif
    lvgl_port_init(&port_cfg);

    ESP_LOGI(TAG, "Add display screen");
    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = panel_io,
        .panel_handle = panel,
        .control_handle = NULL,
        .buffer_size = height / 10 * width, // Buffer for 10 lines
        .double_buffer = 0,
        .hres = (uint32_t)width,
        .vres = (uint32_t)height,
        .monochrome = false,
        /* Rotation values must be same as used in esp_lcd for initial settings of the screen */
        .rotation = {
            .swap_xy = swap_xy,
            .mirror_x = mirror_x,
            .mirror_y = mirror_y,
        },
        .color_format = LV_COLOR_FORMAT_RGB565,
        .flags = {
            .buff_dma = 1,
            .buff_spiram = 0,
            .sw_rotate = 0,
            .swap_bytes = 1,
            .full_refresh = 0,
            .direct_mode = 0,
        }
    };
    lv_display_t *display = lvgl_port_add_disp(&disp_cfg);

    if(offset_x != 0 || offset_y != 0) {
        lv_display_set_offset(display, offset_x, offset_y);
    }

    return display;
}


