
#include "main.h"

extern uint8_t DA_MAC_Address[6];
extern uint8_t SA_MAC_Address[6];
extern uint8_t SA_IP_Address[4];
extern uint8_t DA_IP_Address[4];
//uint32_t output_frame [400] __attribute__((section("EXECUTABLE_MEMORY_SECTION"))) ;
//uint32_t input_frame [400] __attribute__((section("EXECUTABLE_MEMORY_SECTION")));
//uint32_t tx_udp_frame[400] __attribute__((section("EXECUTABLE_MEMORY_SECTION")));

uint32_t output_frame [400]  ;
uint32_t input_frame [400];
uint32_t tx_udp_frame[400] ;
uint16_t adc_data[8000];

bool data_received_flag = false;
void arp(uint32_t* packet);
uint16_t calc_crc16(uint16_t* ipv4_header, uint8_t length);
void get_ethernet_packet(uint32_t* packet);
void collect_spi_data(uint16_t*spi_data_buff);
void collect_spi_data2(uint16_t*spi_data_buff);
uint16_t switch_byte(uint16_t val);
bool  timer_flag = false;
void ipv_4(uint32_t* packet);
void icmp(uint32_t* packet);
void udp(uint32_t* packet);

uint32_t sum_udp_data(uint16_t* header,uint16_t length);
uint16_t calc_crc16_udp(uint16_t* udp_pseudo_header,uint16_t udp_pseudo_header_length,uint8_t* udp_data, uint16_t udp_data_length);
bool rx_done_flag;


#define ETH_TYPE_ARP     switch_byte(0x0806)
#define ETH_TYPE_IPV4     switch_byte(0x0800)

#define ARP_REQ           switch_byte(0x0001)
#define ARP_REP           switch_byte(0x0002)
#define ARP_HTYPE_ETH  switch_byte(0x0001)
#define ARP_PTYPE switch_byte(0x0800)
#define ARP_HLEN 0x06
#define ARP_PLEN 0x4

#define IP_PROTOCOL_UDP 17
#define IP_PROTOCOL_ICMP 1
#define IP_VERSION 4
#define IP_IHL 5
#define IP_TTL 128

#define IP_ADDR_SIZE 4
#define MAC_ADDR_SIZE 6

#define TX_PACKAGE_CONTROL_FIELD 4
#define ETHERNET_HEADER_SIZE 14
#define ARP_HEADER_SIZE 28
#define IP_HEADER_SIZE 20
#define ICMP_HEADER_SIZE 8
#define UDP_HEADER_SIZE 8
#define UDP_PSEUDO_HEADER_SIZE 20
#define UDP_DATA_SIZE 1000

uint16_t udp_data_counter;
uint16_t adc_data_counter;
uint16_t* adc_data_p = adc_data;
uint16_t* udp_data_p = (uint16_t*)tx_udp_frame+(TX_PACKAGE_CONTROL_FIELD+ETHERNET_HEADER_SIZE+IP_HEADER_SIZE+UDP_HEADER_SIZE)/2;
bool udp_data_ready = false;

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

typedef struct
{
  uint8_t type;
  uint8_t code;
  uint16_t crc;
  uint16_t identifier;
  uint16_t sequence;
} icmp_header_s;

typedef struct
{

  uint16_t source_port;
  uint16_t dest_port;
  uint16_t length;
  uint16_t crc;

} udp_header_s;



typedef struct
{

  uint8_t source_ip[4];
  uint8_t dest_ip[4];
  uint8_t reserved;
  uint8_t protocol;
  uint16_t udp_length;
	uint16_t source_port;
  uint16_t dest_port;
  uint16_t length;
  uint16_t crc;

} pseudo_udp_header_s;
pseudo_udp_header_s pseudo_udp_header;
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
  set_ethernet();
  set_adc();
  set_spi();

//CONV_LOW


  ethernet_states = LISTENING;


//  TIMER_Cmd(MDR_TIMER2, ENABLE);
//  ADC1_Start();

//for ( int i = 0; i<8000;i++){

//adc_data_p[i] = switch_byte(i);
	
 
	
