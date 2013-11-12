/***********************************************************************
 * This header file defines a general-purpose SPI interface class.
 * Its main purpose is to communicate with the devices on the SPI interface 
 * using the userspace spidev facility.
 *
 * The class contains four variables:
 * mode        -> defines the SPI mode used. In our case it is SPI_MODE_0.
 * bitsPerWord -> defines the bit width of the data transmitted.
 *        This is normally 8. Experimentation with other values
 *        didn't work for me
 * speed       -> Bus speed or SPI clock frequency. According to
 *                https://projects.drogon.net/understanding-spi-on-the-raspberry-pi/
 *            It can be only 0.5, 1, 2, 4, 8, 16, 32 MHz.
 *                Will use 1MHz for now and test it further.
 * spifd       -> file descriptor for the SPI device
 *
 * The class contains two constructors that initialize the above
 * variables and then open the appropriate spidev device using spiOpen().
 * The class contains one destructor that automatically closes the spidev
 * device when object is destroyed by calling spiClose().
 * The spiWriteRead() function sends the data "data" of length "length"
 * to the spidevice and at the same time receives data of the same length.
 * Resulting data is stored in the "data" variable after the function call.
 * (Based on https://github.com/halherta/RaspberryPi-mcp3008Spi)
 * ****************************************************************************/
#ifndef SPI_H
#define SPI_H
     
#include <string>

using namespace std;

enum SpiStatus { CLOSED, OPEN, ERROR };
 
class Spi {
 
public:
    Spi(std::string dev, unsigned char mode, unsigned int speed, unsigned char bits);
    Spi(int dev);
    Spi();
    ~Spi();

    int open();
    int close();
    int shift(unsigned char *data, int length);
     
private:
    std::string dev;
    unsigned char mode;
    unsigned int speed;
    unsigned char bitsPerWord;

    SpiStatus status;
    int spifd;
     
    void init(std::string dev, unsigned char mode, unsigned int speed, unsigned char bits);
};
 
#endif
