 /* Copyright 2022  CyanDuck
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
*
* (at your option) any later version.
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

#include "oneshot.h"
#include "swapper.h"

#define LA_SYM LT(SYM, KC_TAB)
#define LA_NAV LT(NAV, KC_ESC)


// Defines names for use in layer keycodes and the keymap
enum layers {
    DEF,
    SYM,
    NAV,
    SYS,
};
enum keycodes {
    // Custom oneshot mod implementation with no timers.
    OS_SHFT = SAFE_RANGE,
    OS_CTRL,
    OS_ALT,	      	
    OS_CMD,

    SW_WIN,  // Switch to next window         (cmd-tab)
	SW_LANG, // Switch to next input language (ctl-spc)
};

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

void matrix_scan_user(void) {
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 1250) {
      unregister_code(KC_LGUI);
      is_alt_tab_active = false;
    }
  }
} 


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* QWERTY
 *
 * ,----------------------------------.                      ,----------------------------------.
 * |   Q  |   W  |   E  |   R  |   T  |                      |   Y  |   U  |   I  |   O  |   P  |
 * |------+------+------+------+------|                      |------+------+------+------+------|
 * |   A  |   S  |   D  |   F  |   G  |                      |   H  |   J  |   K  |   L  |   ;  |
 * |------+------+------+------+------|  ,-----.    ,-----.  |------+------+------+------+------|
 * |   Z  |   X  |   C  |   V  |   B  |  |CAPS |    |NUMLK|  |   N  |   M  |   ,  |   .  |   /  |
 * `----------------------------------'  `-----'    `-----'  `----------------------------------'
 *          ,-----.   ,--------------------.            ,--------------------.      ,-----. 
 *          |  1  |   | Tab(2) | SPACE | Enter(1)  |    |  BSPC  | SHFT | CMD/CTRL| |  4  |
 *          `-----'   `--------------------'            `--------------------'   	`-----'
 */
[DEF] = LAYOUT(
	KC_Q,				KC_W,				KC_E,				KC_R,					KC_T,			KC_Y,	KC_U,				KC_I,				KC_O,				KC_P,
	KC_A,				KC_S,				KC_D,	            KC_F,					KC_G,			KC_H,	KC_J,				KC_K,				KC_L,				KC_SCLN,
	KC_Z,				KC_X,				KC_C,				KC_V,					KC_B,			KC_N,	KC_M,				KC_COMM,			KC_DOT,				KC_SLSH,
	KC_1,			    LA_SYM,				KC_SPC,				MT(MOD_LGUI, KC_ENT),	KC_CAPS,		KC_NUM,	KC_BSPC,			OSM(MOD_RSFT),		LA_NAV,				KC_4
),

/* SYM (1)
 *
 * ,----------------------------------.                      ,----------------------------------.
 * |      |  F7 |   F8  |  F9  | F10  |                      |   +  |   7  |   8  |   9  |   0  |
 * |------+------+------+------+------|                      |------+------+------+------+------|
 * | Home |  F4  |   F5 |  F6  | F11  |                      |   -  |   4  |   5  |   6  |   "  |
 * |------+------+------+------+------|  ,-----.    ,-----.  |------+------+------+------+------|
 * |      |  F1  |  F2  |  F3  | F13  |  |CAPS |    |NUMLK|  |   =  |   1  |   2  |   3  |   '  |
 * `----------------------------------'  `-----'    `-----'  `----------------------------------'
 *          ,-----.   ,--------------------.            ,--------------------.      ,-----. 		
 *          |  1  |   | DEL | SPACE | ENT()  |            |  ESC  | SHFT | ENTER |  |  4  |
 *          `-----'   `--------------------'            `--------------------'      `-----'
 */
[SYM] = LAYOUT(
	KC_NO,		KC_PGUP,		KC_UP,			KC_PGDN,		KC_NO,				KC_EQL,	KC_7,		KC_8,		KC_9,			KC_0,
	KC_HOME,	KC_LEFT,		KC_DOWN,		KC_RGHT,		KC_END,				KC_MINS,	KC_4,		KC_5,		KC_6,			KC_DQT,
	KC_NO,		KC_NO,			KC_INS,			KC_NO,			KC_NO,				KC_EQL,		KC_1,		KC_2,		KC_3,			KC_QUOT,
	KC_1,		LT(1,KC_TAB),	KC_SPC,			OS_CMD,			KC_CAPS,			KC_NUM,		KC_DEL,		KC_LSFT,	LT(2,KC_DEL),	KC_4
),

