# Description of files
- descriptor_mode\<n\>.txt: These are the descriptors that `lsusb -v`
  outputs in each of the 4 operating modes of the CH347.

- mode2_n_hid_desc.txt:
The USB HID descriptors were dumped using `usbhid-dump -d 1a86:55db -e d` 
which was then passed through `hidrd-convert -i hex -o spec`
to convert them into a human readable format.
