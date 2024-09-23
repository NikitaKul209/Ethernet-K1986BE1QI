
#include "main.h"












// ????? ??? ??????????? ethernet
#define ETH_RAM_BASE_ADDR	(unsigned int)0x38000000
// ?????? ?????? ??? ethernet
#define ETH_RAM_FULL_SIZE	8192	// ? ??????
// ???????? ???
#define ETH_RAM_HALF	(ETH_RAM_FULL_SIZE >> 1)
// ?????? ??? ?????????
#define ETH_RAM_SIZE_R	ETH_RAM_HALF
// ?????? ??? ???????????
#define ETH_RAM_SIZE_X	ETH_RAM_HALF
// ?????? ?????? ??????? ????????? ? ???????????
#define ETH_RAM_BASE_R		ETH_RAM_BASE_ADDR
#define ETH_RAM_BASE_X		(ETH_RAM_BASE_ADDR + ETH_RAM_SIZE_R)




#define FCLK		16							// ������� ���������� ���
extern unsigned int PLL_Mul;					// ����������� ��������� PLL
#define CPU_CLK		(FCLK * (PLL_Mul + 1))
#define	DELAY(d)	(FCLK * (PLL_Mul + 1) * (d) / 1000 + 1)	// FCLK * (PLL_Mul + 1) * d / 1000 + 1

//---------------------------------------------------------------------------------------------------
// �������� �����
#define BM(n)		(1ul << (n))

#define LOBYTE(w)         (unsigned char)(w)
#define HIBYTE(w)         (unsigned char)((w) >> 8)
#define MAKEWORD(lo, hi)  ((unsigned short)(unsigned char)(lo) | ((unsigned short)(unsigned char)(hi) << 8))

#define HINIBBLE(b)			(((unsigned char)(b) & 0xF0) >> 4)
#define LONIBBLE(b)			((unsigned int)(b) & 0x0F)

#define EXCHANGE(a) (((unsigned short)(a) >> 8) | (((unsigned short)(a) & 0xFF) << 8))

#define NO	((unsigned short)(-1))
//---------------------------------------------------------------------------------------------------
#define PHY_ADDR			0x1C
#define PHY_DEFAULT_MODE	PHY_MODE_10BaseT_FDn

//------------------------------------------------------------
// �������� �������� ������� ������������ ���� ��������� BAG � JitterWnd � ���
#define PSC_VAL(v)	(CPU_CLK * (v) - 1)
// �������� ��� �������� ������� ���������� ������� � ���
#define BAG_PERIOD(t)	(CPU_CLK * (t))
// �������� ��� �������� �������� ��� �������� ������� � ���
#define JITTER_WND(t)	(CPU_CLK * (t) - 1)

// ���������� ������� ������
#define EXCH_ATTEMPT	10
// ������ ���� ��������
#define COLL_WND		(1ul << 7)

//------------------------------------------------------------
//------------------------------------------------------------
// ����������� ��� ������ PHY
// ������� ����������
#define bPHY_BN_nRST				BM(0)

#define PHY_CTRL_MODE				1
#define mPHY_CTRL_MODE				(0x7ul << PHY_CTRL_MODE)

#define bPHY_CTRL_FX_EN				BM(7)
#define bPHY_CTRL_MDI				BM(8)
#define bPHY_CTRL_MDIO_SEL			BM(9)
#define bPHY_CTRL_MDC				BM(10)

#define PHY_CTRL_PHYADD				11
#define mPHY_CTRL_PHYADD			(0x1Ful << PHY_CTRL_PHYADD)

// ������ ������
enum
{
  PHY_MODE_10BaseT_HDn, 	// n - ��� ��������������
  PHY_MODE_10BaseT_FDn,
  PHY_MODE_100BaseT_HDn,
  PHY_MODE_100BaseT_FDn,
  PHY_MODE_100BaseT_HD,
  PHY_MODE_TRANSLATOR, 	// �����������
  PHY_MODE_LOW_CONS,		// ���������� �����������
  PHY_MODE_AUTO,			// �������������� �����
  PHY_MODE_CNT
};

