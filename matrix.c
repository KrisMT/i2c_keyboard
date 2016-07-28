#include "keycode.h"
#include <avr/io.h>

unsigned char matrix[4][MATRIX_ROWS * MATRIX_COLS];
unsigned char *matrix_next = matrix[0];
unsigned char *matrix_curr = matrix[1];
unsigned char *matrix_debounced = matrix[2];
unsigned char *matrix_report = matrix[3];

typedef struct {
  unsigned char curr_layer;
  unsigned char layer[4];
}keyboard_t;

typedef struct {
  unsigned char modified;
  unsigned char report_id;
  unsigned char keys[13];
  unsigned char key;
}report_t;

report_t report;

const unsigned char keys_mask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

unsigned char BCD_LUT[16] = {0b01111110, 0b00110000, 0b01101101, 0b01111001, 0b00110011, 0b01011011, 0b01011111, 0b01110000,
                             0b01111111, 0b01111011, 0b01110111, 0b00011111, 0b01001110, 0b00111101, 0b01001111, 0b01000111};

unsigned char read_row(unsigned char pin)
{
  unsigned char data;
  PORTA = ~keys_mask[pin];
  _delay_ms(5);
  data = ~PINC;
  return (data >> 2);
}

void out_led(unsigned char val)
{
  PORTD = ~BCD_LUT[val];
}

void matrix_scan()
{
  unsigned char i;
  unsigned char data;

  //_delay_ms(20); //#include<util/delay.h>
  for(i=0; i<MATRIX_ROWS; i++)
  {
    data = read_row(i);
    matrix_next[i*MATRIX_COLS + 0] = (data&0x01);
    matrix_next[i*MATRIX_COLS + 1] = (data&0x02);
    matrix_next[i*MATRIX_COLS + 2] = (data&0x04);
    matrix_next[i*MATRIX_COLS + 3] = (data&0x08);
    matrix_next[i*MATRIX_COLS + 4] = (data&0x10);
    matrix_next[i*MATRIX_COLS + 5] = (data&0x20);
  }
}

void take_action(keyboard_t* record)
{
  unsigned char i;
  unsigned char keycode;
  unsigned char pos, index;

  memset(&report, 0, sizeof(report_t));

  for(i=0; i< MATRIX_ROWS*MATRIX_COLS; i++)
  {
    keycode = matrix_report[i];
    pos = keycode >> 3;
    index = keycode & 0x07;
    switch(keycode)
    {
      case KC_A ... KC_KP_EQUAL: //NKRO 103 key
        report.keys[pos] |= keys_mask[index];
        break;

        // Another layer
      case 0xB0 ... 0xB4:    //layer 0 ... 4
        record->layer[0] = (keycode & 0x0F);
        break;
      case 0xB5 ... 0xBC:    //layer 5 ... 12
        break;
      case 0xBD ... 0xBE:    //layer RAISE and LOWER >> do nothing
        break;
      case 0xBF:       //layer 15
        record->layer[0] = (keycode & 0x0F);
        break;

      case KC_TILD:
        report.modified |= keys_mask[1];
        report.keys[KC_GRV>>3] |= keys_mask[KC_GRV & 0x07];
        break;
      case KC_EXLM:
        report.modified |= keys_mask[1];
        report.keys[KC_1>>3] |= keys_mask[KC_1 & 0x07];
        break;
      case KC_AT:
        report.modified |= keys_mask[1];
        report.keys[KC_2>>3] |= keys_mask[KC_2 & 0x07];
        break;
      case KC_HASH:
        report.modified |= keys_mask[1];
        report.keys[KC_3>>3] |= keys_mask[KC_3 & 0x07];
        break;
      case KC_DLR:
        report.modified |= keys_mask[1];
        report.keys[KC_4>>3] |= keys_mask[KC_4 & 0x07];
        break;
      case KC_PERC:
        report.modified |= keys_mask[1];
        report.keys[KC_5>>3] |= keys_mask[KC_5 & 0x07];
        break;
      case KC_CIRC:
        report.modified |= keys_mask[1];
        report.keys[KC_6>>3] |= keys_mask[KC_6 & 0x07];
        break;
      case KC_AMPR:
        report.modified |= keys_mask[1];
        report.keys[KC_7>>3] |= keys_mask[KC_7 & 0x07];
        break;
      case KC_ASTR:
        report.modified |= keys_mask[1];
        report.keys[KC_8>>3] |= keys_mask[KC_8 & 0x07];
        break;
      case KC_LPRN:
        report.modified |= keys_mask[1];
        report.keys[KC_9>>3] |= keys_mask[KC_9 & 0x07];
        break;
      case KC_RPRN:
        report.modified |= keys_mask[1];
        report.keys[KC_0>>3] |= keys_mask[KC_0 & 0x07];
        break;
      case KC_UNDS:
        report.modified |= keys_mask[1];
        report.keys[KC_MINUS>>3] |= keys_mask[KC_MINUS & 0x07];
        break;
      case KC_PLUS:
        report.modified |= keys_mask[1];
        report.keys[KC_EQUAL>>3] |= keys_mask[KC_EQUAL & 0x07];
        break;
      case KC_LCBR:
        report.modified |= keys_mask[1];
        report.keys[KC_LBRACKET>>3] |= keys_mask[KC_LBRACKET & 0x07];
        break;
      case KC_RCBR:
        report.modified |= keys_mask[1];
        report.keys[KC_RBRACKET>>3] |= keys_mask[KC_RBRACKET & 0x07];
        break;
      case KC_PIPE:
        report.modified |= keys_mask[1];
        report.keys[KC_BSLASH>>3] |= keys_mask[KC_BSLASH & 0x07];
        break;
      case KC_MUTE:
        report.key = 0xA4;
        break;
      case KC_VOLU:
        report.key = 0x18;
        break;
      case KC_VOLD:
        report.key = 0x19;
        break;

      case 0xE0 ... 0xE7:   //Modified key
        report.modified |= keys_mask[index];
        break;
    }
  }
}

