# Reverse-engineering the SpaceMouse Enterprise

I recently bought a SpaceMouse Enterprise 3d mouse. I have wanted one for some time, but the price has always been prohibitive. However, I recently noticed that Amazon were selling a kit containing the 3d mouse, a CadMouse, and a mouse mat.

I had looked through the forums and seen that 3dconnexion offered no linux support

I did not realise that the device would not work at all

Altered spacenavd

No screen action. FreeCad button problem

usbmon + wireshark

capture control signals and work out lcd brightness

## Wed 4th Dec

I still feel as though the rgb colour issue is due to the screen being bgr instead of rgb565, so I wrote a small c snippet to read the raw bitmap data and convert it. Since the decompressed data is little-endian and the bitmap needs to be big-endian, I converted it in the same program.

This proved successful, and the image captured from the USB can now be decompressed and displayed correctly.

Now to try and reverse the process.

## Friday, 6th December

Success.

After wondering why the compressed data was different, I had a further look at the zlib documentation. I discovered that there are extended commands, deflateInit2 and inflateInit2 that take extra arguments. One of these is windowBits, which switches compression and decompression to using raw deflate, which has no header. Since the data in the captured stream does not seem to have a header, this seemed promising.

Another parameter is the zlib strategy. There is an option of Z_FIXED, which disabled dynamic huffman codes. The zlib documentation states that this allows for simpler decoding, which makes sense as there is probably a relatively simple microcontroller on the SpaceMouse.

I altered the zpipe code to use the new commands and attempted to decompress and then recompress the captured stream. They matched. Well, the first bytes at least, but it showed that I was on the right track.

It is important to modify bytes 0x02 and 0x03 to reflect the length of the compressed data, as otherwise the decompression on the SpaceMouse can stall or become corrupted, requiring a hard reset. Since this length is a 16-bit value, it is necessary to ensure that the compressed bitmap is no bigger than 65535 bytes.

Draw a 640x150 pixel image in gimp.
Export as a bitmap with no colour space information, R5G6B5, flipped vertically
dd if=<existing bulk header> bs=1 count=512 > header.bulk
dd if=<bitmap> bs=1 skip=70 | ./bgr | ./zpipe > body.bulk
cat header.bulk body.bulk > combined.bulk
ls -al combined.bulk and modify bytes 0x02-0x03

## Saturday, 7th December

Now that I know that I can control the LCD, my next step is to consolidate the various steps and separate programs into a single binary. The plan is to be able to read an SVG or PNG file

One issue that cropped up is that sometimes the SpaceMouse is captured by the usb hid driver, and seen as some sort of ordinary mouse. 

I have had some problems with spacenavd crashing, or failing to find the SpaceMouse. I have patched spacenavd to add the SpaceMouse to the list of supported devices, but I have also added a blacklist to prevent spacenavd from grabbing the CadMouse, since spacenavd v0.6 seems to assume that any device made by 3DConnexion is a suitable candidate.

I wrote a quick extension for Inkscape, so that I can send images directly to the LCD from a menu. This will enable me to quickly test the appearance of various styles and layouts.
