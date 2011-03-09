#!/bin/sh

find . -iname *.dat -exec sed -i -e 's,MODIFICATOR_POINT_OPBOX_TYPE,MODIFICATOR_POINT_OPBOX,g' '{}' \;


