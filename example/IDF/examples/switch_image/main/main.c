#include <stdio.h>

void app_main(void)
{
    extern void board_init(void);
    board_init();

    extern void market_demo(void);
    market_demo();
}