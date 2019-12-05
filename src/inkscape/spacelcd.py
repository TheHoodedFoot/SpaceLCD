#!/usr/bin/env python3.7

import sys
import os.path
if not os.path.isfile("/home/thf/projects/spacelcd/src/_spacelcd.so"):
    print("The spacelcd python library is not available.", file=sys.stderr)
    exit(1)

sys.path.append("/home/thf/projects/spacelcd/src")

from _spacelcd import svgtolcd

import inkex.command
import inkex.svg

# For duplicating and processing the SVG file
from subprocess import Popen, PIPE
from shutil import copy2

class ExportSpaceLCD(inkex.EffectExtension):

    def __init__(self):
        super(ExportSpaceLCD, self).__init__()

    def effect(self):
        # (ref, self.tmp_png) = tempfile.mkstemp('.png')
        # kwargs = {'export-file': self.tmp_png,
        #           'export-type': 'png',
        #           'export-width': 640,
        #           'export-height': 150 }
        # inkex.command.inkscape(self.options.input_file, **kwargs)
        # os.system('geeqie ' + self.tmp_png)
        # os.remove(self.tmp_png)
        
        # print(str(self.document.getroot().tostring()), file=sys.stderr)
        file = self.options.input_file
        tempfile = os.path.splitext(file)[0] + "-multicut.svg"
        copy2(file, tempfile)

        # Pipe the file through svg.c
        p = Popen(
            "cat " + tempfile + " | scour --enable-comment-stripping --strip-xml-prolog --remove-titles --remove-descriptions --remove-metadata --no-line-breaks --strip-xml-space --enable-id-stripping --shorten-ids > /tmp/out.txt",
            shell=True,
            stdout=PIPE,
            stderr=PIPE)
        f = p.communicate()[0]

        fh = open("/tmp/out.txt", 'r')
        svgtolcd(fh.read(), 0x11)
        fh.close()

        # return self.document

if __name__ == "__main__":
    ExportSpaceLCD().run()
