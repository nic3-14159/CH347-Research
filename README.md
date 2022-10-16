# CH347 Research
This project aims to document the USB packet format of the WCH CH347, a
high-speed USB chip providing UART, I2C, SPI, and JTAG interfaces. The official
[datasheet](http://www.wch-ic.com/downloads/CH347DS1_PDF.html) does not have any
information about the low level programming interface and mainly covers
electrical specifications and functional descriptions. There is a [demonstration
application](http://www.wch-ic.com/downloads/CH347EVT_ZIP.html) for Windows
which does include source code, but this is only for the application itself and
relies on calls into a proprietary library (CH347DLL) of which the API is
documented in an included application development manual.

Although this library could theoretically be disassembled and reverse
engineered, simply calling the functions and observing USB packets in tools such
as Wireshark can often be easier, and this is the route taken by this project.
Included in this repo are the source code for tools developed to assist in such
a task, generated outputs, and analyses of the data. The library headers are not
included in this repo and can be found in the demonstration application. The
library DLL and vendor drivers are strangely not included in those downloads and
are found in an updated [CH341 driver
package](https://www.wch.cn/downloads/CH341PAR_ZIP.html) that appears to only be
availble on the Chinese version of WCH's website.