//------------------------------------------------------------
// ������� ��������� PHY
#define bPHY_STAT_LED0		BM(0)	// 0 - 100����, 1 - 10����
#define bPHY_STAT_LED1		BM(1)	// 0 - link ���, 1 - link ����
#define bPHY_STAT_LED2		BM(2)	// 0 - ������� CRS, 1 - ���������� CRS
#define bPHY_STAT_LED3		BM(3)	// 0 - fill-duplex, 1 - half-duplex
#define bPHY_STAT_READY		BM(4)	// 0 - �� �����, 1 - �����
#define bPHY_STAT_CRS		BM(5)	// 1 - ����������� �����
#define bPHY_STAT_COL		BM(6)	// 1 - ��������
#define bPHY_STAT_FX_VALID	BM(8)	// 1 - ����� � ����� FX
#define bPHY_STAT_MDO		BM(9)	// ��������� ������ ������ SMI ��� ������� ���������� 
#define bPHY_STAT_MDINT		BM(10)	// ���������� �� ����� PHY

//------------------------------------------------------------
//------------------------------------------------------------
// ����� ��� ����������� ethernet
#define ETH_RAM_BASE_ADDR	(unsigned int)0x38000000
// ������ ������ ��� ethernet
#define ETH_RAM_FULL_SIZE	8192	// � ������
// �������� ���
#define ETH_RAM_HALF	(ETH_RAM_FULL_SIZE >> 1)
// ������ ��� ���������
#define ETH_RAM_SIZE_R	ETH_RAM_HALF
// ������ ��� �����������
#define ETH_RAM_SIZE_X	ETH_RAM_HALF
// ������ ������ ������� ��������� � �����������
#define ETH_RAM_BASE_R		ETH_RAM_BASE_ADDR
#define ETH_RAM_BASE_X		(ETH_RAM_BASE_ADDR + ETH_RAM_SIZE_R)

//------------------------------------------------------------
//------------------------------------------------------------
// Ethernet

// ������� ��������� �������� ������
// ���� ���������� �������� ������
#define mETH_X_LENGTH		0xFFFFul
// ���� ��������� �������� ������
#define ETH_X_STAT_RCNT		16
#define mETH_X_STAT_RCNT	(0xFul << ETH_X_STAT_RCNT)
// ���� ���������� ������� �������� ������
#define bETH_X_STAT_RL		BM(20)
// ���� ��������� late collision �� ����� �������� ������
#define bETH_X_STAT_LC		BM(21)
// ���� ����������� ������ ��������
#define bETH_X_STAT_UR		BM(22)

//------------------------------------------------------------
// ������� ��������� ������ ������
// ���������� ���� � ������ ������� ��������� � CRC
#define mETH_R_LENGTH		0xFFFFul
// ������� ������ PAUSE
#define bETH_R_PF_ERR		BM(16)
// ������� ������ ����������
#define bETH_R_CF_ERR		BM(17)
// ������� ���������� ����� ������ 1518 �������
#define bETH_R_LF_ERR		BM(18)
// ������� ������������� ������ ������
#define bETH_R_SF_ERR		BM(19)
// ������� �������������� ����� �������� ������� � �������
// ��������� � ���� ������
#define bETH_R_LEN_ERR		BM(20)
// ���������� ��� � ������ �� ������ 8
#define bETH_R_DN_ERR		BM(21)
// ������� �������������� CRC ������
#define bETH_R_CRC_ERR		BM(22)
// ������� ������� � ������ ��������� nibbles
#define bETH_R_SMB_ERR		BM(23)
// ������� ���������� ������
#define bETH_R_MCA			BM(24)
// ������� ������������������ ������
#define bETH_R_BCA			BM(25)
// ������� ��������������� ������
#define bETH_R_UCA			BM(26)

//------------------------------------------------------------
// ������� ������ ���������� ������
// ������ ���� ��������
#define mETH_CTRL_COL_WND	0xFFul
// �������������� ��������� ������ PAUSE
#define bETH_CTRL_PAUSE_EN	BM(8)
// ����� ������������������ ������� ��������
#define bETH_CTRL_DTRM_EN	BM(9)
// �������������� ����� ������
#define bETH_CTRL_HD_EN		BM(10)
// ��������� ������ ���������� �������� ������� ???
#define bETH_CTRL_EXT_EN	BM(11)
// ����� ������ ������
#define ETH_CTRL_BUFF_MODE	12
// ������ ������
enum
{
  BWM_LIN,	// ��������
  BWM_AUTO,	// �������������� ��������� ����������
  BWM_FIFO
};
#define mETH_CTRL_BUFF_MODE	(0x3ul << ETH_CTRL_BUFF_MODE)
// ����� ����������� ???
#define bETH_CTRL_RCLR_EN	BM(14)	// ����� ����������� 1 - �������
#define bETH_CTRL_XRST		BM(16)	// ����� ����������� 1 - �������
// ����� ���������
#define bETH_CTRL_RRST		BM(17)
// ����� ��
#define bETH_CTRL_DLB		BM(18)
// ���������� ��������������� ��������� ���������� FIFO
// ��������� � ������ �������
#define bETH_CTRL_DBG_RF_EN	BM(28)
// ���������� ��������������� ��������� ���������� FIFO
// �����������
#define bETH_CTRL_DBG_XF_EN	BM(29)
// ����� ������ � ������ �������
#define ETH_DBG_MODE		30
enum
{
  DBG_WM_FREERUN,
  DBG_WM_HALT = 2,
  DBG_WM_STOP
};
#define mETH_DBG_MODE		(0x3ul << ETH_DBG_MODE)

