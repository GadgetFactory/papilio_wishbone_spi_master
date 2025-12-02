#include "WishboneSPI.h"

static SPIClass* _wbSpi = nullptr;
static int _wbCs = -1;
static bool _fpgaReady = false;

void wishboneInit(SPIClass* spi, int cs_pin) {
  _wbSpi = spi;
  _wbCs = cs_pin;
  pinMode(_wbCs, OUTPUT);
  digitalWrite(_wbCs, HIGH);
  
  // Wait for FPGA to be ready
  wishboneWaitForFPGA();
}

bool wishboneWaitForFPGA(unsigned long bootloaderDelayMs, unsigned long timeoutMs) {
  // Wait for FPGA bootloader (typically 3 seconds) plus margin
  Serial.println("Waiting for FPGA bootloader...");
  delay(bootloaderDelayMs);
  
  // Poll until we can communicate with the FPGA
  Serial.println("Waiting for FPGA to be ready...");
  unsigned long start = millis();
  int attempts = 0;
  
  while (millis() - start < timeoutMs) {
    if (wishboneIsReady()) {
      Serial.print("FPGA ready after ");
      Serial.print(attempts * 100);
      Serial.println("ms additional wait");
      _fpgaReady = true;
      return true;
    }
    delay(100);
    attempts++;
  }
  
  Serial.println("Warning: FPGA may not be responding correctly");
  _fpgaReady = false;
  return false;
}

bool wishboneIsReady() {
  if (!_wbSpi) return false;
  
  // Try to read video mode register at address 0x0000
  // Should return a valid mode (0, 1, or 2)
  _wbSpi->beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  digitalWrite(_wbCs, LOW);
  
  _wbSpi->transfer(0x00);  // CMD: Read command
  _wbSpi->transfer(0x00);  // ADDR_HIGH: 0x00
  _wbSpi->transfer(0x00);  // ADDR_LOW: 0x00
  delayMicroseconds(2);
  uint8_t mode = _wbSpi->transfer(0x00);  // DATA: read result
  
  digitalWrite(_wbCs, HIGH);
  _wbSpi->endTransaction();
  
  return (mode <= 2);
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
