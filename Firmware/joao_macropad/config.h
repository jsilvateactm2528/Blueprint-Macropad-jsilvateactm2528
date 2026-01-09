#pragma once

#include "config_common.h"

/* Matrix size */
#define MATRIX_ROWS 3
#define MATRIX_COLS 3

/* Matrix Pins based on your schematic */
#define MATRIX_ROW_PINS { GP26, GP27, GP28 }
#define MATRIX_COL_PINS { GP29, GP1, GP7 }

#define DIODE_DIRECTION COL2ROW

/* Encoder Rotation Pins */
#define ENCODERS_PAD_A { GP6, GP4 }
#define ENCODERS_PAD_B { GP3, GP2 }
#define ENCODER_RESOLUTION 4

/* RGB LED Configuration (GP0 / Pin 7) */
#define RGB_DI_PIN GP0
#define RGBLED_NUM 7
#define RGBLIGHT_HUE_STEP 8
#define RGBLIGHT_SAT_STEP 8
#define RGBLIGHT_VAL_STEP 8

/* OLED Brightness */
#define OLED_BRIGHTNESS 128