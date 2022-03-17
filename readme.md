# Picotronix: Pico-based driver for Tektronix vector displays.

Picotronix is an adapter board that can interface with Tektronix 603/604 series vector displays,
and is capable of using the storage features if they are available.

It connects to the remote DB25 connector to provide all the inputs and controls, and can be interfaced
over usb for PC control or directly run it's own visuals.

It also has two ESP-01-compatible pin headers for IO expansions to meet specific application needs.




## Project Structure

Since this project is multi-domain (PC library, RP2040 Firmware, and KiCAD PCB),
the project is split up into respective subfolders. To facilitate communication between the Pico and
the PC, `Cap'n Proto` is used as a protocol exchange library generator.
