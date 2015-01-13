gdm3d-uav
=========

Goal: receive mavlink messages from APM/Autopilot via UART link, ADC analog chanels via SPI, and send mavlink messages through udp protocal(WIFI).

Status
------

The following table sumarizes the *development status*:

| Milestone | Status |
| ----------|--------|
| UART receive MAVLINK protocal messages from autopilot board ( e.g. APM ) | Ok |
| UDP broacasting MAVLINK protocal messages to ground station ( Hardware: BeagleBone Black board connect with USB-Wifi adapter )| Ok |
| ADC converting the analog channels through SPI port | Ok |
| Test with UAV and qgroundcontrol | Ok |


*Status: `Ok`, `Coded` (needs test), `WIP` (work in progress), `Issue`, `ToDo`*

Test Platforms:
-----------
Rsapberry pi A+

Usage:
-----------
gdm3d-uav -d <serial device node connecting APM radio port> -b <baudrate> -i <Ground station IP address>

Example:
----
gdm3d-uav -d /dev/ttyUSB0 -b 115200 -i 192.168.10.101