//------------------------------------------------------------
// ������� ���������� ������������
// ������������ ���-�� ������� ������
#define mETH_X_CFG_RTRY_CNT		0xFul
// �������� ����� ����� ��������� �������
#define bETH_X_CFG_IPG_EN		BM(4)
// ���������� ������ ������������� ����������� CRC
#define bETH_X_CFG_CRC_EN		BM(5)
// ������������ ������ ����������
#define bETH_X_CFG_PRE_EN		BM(6)
// ���������� ������ �� ����������� ������ PAD-���
#define bETH_X_CFG_PAD_EN		BM(7)
// ����� ������ ������ ������ EVNT[1]
#define ETH_X_CFG_EVNT_MODE		8
enum
{
  EVNT_MODE_XFIFO_EMPTY,		// ����
  EVNT_MODE_XFIFO_NEAR_EMPTY,	// ����� ����
  EVNT_MODE_XFIFO_HALF,		// ����� �� ��������
  EVNT_MODE_XFIFO_NEAR_FULL,	// ����� �����
  EVNT_MODE_XFIFO_FULL,		// �����
  EVNT_MODE_XFIFO_SND_FNSH, 	// �������� ���������
  EVNT_MODE_XFIFO_READ_WORD_FROM_BUF,	// ������ ����� ������ �� ������
  EVNT_MODE_XFIFO_NEXT_ATTM	// ��������� ������� �������� ������
};
#define mETH_X_CFG_EVNT_MODE	(0x7ul << ETH_X_CFG_EVNT_MODE)
// ������� ���������� ��� ��� �������� ������ ������
#define bETH_X_CGF_MSB1ST		BM(12) // 0 - LSB, 1 - MSB
// ������� ���������� ����
#define bETH_X_CFG_BE			BM(14)	// 0 - little endian, 1 - big endian
// ���������� ������ �����������
#define bETH_X_CFG_EN			BM(15)

//------------------------------------------------------------
// ������� ���������� ����������
// ����� ������� � ��������� MAC-�������, � ����������� �� HASH �������
#define bETH_R_CFG_MCA_EN		BM(0)
// ����� ������� � ����������������� MAC
#define bETH_R_CFG_BCA_EN		BM(1)
// ����� ������� � MAC	��������� � �������� MAC_Address
#define bETH_R_CFG_UCA_EN		BM(2)
// ����� ������� ��� ���������� MAC
#define bETH_R_CFG_AC_EN		BM(3)
// ���������� ������ ������� � ��������
#define bETH_R_CFG_EF_EN		BM(4)
// ���������� ������ ����������� �������
#define bETH_R_CFG_CF_EN		BM(5)
// ���������� ������ ������� ������� ������ ������������
#define bETH_R_CFG_LF_EN		BM(6)
// ���������� ������ ������� ������� ������ �����������
#define bETH_R_CFG_SF_EN		BM(7)

// ����� ������ ������ ������ EVNT[1]
#define ETH_R_CFG_EVNT_MODE		8
enum
{
  EVNT_MODE_RFIFO_NO_EMPTY,		// �� ����
  EVNT_MODE_RFIFO_NEAR_NO_EMPTY,	// ����� �� ����
  EVNT_MODE_RFIFO_HALF,		// ���� �� ��������
  EVNT_MODE_RFIFO_NEAR_NO_FULL,	// ����� �� �����
  EVNT_MODE_RFIFO_NO_FULL,		// �� �����
  EVNT_MODE_RFIFO_RCV_FNSH, 	// ����� ��������
  EVNT_MODE_RFIFO_WRITE_WORD_TO_BUF,	// ������� ������ � �����
  EVNT_MODE_RFIFO_KICK		// �������� �����
};
#define mETH_R_CFG_EVNT_MODE	(0x7ul << ETH_R_CFG_EVNT_MODE)
// ������� ���������� ��� ��� ������ ������ ������
#define bETH_R_CGF_MSB1ST		BM(12) // 0 - LSB, 1 - MSB
// ������� ���������� ����
#define bETH_R_CFG_BE			BM(14)	// 0 - little endian, 1 - big endian
// ���������� ������ ���������
#define bETH_R_CFG_EN			BM(15)

