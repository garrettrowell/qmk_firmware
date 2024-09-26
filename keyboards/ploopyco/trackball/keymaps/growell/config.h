/* Copyright 2020 Christopher Courtney, aka Drashna Jael're  (@drashna) <drashna@live.com>
 * Copyright 2019 Sunjun Kim
 * Copyright 2020 Ploopy Corporation
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

/* disable action features */
//#define NO_ACTION_LAYER
//#define NO_ACTION_TAPPING
//#define NO_ACTION_ONESHOT

#define ROTATIONAL_TRANSFORM_ANGLE 20

// If board has a debug LED, you can enable it by defining this
// #define DEBUG_LED_PIN F7

/* PMW33XX Settings */
#define PMW33XX_CS_PIN             B0
#define POINTING_DEVICE_INVERT_Y


#define ENCODER_BUTTON_COL 1
#define ENCODER_BUTTON_ROW 0
/* Custom encoder needs to specify just how many encoders we have */
#define NUM_ENCODERS 1

#define ENCODER_A_PINS { F0 }
#define ENCODER_B_PINS { F4 }

#define PLOOPY_SCROLL_DEBOUNCE 5
#define PLOOPU_SCROLL_BUTTON_DEBOUNCE 100
#define TAPPING_TERM 175
#define TAPPING_TERM_PER_KEY
#define PLOOPY_DPI_OPTIONS { 400, 600, 800, 1000, 1200, 1600, 2400 }
#define PLOOPY_DPI_DEFAULT 1
#define PLOOPY_DRAGSCROLL_FIXED 1
#define PLOOPY_DRAGSCROLL_INVERT 1
#define PLOOPY_DRAGSCROLL_DPI 100

// Makes debugging take less space. must use `uprint();`
#define NO_DEBUG