void take_keycode(keyboard_t* record)
{
  unsigned char layer = record->layer[record->curr_layer];
  unsigned char i, j;
  for(i=0; i< MATRIX_ROWS; i++)
    for(j=0; j< MATRIX_COLS; j++)
    {
      matrix_report[i*MATRIX_COLS + j] = (matrix_debounced[i*MATRIX_COLS + j])?pgm_read_byte(&(keymaps[layer][i][j])):0;
    }
}

void take_tri_layer(keyboard_t* record)
{
  unsigned char LOWER_key;
  unsigned char RAISE_key;
  unsigned char layer = record->layer[record->curr_layer];
  LOWER_key = (pgm_read_byte(&(keymaps[layer][3][4])) == LOWER)?matrix_debounced[22]:0;
  RAISE_key = (pgm_read_byte(&(keymaps[layer][7][1])) == RAISE)?matrix_debounced[43]:0;

  if(LOWER_key && RAISE_key)
  {
    record->curr_layer = 3;

    record->layer[0] = (matrix_debounced[31])?_QWERTY :record->layer[0];
    record->layer[0] = (matrix_debounced[32])?_WORKMAN:record->layer[0];
    record->layer[0] = (matrix_debounced[33])?_COLEMAK:record->layer[0];
    record->layer[0] = (matrix_debounced[34])?_DVORAK :record->layer[0];
    record->layer[0] = (matrix_debounced[35])?_PLOVER :record->layer[0];
  }
  else if(RAISE_key) record->curr_layer = 2;
  else if(LOWER_key) record->curr_layer = 1;
  else               record->curr_layer = 0;

  //Take LED layer PB3 ~ PB0
  layer = record->layer[record->curr_layer];
  out_led(layer);
}

void matrix_debounce()
{
  unsigned char i;
  for(i=0; i< MATRIX_ROWS*MATRIX_COLS; i++)
  {
    matrix_debounced[i] = (matrix_next[i] == matrix_curr[i])?matrix_curr[i]:0;
  }
}

void swap_matrix()
{
  unsigned char *ptr_matrix_swap;
  /* Swap matrix */
  ptr_matrix_swap = matrix_next;
  matrix_next     = matrix_curr;
  matrix_curr     = ptr_matrix_swap;
}

keyboard_t record = {
  .curr_layer = 0,
  .layer = {_QWERTY, _LOWER, _RAISE, _ADJUST}
};

void keyboard_task(volatile unsigned char *buff)
{
  //unsigned char i, j;
  matrix_scan();
  matrix_debounce();
  take_tri_layer(&record);
  take_keycode(&record);
  take_action(&record);
  swap_matrix();
  memcpy((void*)buff, &report, 16);
  /*
  for( i=0; i<MATRIX_ROWS; i++)
    for( j=0; j<MATRIX_COLS; j++)
    {
      buff[0x80 + i*16 + j] = matrix_debounced[i*MATRIX_ROWS + j];
    }
  */
}
