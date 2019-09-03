#ifndef __CRCINCLUDE__
#define __CRCINCLUDE__
#define USHORT unsigned short
#define BYTE unsigned char
////////----------  CRC16 using x^16+x^15+x^2+1 polynomial generator----//
USHORT calccrc(USHORT Oldcrc,BYTE crcbuf);
unsigned short CRC16(const unsigned char *puchMsg, unsigned short usDataLen);
///////////////

////////--------  ccitt_16 using x^16+x^12+x^5+1 polynomial generator----//
unsigned short CIs_update_crc(short length, BYTE *pt, short oldcrc);
unsigned short CIs_crc16 (unsigned int count, BYTE *buffer);
unsigned short
LowHighCrc16 (
	unsigned int	count,
	BYTE *			buffer,
	unsigned char *	lowbyte,
	unsigned char *	highbyte
);
//unsigned short CRC16(unsigned char *puchMsg, unsigned short usDataLen);
//USHORT calccrc(USHORT Oldcrc,BYTE crcbuf);

#endif