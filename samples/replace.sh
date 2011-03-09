#!/bin/sh

find . -iname \*.dat -exec sed -i -e 's,final_height,pixel_height_final,g' '{}' \;