//------------------------------------------------------------
// ������� ����� IMR � ������ IFR ����������
// ��������� ������� ��������� ������
#define bETH_IMR_RF_OK			BM(0)
#define bETH_IFR_RF_OK			bETH_IMR_RF_OK
// ������ ������ ��-�� ���������� ����� � ������
#define bETH_IMR_MISSED_F		BM(1)
#define bETH_IFR_MISSED_F		bETH_IMR_MISSED_F
// ������������ ������ ���������
#define bETH_IMR_OVF			BM(2)
#define bETH_IFR_OVF			bETH_IMR_OVF
// ������� ������ � ������ ��� ������ ������
#define bETH_IMR_SMB_ERR		BM(3)
#define bETH_IFR_SMB_ERR		bETH_IMR_SMB_ERR
// ������������ CRC ������ �������� ������ � CRC ������
#define bETH_IMR_CRC_ERR		BM(4)
#define bETH_IFR_CRC_ERR		bETH_IMR_CRC_ERR
// ����� ����������� �������
#define bETH_IMR_CF				BM(5)
#define bETH_IFR_CF				bETH_IMR_CF
// ����� ������ ����� ������������
#define bETH_IMR_LF				BM(6)
#define bETH_IFR_LF				bETH_IMR_LF
// ����� ������ ����� �����������
#define bETH_IMR_SF				BM(7)
#define bETH_IFR_SF				bETH_IMR_SF
//  �������� �������� ������
#define bETH_IMR_XF_OK			BM(8)
#define bETH_IFR_XF_OK			bETH_IMR_XF_OK
// ������� ������ ��� �������� ������
#define bETH_IMR_XF_ERR			BM(9)
#define bETH_IFR_XF_ERR			bETH_IMR_XF_ERR
// ����������� ������ �����������
#define bETH_IMR_UNDF			BM(10)
#define bETH_IFR_UNDF			bETH_IMR_UNDF
// ������� late collision � �����
#define bETH_IMR_LC				BM(11)
#define bETH_IFR_LC				bETH_IMR_LC
// ������ ������� �� ����� �������� � �������������� ������
#define bETH_IMR_CRS_LOST		BM(12)
#define bETH_IFR_CRS_LOST		bETH_IMR_CRS_LOST
// ���������� �� MDIO ����������
#define bETH_IMR_MDIO_INT		BM(14)
#define bETH_IFR_MDIO_INT		bETH_IMR_MDIO_INT
// ���������� ������� ������� �� MDIO ����������
#define bETH_IMR_MII_RDY		BM(15)
#define bETH_IFR_MII_RDY		bETH_IMR_MII_RDY

//------------------------------------------------------------
// ������� �������
// ����� ��������� ����
#define	bETH_STAT_R_EMPTY		BM(0)
// ����� ��������� ����� ����
#define	bETH_STAT_R_AEMPTY		BM(1)
// ����� ��������� ���������
#define	bETH_STAT_R_HALF		BM(2)
// ����� ��������� ����� �����
#define	bETH_STAT_R_AFULL		BM(3)
//  ����� ��������� �����
#define	bETH_STAT_R_FULL		BM(4)

// ���������� ��������, �� �� ��������� �������
#define	ETH_STAT_R_CNT			5
#define	mETH_STAT_R_CNT			(0x7ul << ETH_STAT_R_CNT)

// ����� ����������� ����
#define bETH_STAT_X_EMPTY		BM(8)
// ����� ����������� ����� ����
#define bETH_STAT_X_AEMPTY		BM(9)
// ����� ����������� ���������
#define bETH_STAT_X_HALF		BM(10)
// ����� ����������� ����� �����
#define bETH_STAT_X_AFULL		BM(11)
// ����� ����������� �����
#define bETH_STAT_X_FULL		BM(12)

