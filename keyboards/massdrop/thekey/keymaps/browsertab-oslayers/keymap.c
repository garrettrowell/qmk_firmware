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
#define MAC 1
#define LINUX 2

enum custom_keycodes {
    WIN_URL = SAFE_RANGE,
    LIN_URL,
    MAC_URL,
};

// Tap Dance declarations
enum {
    TD_LIN_COPY,
    TD_LIN_PASTE,
};

// Tap Dance definitions
qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Control c, twice for Control Shift c
    [TD_LIN_COPY] = ACTION_TAP_DANCE_DOUBLE(LCTL(KC_C), LCTL(LSFT(KC_C))),
    // Tap once for Control v, twice for Control Shift v
    [TD_LIN_PASTE] = ACTION_TAP_DANCE_DOUBLE(LCTL(KC_V), LCTL(LSFT(KC_V))),
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [WINDOWS] = LAYOUT(
        WIN_URL, LCTL(KC_C), LCTL(KC_V) 
    ),
    [LINUX] = LAYOUT(
        LIN_URL, TD(TD_LIN_COPY), TD(TD_LIN_PASTE)
    ),
    [MAC] = LAYOUT(
        MAC_URL, LGUI(KC_C), LGUI(KC_V)
    ),

};


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case WIN_URL:
            if (record->event.pressed) {
		// Tap windows key, type 'run', delay for 500ms, tap enter, delay 500mils, type command to open SO in default browser, tap enter
                SEND_STRING(SS_DOWN(X_LGUI) "r" SS_DELAY(500) SS_UP(X_LGUI) "explorer https://stackoverflow.com" SS_TAP(X_ENT));
            }
            break;
	case LIN_URL:
            if (record->event.pressed) {
		// Hold down Alt, Hold down F2, delay 500ms, release Alt + F2, type command to open SO in default browser, tap enter
                SEND_STRING(SS_DOWN(X_LALT) SS_DOWN(X_F2) SS_DELAY(500) SS_UP(X_LALT) SS_UP(X_F2) "xdg-open https://stackoverflow.com" SS_TAP(X_ENT));
            }
            break;
	case MAC_URL:
            if (record->event.pressed) {
		// Hold down CMD, Hold down Space, delay 500ms, release CMD + Space, type SO url in spotlight, tap enter
		// You'll likely want to disable the "Developer" category for spotlight results... seeing as how developers like to throw in SO links in comments
                SEND_STRING(SS_DOWN(X_LGUI) SS_DOWN(X_SPACE) SS_DELAY(500) SS_UP(X_LGUI) SS_UP(X_SPACE) "https://stackoverflow.com" SS_TAP(X_ENT));
            }
            break;
        default:
            break;
    }
    return true;
}
