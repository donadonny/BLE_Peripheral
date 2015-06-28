#include"DHT11.h"

/**************************************************
  �ӿڶ��壬��ֲ�˳���ֻ���޸����к궨�����ʱ����
**************************************************/
#define IN_DQ		P0_0  //�������ݶ˿�
#define DQ_PIN		0     //��������IN_DQһ��
#define DQ_PORT		P0DIR


#define SET_OUT DQ_PORT|=BV(DQ_PIN);asm("NOP");asm("NOP")
#define SET_IN  DQ_PORT&=~(BV(DQ_PIN));asm("NOP");asm("NOP")

#define CL_DQ  IN_DQ=0;asm("NOP");asm("NOP")
#define SET_DQ IN_DQ=1;asm("NOP");asm("NOP") 


uint8  CheckSum;
uint8  tmp8BitValue;

/*��������*/
void Read8Bit(void);
static void Delay_nus(uint16 s);


static void Delay_nus(uint16 s) 
{
  while (s--)
  {
#if 0
    asm("NOP");
    asm("NOP");
    asm("NOP");
#else
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
	asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");asm("NOP");
#endif
  }
}

void Read8Bit(void)
{
  static uint8  OverTimeCnt = 0;
  uint8 i,tmpBit;
  
  for(i=0;i<8;i++)
  {
    OverTimeCnt = 2;
    while((IN_DQ == 0)&&OverTimeCnt++);
    //while(IN_DQ == 0);
    Delay_nus(19);//12
    if(IN_DQ == 1)
      tmpBit = 1;
    else
      tmpBit = 0;
    OverTimeCnt = 2;
    while((IN_DQ == 1)&&OverTimeCnt++);
    //while(IN_DQ == 1);
    //��ʱ������forѭ��		  
    if(OverTimeCnt==1)
      break;
    
    tmp8BitValue<<=1;
    tmp8BitValue|=tmpBit;        //0
  
  }

}

/*
//how to use ReadValue
uint8 Sensor[4];
ReadValue(Sensor);

Sensor[0]
-->ʪ������ֵ
Sensor[1]
-->ʪ��С��ֵ

Sensor[2]
-->�¶�����ֵ
Sensor[3]
-->�¶�С��ֵ
*/
int ReadValue(uint8 *sv)
{
  static uint8  OverTimeCnt = 0;
  SET_OUT; 
  CL_DQ; 
  Delay_nus(20000);//������������18ms  
  SET_DQ;
  Delay_nus(20);//������������������ ������ʱ20us-40us
  SET_IN;
  if(IN_DQ == 0)
  {
    OverTimeCnt = 2;
    while((IN_DQ == 0)&&OverTimeCnt++);

    while(IN_DQ == 1);
    //���ݽ���״̬	
    Read8Bit();
    sv[0]=tmp8BitValue;
    Read8Bit();
    sv[1]=tmp8BitValue;
    Read8Bit();
    sv[2]=tmp8BitValue;
    Read8Bit();
    sv[3]=tmp8BitValue;
	
    Read8Bit();
    CheckSum = tmp8BitValue;
    
    if(CheckSum == sv[0]+sv[1]+sv[2]+sv[3])
    {
      CheckSum = 0xff;
    
    }

    return 1;
  }

  return 0;
}
