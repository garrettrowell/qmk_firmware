# 'growell' Keymap

## Layers

```
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     [0] = LAYOUT( KC_BTN1, TD(MSE_BTN2), TD(MSE_BTN3), KC_BTN4, KC_BTN2),
     [1] = LAYOUT( _______, _______, _______, DPI_INC, DPI_DEC),
     [2] = LAYOUT( _______, _______, _______, DS_V_INC, DS_V_DEC),
     [3] = LAYOUT( _______, _______, _______, DS_H_INC, DS_H_DEC),
};
```

## Custom Keycodes

|Keycode|Description|
|---|---|
|DPI_INC|Increment the DPI config to use|
|DPI_DEC|Decrement the DPI config to use|
|DS_V_INC|Increment the DragScroll V Divisor to use|
|DS_V_DEC|Decrement the DragScroll V Divisor to use|
|DS_H_INC|Increment the DragScroll H Divisor to use|
|DS_H_DEC|Decrement the DragScroll H Divisor to use|

## Tapdance

### Keycodes

|Keycode|Description|
|---|---|
|MSE_BTN2|Call the Tap Dance designed for BTN2|
|MSE_BTN3|Call the Tap Dance designed for BTN3|

### MSE_BTN2 Behavior

|Tap Dance State|Description|
|---|---|
|TD_SINGLE_TAP|tap_code16(KC_BTN3)|
|TD_SINGLE_HOLD|Enable DragScroll until released|
|TD_DOUBLE_TAP|Toggle DragScroll|
|TD_DOUBLE_HOLD|Enable Layer 2 until released|

### MSE_BTN3 Behavior

|Tap Dance State|Description|
|---|---|
|TD_SINGLE_TAP|tap_code16(KC_BTN5)|
|TD_SINGLE_HOLD|Enable Layer 1 until released|
|TD_DOUBLE_TAP|Toggle Dividing the DPI in half|
|TD_DOUBLE_HOLD|Enable Layer 3 until released|

## config.h Settings

|Settings|Value|Description|Custom|
|---|---|---|---|
|PLOOPY_SCROLL_DEBOUNCE|5|Number of milliseconds between registering scroll events.|No|
|PLOOPY_SCROLL_BUTTON_DEBOUNCE|100|Time in milliseconds to ignore scroll events after pressing scroll wheel.|No|
|PLOOPY_DPI_OPTIONS|{ 400, 600, 800, 1000, 1200, 1600, 2400 }|Array of DPI settings to cycle through.|No|
|PLOOPY_DPI_DEFAULT|3|The default PLOOPY_DPI_OPTIONS to use. (1000)|No|
|PLOOPY_DRAGSCROLL_INVERT|1|0 default DragScroll direction. 1 reverses the DragScroll direction.|No|
|PLOOPY_DRAGSCROLL_DIVISORS_H|{ 10, 20, 30, 40, 50, 100 }|Array of horizontal movement divisors for DragScroll to cycle through.|Yes|
|PLOOPY_DRAGSCROLL_DIVISORS_V|{ 10, 20, 30, 40, 50, 100}|Array of vertical movement divisors for DragScroll to cycle through.|Yes|
|PLOOPY_DRAGSCROLL_DEFAULT_H|4|The default PLOOPY_DRAGSCROLL_DIVISORS_H to use. (50)|Yes|
|PLOOPY_DRAGSCROLL_DEFAULT_V|4|The default PLOOPY_DRAGSCROLL_DIVISORS_V to use. (50)|Yes|
