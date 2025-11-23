# Gateware for papilio_wishbone_spi_master

This directory contains the FPGA-side Verilog modules associated with the `papilio_wishbone_spi_master` library.

Files:
- `simple_spi_wb_bridge_debug.v` - SPI-to-Wishbone bridge with UART debug output.
- `wb_address_decoder.v` - Simple address decoder that maps address regions to slaves.
- `uart_tx.v` - Minimal UART transmitter used by the bridge for debug output.

Usage:
- Copy these files into your FPGA project `src/gateware` folder or include the folder in your Gowin project file (`.gprj`).
- The MCU library expects the bridge and decoder to exist in the FPGA design and to expose an 8-bit Wishbone bus.
- Ensure your `top.v` instantiates the bridge and decoder with matching port names (see project example `top.v`).

Build:
- Build the gateware using the vendor build flow (Gowin IDE / `gw_sh.exe build.tcl`).
