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
#include QMK_KEYBOARD_H

// Tap Dance keycodes
enum td_keycodes {
    MSE_BTN2, // Hold scroll wheel for momentary dragscroll. Double tap toggles dragscroll.
    MSE_BTN3, // If held allow DPI changes. Double tap toggles precision mode.
};

// Define a state for when we're holding down a button
bool btn2_held        = false;
bool btn2_double_held = false;
bool btn2_triple_held = false;
bool btn3_held        = false;
bool btn3_double_held = false;

// Define a state for our modes
bool drag_scroll    = false;
bool precision_mode = false;

// Define a type containing as many tapdance states as you need
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_SINGLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_TAP,
    TD_TRIPLE_SINGLE_TAP,
    TD_TRIPLE_HOLD,
    TD_TRIPLE_TAP
} td_state_t;

// Create a global instance of the tapdance state type
static td_state_t td_state;

// Function to determine the current tapdance state
td_state_t curr_dance(tap_dance_state_t *state);

// `finished` and `reset` functions for each tapdance keycode
void mseBtn2_finished(tap_dance_state_t *state, void *user_data);
void mseBtn2_reset(tap_dance_state_t *state, void *user_data);
void mseBtn3_finished(tap_dance_state_t *state, void *user_data);
void mseBtn3_reset(tap_dance_state_t *state, void *user_data);

// Define custom keycodes for increment/decrement dpi
enum custom_keycodes {
    DPI_INC = SAFE_RANGE,
    DPI_DEC,
    DS_V_INC,
    DS_V_DEC,
    DS_H_INC,
    DS_H_DEC,
};

// Process custom keycodes
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case DPI_INC:
            if (record->event.pressed) {
                cycle_dpi(0);
                xprintf("DPI set to: %u\n", pmw33xx_get_cpi(0));
            } else {
                // When keycode is released
            }
            break;
        case DPI_DEC:
            if (record->event.pressed) {
                cycle_dpi(1);
                xprintf("DPI set to: %u\n", pmw33xx_get_cpi(0));
            } else {
                // When keycode is released
            }
            break;
        case DS_V_INC:
            if (record->event.pressed) {
                cycle_dragscroll_speed_v(0);
                xprintf("Dragscroll Speed V Divisor set to: %u\n", dragscroll_divisors_v[keyboard_config.dragscroll_v_config]);
            } else {
                // When keycode is released
            }
            break;
        case DS_V_DEC:
            if (record->event.pressed) {
                cycle_dragscroll_speed_v(1);
                xprintf("Dragscroll Speed V Divisor set to: %u\n", dragscroll_divisors_v[keyboard_config.dragscroll_v_config]);
            } else {
                // When keycode is released
            }
            break;
        case DS_H_INC:
            if (record->event.pressed) {
                cycle_dragscroll_speed_h(0);
                xprintf("Dragscroll Speed H Divisor set to: %u\n", dragscroll_divisors_h[keyboard_config.dragscroll_h_config]);
            } else {
                // When keycode is released
            }
            break;
        case DS_H_DEC:
            if (record->event.pressed) {
                cycle_dragscroll_speed_h(1);
                xprintf("Dragscroll Speed H Divisor set to: %u\n", dragscroll_divisors_h[keyboard_config.dragscroll_h_config]);
            } else {
                // When keycode is released
            }
            break;
    }
    return true;
};

// Keymap layer(s)
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
     [0] = LAYOUT( KC_BTN1, TD(MSE_BTN2), TD(MSE_BTN3), KC_BTN4, KC_BTN2),
     [1] = LAYOUT( _______, _______, _______, DPI_INC, DPI_DEC),
     [2] = LAYOUT( _______, _______, _______, DS_V_INC, DS_V_DEC),
     [3] = LAYOUT( _______, _______, _______, DS_H_INC, DS_H_DEC),
};

// Determine the tapdance state to return
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        // Interrupted means some other buttion was pressed in the tapping term
        if (state->interrupted || !state->pressed) {
            xprintf("TD_SINGLE_TAP\n");
            return TD_SINGLE_TAP;
        } else {
            xprintf("TD_SINGLE_HOLD\n");
            return TD_SINGLE_HOLD;
        }
    } else if (state->count == 2) {
        if (state->interrupted) {
            xprintf("TD_DOUBLE_SINGLE_TAP\n");
            return TD_DOUBLE_SINGLE_TAP;
        } else if (state->pressed) {
            xprintf("TD_DOUBLE_HOLD\n");
            return TD_DOUBLE_HOLD;
        } else {
            xprintf("TD_DOUBLE_TAP\n");
            return TD_DOUBLE_TAP;
        }
    } else if (state->count == 3) {
        if (state->interrupted) {
            xprintf("TD_TRIPLE_SINGLE_TAP\n");
            return TD_TRIPLE_SINGLE_TAP;
        } else if (state->pressed) {
            xprintf("TD_TRIPLE_HOLD\n");
            return TD_TRIPLE_HOLD;
        } else {
            xprintf("TD_TRIPLE_TAP\n");
            return TD_TRIPLE_TAP;
        }
    } else {
        xprintf("TD_UNKNOWN\n");
        // Any number higher than the maximum state value you return above
        return TD_UNKNOWN;
    }
}

