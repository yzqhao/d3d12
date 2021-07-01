#include "RHIDataBuffer.h"
/*
enum DateType
{
	DT_FLOAT32_1,// 1D VSREAL expanded to (value, 0., 0., 1.)
	DT_FLOAT32_2,// 2D VSREAL expanded to (value, value, 0., 1.)
	DT_FLOAT32_3,// 3D VSREAL expanded to (value, value, value, 1.)
	DT_FLOAT32_4,// 4D VSREAL

	DT_USHORT,
	DT_UINT,

	DT_UBYTE4,	// 4D unsigned byte
	DT_UBYTE4N, // Each of 4 bytes is normalized by dividing to 255.0

	DT_SHORT2,	// 2D signed short expanded to (value, value, 0., 1.)
	DT_SHORT2N,	// 2D signed short normalized (v[0]/32767.0,v[1]/32767.0,0,1)
	DT_USHORT2N,// 2D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,0,1)

	DT_SHORT4,	// 4D signed short
	DT_SHORT4N,	// 4D signed short normalized (v[0]/32767.0,v[1]/32767.0,v[2]/32767.0,v[3]/32767.0)
	DT_USHORT4N,// 4D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,v[2]/65535.0,v[3]/65535.0)

	DT_FLOAT16_2,// Two 16-bit floating point values, expanded to (value, value, 0, 1)
	DT_FLOAT16_4,// Four 16-bit floating point values

	DT_COLOR,	
};
*/
unsigned int RHIDataBuffer::ms_uiDateTypeByte[DT_MAXNUM] =
{
	4,
	8,
	12,
	16,

	2,
	4,

	4,
	4,

	4,
	4,
	4,

	8,
	8,
	8,

	4,
	8,

	4
};
unsigned int RHIDataBuffer::ms_uiDateTypeChannel[DT_MAXNUM] =
{
	1,
	2,
	3,
	4,

	1,
	1,

	4,
	4,

	2,
	2,
	2,

	4,
	4,
	4,

	2,
	4,

	4

};

RHIDataBuffer::RHIDataBuffer()
{

}

RHIDataBuffer::~RHIDataBuffer()
{
	SAFE_DELETE_ARRAY(m_pDate);
}

bool RHIDataBuffer::SetDate(const void *pDate, unsigned int uiNum, unsigned int uiDT)
{
	if(uiDT >= DT_MAXNUM || !pDate || !uiNum)
		return false;

	m_uiDT = uiDT;
	m_uiNum = uiNum;

	SAFE_DELETE_ARRAY(m_pDate);

	m_pDate = new unsigned char[GetSize()];

	if(!m_pDate)
		return false;

	memcpy(m_pDate, pDate, GetSize());
	m_uiSize = GetSize();
	return true;
}

bool RHIDataBuffer::AddDate(const void *pDate, unsigned int uiNum, unsigned int uiDT)
{
	if(uiDT >= DT_MAXNUM || !pDate || !uiNum)
		return false;
	if(m_uiDT != uiDT)
		return false;


	unsigned char * Temp = NULL;
	Temp = new unsigned char[ ( uiNum + m_uiNum )* GetStride()];
	if(!Temp)
		return false;

	memcpy(Temp, m_pDate, GetStride() * m_uiNum);
	memcpy(Temp + GetStride() * m_uiNum,pDate,GetStride() * uiNum);

	SAFE_DELETE_ARRAY(m_pDate);
	m_uiNum += uiNum;
	m_pDate = Temp;
	m_uiSize = GetSize();
	return true;
}

bool RHIDataBuffer::CreateEmptyBuffer(unsigned int uiNum, unsigned int uiDT)
{
	if(uiDT >= DT_MAXNUM || !uiNum)
		return false;

	m_uiDT = uiDT;
	m_uiNum = uiNum;
	SAFE_DELETE_ARRAY(m_pDate);

	m_pDate = new unsigned char[GetSize()];

	if(!m_pDate)
		return false;

	memset(m_pDate,0,GetSize());
	m_uiSize = GetSize();
	return true;
}