//------------------------------------------------------------
// ������� ���������� MDIO
// ����� �������� PHY
#define mETH_PHY_MDIO_REG			0x1Ful
// ����������� ������� �������� ������� ��� MDIO
#define ETH_PHY_MDIO_DIV			5
#define mETH_PHY_MDIO_DIV			(0x7ul << ETH_PHY_MDIO_DIV)
// ����� ������
#define ETH_PHY_MDIO_ADDR			5
#define mETH_PHY_MDIO_ADDR			(0x1Ful << ETH_PHY_MDIO_ADDR)
// ��������
#define bETH_PHY_MDIO_OP			BM(13)	// 1 - rd, 0 - wr
// ����� ��������
#define bETH_PHY_MDIO_PRE_EN		BM(14)	// 1 -  � ����������
// ����������/��������� ������ �� MDIO
#define bETH_PHY_MDIO_RDY			BM(15)

unsigned int CalcSum(void* buf, unsigned int size);
unsigned short CalcHeaderCS(void* buf, unsigned int size);


extern uint8_t DA_MAC_Address[6];
extern uint8_t SA_MAC_Address[6];
extern uint8_t SA_IP_Address[4];
extern uint8_t DA_IP_Address[4];
uint32_t output_frame [150] ;
uint32_t input_frame [150] ;

unsigned int ethernet_WriteRAM(void* buf, unsigned int size);
void arp(uint32_t* packet);
uint16_t calc_crc16(uint16_t* ipv4_header, uint8_t length);
void ARP_REQUEST(void);
uint16_t switch_byte(uint16_t val);
bool  timer_flag = false;
void ipv_4(uint32_t* packet);
void icmp(uint32_t* packet);












#define ETH_TYPE_ARP     switch_byte(0x0806)
#define ETH_TYPE_IPV4     switch_byte(0x0800)

#define ARP_REQ           switch_byte(0x0001)
#define ARP_REP           switch_byte(0x0002)
#define ARP_HTYPE_ETH  switch_byte(0x0001)
#define ARP_PTYPE switch_byte(0x0800)
#define ARP_HLEN 0x06
#define ARP_PLEN 0x4


#define IP_PROTOCOL_UDP 0x16
#define IP_PROTOCOL_ICMP 0x1
#define IP_VERSION 4
#define IP_IHL 5
#define IP_TTL 64




#define IP_ADDR_SIZE 4
#define MAC_ADDR_SIZE 6

#define ETHERNET_HEADER_SIZE 14
#define ARP_HEADER_SIZE 28
#define IP_HEADER_SIZE 20
#define ICMP_HEADER_SIZE 8

typedef struct
{

  uint8_t dest_mac [6];
  uint8_t source_mac [6];
  uint16_t 	ethernet_type;
} ethernet_header_s;

typedef struct
{


  uint16_t hw_type;
  uint16_t pr_type;
  uint8_t hw_length;
  uint8_t pr_length;
  uint16_t operation;
  uint8_t	sender_mac[6];
  uint8_t sender_ip[4];
  uint8_t target_mac[6];
  uint8_t target_ip[4];

} arp_packet_s;

typedef struct
{

  uint8_t version_ihl;
  uint8_t type_of_service;
  uint16_t total_length;
  uint16_t identification;
  uint16_t flags_and_fragment_offset;
  uint8_t time_to_live;
  uint8_t high_level_protocol;
  uint16_t crc;
  uint8_t sender_ip[4];
  uint8_t target_ip[4];

} ip_header_s;

typedef struct{



uint8_t type;
uint8_t code;
uint16_t crc;
uint16_t identifier;
uint16_t sequence;
}icmp_header_s;

enum
{
	LISTENING,
  ARP,
  IPV4,

} ethernet_states;

enum
{

  UDP,
  ICMP,
	PASS,

} ipv4_states;




int main(void)
{
  set_clk();
  set_timer();
  set_port();
//  set_adc();
//  set_ethernet();
  NVIC_EnableIRQ(ETHERNET_IRQn);
  Ethernet_Init();
  ethernet_states = LISTENING;







//  TIMER_Cmd(MDR_TIMER2, ENABLE);
//    ADC1_Start();


  PORT_WriteBit(MDR_PORTA, PORT_Pin_9,true);
  uint8_t val = 0;
			 ETH_Start(MDR_ETHERNET1);
  while(1)

    {




      switch(ethernet_states)
        {


        case ARP:
          arp(input_frame);
          break;
        case IPV4:
          ipv_4(input_frame);
          switch(ipv4_states)
            {
            case ICMP:
							icmp(input_frame);
              break;
            case UDP:
              break;
						case PASS:
              break;
            }
          break;
        case LISTENING:
          break;

        }


//      if(timer_flag)
//        {

//          val = ~val;
//          PORT_ResetBits(MDR_PORTC,PORT_Pin_9);
//          PORT_WriteBit(MDR_PORTA, PORT_Pin_9,val);
//          timer_flag = false;

////          ARP_REQUEST();
//          TIMER_Cmd(MDR_TIMER2, ENABLE);


//        }





    }



}

