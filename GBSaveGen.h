#ifndef GBSAVEGEN_H
#define GBSAVEGEN_H

#define ROM_HEADER_SIZE 0x150

struct GBRomHeader {
	char entry[4];
	char nintendo_logo[0x30];
	char title[16];
	char new_licensee_code[2];
	char sgb;
	char cart_type;
	char rom_size; // 32KB << rom_size == actual rom size
	char sram_size; // check enum for values. TODO: detect MBC2 if applicable
	char dest_code;
	char old_licensee_code;
	char mask_version_num;
	char header_checksum; // check this. Algo = x=0:FOR i=0134h TO 014Ch:x=x-MEM[i]-1:NEXT
	char global_checksum[2]; // unused
}__attribute__((packed));

struct GBRomHeaderv2 {
	char entry[4];
	char nintendo_logo[0x30];
	char title[15];
	char cgb_flag;
	char new_licensee_code[2];
	char sgb;
	char cart_type;
	char rom_size; // 32KB << rom_size == actual rom size
	char sram_size; // check enum for values. TODO: detect MBC2 if applicable
	char dest_code;
	char old_licensee_code;
	char mask_version_num;
	char header_checksum; // check this. Algo = x=0:FOR i=0134h TO 014Ch:x=x-MEM[i]-1:NEXT
	char global_checksum[2]; // unused
}__attribute__((packed));

/* Cartridge types:

 00h  ROM ONLY                 15h  MBC4
 01h  MBC1                     16h  MBC4+RAM
 02h  MBC1+RAM                 17h  MBC4+RAM+BATTERY
 03h  MBC1+RAM+BATTERY         19h  MBC5
 05h  MBC2                     1Ah  MBC5+RAM
 06h  MBC2+BATTERY             1Bh  MBC5+RAM+BATTERY
 08h  ROM+RAM                  1Ch  MBC5+RUMBLE
 09h  ROM+RAM+BATTERY          1Dh  MBC5+RUMBLE+RAM
 0Bh  MMM01                    1Eh  MBC5+RUMBLE+RAM+BATTERY
 0Ch  MMM01+RAM                20h  MBC6
 0Dh  MMM01+RAM+BATTERY        22h  MBC7+SENSOR+RUMBLE+RAM+BATTERY
 0Fh  MBC3+TIMER+BATTERY
 10h  MBC3+TIMER+RAM+BATTERY   FCh  POCKET CAMERA
 11h  MBC3                     FDh  BANDAI TAMA5
 12h  MBC3+RAM                 FEh  HuC3
 13h  MBC3+RAM+BATTERY         FFh  HuC1+RAM+BATTERY

*/

/* RAM sizes:

 00h - None
 01h - 2 KBytes
 02h - 8 Kbytes
 03h - 32 KBytes (4 banks of 8KBytes each)
 04h - 128 KBytes (16 banks of 8KBytes each)
 05h - 64 KBytes (8 banks of 8KBytes each)

 NOTE: MBC2 is always 00h, but RAM is actually 512 x 4 bits

*/

void usage();

#endif