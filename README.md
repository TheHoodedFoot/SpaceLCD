# SpaceLCD

![SpaceLCD logo](/res/img/logo.svg)

**SpaceLCD** displays SVG images on the SpaceExplorer Enterprise 3D mouse. It can also contol the LCD brightness.

## Getting Started



### Prerequisites

SpaceLCD requires libusb to control the 3D mouse, librsvg to parse the SVG and also zlib to compress the images.

Optional requirements are [swig](http://www.swig.org) for generating the Python bindings, and [Unity](http://www.throwtheswitch.org/unity) for running the unit tests.

### Installing

Clone the repository and build the library and Python module:

```
git clone --recursive https://github.com/TheHoodedFoot/spacelcd.git
cd spacelcd
make
```

### Permissions

Spacelcd needs write permissions for the SpaceMouse. There are some example udev rules included.

## Code Example

~~~{.py}
import spacelcd

with open ("logo.svg", "r") as svgfile:
    svg=svgfile.readlines()

spacelcd.svgtolcd(svg[0], spacelcd.scroll.left)
~~~

![Photo showing SpaceLCD logo](/res/img/photo.jpg)
## Background

The SpaceMouse Enterprise has a 640x150 pixel display with 16-bit R5G6B5 colour support. Images are sent to the display with a 512-byte header, followed by the bitmap, which is compressed using raw deflate.

SpaceLCD is only used to control the display; [spacenavd](http://spacenav.sourceforge.net) and [libspnav](http://spacenav.sourceforge.net) are required in order to read the motion data from the mouse.

## Limitations

The official 3DConnexion driver has some support for animation, which is not currently understood and therefore not implemented here. The proprietary driver can also use partial screen updates, whereas SpaceLCD uploads the full screen each time.

SpaceLCD has only been tested using one SpaceExplorer Enterprise, with USB ID 256f:c633, bought as part of the SpaceMouse Enterprise Kit 3DX-700058.

The header uses a 16-bit value to hold the length of the compressed bitmap, limiting its size to 65535 bytes.

Operating system support is currently GNU/Linux only.

## Using the SpaceMouse Enterprise with as a 3d mouse with spacenavd

The current version of [spacenavd](http://spacenav.sourceforge.net), 0.6, does not include the USB vendor and product ids for the SpaceMouse Enterprise, so although SpaceLCD can control the display, the device itself will not work as a 3d mouse. A patch for spacenavd is included [here](/res/etc/add_spaceexplorer_enterprise.patch) which adds the SpaceMouse Enterprise as a recognised device. The patch also blacklists the CadMouse, which is a regular mouse that comes as part of the SpaceMouse Enterprise kit, which spacenavd mistakenly recognises as a 3d mouse.

## Bugs

### Device Lockup

The SpaceMouse is vulnerable to corrupted data, which can cause the display to become unresponsive, requiring a hard reset. SpaceLCD will also generate corrupt data under certain circumstances, which is being investigated.

### Endianness Issues

Because this code has only been developed on an x86 system, the endianness of 16-bit words has probably been taken for granted in several places, so it may work incorrectly on another architecture.

## Inkscape Plugin

The code also includes a plugin for Inkscape version 1.0, which requires [scour](http://github.com/scour-project/scour) to be installed. The plugin uploads the current Inkscape drawing to the SpaceMouse, which is useful for developing a GUI.

## Built With

* [libusb](https://libusb.info) - Portable, userspace USB access
* [zlib](https://www.zlib.net) - Compression algorithm used by the SpaceMouse
* [librsvg](https://github.com/GNOME/librsvg) - Used to generate a bitmap from SVG data

## Authors

* **Andrew Black** - *Initial work* - [github](https://github.com/TheHoodedFoot)

## License

This project is licensed under the zlib License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* spacenavd / libspnav
* zlib example
