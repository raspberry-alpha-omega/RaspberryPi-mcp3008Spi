#include "spi.h"

#define WRITE 0x40
#define READ  0x41

int main(void) {
  Spi lcd(1);

  unsigned char setup[] = { 
    WRITE, 0x0A, 0x08,
    READ,  0x0A, 0x00,
    WRITE, 0x00, 0xFF,
    WRITE, 0x0C, 0xFF,
    WRITE, 0x13, 0x00,
    WRITE, 0x01, 0x00,
    WRITE, 0x04, 0xFF
  };

  unsigned char all_on[] = {
    WRITE, 0x13, 0xFF
  };

  lcd.open();

  lcd.shift(setup, sizeof(setup));
  sleep(1);
  lcd.shift(all_on, sizeof(all_on));

  lcd.close();

  return 0;
}
