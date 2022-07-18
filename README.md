# nrf52-rtt-demo

- This demo requires an [NRF52-DK](https://www.nordicsemi.com/Products/Development-hardware/nrf52-dk) development board running the [DAPLink](https://daplink.io/) MCU firmware.

- Download the [nRF52 SDK](https://www.nordicsemi.com/Products/Development-hardware/nrf52-dk) and add a symlink to it in the root folder of this repo.

- Adjust the `Makefile` as needed and compile using `make`. Flash via [pyOCD](https://pyocd.io/) using `make flash`.

- launch an RTT terminal using `pyocd rtt` or using [DAP.js](https://github.com/ARMmbed/dapjs)'s [web-based RTT terminal](https://github.com/ARMmbed/dapjs/tree/master/examples/rtt).
