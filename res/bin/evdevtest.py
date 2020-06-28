#!/usr/bin/env python

import evdev
import re

# It is also possible to use the evtest console command to examine button presses.

# Ensure that spacenavd is not running.

devices = [evdev.InputDevice(path) for path in evdev.list_devices()]
regex = re.compile('.*3Dconnexion SpaceMouse Enterprise.*')
for device in devices:
    print(device.path, device.name, device.phys)
    if(regex.match(device.name)):
        print("Found spacemouse: " + device.path)
        spacemouse = device.path

device = evdev.InputDevice(spacemouse)
print(device)

for event in device.read_loop():
    if event.type == evdev.ecodes.EV_KEY:
        print("Code: %s" % event.code)
