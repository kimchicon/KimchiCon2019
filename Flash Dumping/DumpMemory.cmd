mkdir Memory
esptool --chip esp32 --port COM3 --baud 921600 dump_mem 0x40000000 0xC2000 Memory\0x40000000.bin
pause