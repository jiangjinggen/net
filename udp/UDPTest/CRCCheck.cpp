// CRCCheck.cpp: implementation of the CCRCCheck class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "CRCCheck.h"
//#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static char THIS_FILE[] = __FILE__;
//#endif


//unsigned long CCRCCheck::m_pdwCrcTab[256];
bool CCRCCheck::m_bCrcTableGenerated = false;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCRCCheck::CCRCCheck()
	:m_nOrder(16), m_dwPolynom(0x1002), m_bRefIn(false), m_bRefOut(false), 
		m_dwCrcInit(0), m_dwCrcXOR(0)
{
}

CCRCCheck::CCRCCheck(int nOrder, unsigned long dwPolynom, bool bRefIn, bool bRefOut, 
					 unsigned long dwCrcInit, unsigned long dwCrcXOR)
		: m_nOrder(nOrder), m_dwPolynom(dwPolynom), m_bRefIn(bRefIn), m_bRefOut(bRefOut), 
		m_dwCrcInit(dwCrcInit), m_dwCrcXOR(dwCrcXOR)
{
	m_dwCrcMask = ((unsigned long)1<<m_nOrder)-1;
	Generate_crc_table(m_dwPolynom, m_dwCrcMask, m_bRefIn, m_nOrder);
}

CCRCCheck::~CCRCCheck()
{
	delete []m_pdwCrcTab;
}

void CCRCCheck::Init(int nOrder, unsigned long dwPolynom, bool bRefIn, bool bRefOut, 
					 unsigned long dwCrcInit, unsigned long dwCrcXOR)
{
	m_nOrder = nOrder;
	m_dwPolynom = dwPolynom;
	m_bRefIn = bRefIn;
	m_bRefOut = bRefOut;
	m_dwCrcInit = dwCrcInit;
	m_dwCrcXOR = dwCrcXOR;

	m_dwCrcMask = ((unsigned long)1<<m_nOrder)-1;
	Generate_crc_table(m_dwPolynom, m_dwCrcMask, m_bRefIn, m_nOrder);
}

unsigned long CCRCCheck::GetCRC(unsigned char* p, unsigned long len)
{

	// fast lookup table algorithm without augmented zero bytes, e.g. used in pkzip.
	// only usable with polynom orders of 8, 16, 24 or 32.

	unsigned long crc = m_dwCrcInit;

	if (m_bRefIn)
		crc = Reflect(crc, m_nOrder);

	if (!m_bRefIn)
		while (len--)
			crc = (crc << 8) ^ m_pdwCrcTab[ ((crc >> (m_nOrder-8)) & 0xff) ^ *p++];
	else
		while (len--)
			crc = (crc >> 8) ^ m_pdwCrcTab[ (crc & 0xff) ^ *p++];

	if (m_bRefOut^m_bRefIn)
		crc = Reflect(crc, m_nOrder);
	crc^= m_dwCrcXOR;
	crc&= m_dwCrcMask;

	return(crc);
}

void CCRCCheck::Generate_crc_table(unsigned long dwPolynom, unsigned long dwCrcMask, bool bRefIn, 
								   int nOrder)
{
	if (m_bCrcTableGenerated)
		return;

	m_pdwCrcTab = new unsigned long [256];
	// make CRC lookup table used by table algorithms

	unsigned long crchighbit = (unsigned long)1<<(nOrder-1);

	int i, j;
	unsigned long bit, crc;

	for (i=0; i<256; i++) {

		crc=(unsigned long)i;
		if (bRefIn)
			crc=Reflect(crc, 8);
		crc<<= m_nOrder-8;

		for (j=0; j<8; j++) {

			bit = crc & crchighbit;
			crc<<= 1;
			if (bit) crc^= dwPolynom;
		}			

		if (bRefIn)
			crc = Reflect(crc, nOrder);
		crc&= dwCrcMask;
		m_pdwCrcTab[i]= crc;
	}

	m_bCrcTableGenerated = true;
}

unsigned long CCRCCheck::Reflect (unsigned long crc, int bitnum)
{

	// reflects the lower 'bitnum' bits of 'crc'

	unsigned long i, j=1, crcout=0;

	for (i=(unsigned long)1<<(bitnum-1); i; i>>=1) {
		if (crc & i) crcout|=j;
		j<<= 1;
	}
	return (crcout);
}