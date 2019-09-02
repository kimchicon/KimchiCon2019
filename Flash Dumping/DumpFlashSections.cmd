mkdir FlashSections
esptool --chip esp32 --port COM3 --baud 921600 read_flash 0x1000 0x1000 FlashSections\bootloader_qio_80m.bin 
esptool --chip esp32 --port COM3 --baud 921600 read_flash 0xe000 0x1000 FlashSections\boot_app0.bin 
esptool --chip esp32 --port COM3 --baud 921600 read_flash 0x8000 0x1000 FlashSections\KimchiConLogo.ino.partitions.bin
esptool --chip esp32 --port COM3 --baud 921600 read_flash 0x10000 0x10000 FlashSections\KimchiConLogo.ino.bin
pause