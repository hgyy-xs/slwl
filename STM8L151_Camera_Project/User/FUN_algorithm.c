


#include "main.h"


//  ��˾�ַ�
#define COMPANY1_STRING		0X59	// 'Y'
#define COMPANY2_STRING		0X47	// 'G'
// ��Ŀ�ַ�
#define PROJECT1_STRING		0X44	// 'D'
#define PROJECT2_STRING		0X57	// 'W'
#define CAL_STRING			0X53	// 'S'
//SN�ַ�������
#define SERIALNUM1 0
#define SERIALNUM2 1
#define SERIALNUM3 2
#define EQUIPMENT1 3
#define EQUIPMENT2 4



/*===============================================
				�Ƚϳ�������	
data1 :�Ƚϵ�����
leng1:�Ƚ����ݵĳ���
data2 :�Ƚϵĳ���
leng2:�Ƚϳ����ĳ���

����0����Ƚ�ʧ��
����1����Ƚϳɹ�
=================================================*/
unsigned char ALG_CmpsConst( unsigned char *data1,unsigned int leng1,const unsigned char *data2,unsigned int leng2)
{
	while(leng1)
	    {
	    if(*data1!=*data2) return 0;
	    data1++;data2++;leng1--;leng2--;
	    if(!leng2) return 1;
	    }
	return 0;  
}	

/*===============================================
				�Ƚϳ�������	
data1 :�Ƚϵ�����
leng1:�Ƚ����ݵĳ���
data2 :�Ƚϵ�����
leng2:�Ƚ����ݵĳ���

����0����Ƚ�ʧ��
����1����Ƚϳɹ�
=================================================*/
unsigned char ALG_CmpsData( unsigned char *data1,unsigned int leng1,unsigned char *data2,unsigned int leng2)
{
	while(leng1)
	    {
	    if(*data1!=*data2) return 0;
	    data1++;data2++;leng1--;leng2--;
	    if(!leng2) return 1;
	    }
	return 0;  
}	


//*****************************************************************����RAM����//
void ALG_CopyRam(unsigned char *data1,unsigned int leng1,unsigned char *data2,unsigned int *leng2)
    {
	*leng2=leng1;
    while(leng1)
	    { 
	    *data2=*data1;data2++;data1++;leng1--;
	    }
	}

/************************************************************************/
/*                  ASCIIת��Ϊʮ������                                 */
/************************************************************************/
unsigned char ALG_AsciitoHex(unsigned char asc)
{
	unsigned char tmp=0xff;
	
	if ((asc>=0x30)&&(asc<=0x39))	// 0-9
	{
		tmp=asc-0x30;
	}
	else if ((asc>=0x41)&&(asc<=0x46))		// A-F
	{
		tmp=asc-0x37;
	}
	else if ((asc>=0x61)&&(asc<=0x66))		// a-f
	{
		tmp=asc-0x57;
	}
	return tmp;
}

/************************************************************************/
/*              ʮ������ת��ΪASCII��                                */
/************************************************************************/

unsigned char ALG_HextoAscii(unsigned char tmp)
{
  unsigned char asc;
  asc= tmp;
  return asc;
}


char ALG_HexToASCII(unsigned char  data_hex)
{ 
   char  ASCII_Data;
   ASCII_Data=data_hex & 0x0F;
   if(ASCII_Data<10) 
    ASCII_Data=ASCII_Data;//+0x30; //��0--9��
   else  
    ASCII_Data=ASCII_Data;//;+0x37;       //��A--F��
   return ASCII_Data;
}

void ALG_HexGroupToString(char *OutStrBuffer, unsigned char *InHexBuffer, unsigned int HexLength)
{
   unsigned int i, k=0;
   for(i=0;i<HexLength;i++)
   {
      OutStrBuffer[k++]=ALG_HexToASCII((InHexBuffer[i]>>4)&0x0F);
      OutStrBuffer[k++]=ALG_HexToASCII(InHexBuffer[i]&0x0F);
   }
   OutStrBuffer[k]='\0';
}



