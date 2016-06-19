/*****************************************************************************
*
* File              : init_driver.c
* Compiler          : AVR GCC 3.4.2
* Revision          : $Revision: 0.1 $
* Date              : $Date: 2016/05/13 $
* Updated by        : $Author: Xie jian $
*
* Support mail      : 1483792843@qq.com
*
* Supported devices : Atmel ATmega256RFR2
*
* Description       : �شż��sensor������
*                     
*
****************************************************************************/
#include <avr/io.h>
#define uint unsigned int
#define uchar unsigned char


/*
*name:Mag_Sensor_Init
*function:���������ʼ��
*need:void
*return:void
*/
void Mag_Sensor_Init( void );
/*
*name:Power_On
*function:������Դ
*need:void
*return:void
*/
void Power_On(void);
/*
*name:Power_Off
*function:�رյ�Դ
*need:void
*return:void
*/
void Power_Off(void);
/*
*name: LED_Off
*function:�ر�LED
*need:void
*return:void
*/
void LED_Off(void);
/*
*name: LED_On
*function:����LED
*need:void
*return:void
*/
void LED_On(void);
/*
*name: ADC_Init
*function:ADC��ʼ�� 
*need:void
*return: void
*/
void ADC_Init(void);
/*
*name:ADC_Conver
*function:ADC�ɼ�
*need:void
*return: 16bit����Ч10bit��
*/
uint ADC_Conver(void);
/*
*name: Mag_X_Init
*function:X���ź�·����ʼ��
*need:void
*return:void
*/
void Mag_X_Init(void);
/*
*name: Mag_Y_Init
*function:X���ź�·����ʼ��
*need:void
*return:void
*/
void Mag_Y_Init(void);
/*
*name: Mag_Z_Init
*function:X���ź�·����ʼ��
*need:void
*return:void
*/
void Mag_Z_Init(void);
/*
*name: Mag_X_Sample
*function:X�����ݲɼ�
*need:void
*return:16bit����Ч10bit��
*/
uint Mag_X_Sample(void);
/*
*name: Mag_Y_Sample
*function:X�����ݲɼ�
*need:void
*return:16bit����Ч10bit��
*/
uint Mag_Y_Sample(void);
/*
*name: Mag_Z_Sample
*function:X�����ݲɼ�
*need:void
*return:16bit����Ч10bit��
*/
uint Mag_Z_Sample(void);
/*
*name:Mag_XYZ_SR
*function:�Ŵ�������λ
*need:void
*return:void
*/
void Mag_XYZ_SR(void);
/*
*name:Usart1_Int
*function��initialization USART
*need��bauddata (10bit)
*return:void
*/
void Usart1_Int(uint baud);
/*
*name:Usart1_Send
*function:send
*need:unsigned char
*return:void
*/
void Usart1_Send(uchar data);
/*
*name:Usart1_Rev
*function:receview
*need:void
*return��unsigned char��8bit��
*/
uchar Usart1_Rev(void);



/****************��������ʼ��******************/
/*
*function:sensor initialization
*/
void Mag_Sensor_Init(void)
{
		DDRG = (1<<DDG0)|(1<<DDG1);				//MAG_A_EN#��X��-->PG0	AMUX_EN#-->PG1
		PORTG |= (1<<PORTG0);					//Disable the X axis magnetic sensors
		PORTG &= ~(1<<PORTG1); 					//Disable the Multiplexer and select
		//PORTG &= ~(1<<PORTG0);				//Enable the X axis magnetic sensors
		//PORTG |= 1<<PORTG1;					//Enable the Multiplexer and select
		DDRE |= (1<<DDE0)|(1<<DDE4);			//MAG_B_EN#��Y��-->PE0  MAG_C_EN#��Z��-->PG0
		PORTE |= (1<<PORTE0)|(1<<PORTE4);		//Disable the Y and Z axis magnetic sensors
		//PORTE &= ~((1<<PORTE0)|(1<<PORTE4));	//Enable the Y and Z axis magnetic sensors
		DDRD |= 1<<DDD4;						//PWR_EN#-->PD4
		PORTD |= 1<<PORTD4;						//Disable the power��no power��
		//PORTD &= ~(1<<PORTD4);					//Enable the power ��power on��
		DDRD |= (1<<DDD0)|(1<<DDD1);		    //AMUX_ADDRA-->PD1��low��  AMUX_ADDRB-->PD0��high��select 0 channel��X��
		DDRB |= (1<<DDB7)|(1<<DDB0);			//PWM-->PB7		RS_STRAP#-->PB0
		PORTB &= ~(1<<PORTB7);					//LED off
		//PORTB |= (1<<PORTB7);					//LED on
}


/****************������Դ********************/
/*
*function:Power on
*/
void Power_On(void)
{
		DDRD |= 1<<DDD4;
		PORTD &= ~(1<<PORTD4); 					//Power on
}



/****************�رյ�Դ********************/
/*
*function:Power off
*/
void Power_Off(void)
{
		DDRD |= 1<<DDD4;
		PORTD |= 1<<PORTD4; 					//Power off
}



