#include QMK_KEYBOARD_H

#include "oneshot.h"
#include "swapper.h"


#define HOME G(KC_LEFT)
#define END G(KC_RGHT)
#define FWD G(KC_RBRC)
#define BACK G(KC_LBRC)
#define TAB_L G(S(KC_LBRC))
#define TAB_R G(S(KC_RBRC))
#define SPACE_L A(G(KC_LEFT))
#define SPACE_R A(G(KC_RGHT))
#define LA_SYM TT(SYM)
#define LA_NAV TT(NAV)
#define LA_DEF TO(DEF)
#define LA_SYS TG(SYS)
#define SW_PANE A(KC_GRV)
#define SHIFTER OSM(MOD_RSFT)

#define BWRD A(KC_BSPC)
#define BLINE G(KC_BSPC)

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

//Tap dance 
enum tapdance 
{
    TD_SHIFTER, 
};

qk_tap_dance_action_t tap_dance_actions[] = {
    // Tap once for Escape, twice for Caps Lock
    [TD_SHIFTER] = ACTION_TAP_DANCE_DOUBLE(OSM(MOD_RSFT), KC_CAPS),
};




const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [DEF] = LAYOUT_reviung41(
        KC_GRV,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,             KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,         KC_BSPC,
        KC_TAB,   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,             KC_H,    KC_J,    KC_K,    KC_L,     KC_SCLN,     KC_QUOT,
        LA_NAV,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,   KC_SLASH,    LA_SYM,
                                             KC_ESC, KC_LGUI, KC_SPC,   SHIFTER,    KC_ENT
    ),


    [SYM] = LAYOUT_reviung41(
        KC_ESC,   KC_ESC,  KC_7, KC_8,  KC_9, KC_0,                 KC_CIRC, KC_RPRN, KC_RCBR, KC_RBRC, KC_GRV,   KC_NO,
        _______,  KC_MINS, KC_4, KC_5,  KC_6, KC_EQL,               KC_HASH, OS_CMD,  OS_ALT,  OS_CTRL, OS_SHFT,  KC_NO,
        LA_DEF,   KC_LBRC, KC_1, KC_2,  KC_3, KC_RBRC,              XXXXXXX, KC_BSLS, OS_CMD,  OS_ALT,  OS_CTRL,  LA_NAV,
                                             _______, OS_SHFT,   _______,  _______, _______
    ),

    [NAV] = LAYOUT_reviung41(
        KC_TAB,    QK_BOOT,  SW_WIN, KC_VOLU, SPACE_L, SPACE_R,         KC_PGUP,    HOME,       KC_UP,   END,     KC_DEL,       KC_BSPC,
        _______,   BACK,    TAB_L,  TAB_R,    OS_SHFT, KC_VOLD,         KC_CAPS,    KC_LEFT,    KC_DOWN, KC_RGHT, KC_BSPC,      LA_SYS,
        LA_DEF,     OS_CTRL, OS_ALT, OS_CMD,  FWD,     KC_MPLY,         KC_PGDN,    OS_SHFT,    OS_CMD,  OS_ALT,  OS_CTRL,      LA_SYM,     
                                              _______, OS_SHFT, _______, _______, _______
    ),

    [SYS] = LAYOUT_reviung41(
        KC_ESC,    KC_7,    KC_5,    KC_3,    KC_1,    KC_9,       KC_8,    KC_0,    KC_2,    KC_4,     KC_6,      _______,
        _______,    OS_SHFT, OS_CTRL, OS_ALT,  OS_CMD,  KC_F11,     KC_F10,  OS_CMD,  OS_ALT,  OS_CTRL,  OS_SHFT,   _______,
        LA_DEF,    KC_F7,   KC_F5,   KC_F3,   KC_F1,   KC_F9,       KC_F8,   KC_F12,  QK_BOOT,   QK_RBT, KC_Z,     _______,
                                   _______, _______, _______, _______, _______
    ),
};

//RGB Lighting definitions
const rgblight_segment_t PROGMEM capslock_layer[] = RGBLIGHT_LAYER_SEGMENTS({0, 11, HSV_RED});
// Light LEDs 9 & 10 in cyan when keyboard layer 1 is active
const rgblight_segment_t PROGMEM default_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 11 , HSV_WHITE}
);
// Light LEDs 11 & 12 in purple when keyboard layer 2 is active
const rgblight_segment_t PROGMEM symbol_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 11, HSV_PURPLE}
);
// Light LEDs 13 & 14 in green when keyboard layer 3 is active
const rgblight_segment_t PROGMEM navigation_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 11, HSV_GREEN}
);

// Now define the array of layers. Later layers take precedence
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    capslock_layer,
    default_layer,    // Overrides caps lock layer
    symbol_layer,    // Overrides other layers
    navigation_layer     // Overrides other layers
);

void keyboard_post_init_user(void) {
    // Enable the LED layers
    rgblight_layers = my_rgb_layers;
}

bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(0, led_state.caps_lock);
    return true;
}


//Callum modifications
bool is_oneshot_cancel_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
        return true;
    default:
        return false;
    }
}

bool is_oneshot_ignored_key(uint16_t keycode) {
    switch (keycode) {
    case LA_SYM:
    case LA_NAV:
    case KC_LSFT:
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


layer_state_t default_layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(1, layer_state_cmp(state, DEF));
    return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(2, layer_state_cmp(state, SYM));
    rgblight_set_layer_state(3, layer_state_cmp(state, NAV));
    return update_tri_layer_state(state, SYM, NAV, SYS);
}

//Combos
const uint16_t PROGMEM caps_c[] = {KC_LGUI, SHIFTER, COMBO_END};
combo_t key_combos[COMBO_COUNT] = {
    COMBO(caps_c, KC_CAPS),
};