void arp(uint32_t* packet)
{

  uint8_t* byte_buffer = (uint8_t*)packet;
  ethernet_header_s* rec_ethernet_header = (ethernet_header_s*)byte_buffer;
  arp_packet_s* rec_arp_packet = (arp_packet_s*)(byte_buffer + ETHERNET_HEADER_SIZE);


  if (memcmp(rec_arp_packet->target_ip, SA_IP_Address, IP_ADDR_SIZE) != 0)
    {
      ethernet_states = LISTENING;
      return;
    }
  if (((rec_arp_packet->operation)) == ARP_REQ)
    {

			output_frame[0]= 42;
			uint8_t* tx_byte_buffer = (uint8_t*)output_frame+4;
      ethernet_header_s* tx_ethernet_header =(ethernet_header_s*) tx_byte_buffer;
      arp_packet_s* tx_arp_packet =(arp_packet_s*) (tx_byte_buffer+ETHERNET_HEADER_SIZE);


      memcpy(tx_ethernet_header->dest_mac, DA_MAC_Address,6);
      memcpy(tx_ethernet_header->source_mac,SA_MAC_Address,6);
      tx_ethernet_header->ethernet_type = ETH_TYPE_ARP;

      tx_arp_packet->hw_type = ARP_HTYPE_ETH;
      tx_arp_packet->pr_type = ARP_PTYPE;
      tx_arp_packet->hw_length =ARP_HLEN;
      tx_arp_packet->pr_length =ARP_PLEN;
      tx_arp_packet->operation = ARP_REP;
      memcpy(tx_arp_packet->sender_mac, SA_MAC_Address,6);
      memcpy(tx_arp_packet->sender_ip,SA_IP_Address,6);
      memcpy(tx_arp_packet->target_mac,rec_arp_packet->sender_mac,6);
      memcpy(tx_arp_packet->target_ip,rec_arp_packet->sender_ip,6);
////
//			uint32_t* ptr_buf = (uint32_t*) output_frame;
//			uint32_t len = 42;
//			ETH_SendFrame(MDR_ETHERNET1,	output_frame,42);
//
//      ethernet_WriteRAM(tx_byte_buffer,ETHERNET_HEADER_SIZE+ARP_HEADER_SIZE);
			ETH_SendFrame(MDR_ETHERNET1,output_frame, ARP_HEADER_SIZE + ETHERNET_HEADER_SIZE);
			

    

    }


  ethernet_states = LISTENING;
}

void ipv_4(uint32_t* packet)
{

  uint8_t* byte_buffer = (uint8_t*)packet;
  ethernet_header_s* rec_ethernet_header = (ethernet_header_s*)byte_buffer;
  ip_header_s* rec_ip_header = (ip_header_s*)(byte_buffer+ETHERNET_HEADER_SIZE);

	
	uint16_t crc_rec = (rec_ip_header->crc);
	 rec_ip_header->crc = 0x0;
uint16_t crc16 = calc_crc16((uint16_t*)rec_ip_header,IP_HEADER_SIZE);


  if(crc_rec== calc_crc16((uint16_t*)rec_ip_header,IP_HEADER_SIZE))
    {

      if (memcmp(rec_ip_header->target_ip, SA_IP_Address, IP_ADDR_SIZE) != 0)
        {
          ethernet_states = LISTENING;
						NVIC_EnableIRQ(ETHERNET_IRQn);
          return;
        }


      if (rec_ip_header->high_level_protocol == IP_PROTOCOL_ICMP)
        {

          ipv4_states = ICMP;
					
						NVIC_EnableIRQ(ETHERNET_IRQn);
					return;

        }
      else if (rec_ip_header->high_level_protocol == IP_PROTOCOL_UDP)
        {

          ipv4_states = UDP;

					 return;

        }

    }

}


