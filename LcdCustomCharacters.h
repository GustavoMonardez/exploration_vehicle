/**
 * @file LcdCustomCharacters.h
 *
 * @brief Display custom characters
 *
 * @author Gustavo Monardez
 *
 */
#pragma once

uint8_t select_arrow[8] = {
	B00000,
	B00100,
	B00110,
	B11111,
	B00110,
	B00100,
	B00000,
};

uint8_t back_arrow[8] = {
  B00000,
  B00101,
  B01101,
  B11111,
  B01100,
  B00100,
  B00000,
  B00000
};

uint8_t thermometer[8] = {
  B00100,
  B01110,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00000
};

uint8_t battery[8] = {
  B00110,
  B01111,
  B01001,
  B01111,
  B01111,
  B01111,
  B01111,
  B00000
};

uint8_t dot[8] = {
  B00000,
  B00000,
  B01100,
  B11110,
  B11110,
  B01100,
  B00000,
  B00000
};

uint8_t percent[8] = {
  B11000,
  B11001,
  B00010,
  B00100,
  B01000,
  B10011,
  B00011,
  B00000
};

uint8_t sun[8] = {
  B00100,
  B10101,
  B01110,
  B11011,
  B01110,
  B10101,
  B00100,
  B00000
};

uint8_t blank[8] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000
};