//
// Handle the possible states for each tapdance keycodes you define
//

// BTN2
// Single tap calls KC_BTN3 (mouse wheel click)
// Holding simply holds KC_BTN3
// Double tap toggles drag scroll
// Double tap hold enables momentary drag scroll
// Triple tap hold enables dragscroll divisor v config
void mseBtn2_finished(tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            xprintf("BTN2 Single Tap\n");
            tap_code16(KC_BTN3);
            break;
        case TD_SINGLE_HOLD:
            xprintf("BTN2 Single Tap Hold\n");
            btn2_held = true;
            register_code16(KC_BTN3);
            break;
        case TD_DOUBLE_TAP:
            xprintf("BTN2 Double Tap\n");
            drag_scroll ^= 1;
            toggle_drag_scroll();
            break;
        case TD_DOUBLE_HOLD:
            xprintf("BTN2 Single Tap Hold\n");
            btn2_double_held = true;
            drag_scroll      = true;
            toggle_drag_scroll();
            break;
        case TD_TRIPLE_HOLD:
            xprintf("BTN2 Triple Tap Hold\n");
            btn2_triple_held = true;
            layer_on(2);
            break;
        default:
            break;
    }
}

void mseBtn2_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_HOLD:
            xprintf("BTN2 Single Tap Released\n");
            btn2_held = false;
            unregister_code16(KC_BTN3);
            break;
        case TD_DOUBLE_HOLD:
            xprintf("BTN2 Double Tap Released\n");
            btn2_double_held = false;
            drag_scroll      = false;
            toggle_drag_scroll();
            break;
        case TD_TRIPLE_HOLD:
            xprintf("BTN2 Triple Tap Released\n");
            btn2_triple_held = false;
            layer_off(2);
            break;
        default:
            break;
    }
}

// BTN3
// Single tap KC_BTN5 (browser forward)
// Holding enables DPI configuration
// Double tap toggles precision mode
// Double tap hold enables dragscroll divisor h config
void mseBtn3_finished(tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            xprintf("BTN3 Single Tap\n");
            tap_code16(KC_BTN5);
            break;
        case TD_SINGLE_HOLD:
            xprintf("BTN3 Single Tap Hold\n");
            layer_on(1);
            btn3_held = true;
            break;
        case TD_DOUBLE_TAP:
            xprintf("BTN3 Double Tap\n");
            if (!precision_mode) {
                xprintf("precision_mode enabled\n");
                pmw33xx_set_cpi(0, (dpi_array[keyboard_config.dpi_config] / 2));
                xprintf("DPI set to: %u\n", pmw33xx_get_cpi(0));
                precision_mode = true;
            } else {
                xprintf("precision_mode disabled\n");
                pmw33xx_set_cpi(0, dpi_array[keyboard_config.dpi_config]);
                xprintf("DPI set to: %u\n", pmw33xx_get_cpi(0));
                precision_mode = false;
            }
            break;
        case TD_DOUBLE_HOLD:
            xprintf("BTN3 Double Tap Hold\n");
            btn3_double_held = true;
            layer_on(3);
            break;
        default:
            break;
    }
}

void mseBtn3_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_HOLD:
            xprintf("BTN3 Single Tap Released\n");
            btn3_held = false;
            layer_off(1);
            break;
        case TD_DOUBLE_HOLD:
            xprintf("BTN3 Double Tap Released\n");
            btn3_double_held = false;
            layer_off(3);
        default:
            break;
    }
}

// Define `ACTION_TAP_DANCE_FN_ADVANCED()` for each tapdance keycode, passing in the `finished` and `reset` functions
tap_dance_action_t tap_dance_actions[] = {
    [MSE_BTN2] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, mseBtn2_finished, mseBtn2_reset),
    [MSE_BTN3] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, mseBtn3_finished, mseBtn3_reset),
};

// Set a long-ish tapping term for tap-dance keys
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
            return 275;
        default:
            return TAPPING_TERM;
    }
};
