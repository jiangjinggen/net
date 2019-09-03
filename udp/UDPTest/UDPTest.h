#ifndef UDP_TEST_H_
#define UDP_TEST_H_
// ITCS Address Format
union ITCSAddr {
	unsigned long nITCSAddrAll;
	struct {
		unsigned long nProcessNum	: 5;	// 0, 1: Reserved; 2: Beacon; 3: Status Update/Diff Correction; 4: Crossing; \
											// 5: File System; 6: Log String; 7: Block Occupancies; 8: Train Information MSG; \
											// 9: Train Health Status; 10-32: Reserved
		unsigned long nEquipmentID	: 14;	// IP addr of the equipment; RBC: physical chassis ID
		unsigned long nSysNumber	: 7;	// 0x01: Tibet; 0x7F: Michigan; arbitrarily assigned, railroad/customer
		unsigned long nLocalDevAddr	: 3;	// 0: wild; 1, 2, 3: Reserved; 5: MMT Poller; 6: RTU; 7: CTC Office
		unsigned long nEquipmentType: 3;	// 0: wild; 1: OBC; 2, 3, 4, 6: Reserved; 5: Office System; 7: Wayside
	};
};

/////////////////////////////////////////////////////////
// OBC Health Message Format (OBC --> ITCS Poller )
//    - L2 Header
//    - L3 Header
//    - NonVital Layer5 Header
//    - App Layer Header
//    - Train Health Information
//    - Layer 5 CRC16
struct TrainHealthInfo
{
	unsigned long dwSW_CRC;
	unsigned long dwSrc_Line_Number;
	unsigned char byteHW_Module_ID;
	unsigned char byteSW_Module_ID;
	unsigned char byteCheckType;
	unsigned char byteCheckData;
	unsigned short wCheck_Number;
};

struct ITCS_AppLayer
{
	unsigned short wAppLayerLable;	// 0x002C
	unsigned char byteAppLayerVer;	// 0x01
	unsigned short wMsgLen;
//	unsigned char* pbyteAppLData;
	TrainHealthInfo thi;
};

struct ITCS_NVL5
{
	unsigned char byteNVL5MsgID;	// 0x82
	ITCSAddr srcL5Addr;
	ITCSAddr destL5Addr;
	unsigned char byteMsgLabel;		// 0x00
//	unsigned char *pbyteL5Data;
	ITCS_AppLayer NVL5Data;
	unsigned short wL5CRC16;
};

struct ITCS_L3
{
	unsigned char nL3MsgID : 3;		// 0x01
	unsigned char nL3Reserved : 5;
	unsigned char byteStreamID;
//	unsigned char* pbyteL3Data;
	ITCS_NVL5 L3Data;
};

struct ITCS_L2 
{
	ITCSAddr destL2Addr;
	ITCSAddr srcL2Addr;
//	unsigned char* pbyteL2Data;
	ITCS_L3 L2Data;
};
#endif