/************************************************************************/
/*                           ��ϢASCIIת��Ϊ��������                     */
/************************************************************************/
void ALG_ASCIItoDATA(unsigned char *pdata1,unsigned char *pdata2,unsigned char *leng2)
{
	unsigned char cnt=0;
	unsigned char tmp1,tmp2;
	
	tmp1=*pdata1++;
	tmp2=*pdata1++;
	*leng2=(ALG_AsciitoHex(tmp1)<<4)+ALG_AsciitoHex(tmp2);
	for (;cnt<(*leng2);cnt++)
	{
		tmp1=*pdata1++;
		tmp2=*pdata1++;
		*pdata2=(ALG_AsciitoHex(tmp1)<<4)+ALG_AsciitoHex(tmp2);
		pdata2++;
	}
}

/************************************************************************/
/*                           ��Ҫ������ֻ�����תΪPDU��ʽ                  */
/************************************************************************/
void ALG_ConvPDU(unsigned char *pdata1,unsigned char *pdata2)
{
	unsigned char cnt=0;
	unsigned char tmp1,tmp2;
	
	for (;cnt<6;cnt++)
	{
		if(cnt<5)
		{
			tmp1	=	*pdata1++;
			tmp2	= 	*pdata1++;
		}
		else
		{
			tmp1 	=	*pdata1;
			tmp2	= 	0x46; // 'F'	
		}
		*pdata2	=	tmp2;
		pdata2++;
		*pdata2 =	tmp1;
		pdata2++;
	}
	
}


/*
	����CRC

ע:����ֵΪCRC
*/
unsigned char ALG_CrcCal(unsigned int length, unsigned char cmd)
{
	u8 len1,len2;

	len2 = (u8)(length>>8);
	len1 = ((u8)length)^cmd;
	len1 ^=len2;
	if( len1 %2 )
	{
		len1^=COMPANY2_STRING;  // ��G��
	}
	else
	{
		len1^=COMPANY1_STRING;  // ��Y��
	}
	return (len1+1);

}






unsigned char ALG_CmdCrcCal(unsigned char *pdata, unsigned int len)
{
	unsigned int i;
	unsigned char xorByte = pdata[0];
    for(i=1;i<len;i++)
	{
		xorByte ^= pdata[i];
	}
	xorByte++;
    return xorByte; 
}



/************************************************************************/
/*              ʮ������ʾ                                              */
/************************************************************************/

unsigned char ALG_HextoAlg(unsigned char tmp1)
{
  unsigned char tmp;

  unsigned char cnt,cnt1;
  tmp = tmp1;
  cnt = tmp / 10;
  cnt1 = tmp % 10;
  
  tmp = (cnt<<4) + cnt1;
  
  return tmp;
}

unsigned char ALG_CmdCrcCal1(unsigned char *pdata, unsigned int len)
{
	unsigned int i;
	unsigned char xorByte = pdata[0];
    for(i=1;i<len;i++)
	{
		xorByte ^= pdata[i];
	}
    return xorByte; 
}

/*
 * ��    ��: ALG_SN_Code
 * ��    ��: ��ȡSN�豸�ŵļ�����
 * �������: pdata: SNԭʼ����
 * �������: ��
 * �� �� ֵ: ������
 * ��    ��:
 */
u8 ALG_SN_Code(u8 *pdata){
	u8 Code = 0;
	Code = pdata[SERIALNUM1]^pdata[SERIALNUM2];	
	Code ^= pdata[SERIALNUM3];
	return ++Code;
}

/*
 * ��    ��: ALG_SN_Encrypt
 * ��    ��: ��SN�豸�Ž��м���
 * �������: pdata: SNԭʼ����
 *			 Code: ������
 * �������: ��
 * �� �� ֵ: ��
 * ��    ��:
 */
void ALG_SN_Encrypt(u8 *pdata, u8 Code){
	pdata[EQUIPMENT1] ^= Code;
	pdata[EQUIPMENT2] ^= Code;
}





