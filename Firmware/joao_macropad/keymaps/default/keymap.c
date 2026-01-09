#include QMK_KEYBOARD_H

// --- OLED Menu Definitions ---
enum menu_options {
    OPT_COPY, OPT_PASTE, OPT_UNDO, OPT_REDO, OPT_SAVE, OPT_TASK, OPT_SCREEN, _MENU_COUNT
};
uint8_t current_menu_item = 0;

// --- RGB Color Cycle Definitions ---
static uint32_t rgb_colors[] = {
    RGB_RED,    // Red
    RGB_YELLOW, // Yellow
    RGB_GREEN,  // Green
    RGB_BLUE    // Blue
};
static uint8_t current_rgb_color_idx = 0;
static uint32_t color_timer = 0;
#define COLOR_CHANGE_INTERVAL 5000 // 5 seconds

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT(
        KC_1,    KC_2,    KC_MUTE,  // SW3: Encoder 1 Click (Mute)
        KC_3,    KC_4,    KC_NO,    // SW11: Encoder 2 Click (Select)
        KC_5,    KC_6,    KC_7      
    )
};

// --- Initialization and Background Tasks ---
void keyboard_post_init_user(void) {
    rgblight_enable_noeeprom(); 
    rgblight_set_color(rgb_colors[current_rgb_color_idx]); 
    color_timer = timer_read32(); 
}

void housekeeping_task_user(void) {
    // Cycle colors every 5 seconds
    if (timer_elapsed32(color_timer) > COLOR_CHANGE_INTERVAL) {
        current_rgb_color_idx = (current_rgb_color_idx + 1) % (sizeof(rgb_colors) / sizeof(rgb_colors[0]));
        rgblight_set_color(rgb_colors[current_rgb_color_idx]);
        color_timer = timer_read32();
    }
}

// --- Encoder Logic ---
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { // Volume Knob
        clockwise ? tap_code(KC_VOLU) : tap_code(KC_VOLD);
    } else if (index == 1) { // OLED Menu Navigation
        if (clockwise) {
            current_menu_item = (current_menu_item + 1) % _MENU_COUNT;
        } else {
            current_menu_item = (current_menu_item == 0) ? _MENU_COUNT - 1 : current_menu_item - 1;
        }
    }
    return true;
}

// --- Selection Logic (SW11 Click) ---
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Matches SW11 position: Row 1, Col 2
    if (record->event.row == 1 && record->event.col == 2 && record->event.pressed) {
        switch (current_menu_item) {
            case OPT_COPY:   tap_code16(C(KC_C)); break;
            case OPT_PASTE:  tap_code16(C(KC_V)); break;
            case OPT_UNDO:   tap_code16(C(KC_Z)); break;
            case OPT_REDO:   tap_code16(C(KC_Y)); break;
            case OPT_SAVE:   tap_code16(C(KC_S)); break;
            case OPT_TASK:   tap_code16(C(S(KC_ESC))); break;
            case OPT_SCREEN: tap_code16(G(S(KC_S))); break;
        }
        return false;
    }
    return true;
}

// --- OLED Interface ---
#ifdef OLED_ENABLE
bool oled_task_user(void) {
    oled_write_P(PSTR("--- JOAO PAD ---\n"), false);
    switch (current_menu_item) {
        case OPT_COPY:   oled_write_P(PSTR("> Copy        "), false); break;
        case OPT_PASTE:  oled_write_P(PSTR("> Paste       "), false); break;
        case OPT_UNDO:   oled_write_P(PSTR("> Undo        "), false); break;
        case OPT_REDO:   oled_write_P(PSTR("> Redo        "), false); break;
        case OPT_SAVE:   oled_write_P(PSTR("> Save        "), false); break;
        case OPT_TASK:   oled_write_P(PSTR("> Task Manager"), false); break;
        case OPT_SCREEN: oled_write_P(PSTR("> Screenshot  "), false); break;
    }
    return false;
}
#endif