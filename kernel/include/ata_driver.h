#ifndef ATA_DRIVER_H
#define ATA_DRIVER_H
#include <stdint.h>
void ata_read_sectors(uint32_t target_address, uint32_t LBA, uint8_t sector_count);
#endif
