#ifndef PINPU_H
#define PINPU_H

#include <Arduino.h>


class PINPU
{
private:
	 
public:

    void tft_init(void);
    void tft_run(void);
    void ws281x_DrawPoint(uint8_t x, uint8_t y, uint8_t t);
    void ws281x_closeAll(void); // 关闭全部的灯
    void ws281x_Showchar(uint8_t x, uint8_t y, uint8_t chr, uint8_t t, uint8_t size);
    uint32_t mypow(uint8_t m, uint8_t n);
    void ws2812_ShowNum(uint8_t x, uint8_t y, uint64_t num, uint8_t len,uint8_t corol, uint8_t size);
    void ws2812_ShowData(void);
    void ws2812_Showpice(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b);
    void ws281x_ShowTemp(uint8_t x, uint8_t y, uint8_t chr, uint8_t t, uint8_t size);//显示图标

};

#endif