void icmp(uint32_t* packet){

	uint8_t* byte_buffer = (uint8_t*)packet;
  ethernet_header_s* rec_ethernet_header = (ethernet_header_s*)byte_buffer;
  ip_header_s* rec_ip_header = (ip_header_s*)(byte_buffer+ETHERNET_HEADER_SIZE);
	icmp_header_s* rec_icmp_header = (icmp_header_s*)(byte_buffer+ETHERNET_HEADER_SIZE+IP_HEADER_SIZE);
	
	
	uint8_t* tx_byte_buffer = (uint8_t*)output_frame;
  ethernet_header_s* tx_ethernet_header =(ethernet_header_s*) tx_byte_buffer;
	ip_header_s* tx_ip_header = (ip_header_s*)(tx_byte_buffer+ETHERNET_HEADER_SIZE);
  icmp_header_s* tx_icmp_packet =(icmp_header_s*) (tx_byte_buffer+ETHERNET_HEADER_SIZE+IP_HEADER_SIZE);
	
	
	memcpy(tx_ethernet_header->dest_mac, rec_ethernet_header->source_mac,6);
  memcpy(tx_ethernet_header->source_mac,SA_MAC_Address,6);
	tx_ethernet_header->ethernet_type = ETH_TYPE_IPV4;
	
	tx_ip_header->version_ihl = 0x45;
	tx_ip_header->type_of_service = rec_ip_header->type_of_service;
	tx_ip_header->total_length = IP_HEADER_SIZE+ICMP_HEADER_SIZE;
	tx_ip_header->identification = rec_ip_header->identification;
	tx_ip_header->flags_and_fragment_offset = rec_ip_header->flags_and_fragment_offset;
	tx_ip_header->time_to_live = IP_TTL;
	tx_ip_header->high_level_protocol = IP_PROTOCOL_ICMP;
	memcpy(DA_IP_Address,	tx_ip_header->sender_ip,IP_ADDR_SIZE);
	memcpy(rec_ip_header->sender_ip,	tx_ip_header->target_ip,IP_ADDR_SIZE);
	tx_ip_header->crc = 0x0;
	tx_ip_header->crc = calc_crc16((uint16_t*)tx_ip_header,IP_HEADER_SIZE);
	
	
	tx_icmp_packet->code = 0x0;
	tx_icmp_packet->type = 0x0;
	tx_icmp_packet->identifier = rec_icmp_header->identifier;
	tx_icmp_packet->sequence = rec_icmp_header->sequence;
	memcpy(tx_byte_buffer+ETHERNET_HEADER_SIZE+IP_HEADER_SIZE+ICMP_HEADER_SIZE,byte_buffer+ETHERNET_HEADER_SIZE+IP_HEADER_SIZE+ICMP_HEADER_SIZE,32);
	tx_icmp_packet->crc = 0;
	tx_icmp_packet->crc = calc_crc16((uint16_t*)tx_icmp_packet,ICMP_HEADER_SIZE+32);

  ethernet_WriteRAM(tx_byte_buffer,ETHERNET_HEADER_SIZE+IP_HEADER_SIZE+ICMP_HEADER_SIZE+32);
}



uint16_t calc_crc16(uint16_t* ipv4_header,uint8_t length)
{
  uint32_t sum = 0;
  for (int i=0; i<length/2; i++)
    {
      sum+=ipv4_header[i];
      if(sum&0xffff0000)
        {

          sum = (sum & 0xffff)+(sum>>16);
        }

    }
  uint16_t crc =  ~sum&0xffff;
		 return crc;

}


uint16_t switch_byte(uint16_t val)
{

  return ((val>>8)|(val<<8));


}


void TIMER2_IRQHandler(void)
{
  if (TIMER_GetITStatus(MDR_TIMER2, TIMER_STATUS_CNT_ARR))
    {
      TIMER_ClearITPendingBit(MDR_TIMER2, TIMER_STATUS_CNT_ARR);
      RST_CLK_ADCclkSelection(RST_CLK_ADCclkCPU_C1);
      TIMER_Cmd(MDR_TIMER2, DISABLE);
      TIMER_SetCounter(MDR_TIMER2,0x0);
      timer_flag = true;
    }
}
void ADC_IRQHandler (void)
{
  if (ADC1_GetITStatus(ADC1_IT_END_OF_CONVERSION)==SET)

    {
      uint16_t adc_val = ADC1_GetResult();
      float voltage = adc_val;
      voltage = voltage /4096*3.3;
      ADC1_Start();

    }

}

