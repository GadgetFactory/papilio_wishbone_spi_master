#ifndef WISHBONESPI_H
#define WISHBONESPI_H

#include <Arduino.h>
#include <SPI.h>

// Initialize the Wishbone SPI helper with an SPIClass instance and CS pin
void wishboneInit(SPIClass* spi, int cs_pin);

// 8-bit access helpers (8-bit address, 8-bit data)
void wishboneWrite8(uint16_t address, uint8_t data);
uint8_t wishboneRead8(uint16_t address);

// 16-bit address, 8-bit data helpers (for larger address spaces like debug regs)
void wishboneWrite16(uint16_t address, uint8_t data);
uint8_t wishboneRead16(uint16_t address);

// 32-bit access helpers
void wishboneWrite(uint32_t address, uint32_t data);
uint32_t wishboneRead(uint32_t address);

// Wishbone SPI protocol command values (master-side)
#define CMD_WRITE 0x01
#define CMD_READ  0x02

#endif // WISHBONESPI_H
