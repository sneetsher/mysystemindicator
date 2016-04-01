# mysystemindicator or SystemIndicatorTest

A system indicator test/demo for Ubuntu Unity desktop (It is NOT an AppIndicator). It could be showed on Unity panel in regular user session, lock screen, greeter screen and even on uniquity screen (Ubuntu installer). In similar way as the other Ubuntu/Canonical system indicators, example indicator-sound...etc

This something I was looking for it quiet some time, if you are looking to read a long story check here: [How to develop a System Indicator for Unity?](https://askubuntu.com/questions/690769/how-to-develop-a-system-indicator-for-unity)

Mean part of the code took from `libindicator` source code (exactly: `tests/indicator-test-service.c`)

## Installation

Steps to build & test

    mkdir build
    cd build
    camke -DCMAKE_INSTALL_PREFIX:PATH=/usr ..
    make
    sudo make install

To check quickly in greeter screen you need to reboot or restart lightdm

    sudo service lightdm restart

## Notes

- Exit does not work as expected because dbus respawn it. In greeter, it makes lightdm/X crash. It seems, lightdm does not expect remove of a system indicator.

    Better, to keep it this way as others may fall in the same pit.

- System indicator has advantage of loading dynamically generated icon through g_menu (over AppIndicator that suports only static file loading)

## Screenshots

- Regular user session

    ![screenshot/Screenshot from 2016-04-01 15-14-44 Regular User Session.png]

- Lock screen

    ![screenshot/Screenshot from 2016-04-01 15-15-07 Lock Screen 1.png] ![screenshot/Screenshot from 2016-04-01 15-15-41 Lock Screen 2.png]
    
- Greeter or Login screen