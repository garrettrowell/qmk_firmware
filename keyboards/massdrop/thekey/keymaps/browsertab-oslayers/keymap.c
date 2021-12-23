/* Copyright 2021 Joe Maples <joe@maples.dev>
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

// Layers for readability
#define WINDOWS 0
#define LINUX 1
#define MAC 2

enum custom_keycodes {
    COPY = SAFE_RANGE,
    PASTE,
};

// Tap Dance declarations
enum {
    TD_LIN_COPY,
    TD_LIN_PASTE,
    TD_SO,
};

void dance_so(qk_tap_dance_state_t* state, void* user_data) {
    if (state->count == 1) {
        if (IS_LAYER_ON(WINDOWS)) {
            // Tap windows key, type 'run', delay for 500ms, tap enter, delay 500mils, type command to open SO in default browser, tap enter
            SEND_STRING(SS_DOWN(X_LGUI) "r" SS_DELAY(500) SS_UP(X_LGUI) "explorer https://stackoverflow.com" SS_TAP(X_ENT));
        } else if (IS_LAYER_ON(LINUX)) {
            // Hold down Alt, Hold down F2, delay 500ms, release Alt + F2, type command to open SO in default browser, tap enter
            SEND_STRING(SS_DOWN(X_LALT) SS_DOWN(X_F2) SS_DELAY(500) SS_UP(X_LALT) SS_UP(X_F2) "xdg-open https://stackoverflow.com" SS_TAP(X_ENT));
        } else if (IS_LAYER_ON(MAC)) {
            // Hold down CMD, Hold down Space, delay 500ms, release CMD + Space, type SO url in spotlight, tap enter
            // You'll likely want to disable the "Developer" category for spotlight results... seeing as how developers like to throw in SO links in comments
            SEND_STRING(SS_DOWN(X_LGUI) SS_DOWN(X_SPACE) SS_DELAY(500) SS_UP(X_LGUI) SS_UP(X_SPACE) "https://stackoverflow.com" SS_TAP(X_ENT));
        }
    } else if (state->count == 2) {
        // Call the function that KC_LEAD keycode does
        qk_leader_start();
    } else if (state->count == 3) {
        // Move to windows layer
        layer_move(WINDOWS);
    }
}

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Control c, twice for Control Shift c
    [TD_LIN_COPY] = ACTION_TAP_DANCE_DOUBLE(LCTL(KC_C), LCTL(LSFT(KC_C))),
    // Tap once for Control v, twice for Control Shift v
    [TD_LIN_PASTE] = ACTION_TAP_DANCE_DOUBLE(LCTL(KC_V), LCTL(LSFT(KC_V))),
    //Single tap open so in browser tab, twice enables leader key, three times switches to windows layer
    [TD_SO] = ACTION_TAP_DANCE_FN(dance_so),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [WINDOWS] = LAYOUT(
        TD(TD_SO), COPY, PASTE
    ),
    [LINUX] = LAYOUT(
        TD(TD_SO), TD(TD_LIN_COPY), TD(TD_LIN_PASTE)
    ),
    [MAC] = LAYOUT(
        TD(TD_SO), COPY, PASTE
    ),

};

LEADER_EXTERNS();
void matrix_scan_user(void) {
  LEADER_DICTIONARY() {
    leading = false;
    leader_end();

    SEQ_ONE_KEY(COPY) {
      layer_move(LINUX);
    }
    SEQ_ONE_KEY(PASTE) {
      layer_move(MAC);
    }
    SEQ_ONE_KEY(TD(TD_LIN_COPY)) {
      layer_move(LINUX);
    }
    SEQ_ONE_KEY(TD(TD_LIN_PASTE)) {
      layer_move(MAC);
    }

  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case COPY:
            if (record->event.pressed) {
                if (IS_LAYER_ON(WINDOWS)) {
                   tap_code16(LCTL(KC_C));
                } else if (IS_LAYER_ON(MAC)) {
                   tap_code16(LGUI(KC_C));
                }
            }
            break;
        case PASTE:
            if (record->event.pressed) {
                if (IS_LAYER_ON(WINDOWS)) {
                   tap_code16(LCTL(KC_V));
                } else if (IS_LAYER_ON(MAC)) {
                  tap_code16(LGUI(KC_V));
                }
            }
            break;
        default:
            break;
    }
    return true;
}
