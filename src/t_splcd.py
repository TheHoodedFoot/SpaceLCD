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
    #     self.spacelcd = spacelcd.t_spacelcd()

    def test_sendReceive(self):
        svg = "<svg width=\"640\" height=\"150\" version=\"1.1\"><rect x=\"20\" \"y=\"20\" width=\"600\" height=\"110\" ry=\"0\" " "fill=\"#ff8800\"/></svg>"

        # print(svg, file=sys.stderr)
        self.spacelcd = spacelcd.svgtolcd(svg, 0x15)
        return None

    # def tearDown(self):
    #     return None


if __name__ == '__main__':
    unittest.main()
