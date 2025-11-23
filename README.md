# Papilio Wishbone SPI Master — `lib/papilio_wishbone_spi_master`

This library provides a simple SPI-based master for talking to an FPGA-side Wishbone bridge. It implements small helpers for 8-bit and 32-bit register accesses.

Files
- `src/WishboneSPI.h` — Public header with API and protocol constants (`CMD_WRITE`, `CMD_READ`).
- `src/WishboneSPI.cpp` — Implementation using Arduino `SPIClass`.

Usage
- Call `wishboneInit(spi, csPin)` to initialize.
- Use `wishboneWrite8`, `wishboneRead8`, `wishboneWrite`, `wishboneRead` for register accesses.

Notes
- The library defines `CMD_WRITE` / `CMD_READ` macros for the serial protocol; those are intended to be used by clients that implement or debug the bridge protocol.
