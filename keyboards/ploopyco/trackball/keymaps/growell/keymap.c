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
    MSE_BTN3_DRAG // Hold scroll wheel for momentary dragscroll
};

// Define a state for when we're holding down BTN3
bool btn3_held = false;

// Define a type containing as many tapdance states as you need
typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_SINGLE_TAP,
    TD_DOUBLE_HOLD,
    TD_DOUBLE_TAP
} td_state_t;

// Create a global instance of the tapdance state type
static td_state_t td_state;

// Function to determine the current tapdance state
td_state_t curr_dance(tap_dance_state_t *state);

// `finished` and `reset` functions for each tapdance keycode
void mseBtn3_finished(tap_dance_state_t *state, void *user_data);
void mseBtn3_reset(tap_dance_state_t *state, void *user_data);

// Keymap layer(s)
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( KC_BTN1, TD(MSE_BTN3_DRAG), KC_BTN5, KC_BTN4, KC_BTN2 ),
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
    } else {
        xprintf("TD_UNKNOWN\n");
        // Any number higher than the maximum state value you return above
        return TD_UNKNOWN;
    }
}

// Handle the possible states for each tapdance keycodes you define
void mseBtn3_finished(tap_dance_state_t *state, void *user_data) {
    td_state = cur_dance(state);
    switch (td_state) {
        case TD_SINGLE_TAP:
            xprintf("single tap fin\n");
            register_code16(KC_BTN3);
        break;
        case TD_SINGLE_HOLD:
            xprintf("HOLDING WHEEL\n");
	    toggle_drag_scroll();
        break;
	case TD_DOUBLE_TAP:
            xprintf("Double tap wheel fin\n");
        break;
        default:
        break;
    }
}

void mseBtn3_reset(tap_dance_state_t *state, void *user_data) {
    switch (td_state) {
        case TD_SINGLE_TAP:
            xprintf("single tap reset\n");
            unregister_code16(KC_BTN3);
        break;
        case TD_SINGLE_HOLD:
            xprintf("wheel released\n");
	    toggle_drag_scroll();
        break;
        default:
        break;
    }
}

// Define `ACTION_TAP_DANCE_FN_ADVANCED()` for each tapdance keycode, passing in the `finished` and `reset` functions
tap_dance_action_t tap_dance_actions[] = {
    [MSE_BTN3_DRAG] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, mseBtn3_finished, mseBtn3_reset)
};