/* NAV(2)
 *  
 * ,----------------------------------.                      ,----------------------------------.
 * |   %  |   @  |   [  |   ]  |   \  |                      |   [  |   (  |  UP  |   )   |  ]   |
 * |------+------+------+------+------|                      |------+------+------+------ +------|
 * |  ALT |  WIN | SHFT | CTRL |   ~  |                      |  WBK | LEFT | DOWN | RIGHT | WFWD |
 * |------+------+------+------+------|  ,-----.    ,-----.  |------+------+------+------ +------|
 * | TOGL |   £  |   {  |   }  | BOOT |  |CAPS |    |NUMLK|  |   _   | CTRL | SHFT | WIN   | ALT  |
 * `----------------------------------'  `-----'    `-----'  `----------------------------------'
 *          ,-----.   ,--------------------.            ,--------------------.   ,-----. 
 *          |  1  |   | DEL | SPACE | TAB  |            |  ESC  | PS | ENTER |   |  4  |
 *          `-----'   `--------------------'            `--------------------'   `-----'
 */
[NAV] = LAYOUT(
	KC_PERC,	KC_AT,			KC_LBRC,		KC_RBRC,		KC_BSLS,		KC_LBRC,	KC_LPRN,	KC_PIPE,	KC_RPRN,		KC_RBRC,
	OS_ALT,	    OS_CMD,			OS_SHFT,		OS_CTRL,		KC_PIPE,		KC_LEFT,	KC_DOWN,	KC_UP,		KC_RGHT,		KC_QUOT,
	CG_TOGG,		KC_NO,			KC_LCBR,		KC_RCBR,		QK_BOOT,	KC_UNDS,	OS_CTRL,	OS_SHFT,	OS_CMD,			OS_ALT,
	KC_1,		LT(3,KC_TAB),	KC_SPC,			LT(1,KC_ENT),	KC_CAPS,		KC_NUM,		OS_CMD,		OS_SHFT,	LT(2,KC_BSPC),	KC_4
),

/* FUNC (3)
 *
 * ,----------------------------------.                      ,----------------------------------.
 * |      |      |      |      |      |                      |      |  &   |  (   |  )  |  F11 |
 * |------+------+------+------+------|                      |------+------+------+------+------|
 * |      |      |      |      |      |                      |      |  $   |  %   |  ^  |  F12 |
 * |------+------+------+------+------|  ,-----.    ,-----.  |------+------+------+------+------|
 * | Reset|      |      |      |      |  |CAPS |    |NUMLK|  |  F10 |  !   |   @  |  #  |  F13 |
 * `----------------------------------'  `-----'    `-----'  `----------------------------------'
 *          ,-----.   ,--------------------.            ,--------------------.   ,-----. 
 *          |  1  |   | DEL | SPACE | TAB  |            |  ESC  | BS | ENTER |   |  4  |
 *          `-----'   `--------------------'            `--------------------'   `-----'
 */
[SYS] =  LAYOUT(
	KC_NO,		KC_NO,			KC_NO,			KC_NO,				KC_NO,			KC_NO,	KC_AMPR,	KC_ASTR,	KC_LPRN,		KC_RPRN,
	KC_NO,		KC_NO,			KC_NO,			KC_NO,				KC_NO,			KC_NO,	KC_DLR,		KC_PERC,	KC_CIRC,		KC_TILD,
	QK_BOOT,	KC_NO,			KC_NO,			KC_NO,				KC_NO,			KC_F10,	KC_EXLM,	KC_AT,		KC_HASH,		KC_PIPE,
	KC_1,		LT(3,KC_TAB),	KC_SPC,			LT(1,KC_ENT),		KC_CAPS,		KC_NUM,	OS_CMD,		OS_SHFT,	LT(2,KC_BSPC),	KC_4
),
};

//Callum modifications
bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
        return true;
		case LA_SYM:
    	case LA_NAV:
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case LA_NAV:
	case LA_SYM:
	case OS_SHFT:
    case OS_CTRL:
    case OS_ALT:
    case OS_CMD:
        return true;
    default:
        return false;
    }
}

bool sw_win_active = false;
bool sw_lang_active = false;

oneshot_state os_shft_state = os_up_unqueued;
oneshot_state os_ctrl_state = os_up_unqueued;
oneshot_state os_alt_state = os_up_unqueued;
oneshot_state os_cmd_state = os_up_unqueued;


bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    update_swapper(
        &sw_win_active, KC_LGUI, KC_TAB, SW_WIN,
        keycode, record
    );
    update_swapper(
        &sw_lang_active, KC_LCTL, KC_SPC, SW_LANG,
        keycode, record
    );

    update_oneshot(
        &os_shft_state, KC_LSFT, OS_SHFT,
        keycode, record
    );
    update_oneshot(
        &os_ctrl_state, KC_LCTL, OS_CTRL,
        keycode, record
    );
    update_oneshot(
        &os_alt_state, KC_LALT, OS_ALT,
        keycode, record
    );
    update_oneshot(
        &os_cmd_state, KC_LCMD, OS_CMD,
        keycode, record
    );

    return true;
	}
