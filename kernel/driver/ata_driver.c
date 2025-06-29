#include <stdint.h>
#include "dadio_driver.h"
#include "hal_driver.h"

#define STATUS_BSY 0x80
#define STATUS_RDY 0x40
#define STATUS_DRQ 0x08
#define STATUS_DF 0x20
#define STATUS_ERR 0x01

static void ata_wait_bsy();
static void ata_wait_drq();
void ata_read_sectors(uint32_t target_address, uint32_t LBA, uint8_t sector_count)
{
	ata_wait_bsy();
	write_port(0x1F6, 0xE0 | ((LBA >> 24) & 0xF));
	write_port(0x1F2, sector_count);
	write_port(0x1F3, (uint8_t)LBA);
	write_port(0x1F4, (uint8_t)(LBA >> 8));
	write_port(0x1F5, (uint8_t)(LBA >> 16));
	write_port(0x1F7, 0x20);

	uint16_t* target = (uint16_t*)target_address;
	for (int j = 0; j < sector_count; j++) {
		ata_wait_bsy();
		ata_wait_drq();
		for (int i = 0; i < 256; i++)
			target[i] = read_port_word(0x1F0);
		target += 256;
	}
}
static void ata_wait_bsy() {
	while (read_port(0x1F7) & STATUS_BSY);
}
static void ata_wait_drq() {
	while (!(read_port(0x1F7) & STATUS_RDY));
}
