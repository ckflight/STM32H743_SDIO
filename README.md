# STM32H743 SDMMC Raw Read/Write DMA Test

This project is a bare-metal / STM32CubeIDE test application for validating **high-speed raw SD card sector write and readback** on **STM32H743** using **SDMMC1 + DMA**.

It demonstrates:
- multi-block raw sector writes
- DMA-based raw sector reads
- cache-coherent buffer handling on STM32H7
- large-scale data integrity verification (no FatFS)

---

## Features

- STM32H743 + SDMMC1 (4-bit mode)
- DMA-based multi-block write/read
- D-Cache aware implementation
- 32-byte aligned DMA buffers
- FSM-based control flow
- Byte-by-byte verification
- ~18 MB/s write throughput (measured)

---

## Test Configuration

### SDMMC Setup

- Peripheral: `SDMMC1`
- Bus width: `4-bit`
- Clock edge: `RISING`
- Hardware flow control: `ENABLE`
- Clock divider: `0`
- Speed mode: `AUTO`

---

### Write Phase

- `NUMBER_OF_SECTORS_TO_WRITE = 450`
- Buffer size: `450 × 512 = 230400 bytes`
- Repeated: `512` times

**Total written:**

- Sectors: `512 × 450 = 230400`
- Bytes: `230400 × 512 = 117964800`
- Size: ~**112.5 MB**

---

### Read Phase

- `NUMBER_OF_SECTORS_TO_READ = 256`
- Buffer size: `256 × 512 = 131072 bytes`
- Read back in chunks and verified

---

## State Machine
