# Amazfitbip-FreeRTOS
Warning: this is not (yet?) a complete replacement. It is a starting project to experiment on the platform. At this stage,
the firmware must be loaded with an st-link v2 connected directly to SWDIO and SWCLK.

[Information on the hardware](https://github.com/amazfitbip/documentation)

## How to compile
Download the toolchain [from here](https://developer.arm.com/open-source/gnu-toolchain/gnu-rm/downloads).

Extract the folder to your preferred location, e.g. to your home folder. Then:

    export PATH="$PATH:$HOME/gcc-arm-none-eabi-8-2018-q4-major/bin/"

Remember to change the name accordingly.

Now clone this repository and compile:

    git clone --recursive https://github.com/dslul/Amazfitbip-FreeRTOS
    cd Amazfitbip-FreeRTOS
    make
