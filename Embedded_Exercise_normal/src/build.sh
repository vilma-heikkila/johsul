#!/bin/sh

XILINX_SDK='/opt/xilinx/SDK/2019.1/'
workspace="$HOME/tty/2021-2022/COMP.CE.100/johsul"
project='Embedded_Exercise_normal'

"$XILINX_SDK"/bin/xsct << EOF
setws $workspace
projects -type app -name $project -build
EOF

