#include "WishboneSPI.h"

static SPIClass* _wbSpi = nullptr;
static int _wbCs = -1;

void wishboneInit(SPIClass* spi, int cs_pin) {
  _wbSpi = spi;
  _wbCs = cs_pin;
  pinMode(_wbCs, OUTPUT);
  digitalWrite(_wbCs, HIGH);
}

void wishboneWrite8(uint16_t address, uint8_t data) {
  if (!_wbSpi) return;
  _wbSpi->beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));
  digitalWrite(_wbCs, LOW);
  _wbSpi->transfer(CMD_WRITE);
  _wbSpi->transfer(address & 0xFF);     // 8-bit address
  _wbSpi->transfer(data);
  digitalWrite(_wbCs, HIGH);
  _wbSpi->endTransaction();
  delay(1);
}

uint8_t wishboneRead8(uint16_t address) {
  uint8_t result = 0;
  if (!_wbSpi) return result;
  _wbSpi->beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE1));
  digitalWrite(_wbCs, LOW);
  _wbSpi->transfer(CMD_READ);
  _wbSpi->transfer(address & 0xFF);
  // Some bridges return data immediately; perform a transfer to clock it out
  result = _wbSpi->transfer(0x00);
  digitalWrite(_wbCs, HIGH);
  _wbSpi->endTransaction();
  return result;
}

// 16-bit address, 8-bit data write (for full 16-bit address space)
void wishboneWrite16(uint16_t address, uint8_t data) {
  if (!_wbSpi) return;
  _wbSpi->beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));
  digitalWrite(_wbCs, LOW);
  _wbSpi->transfer(CMD_WRITE);
  _wbSpi->transfer((address >> 8) & 0xFF);  // Address high byte
  _wbSpi->transfer(address & 0xFF);          // Address low byte
  _wbSpi->transfer(data);
  digitalWrite(_wbCs, HIGH);
  _wbSpi->endTransaction();
  delay(1);
}

// 16-bit address, 8-bit data read (for full 16-bit address space)
uint8_t wishboneRead16(uint16_t address) {
  uint8_t result = 0;
  if (!_wbSpi) return result;
  _wbSpi->beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE0));
  digitalWrite(_wbCs, LOW);
  _wbSpi->transfer(CMD_READ);
  _wbSpi->transfer((address >> 8) & 0xFF);  // Address high byte
  _wbSpi->transfer(address & 0xFF);          // Address low byte
  result = _wbSpi->transfer(0x00);           // Read result
  digitalWrite(_wbCs, HIGH);
  _wbSpi->endTransaction();
  return result;
}

void wishboneWrite(uint32_t address, uint32_t data) {
  if (!_wbSpi) return;
  _wbSpi->beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE1));
  digitalWrite(_wbCs, LOW);
  _wbSpi->transfer(CMD_WRITE);
  _wbSpi->transfer((address >> 24) & 0xFF);
  _wbSpi->transfer((address >> 16) & 0xFF);
  _wbSpi->transfer((address >> 8) & 0xFF);
  _wbSpi->transfer(address & 0xFF);
  _wbSpi->transfer((data >> 24) & 0xFF);
  _wbSpi->transfer((data >> 16) & 0xFF);
  _wbSpi->transfer((data >> 8) & 0xFF);
  _wbSpi->transfer(data & 0xFF);
  digitalWrite(_wbCs, HIGH);
  _wbSpi->endTransaction();
}

uint32_t wishboneRead(uint32_t address) {
  uint32_t data = 0;
  if (!_wbSpi) return data;
  _wbSpi->beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE1));
  digitalWrite(_wbCs, LOW);
  _wbSpi->transfer(CMD_READ);
  _wbSpi->transfer((address >> 24) & 0xFF);
  _wbSpi->transfer((address >> 16) & 0xFF);
  _wbSpi->transfer((address >> 8) & 0xFF);
  _wbSpi->transfer(address & 0xFF);
  data |= ((uint32_t)_wbSpi->transfer(0x00) << 24);
  data |= ((uint32_t)_wbSpi->transfer(0x00) << 16);
  data |= ((uint32_t)_wbSpi->transfer(0x00) << 8);
  data |= (uint32_t)_wbSpi->transfer(0x00);
  digitalWrite(_wbCs, HIGH);
  _wbSpi->endTransaction();
  return data;
}