/****************�ر�LED********************/
/*
*function:LED off
*/
void LED_Off(void)
{
	DDRB |= 1<<DDB7;
	PORTB &= ~(1<<PORTB7);					//LED off	
}


/****************����LED********************/
/*
*function:LED on
*/
void LED_On(void)
{
	DDRB |= 1<<DDB7;
	PORTB |= 1<<PORTB7;						//LED on
}


/****************ADC��ʼ��*************/
/*
*function:ADC initialization
*/
void ADC_Init(void)
{
		ADCSRC = 10<<ADSUT0;               // set start-up time
		ADCSRB = 0<<MUX5;                  // set MUX5 first
		ADMUX = (0<<REFS1)|(1<<REFS0)|(0<<ADLAR)|(0<<MUX4)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0);  // store new ADMUX, 1.8V AREF(AVDD)
		//set prescaler
		ADCSRA =(6<<ADPS0);
}


/****************X�����ݲɼ�*************/
/*
*function:X sensor gather
*return:16bit����Ч10bit��
*/
uint Mag_X_Sample(void)
{
	PORTD &= ~((1<<PORTD0)|(1<<PORTD1));	//select X axis
	ADCSRA |=(1<<ADEN) + (1<<ADSC);			//ADC Enable &start
	do
	{} while( (ADCSRA & (1<<ADSC)));		// wait for conversion end
	ADCSRA &= ~(1<<ADEN);						//close enable
	return ADC;
}


/****************Y�����ݲɼ�*************/
/*
*function:Y sensor gather
*return:16bit����Ч10bit��
*/
uint Mag_Y_Sample(void)
{
	PORTD &= ~(1<<PORTD0);
	PORTD |= 1<<PORTD1;					//select Y axis
	ADCSRA |=(1<<ADEN) + (1<<ADSC);		//ADC Enable &start
	do
	{} while( (ADCSRA & (1<<ADSC)));	// wait for conversion end
	ADCSRA &= ~(1<<ADEN);					//close enable
	return ADC;
}


/****************Z�����ݲɼ�*************/
/*
*function:Z sensor gather
*return:16bit����Ч10bit��
*/
uint Mag_Z_Sample(void)
{
	PORTD |= 1<<PORTD0;
	PORTD &= ~(1<<PORTD1);	//select Z axis
	ADCSRA |=(1<<ADEN) + (1<<ADSC);		//ADC Enable &start
	do
	{} while( (ADCSRA & (1<<ADSC)));	// wait for conversion end
	ADCSRA &= ~(1<<ADEN);				//close enable
	return ADC;
}


/****************X���ź�·����ʼ��*************/
/*
*function��initialization X sensor
*/
void Mag_X_Init(void)
{
	PORTG |= 1<<PORTG1;					//Enable the  Multiplexer and select
	PORTG &= ~(1<<PORTG0);		    		//Enable the X axis magnetic sensors
}


/****************Y���ź�·����ʼ��*************/
/*
*function��initialization Y sensor
*/
void Mag_Y_Init(void)
{
	PORTG |= 1<<PORTG1;					//Enable the  Multiplexer and select
	PORTE &= ~(1<<PORTE0);				//Enable the Y axis magnetic sensors
}


/****************Z���ź�·����ʼ��*************/
/*
*function��initialization Z sensor
*/
void Mag_Z_Init(void)
{
	PORTG |= 1<<PORTG1;					//Enable the  Multiplexer and select
	PORTE &= ~(1<<PORTE4);				//Enable the Z axis magnetic sensors
}


/****************�Ŵ�������λ*************/
/*
*function��reset sensor
*/
void Mag_XYZ_SR(void)
{
	PORTB |=1<<PORTB0;				//reset
	for(uchar i=0;i<100;i++);
	PORTB &= ~(1<<PORTB0);				//close reset	
}

/****************���ڳ�ʼ��*************/
/*
*function��initialization USART
*need��bauddata (10bit)
*/
void Usart1_Int(uint baud)
{
	UBRR1H =(unsigned char)(baud>>8);
	UBRR1L =(unsigned char)baud;
	//�շ�ʹ��
	UCSR1B =( (1<<TXEN1) | (1<<RXEN1) );
	//8����λ 1ֹͣλ ��У��λ
	UCSR1C =( (1<<UCSZ11) | (1<<UCSZ10) );
}
/****************���ڷ���*************/
/*
*function:USART send
*need:unsigned char
*/
void Usart1_Send(uchar data)
{
	while(!(UCSR1A&(1<<UDRE1)));//��ѯ�����Ĵ����Ƿ�Ϊ�գ�UDRE1ΪUDR1Ϊ�յı�־
	
	UDR1 = data;
}

/****************���ڽ���*************/
/*
*function:USART receview
*return��unsigned char
*/
uchar Usart1_Rev(void)
{
	uchar data;
	while (!(UCSR1A&(1<<RXC1)));//��ѯ�����Ĵ��������Ƿ������ɣ�RXC1Ϊ������ɵı�־
	data = UDR1;
	
	return data;
}

