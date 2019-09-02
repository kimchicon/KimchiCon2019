mkdir FlashSections
esptool --chip esp32 --port COM3 --baud 921600 read_flash 0 0xF42400 FlashSections\FullFlash.bin
pause