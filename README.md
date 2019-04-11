# Amazfitbip-FreeRTOS
Warning: this is not (yet?) a complete replacement and was NOT tested on a real device. It is a starting project to experiment on the platform. At this stage,
the firmware must be loaded with an st-link v2 connected directly to SWDIO and SWCLK. You can find the location of these two pins [in this picture](https://github.com/amazfitbip/documentation/blob/master/images/Amazfit_BIP_top_00.png).

When loaded, it should print *hello world!* on the USART3 (PC10 as TX and PC11 as RX, also refer to the picture above).

I suggest to try this only on **bricked** devices. If you have one to donate to this cause, please contact me.

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

## Recommended readings
Beginning STM32: Developing with FreeRTOS, libopencm3 and GCC, by Gay Warren