void ETHERNET_IRQHandler(void)
{
  PORT_SetBits(MDR_PORTC,PORT_Pin_9);
  uint16_t Status;
  ETH_StatusPacketReceptionTypeDef packet;

  // Flash LED1
//	PORT_SetBits (MDR_PORTE, MDB_ETH_LED1);

  // Get packet status and clear IT-flag
  Status = ETH_GetMACITStatusRegister(MDR_ETHERNET1);
	ETH_StatusPacketReceptionTypeDef ETH_StatusPacketReceptionStruct;

  // Proccess valid ETH-packet
  if( (MDR_ETHERNET1->ETH_R_Head != MDR_ETHERNET1->ETH_R_Tail) && (Status & ETH_MAC_IT_RF_OK) )
    {
      ETH_ReceivedFrame(MDR_ETHERNET1,input_frame);
      ethernet_header_s* ethernet_header = (ethernet_header_s*)(input_frame);
      if ( ethernet_header->ethernet_type == ETH_TYPE_ARP )
        {
          ethernet_states = ARP;
        }
      else if ( ethernet_header->ethernet_type == ETH_TYPE_IPV4)
        {
          ethernet_states = IPV4;
        }
    }


  // Copy packet data into buffer
//		packet.Status = ETH_ReceivedFrame(MDR_ETHERNET1, InputFrame);

  // Unicast packet
//		if( packet.Fields.UCA )
//			 ProcessEthIAFrame(InputFrame, packet.Fields.Length);
  // Broadcast packet
//		else if( packet.Fields.BCA )
//			 ProcessEthBroadcastFrame(InputFrame, packet.Fields.Length);

  NVIC_ClearPendingIRQ(ETHERNET_IRQn);
}



unsigned int ethernet_WriteRAM(void* buf, unsigned int size)
{
  unsigned int head, tail;	// ????? ??????, ??????? ?????? ? ?????? ?????? ???????, ? ???
  unsigned short tmp[2];
  unsigned int tmp32;
  unsigned int* src;
  unsigned int* dst;
  unsigned short i;

  head = 	MDR_ETHERNET1->ETH_X_Head;	//
  tail = MDR_ETHERNET1->ETH_X_Tail;	//

  // ?????????? ????????? ???? ? ?????? ???????????
  if(head > tail)
    {
      tmp[0] = head - tail;
      tmp[1] = 0;
    }
  else
    {
      tmp[0] = ETH_RAM_FULL_SIZE - tail;
      tmp[1] = head - ETH_RAM_SIZE_X;
    }

  // ???? ????? ? ?????? ??????, ??? ?? ????? ????????(? ?????? 2-? ??????? ???? ??????????)
  if(size > (tmp[0] + tmp[1] - 2 * sizeof(unsigned int)))
    {
      return 0;
    }

  tmp32 = size;

  src = buf;
  dst = (unsigned int*)(ETH_RAM_BASE_ADDR + tail); //??? ???????????

  *dst++ = tmp32;
  tmp[0] -= sizeof(unsigned int);

  if(dst >= (unsigned int*)(ETH_RAM_BASE_ADDR + ETH_RAM_FULL_SIZE))
    dst = (unsigned int*)ETH_RAM_BASE_X;


  tmp32 = (size + sizeof(unsigned int) - 1)/sizeof(unsigned int);

  if(size <= tmp[0])
    {
      for(i = 0; i < tmp32; i++)
        {
          *dst++ = *src++;
        }
    }
  else
    {
      tmp32 -= tmp[0]/sizeof(unsigned int);

      for(i = 0; i < tmp[0]/sizeof(unsigned int); i++)
        *dst++ = *src++;
      dst = (unsigned int*)ETH_RAM_BASE_X;
      for(i = 0; i < tmp32; i++)
        *dst++ = *src++;
    }

  if(dst >= (unsigned int*)(ETH_RAM_BASE_ADDR + ETH_RAM_FULL_SIZE))
    {
      dst = (unsigned int*)ETH_RAM_BASE_X;
    }

  tmp32 = 0;
  *dst++ = tmp32;

  if(dst >= (unsigned int*)(ETH_RAM_BASE_ADDR + ETH_RAM_FULL_SIZE))
    {
      dst = (unsigned int*)ETH_RAM_BASE_X;
    }

  MDR_ETHERNET1->ETH_X_Tail = (unsigned int)dst;
  MDR_PORTA->RXTX ^= (1 << 13);
  return size;
}