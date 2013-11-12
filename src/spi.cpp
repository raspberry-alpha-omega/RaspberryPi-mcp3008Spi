#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <iostream>
#include <sstream>

#include "spi.h"

using namespace std;

/**********************************************************
 * Open the spidev device "dev" and the setup the spidev 
 * interface.
 * private member variables are used to configure spidev.
 * They must be set appropriately by constructor before calling
 * this function.
 * *********************************************************/
int Spi::open() {
  if (OPEN == this->status) return 0;

  int ret = -1;
  this->spifd = ::open(this->dev.c_str(), O_RDWR);
  if(this->spifd < 0){
    perror("could not open SPI device");
    this->status = ERROR;
  }

  ret = ioctl(this->spifd, SPI_IOC_WR_MODE, &(this->mode));
  if(ret < 0){
    perror("Could not set SPIMode (WR)...ioctl fail");
    this->status = ERROR;
  }
 
  ret = ioctl(this->spifd, SPI_IOC_RD_MODE, &(this->mode));
  if(ret < 0) {
    perror("Could not set SPIMode (RD)...ioctl fail");
    this->status = ERROR;
  }
 
  ret = ioctl(this->spifd, SPI_IOC_WR_BITS_PER_WORD, &(this->bitsPerWord));
  if(ret < 0) {
    perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
    this->status = ERROR;
  }
 
  ret = ioctl(this->spifd, SPI_IOC_RD_BITS_PER_WORD, &(this->bitsPerWord));
  if(ret < 0) {
    perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
    this->status = ERROR;
  }  
 
  ret = ioctl(this->spifd, SPI_IOC_WR_MAX_SPEED_HZ, &(this->speed));    
  if(ret < 0) {
    perror("Could not set SPI speed (WR)...ioctl fail");
    this->status = ERROR;
  }
 
  ret = ioctl(this->spifd, SPI_IOC_RD_MAX_SPEED_HZ, &(this->speed));    
  if(ret < 0) {
    perror("Could not set SPI speed (RD)...ioctl fail");
    this->status = ERROR;
  }

  return ret;
}
 
int Spi::close() {
  if (CLOSED == this->status) return 0;

  int ret = -1;
  ret = ::close(this->spifd);
  if(ret < 0) {
    perror("Could not close SPI device");
    this->status = ERROR;
  }

  return ret;
}
 
/********************************************************************
 * push "length" bytes of data to the spidev device. 
 * Data shifted in from the spidev device is saved back into "data".
 * ******************************************************************/
int Spi::shift(uint8_t *data, int length) {
  struct spi_ioc_transfer spi[length];
 
  for (int i = 0 ; i < length ; ++i) {
    spi[i].tx_buf = (uint64_t)(data + i); // transmit from "data"
    spi[i].rx_buf = (uint64_t)(data + i) ; // receive into "data"
    spi[i].len = sizeof(*(data + i)) ;
    spi[i].delay_usecs = 0 ;
    spi[i].speed_hz = this->speed ;
    spi[i].bits_per_word = this->bitsPerWord ;
    spi[i].cs_change = 0;
  }

  int ret = ioctl(this->spifd, SPI_IOC_MESSAGE(length), &spi) ;
  if(ret < 0){
    perror("Problem transmitting spi data..ioctl");
    this->status = ERROR;
  }
 
  return ret;
}

void Spi::init(std::string dev, unsigned char mode, unsigned int speed, unsigned char bits) {
  this->dev = dev;
  this->mode = mode ;
  this->speed = speed;
  this->bitsPerWord = bits;

  this->spifd = -1;
}

Spi::Spi(std::string dev, unsigned char mode, unsigned int speed, unsigned char bits){
  init(dev, mode, speed, bits);
}

Spi::Spi() {
  init(std::string("/dev/spidev0.0"), SPI_MODE_0, 4000000, 8);
}

std::string operator+(std::string const &a, int b){
  std::ostringstream oss;
  oss<<a<<b;
  return oss.str();
}

Spi::Spi(int dev) {
  init(std::string("/dev/spidev0.") + dev, SPI_MODE_0, 4000000, 8);
}

Spi::~Spi(){
  this->close();
}
