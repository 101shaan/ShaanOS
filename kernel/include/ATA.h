#ifndef ATA_H
#define ATA_H
// This file is deprecated. Use ata_driver.h instead.
void read_sectors_ATA_PIO(uint32_t target_address, uint32_t LBA, uint8_t sector_count);

#endif
