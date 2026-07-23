# Pin Assignment

This document keeps track of the STM32U083RC pin assignments as the project develops.

| Peripheral | MCU Pin(s)           | Purpose                 | Notes                                                     |
| ---------- | -------------------- | ----------------------- | --------------------------------------------------------- |
| SWD        | PA13, PA14           | Programming & Debugging | Reserved for ST-LINK                                      |
| I2C1       | PB8 (SCL), PB9 (SDA) | BME280, OLED            | Using Arduino I2C pins instead of CubeMX default PA9/PA10 |

## Future Assignments

| Peripheral            | Planned Pins | Status       |
| --------------------- | ------------ | ------------ |
| GPS (UART)            | TBD          | Not assigned |
| Battery Monitor (ADC) | TBD          | Not assigned |
| Buttons               | TBD          | Not assigned |

## Notes

* Prefer pins that make sense for the final PCB, not just the Nucleo board.
* Update this file whenever a new peripheral is configured.
* Check for pin conflicts before assigning a new peripheral.
