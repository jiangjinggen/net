// CRCCheck.h: interface for the CCRCCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CRCCHECK_H__B98F8EDA_0795_48AA_8EAB_6B51CC98A65B__INCLUDED_)
#define AFX_CRCCHECK_H__B98F8EDA_0795_48AA_8EAB_6B51CC98A65B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCRCCheck  
{
public:
	CCRCCheck();
	CCRCCheck(int nOrder, unsigned long dwPolynom, bool bRefIn, bool bRefOut, 
		unsigned long dwCrcInit, unsigned long dwCrcXOR);
	virtual ~CCRCCheck();

	void Init(int nOrder, unsigned long dwPolynom, bool bRefIn, bool bRefOut, 
			unsigned long dwCrcInit, unsigned long dwCrcXOR);
	unsigned long GetCRC(unsigned char* p, unsigned long len);

private:
	void Generate_crc_table(unsigned long dwPolynom, unsigned long dwCrcMask, 
						bool bRefIn, int nOrder);
	unsigned long Reflect (unsigned long crc, int bitnum);

//	static unsigned long s_dwCrcTab[256];
	unsigned long *m_pdwCrcTab;

	unsigned long m_dwCrcInit;
	unsigned long m_dwCrcXOR;
	unsigned long m_dwCrcMask;
	unsigned long m_dwPolynom;
	bool m_bRefIn;
	bool m_bRefOut;
	int m_nOrder;

	static bool m_bCrcTableGenerated;
};

#endif // !defined(AFX_CRCCHECK_H__B98F8EDA_0795_48AA_8EAB_6B51CC98A65B__INCLUDED_)
