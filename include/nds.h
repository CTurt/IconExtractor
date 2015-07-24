#pragma once

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef struct sNDSHeader {
	char gameTitle[12];			//!< 12 characters for the game title.
	char gameCode[4];			//!< 4 characters for the game code.
	char makercode[2];			//!< identifies the (commercial) developer.
	u8 unitCode;				//!< identifies the required hardware.
	u8 deviceType;				//!< type of device in the game card
	u8 deviceSize;				//!< capacity of the device (1 << n Mbit)
	u8 reserved1[9];
	u8 romversion;				//!< version of the ROM.
	u8 flags;					//!< bit 2: auto-boot flag.

	u32 arm9romOffset;			//!< offset of the arm9 binary in the nds file.
	u32 arm9executeAddress;		//!< adress that should be executed after the binary has been copied.
	u32 arm9destination;		//!< destination address to where the arm9 binary should be copied.
	u32 arm9binarySize;			//!< size of the arm9 binary.

	u32 arm7romOffset;			//!< offset of the arm7 binary in the nds file.
	u32 arm7executeAddress;		//!< adress that should be executed after the binary has been copied.
	u32 arm7destination;		//!< destination address to where the arm7 binary should be copied.
	u32 arm7binarySize;			//!< size of the arm7 binary.

	u32 filenameOffset;			//!< File Name Table (FNT) offset.
	u32 filenameSize;			//!< File Name Table (FNT) size.
	u32 fatOffset;				//!< File Allocation Table (FAT) offset.
	u32 fatSize;				//!< File Allocation Table (FAT) size.

	u32 arm9overlaySource;		//!< File arm9 overlay offset.
	u32 arm9overlaySize;		//!< File arm9 overlay size.
	u32 arm7overlaySource;		//!< File arm7 overlay offset.
	u32 arm7overlaySize;		//!< File arm7 overlay size.

	u32 cardControl13;			//!< Port 40001A4h setting for normal commands (used in modes 1 and 3)
	u32 cardControlBF;			//!< Port 40001A4h setting for KEY1 commands (used in mode 2)
	u32 bannerOffset;			//!< offset to the banner with icon and titles etc.

	u16 secureCRC16;			//!< Secure Area Checksum, CRC-16.

	u16 readTimeout;			//!< Secure Area Loading Timeout.

	u32 unknownRAM1;			//!< ARM9 Auto Load List RAM Address (?)
	u32 unknownRAM2;			//!< ARM7 Auto Load List RAM Address (?)

	u32 bfPrime1;				//!< Secure Area Disable part 1.
	u32 bfPrime2;				//!< Secure Area Disable part 2.
	u32 romSize;				//!< total size of the ROM.

	u32 headerSize;				//!< ROM header size.
	u32 zeros88[14];
	u8 gbaLogo[156];			//!< Nintendo logo needed for booting the game.
	u16 logoCRC16;				//!< Nintendo Logo Checksum, CRC-16.
	u16 headerCRC16;			//!< header checksum, CRC-16.

	u32 debugRomSource;			//!< debug ROM offset.
	u32 debugRomSize;			//!< debug size.
	u32 debugRomDestination;	//!< debug RAM destination.
	u32 offset_0x16C;			//reserved?

	u8 zero[0x90];
} __attribute__ ((__packed__)) tNDSHeader;

typedef struct sNDSBanner {
  u16 version;			//!< version of the banner.
  u16 crc;				//!< 16 bit crc/checksum of the banner.
  u8 reserved[28];
  u8 icon[512];			//!< 32*32 icon of the game with 4 bit per pixel.
  u16 palette[16];		//!< the pallete of the icon.
  u16 titles[6][128];	//!< title of the game in 6 different languages.
} __attribute__ ((__packed__)) tNDSBanner;
