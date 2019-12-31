#!/usr/bin/env python3

import os.path
if not os.path.isfile("_spacelcd.so"):
    print("The spacelcd python library is not available.")
    exit(1)

import sys
sys.path.append(".")

import spacelcd
import unittest


class Testspacelcd(unittest.TestCase):

    # def setUp(self):
    #	  self.spacelcd = spacelcd.reset()

    def test_01_stringSVGRed(self):
        svg = '<svg version="1.1" viewBox="0 0 640 150"><rect x="20" y="20" width="600" height="110" fill="#ff0000"/></svg>'
        spacelcd.svgtolcd(svg, spacelcd.scroll.none)

    def test_02_stringSVGGreen(self):
        svg = '<svg version="1.1" viewBox="0 0 640 150"><rect x="20" y="20" width="600" height="110" fill="#004400"/></svg>'
        spacelcd.svgtolcd(svg, spacelcd.scroll.none)

    def test_03_stringSVGBlue(self):
        svg = '<svg version="1.1" viewBox="0 0 640 150"><rect x="20" y="20" width="600" height="110" fill="#0000ff"/></svg>'
        spacelcd.svgtolcd(svg, spacelcd.scroll.none)

    def test_x_fileSVG(self):
        with open("../res/img/logo.svg", "r") as svgfile:
            svg = svgfile.readlines()[0]
            spacelcd.svgtolcd(svg, spacelcd.scroll.none)

    # def test_stringSVGGreenCorrupt(self):
    #	  svg = '<svg version="1.1" viewBox="0 0 640 150"><rect x="20" y="20" width="600" height="110" fill="#005500"/></svg>'
    #	  spacelcd.svgtolcd(svg, spacelcd.scroll.none)

    # def tearDown(self):
    #	  return None


if __name__ == '__main__':
    unittest.main()
