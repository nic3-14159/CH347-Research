# USB Transfer Format
## SPI Interface, mode 1
- Byte 0: command
- Byte 1-2: data length, LSB
- Byte 3-n: data

### Commands
- **0xC0**: SPI Interface configuration
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

- **0xC1**: CS control
	- Asserts/deasserts the CS pins
	- USB Bulk Out, data format described below
	- Mask bit: 0 = leave the pin as is, 1 = change the pin
	- Value bit: 0 asserts CS, 1 deasserts CS. The physical behavior of
	the pin depends on the polarity settings. In the active low setting,
	setting the value 0 will drives the pin low, while in the active high
	setting, the pin will be driven high for the same 0 value.
```
c1 0a00 80 00000000 00 00000000
        |           |--Byte 8, bit 7: CS2 mask
        |                      bit 6: CS2 value
        |--Byte 3, bit 7: CS1 mask
                   bit 6: CS1 value
```

- **0xC2**: Write/Read data
	- Shifts out a block of data and returns the data that was shifted in at
the same time
	- Request: USB Bulk Out, data is the bytes to be shifted out
	- Response: USB Bulk In, data is the bytes that were shifted in

- **0xC3**: Read data
	- Shifts in an arbitrary number of bytes
	- Request: USB Bulk Out, data is a 32-bit integer indicating the number
	  of bytes to be read, sent LSB
	- Response: USB Bulk In, data is the data that was read

- **0xC4**: Write data
	- Shift out an arbitrary number of bytes
	- Request: USB Bulk Out, data is the data to be shifted out
	- Response: USB Bulk In, data is a single byte, should be 0

- **0xCA**: Read SPI Config
	- Data format is the same as for command 0xC0
