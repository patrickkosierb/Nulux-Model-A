#ifndef EDP_H
#define EDP_H

// Library: https://github.com/ZinggJM/GxEPD2

#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <GxEPD2_4C.h>
#include <GxEPD2_7C.h>
#define GxEPD2_DISPLAY_CLASS GxEPD2_BW

// #define GxEPD2_DRIVER_CLASS GxEPD2_420     // GDEW042T2   400x300, UC8176 (IL0398), (WFT042CZ15)
// #define GxEPD2_DRIVER_CLASS GxEPD2_420_M01 // GDEW042M01  400x300, UC8176 (IL0398), (WFT042CZ15)
#define GxEPD2_DRIVER_CLASS GxEPD2_154_GDEY0154D67 // GDEY042T81 400x300, SSD1683 (no inking)
//#define GxEPD2_DRIVER_CLASS GxEPD2_420_GYE042A87  // GYE042A87, 400x300, SSD1683 (HINK-E042-A07-FPC-A1)
//#define GxEPD2_DRIVER_CLASS GxEPD2_420_SE0420NQ04 // SE0420NQ04, 400x300, UC8276C (OPM042A2_V1.0)
// GxEPD2_420_GDEY042T81
// SS is usually used for CS. define here for easy change
#ifndef EPD_CS
#define EPD_CS SS
#endif

#if defined(GxEPD2_DISPLAY_CLASS) && defined(GxEPD2_DRIVER_CLASS)

// somehow there should be an easier way to do this
#define GxEPD2_BW_IS_GxEPD2_BW true
#define IS_GxEPD(c, x) (c##x)
#define IS_GxEPD2_BW(x) IS_GxEPD(GxEPD2_BW_IS_, x)

#if defined(ARDUINO_ARCH_ESP32)
#define MAX_DISPLAY_BUFFER_SIZE 65536ul // e.g.
#if IS_GxEPD2_BW(GxEPD2_DISPLAY_CLASS)
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
#endif

#endif

#endif

#endif