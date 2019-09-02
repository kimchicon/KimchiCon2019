

# Using M5Stack With Arduino IDE

You can follow instructions from the following document to flash applications from Arduino IDE.

[M5Core Quick Start - Arduino Win](https://docs.m5stack.com/#/en/quick_start/m5core/m5stack_core_get_started_Arduino_Windows)

* [Xtensa Instruction Set Architecture (ISA)](https://0x04.net/~mwk/doc/xtensa.pdf)

## Windowed registers

```
That’s probably one of the most special feature of the LX6, so that’s what I’ll talk about. The processor contains 64 32-bit registers but only an interval of 16 registers can be seen at each instant. A 4-bit WindowBase special register chooses which range of registers is visible and addressable. This register can be modified by WSR (Write to Special Register) but other instructions have special mechanisms with this feature.
```
* [The Xtensa architecture](https://www.lortex.org/posts/esp32/2018/03/28/the-xtensa-architecture.html)

## CALL

```
CALLN , CALLXN :
CALL4: call a procedure that will move the window by 4 registers.
CALL8: same of 8 registers.
CALL12: you know it. This does not actually move the window but rather takes the 30 first bits of the register, put them in the return address register and use the two last bits to encode the window rotation offset (4, 8 or 12).
```

## ENTRY

```
ENTRY sp, This instruction performs the window rotation according to the two highest bits of the return address and update the stack pointer given .
```


## RETW

```
RETW Rotates back the register window and jump to return address. Note that because the two highest bits are used to encode the window rotation, RETW uses its own two highest bits. Therefore the return address must be in the same 1G as the RETW instruction. I guess that will not be a problem for embedded devices.
```

## Install IDA xtensa plugin

Download https://github.com/themadinventor/ida-xtensa/blob/master/xtensa.py and copy it to the following location:

"%ProgramFiles%\IDA 7.1\procs"