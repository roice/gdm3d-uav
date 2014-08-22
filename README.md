gdm3d-uav
=========

Goal: receive mavlink messages from APM/Autopilot via UART link, ADC analog chanels via SPI, and send mavlink messages through udp protocal(WIFI).

Status
------

The following table sumarizes the *development status*:

| Milestone | Status |
| ----------|--------|
| UART receive MAVLINK protocal messages from autopilot board ( e.g. APM ) | Ok |
| UDP broacasting MAVLINK protocal messages to ground station ( Hardware: BeagleBone Black board connect with USB-Wifi adapter )| Coded |
| ADC converting the analog channels through SPI port | WIP |
| Test with UAV and qgroundcontrol | WIP |


*Status: `Ok`, `Coded` (needs test), `WIP` (work in progress), `Issue`, `ToDo`*

Programming Frame:
-------------

| Timeline | Anuj Deshpande| Siddharth Bharat Purohit| Víctor Mayoral Vilches |
|----------|-------------- |-------------------------|------------------------|
| **Community Bonding** (21st April - 19th May 4 weeks)|Have already begun interacting with the community on ardupilot-discuss, #beaglepilot.Would be possible to being working in the “community bonding” period. | Coding started and trying to pull the timeline as up as possible leaving voids to do more :)| Already started coding and the discussion at #beaglepilot (Freenode). The mentors provided great feedback and tutorials to get started. The goal will be to get familiar with ArduPilot and start coding drivers (userspace ones at the beginning) for the hardware to be used (PixHawk Fire Cape). |

Test Platforms:
-----------
BeaglePilot is currently being tested using the following drones:
- [Erle Robot](http://erlerobot.com)
- BeagleBone Black + PixHawk Fire Cape

Communication & Resources:
--------------
- IRC Freenode #beaglepilot ([logs](http://logs.nslu2-linux.org/livelogs/beaglepilot/))
- [BeaglePilot mailing list](https://groups.google.com/forum/#!forum/beaglepilot)
- [eLinux Wiki](http://elinux.org/BeagleBoard/GSoC/BeaglePilot)
- [BeagleBoard project page](http://beagleboard.org/project/BeaglePilot/)
- [ArduPilot Wiki](http://dev.ardupilot.com/wiki/beaglepilot/)
- [BeaglePilot Blog](http://ardupilotbeaglebone.wordpress.com/)

[![Visit our IRC channel](https://kiwiirc.com/buttons/chat.freenode.net/beaglepilot.png)](https://kiwiirc.com/client/chat.freenode.net/?nick=beaglepilo|?#beaglepilot)