//}
  while(1)

    {
if(rx_done_flag){
rx_done_flag = false;
	get_ethernet_packet(input_frame);

//  collect_spi_data2(adc_data);
}
//      ethernet_PHY_Status();
//     spi_transfer(MDR_SSP1,0xAA);
      switch(ethernet_states)
        {
        case ARP:
          arp(input_frame);
          break;
        case IPV4:
          ipv_4(input_frame);
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



void get_ethernet_packet(uint32_t* packet)
{

  uint32_t Status;
  ETH_StatusPacketReceptionTypeDef ETH_StatusPacketReceptionStruct;
  ETH_StatusPacketReceptionStruct.Status =  ETH_ReceivedFrame(MDR_ETHERNET1,packet);
  ethernet_header_s* ethernet_header = (ethernet_header_s*)(packet);

  if( ETH_StatusPacketReceptionStruct.Fields.BCA)
    {
      if ( ethernet_header->ethernet_type == ETH_TYPE_ARP )
        {
          ethernet_states = ARP;
        }
    }

  else if( ETH_StatusPacketReceptionStruct.Fields.UCA)
    {

      if ( ethernet_header->ethernet_type == ETH_TYPE_IPV4)
        {
          ethernet_states = IPV4;

        }
      else if ( ethernet_header->ethernet_type == ETH_TYPE_ARP )
        {
          ethernet_states = ARP;
        }
    }
  else
    {
      ethernet_states = LISTENING;
    }
}

void make_ethernet_header(uint32_t* rx_packet,uint32_t* tx_packet)
{

  ethernet_header_s* tx_ethernet_header =(ethernet_header_s*) tx_packet;
  ethernet_header_s* rx_ethernet_header =(ethernet_header_s*) rx_packet;
  memcpy(tx_ethernet_header->dest_mac, rx_ethernet_header->source_mac,6);
  memcpy(tx_ethernet_header->source_mac,SA_MAC_Address,6);
  tx_ethernet_header->ethernet_type = ETH_TYPE_ARP;

}

void arp(uint32_t* packet)
{

  uint8_t* rx_byte_buffer = (uint8_t*)packet;
  ethernet_header_s* rx_ethernet_header = (ethernet_header_s*)rx_byte_buffer;
  arp_packet_s* rx_arp_packet = (arp_packet_s*)(rx_byte_buffer + ETHERNET_HEADER_SIZE);


  if (memcmp(rx_arp_packet->target_ip, SA_IP_Address, IP_ADDR_SIZE) == 0)
    {

      if (((rx_arp_packet->operation)) == ARP_REQ)
        {


          uint8_t* tx_byte_buffer = (uint8_t*)output_frame+TX_PACKAGE_CONTROL_FIELD;
          ethernet_header_s* tx_ethernet_header =(ethernet_header_s*) tx_byte_buffer;
          arp_packet_s* tx_arp_packet =(arp_packet_s*) (tx_byte_buffer+ETHERNET_HEADER_SIZE);

          memcpy(tx_ethernet_header->dest_mac, rx_arp_packet->sender_mac,6);
          memcpy(tx_ethernet_header->source_mac,SA_MAC_Address,6);
          tx_ethernet_header->ethernet_type = ETH_TYPE_ARP;

          tx_arp_packet->hw_type = ARP_HTYPE_ETH;
          tx_arp_packet->pr_type = ARP_PTYPE;
          tx_arp_packet->hw_length =ARP_HLEN;
          tx_arp_packet->pr_length =ARP_PLEN;
          tx_arp_packet->operation = ARP_REP;
          memcpy(tx_arp_packet->sender_mac, SA_MAC_Address,6);
          memcpy(tx_arp_packet->sender_ip,SA_IP_Address,6);
          memcpy(tx_arp_packet->target_mac,rx_arp_packet->sender_mac,6);
          memcpy(tx_arp_packet->target_ip,rx_arp_packet->sender_ip,6);

          output_frame[0]= 42;
          ETH_SendFrame(MDR_ETHERNET1,(uint32_t *)output_frame,*(uint32_t*)&output_frame[0]);

        }
    }

  ethernet_states = LISTENING;
}

void ipv_4(uint32_t* packet)
{

  uint8_t* rx_byte_buffer = (uint8_t*)packet;
  ethernet_header_s* rx_ethernet_header = (ethernet_header_s*)rx_byte_buffer;
  ip_header_s* rx_ip_header = (ip_header_s*)(rx_byte_buffer+ETHERNET_HEADER_SIZE);


  uint16_t crc_rec = (rx_ip_header->crc);
  rx_ip_header->crc = 0x0;
  uint16_t crc16 = calc_crc16((uint16_t*)rx_ip_header,IP_HEADER_SIZE);


  if(crc_rec == crc16)
    {

      if (memcmp(rx_ip_header->target_ip, SA_IP_Address, IP_ADDR_SIZE) == 0)
        {

          if (rx_ip_header->high_level_protocol == IP_PROTOCOL_ICMP)
            {

              icmp(input_frame);


            }
          else if (rx_ip_header->high_level_protocol == IP_PROTOCOL_UDP)
            {

              udp(input_frame);

            }
          else
            {
              ethernet_states = LISTENING;
            }
        }
      else
        {
          ethernet_states = LISTENING;
        }

    }
  else
    {
      ethernet_states = LISTENING;
    }




}



void udp(uint32_t* packet)


{
  PORT_SetBits(MDR_PORTE,PORT_Pin_7);
  uint8_t* rx_byte_buffer = (uint8_t*)packet;
  ethernet_header_s* rx_ethernet_header = (ethernet_header_s*)rx_byte_buffer;
  ip_header_s* rx_ip_header = (ip_header_s*)(rx_byte_buffer+ETHERNET_HEADER_SIZE);
  udp_header_s* rx_udp_header = (udp_header_s*)(rx_byte_buffer+ETHERNET_HEADER_SIZE+IP_HEADER_SIZE);

//  pseudo_udp_header_s* rx_pseudo_udp_header;
//  memcpy(rx_pseudo_udp_header->dest_ip,rx_ip_header->target_ip,	IP_ADDR_SIZE) ;
//  memcpy(rx_pseudo_udp_header->source_ip,rx_ip_header->sender_ip,IP_ADDR_SIZE) ;
//  rx_pseudo_udp_header->protocol = IP_PROTOCOL_UDP;
//  rx_pseudo_udp_header->reserved = 0x0;
//  rx_pseudo_udp_header->udp_header = *rx_udp_header;
//  rx_pseudo_udp_header->udp_length = rx_udp_header->length;


//  uint16_t tmp_crc = switch_byte (rx_udp_header->crc) ;
//  rx_udp_header->crc = 0x0;
//	uint16_t calc_udp_crc = calc_crc16_udp((uint16_t*)rx_pseudo_udp_header,UDP_PSEUDO_HEADER_SIZE,(uint16_t*)(uint8_t*) rx_byte_buffer+(ETHERNET_HEADER_SIZE+IP_HEADER_SIZE+UDP_HEADER_SIZE),rx_udp_header->length-UDP_HEADER_SIZE);
//  if (calc_udp_crc ==tmp_crc)
//    {

  uint8_t* tx_byte_buffer = (uint8_t*)tx_udp_frame+TX_PACKAGE_CONTROL_FIELD;
  ethernet_header_s* tx_ethernet_header =(ethernet_header_s*) tx_byte_buffer;
  ip_header_s* tx_ip_header = (ip_header_s*)(tx_byte_buffer+ETHERNET_HEADER_SIZE);
  udp_header_s* tx_udp_header =(udp_header_s*) (tx_byte_buffer+ETHERNET_HEADER_SIZE+IP_HEADER_SIZE);


  memcpy(tx_ethernet_header->dest_mac, rx_ethernet_header->source_mac,MAC_ADDR_SIZE);
  memcpy(tx_ethernet_header->source_mac,SA_MAC_Address,6);
  tx_ethernet_header->ethernet_type = ETH_TYPE_IPV4;

  tx_ip_header->version_ihl = 0x45;
  tx_ip_header->type_of_service = rx_ip_header->type_of_service;
  tx_ip_header->total_length = switch_byte(IP_HEADER_SIZE+UDP_HEADER_SIZE+UDP_DATA_SIZE);
  tx_ip_header->identification = rx_ip_header->identification;
  tx_ip_header->flags_and_fragment_offset = rx_ip_header->flags_and_fragment_offset;
  tx_ip_header->time_to_live = IP_TTL;
  tx_ip_header->high_level_protocol = IP_PROTOCOL_UDP;
  memcpy(tx_ip_header->sender_ip,SA_IP_Address,	IP_ADDR_SIZE);
  memcpy(tx_ip_header->target_ip,rx_ip_header->sender_ip,	IP_ADDR_SIZE);
  tx_ip_header->crc = 0x0;
  tx_ip_header->crc = calc_crc16((uint16_t*)tx_ip_header,IP_HEADER_SIZE);

  tx_udp_header->source_port = switch_byte(UDP_SOURCE_PORT);
  tx_udp_header->dest_port = rx_udp_header->source_port;
  tx_udp_header->length = switch_byte( UDP_HEADER_SIZE+UDP_DATA_SIZE);
  tx_udp_header->crc = 0;


memcpy(pseudo_udp_header.dest_ip,rx_ip_header->sender_ip,	IP_ADDR_SIZE);
memcpy(pseudo_udp_header.source_ip,SA_IP_Address,	IP_ADDR_SIZE);
pseudo_udp_header.reserved = 0x0;
pseudo_udp_header.protocol = IP_PROTOCOL_UDP;
pseudo_udp_header.udp_length = switch_byte(UDP_HEADER_SIZE+UDP_DATA_SIZE);
pseudo_udp_header.source_port = switch_byte(UDP_SOURCE_PORT);
pseudo_udp_header.dest_port =(rx_udp_header->source_port);
pseudo_udp_header.length = switch_byte( UDP_HEADER_SIZE+UDP_DATA_SIZE);
pseudo_udp_header.crc = 0;
 collect_spi_data2(adc_data);
  //add udp_data

//  while(!udp_data_ready) {};

  int j =0;
  for (int i = 0; i<32; i++)
    {

      tx_udp_header->crc = 0;
      memcpy((uint8_t*)tx_udp_frame+(TX_PACKAGE_CONTROL_FIELD+ETHERNET_HEADER_SIZE+IP_HEADER_SIZE+UDP_HEADER_SIZE),(uint8_t*)adc_data_p+(j*1000),UDP_DATA_SIZE);
		tx_udp_header->crc = calc_crc16_udp((uint16_t*)&pseudo_udp_header,UDP_PSEUDO_HEADER_SIZE,(tx_byte_buffer+(ETHERNET_HEADER_SIZE+IP_HEADER_SIZE+UDP_HEADER_SIZE)), UDP_DATA_SIZE);
      tx_udp_frame[0]= ETHERNET_HEADER_SIZE+IP_HEADER_SIZE+UDP_HEADER_SIZE+UDP_DATA_SIZE;
      ETH_SendFrame(MDR_ETHERNET1,(uint32_t *)tx_udp_frame,*(uint32_t*)&tx_udp_frame[0]);
      j++;
      if( j ==16)
        {
          j=0;
        }


    }
		 PORT_ResetBits(MDR_PORTE,PORT_Pin_7);
 
  ethernet_states = LISTENING;


//    }


}

 void collect_spi_data2(uint16_t*spi_data_buff)
{


  for (int i =0 ; i<8000; i++)
    {
      spi_data_buff[i] = ( spi_transfer2(MDR_SSP1,0xAAAA));

    }


}
//void collect_spi_data(uint16_t*spi_data_buff)
//{
//  uint16_t adc_val;
//  uint8_t  adc_val_low;
//  uint8_t adc_val_high;
//  uint8_t tmp;
//  for (int i =0 ; i<8000/2; i++)
//    {
//      adc_val_low = spi_transfer(MDR_SSP1,0xAA);
//      adc_val_high = spi_transfer(MDR_SSP1,0xAA);
//      tmp = spi_transfer(MDR_SSP1,0xAA);
//      adc_val = adc_val_low | adc_val_high<<8;
//      spi_data_buff[i] = adc_val;
//    }
//}

void add_udp_data(uint8_t* output_data)
{






}
void icmp(uint32_t* packet)
{


  uint8_t* rx_byte_buffer = (uint8_t*)packet;
  ethernet_header_s* rx_ethernet_header = (ethernet_header_s*)rx_byte_buffer;
  ip_header_s* rx_ip_header = (ip_header_s*)(rx_byte_buffer+ETHERNET_HEADER_SIZE);
  icmp_header_s* rec_icmp_header = (icmp_header_s*)(rx_byte_buffer+ETHERNET_HEADER_SIZE+IP_HEADER_SIZE);

  uint8_t* tx_byte_buffer = (uint8_t*)output_frame+TX_PACKAGE_CONTROL_FIELD;
  ethernet_header_s* tx_ethernet_header =(ethernet_header_s*) tx_byte_buffer;
  ip_header_s* tx_ip_header = (ip_header_s*)(tx_byte_buffer+ETHERNET_HEADER_SIZE);
  icmp_header_s* tx_icmp_packet =(icmp_header_s*) (tx_byte_buffer+ETHERNET_HEADER_SIZE+IP_HEADER_SIZE);


  memcpy(tx_ethernet_header->dest_mac, rx_ethernet_header->source_mac,MAC_ADDR_SIZE);
  memcpy(tx_ethernet_header->source_mac,SA_MAC_Address,MAC_ADDR_SIZE);
  tx_ethernet_header->ethernet_type = ETH_TYPE_IPV4;

  tx_ip_header->version_ihl = 0x45;
  tx_ip_header->type_of_service = rx_ip_header->type_of_service;
  tx_ip_header->total_length = switch_byte(IP_HEADER_SIZE+ICMP_HEADER_SIZE+32);
  tx_ip_header->identification = rx_ip_header->identification;
  tx_ip_header->flags_and_fragment_offset = rx_ip_header->flags_and_fragment_offset;
  tx_ip_header->time_to_live = IP_TTL;
  tx_ip_header->high_level_protocol = IP_PROTOCOL_ICMP;
  memcpy(tx_ip_header->sender_ip,SA_IP_Address,	IP_ADDR_SIZE);
  memcpy(tx_ip_header->target_ip,rx_ip_header->sender_ip,	IP_ADDR_SIZE);
  tx_ip_header->crc = 0x0;
  tx_ip_header->crc = calc_crc16((uint16_t*)tx_ip_header,IP_HEADER_SIZE);


  tx_icmp_packet->code = 0x0;
  tx_icmp_packet->type = 0x0;
  tx_icmp_packet->identifier = rec_icmp_header->identifier;
  tx_icmp_packet->sequence = rec_icmp_header->sequence;
  memcpy(tx_byte_buffer+ETHERNET_HEADER_SIZE+IP_HEADER_SIZE+ICMP_HEADER_SIZE,rx_byte_buffer+ETHERNET_HEADER_SIZE+IP_HEADER_SIZE+ICMP_HEADER_SIZE,32);
  tx_icmp_packet->crc = 0;
  tx_icmp_packet->crc = calc_crc16((uint16_t*)tx_icmp_packet,ICMP_HEADER_SIZE+32);

  output_frame[0]= ETHERNET_HEADER_SIZE+IP_HEADER_SIZE+ICMP_HEADER_SIZE+32;
  ETH_SendFrame(MDR_ETHERNET1,(uint32_t *)output_frame,*(uint32_t*)&output_frame[0]);
  ethernet_states = LISTENING;

}


uint32_t sum_udp_data(uint16_t* header,uint16_t length)
{

  uint32_t sum = 0;
  for (int i=0; i<length/2; i++)
    {
      sum+=header[i];
    }
		 if (length%2 !=0)
    {
      sum+=*((uint8_t*)header+length-1);
    }
  while(sum>>16)
    {
      sum = (sum & 0xffff)+(sum>>16);
    }


  return sum;

}

uint16_t calc_crc16_udp(uint16_t* udp_pseudo_header,uint16_t udp_pseudo_header_length,uint8_t* udp_data, uint16_t udp_data_length)
{

  uint32_t sum = 0;
  sum +=sum_udp_data(udp_pseudo_header,udp_pseudo_header_length);
  sum +=sum_udp_data((uint16_t*)udp_data,udp_data_length);

  while(sum>>16)
    {
      sum = (sum & 0xffff)+(sum>>16);
    }

  return ~((uint16_t)sum);


}

uint16_t calc_crc16(uint16_t* header,uint8_t length)
{
  uint32_t sum = 0;
  for (int i=0; i<length/2; i++)
    {
      sum+=header[i];
    }
  if (length%2!=0)
    {
      sum+=*((uint8_t*)header+length-1);
    }

  while(sum>>16)
    {

      sum = (sum & 0xffff)+(sum>>16);
    }

  return ~((uint16_t)sum);

}


uint16_t switch_byte(uint16_t val)
{
  return ((val>>8)|(val<<8));
}

void delay(uint32_t ticks)
{
  while(ticks --);
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
      if (adc_data_counter == 0)
        {
          PORT_SetBits(MDR_PORTE,PORT_Pin_6);
        }

      udp_data_ready = false;
      uint16_t adc_val = ADC1_GetResult();
      float voltage = adc_val;
      voltage = voltage /4096*3.3*100;
      adc_data_p[adc_data_counter++] = switch_byte(voltage);
      if(adc_data_counter == 8000)
        {
          adc_data_counter = 0;

//          udp_data_p =(uint16_t*)tx_udp_frame+(TX_PACKAGE_CONTROL_FIELD+ETHERNET_HEADER_SIZE+IP_HEADER_SIZE+UDP_HEADER_SIZE)/2;
          udp_data_ready = true;
          NVIC_DisableIRQ(ADC_IRQn);
          ADC1_Cmd(DISABLE);
          PORT_ResetBits(MDR_PORTE,PORT_Pin_6);
          return;


        }

      ADC1_Start();

    }
  MDR_ADC->ADC1_STATUS = (ADCx_IT_END_OF_CONVERSION | ADCx_IT_OUT_OF_RANGE)<<2;
  NVIC_ClearPendingIRQ(ADC_IRQn);

}

void ETHERNET_IRQHandler(void)
{
  if(ETH_GetMACITStatus(MDR_ETHERNET1,ETH_MAC_IT_RF_OK))
    {
			rx_done_flag = true;
//      get_ethernet_packet(input_frame);
    }

  NVIC_ClearPendingIRQ(ETHERNET_IRQn);

}

