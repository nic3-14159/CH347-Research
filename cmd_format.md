# USB Transfer Format
## SPI Interface, mode 1
- Byte 0: command
- Byte 1-2: data length, LSB
- Byte 3-n: data

## Commands
- 0xC0: SPI Interface configuration
	- Must be preceded by a Read SPI Config command
	- Must be followed by a USB BULK IN request to receive the status
```
c0 1a00 000004010000 00 00 00 000002 00 00 80 000700 0000 00 00 00000000
                      |     |        |     |         |    |  |--CS Polarity: bit 7 CS2
                      |     |        |     |         |    |                  bit 6 CS1
                      |     |        |     |         |    |                  0 = active low
                      |     |        |     |         |    |--Byte 23: SPI Out Default Data
                      |     |        |     |         |--Byte 21-22: SPI operation Interval
                      |     |        |     |--Byte 17 bit 7: byte order: 1 = LSB, 0 = MSB
                      |     |        |--Byte 15 bit 5:3: clock divisor, values from 0-7
                      |     |--Byte 11, bit 0: Clock phase
                      |--Byte 9 bit 1: Clock polarity
```
- 0xC1: CS control
- 0xC2: Write/Read data
	- Shifts out a block of data and returns the data that was shifted in at
the same time
- 0xC3: Read data
	- Shifts in an arbitrary number of bytes
- 0xC4: Write data
	- Shift out an arbitrary number of bytes
        - Must be followed by a USB BULK IN request to retrieve the status
- 0xCA: Read SPI Config
        - Data format is the same as for command 0xC0
