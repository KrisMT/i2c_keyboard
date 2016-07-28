// This is the canonical layout file for the Quantum project. If you want to add another keyboard,
// this is the style you want to emulate.

#include <avr/pgmspace.h>
#include <util/delay.h>
#include "keycode.h"
#include <string.h>

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

const PROGMEM uint8_t keymaps[16][MATRIX_ROWS][MATRIX_COLS] = {

  /* Qwerty
   * ,-----------------------------------------. ,-----------------------------------------.
   * | Tab  |   Q  |   W  |   E  |   R  |   T  | |   Y  |   U  |   I  |   O  |   P  | Bksp |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * | Esc  |   A  |   S  |   D  |   F  |   G  | |   H  |   J  |   K  |   L  |  : ; | " '  |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * | Shift|   Z  |   X  |   C  |   V  |   B  | |   N  |   M  |  < , |  > . |  ? / | Enter|
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * | Ctrl | Alt  |  DEL |  GUI |Lower |Space | |Space |Raise |  GUI |  Alt | Ctrl | Shift|
   * `-----------------------------------------' `-----------------------------------------'
   */
  [_QWERTY] = {
    {KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T},         // ROW 0
    {KC_ESC,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G},          // ROW 1
    {KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B},          // ROW 2
    {KC_LCTL, KC_LALT, KC_DEL,  KC_LGUI, LOWER,   KC_SPC},        // ROW 3
    {KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSPC},       // ROW 4
    {KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT},       // ROW 5
    {KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT },       // ROW 6
    {KC_SPC,  RAISE,   KC_RGUI, KC_RALT, KC_RCTL, KC_RSFT}        // ROW 7
  },

  /* Workman
   * ,-----------------------------------------. ,-----------------------------------------.
   * | Tab  |   Q  |   D  |   R  |   W  |   B  | |   J  |   F  |   U  |   P  |  : ; | Bksp |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * | Esc  |   A  |   S  |   H  |   T  |   G  | |   Y  |   N  |   E  |   O  |   I  | " '  |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * | Shift|   Z  |   X  |   M  |   C  |   V  | |   K  |   L  |  < , |  > . |  ? / |Enter |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * | Ctrl | Alt  |  DEL |  GUI |Lower |Space | |Space |Raise |  GUI |  Alt | Ctrl | Shift|
   * `-----------------------------------------' `-----------------------------------------'
   */
  [_WORKMAN] = {
    {KC_TAB,  KC_Q,    KC_D,    KC_R,    KC_W,    KC_B},         /* ROW 0 */
    {KC_ESC,  KC_A,    KC_S,    KC_H,    KC_T,    KC_G},          /* ROW 1 */
    {KC_LSFT, KC_Z,    KC_X,    KC_M,    KC_C,    KC_V},          /* ROW 2 */
    {KC_LCTL, KC_LALT, KC_DEL,  KC_LGUI, LOWER,   KC_SPC},        /* ROW 3 */
    {KC_J,    KC_F,    KC_U,    KC_P,    KC_SCLN, KC_BSPC},       /* ROW 4 */
    {KC_Y,    KC_N,    KC_E,    KC_O,    KC_I,    KC_QUOT},       /* ROW 5 */
    {KC_K,    KC_L,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT },       /* ROW 6 */
    {KC_SPC,  RAISE,   KC_RGUI, KC_RALT, KC_RCTL, KC_RSFT}        /* ROW 7 */
  },

  /* Colemak
   * ,-----------------------------------------. ,-----------------------------------------.
   * | Tab  |   Q  |   W  |   F  |   P  |   G  | |   J  |   L  |   U  |   Y  |  : ; | Bksp |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * | Esc  |   A  |   R  |   S  |   T  |   D  | |   H  |   N  |   E  |   I  |   O  | ' "  |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * | Shift|   Z  |   X  |   C  |   V  |   B  | |   K  |   M  |  < , |  > . |  ? / |Enter |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * | Ctrl | Alt  |  DEL |  GUI |Lower |Space | |Space |Raise |  GUI |  Alt | Ctrl | Shift|
   * `-----------------------------------------' `-----------------------------------------'
   */
  [_COLEMAK] = {
    {KC_TAB,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_G},         /* ROW 0 */
    {KC_ESC,  KC_A,    KC_R,    KC_S,    KC_T,    KC_D},         /* ROW 1 */
    {KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B},         /* ROW 2 */
    {KC_LCTL, KC_LALT, KC_DEL,  KC_LGUI, LOWER,   KC_SPC},        /* ROW 3 */
    {KC_J,    KC_L,    KC_U,    KC_Y,    KC_SCLN, KC_BSPC},      /* ROW 4 */
    {KC_H,    KC_N,    KC_E,    KC_I,    KC_O,    KC_QUOT},      /* ROW 5 */
    {KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ENT },      /* ROW 6 */
    {KC_SPC,  RAISE,   KC_RGUI, KC_RALT, KC_RCTL, KC_RSFT}        /* ROW 7 */
  },

  /* Dvorak
   * ,-----------------------------------------. ,-----------------------------------------.
   * | Tab  | ' "  |  < , |  > . |   P  |   Y  | |   F  |   G  |   C  |   R  |   L  | Bksp |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * | Esc  |   A  |   O  |   E  |   U  |   I  | |   D  |   H  |   T  |   N  |   S  |  ? / |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * | Shift|  : ; |   Q  |   J  |   K  |   X  | |   B  |   M  |   W  |   V  |   Z  |Enter |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * | Ctrl | Alt  |  DEL |  GUI |Lower |Space | |Space |Raise |  GUI |  Alt | Ctrl | Shift|
   * `-----------------------------------------' `-----------------------------------------'
   */
  [_DVORAK] = {
    {KC_TAB,  KC_QUOT, KC_COMM, KC_DOT,  KC_P,    KC_Y},         /* ROW 0 */
    {KC_ESC,  KC_A,    KC_O,    KC_E,    KC_U,    KC_I},         /* ROW 1 */
    {KC_LSFT, KC_SCLN, KC_Q,    KC_J,    KC_K,    KC_X},         /* ROW 2 */
    {KC_LCTL, KC_LALT, KC_DEL,  KC_LGUI, LOWER,   KC_SPC},        /* ROW 3 */
    {KC_F,    KC_G,    KC_C,    KC_R,    KC_L,    KC_BSPC},      /* ROW 4 */
    {KC_D,    KC_H,    KC_T,    KC_N,    KC_S,    KC_SLSH},      /* ROW 5 */
    {KC_B,    KC_M,    KC_W,    KC_V,    KC_Z,    KC_ENT },      /* ROW 6 */
    {KC_SPC,  RAISE,   KC_RGUI, KC_RALT, KC_RCTL, KC_RSFT}        /* ROW 7 */
  },

  /* Plover layer (http://opensteno.org)
   * ,-----------------------------------------. ,-----------------------------------------.
   * |   #  |   #  |   #  |   #  |   #  |   #  | |   #  |   #  |   #  |   #  |   #  |   #  |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * |      |   S  |   T  |   P  |   H  |   *  | |   *  |   F  |   P  |   L  |   T  |   D  |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * |      |   S  |   K  |   W  |   R  |   *  | |   *  |   R  |   B  |   G  |   S  |   Z  |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * | Exit |      |      |      |   A  |   O  | |   E  |   U  |      |      |      |      |
   * `-----------------------------------------' `-----------------------------------------'
   */

  [_PLOVER] = {
    {KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1},       /* ROW 0 */
    {_______, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T},       /* ROW 1 */
    {_______, KC_A,    KC_S,    KC_D,    KC_F,    KC_G},       /* ROW 2 */
    {EXT_PLV, _______, _______, _______, KC_C,    KC_V},       /* ROW 3 */
    {KC_1,    KC_1,    KC_1,    KC_1,    KC_1,    KC_1},       /* ROW 4 */
    {KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC},    /* ROW 5 */
    {KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT},    /* ROW 6 */
    {KC_N,    KC_M,    _______, _______, _______, _______}     /* ROW 7 */
  },
  /* jackdaw
   * ,-----------------------------------------. ,-----------------------------------------.
   * |      |      |      |      |      |      | |      |      |      |      |      |      |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * |      |   A  |   C  |   W  |   N  |      | |      |   R  |   L  |   C  |   T  |   E  |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * |      |   S  |   T  |   H  |   R  |      | |      |   N  |   G  |   H  |   S  |   Y  |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * | Exit |      |      |   E  |   O  |   A  | |   A  |   U  |   I  |      |      |      |
   * `-----------------------------------------' `-----------------------------------------'
   */

  /* Lower
   * ,-----------------------------------------. ,-----------------------------------------.
   * |   ~  |   !  |   @  |   #  |   $  |   %  | |   ^  |   &  |   *  |   (  |   )  | Bksp |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  | |      |   _  |   +  |   {  |   }  |  |   |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 | |      | MUTE | Vol- | Vol+ |      |Enter |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * | Ctrl | Alt  |  DEL |  GUI |Lower |Space | |Space |Raise |  GUI |  Alt | Ctrl | Shift|
   * `-----------------------------------------' `-----------------------------------------'
   */
  [_LOWER] = {
    {KC_TILD, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC},      /* ROW 0 */
    {KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6},        /* ROW 1 */
    {KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12},       /* ROW 2 */
    {KC_LCTL, KC_LALT, KC_DEL,  KC_LGUI, LOWER,   KC_SPC},        /* ROW 3 */
    {KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC},      /* ROW 4 */
    {_______, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE},      /* ROW 5 */
    {_______, KC_MUTE, KC_VOLD, KC_VOLU, _______, KC_ENT},       /* ROW 6 */
    {KC_SPC,  RAISE,   KC_RGUI, KC_RALT, KC_RCTL, KC_RSFT}        /* ROW 7 */
  },

  /* Raise
   * ,-----------------------------------------. ,-----------------------------------------.
   * |   `  |   1  |   2  |   3  |   4  |   5  | |   6  |   7  |   8  |   9  |   0  | Bksp |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * |      |      |  Up  |      |      |      | |      |   -  |   =  |   [  |   ]  |  \   |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * |      | Left | Down | Right|      |      | |      | MUTE | Vol- | Vol+ |      |Enter |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * | Ctrl | Alt  |  DEL |  GUI |Lower |Space | |Space |Raise |  GUI |  Alt | Ctrl | Shift|
   * `-----------------------------------------' `-----------------------------------------'
   */
  [_RAISE] = {
    {KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5},         /* ROW 0 */
    {_______, _______, KC_UP,   _______, _______, _______},      /* ROW 1 */
    {_______, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______},      /* ROW 2 */
    {KC_LCTL, KC_LALT, KC_DEL,  KC_LGUI, LOWER,   KC_SPC},        /* ROW 3 */
    {KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC},      /* ROW 4 */
    {_______, KC_MINS, KC_EQL,  KC_LBRC, KC_RBRC, KC_BSLS},      /* ROW 5 */
    {_______, KC_MUTE, KC_VOLD, KC_VOLU, _______, KC_ENT},       /* ROW 6 */
    {KC_SPC,  RAISE,   KC_RGUI, KC_RALT, KC_RCTL, KC_RSFT}        /* ROW 7 */
  },

  /* Adjust (Lower + Raise) >>> Internal keyboard configuration
   * ,-----------------------------------------. ,-----------------------------------------.
   * |      |      |      |      |      |      | |      |      |      |      |      |      |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * |      |      |      |      |      |      | |      |Qwerty|Workmn|Colemk|Dvorak|Plover|
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * |      |      |      |      |      |      | |      |      |      |      |      |      |
   * |------+------+------+------+------+------| |------+------+------+------+------+------|
   * |      |      |      |      |Lower |      | |      |Raise |      |      |      |      |
   * `-----------------------------------------' `-----------------------------------------'
   */
  [_ADJUST] = {
    {_______, _______, _______, _______, _______, _______},      /* ROW 0 */
    {_______, _______, _______, _______, _______, _______},      /* ROW 1 */
    {_______, _______, _______, _______, _______, _______},      /* ROW 2 */
    {_______, _______, _______, _______, LOWER,   _______},       /* ROW 3 */
    {_______, _______, _______, _______, _______, _______},      /* ROW 4 */
    {_______, QWERTY,  WORKMAN, COLEMAK, DVORAK,  PLOVER},       /* ROW 5 */
    {_______, _______, _______, _______, _______, _______},      /* ROW 6 */
    {_______, RAISE,   _______, _______, _______, _______}        /* ROW 7 */
  }
};

