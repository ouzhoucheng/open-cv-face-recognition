/*******************************************************************************
* 文件名		: mlx90614.c
* 作  者	: 
* 版  本	: 
* 日  期	: 2013-08-07
* 描  述	: mlx90614函数
PB6：SCL
PB7：SDA
在主函数中先初始化SMBus_Init();
需要读取温度就调用temp=SMBus_ReadTemp();  //读取温度，temp是浮点数，转整数：i=ceil(temp); 
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "mlx90614.h"
#include <stdio.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define SMBUS_SCK_H()	    HAL_GPIO_WritePin(SMBUS_PORT, SMBUS_SCK, GPIO_PIN_SET); //置高电平
#define SMBUS_SCK_L()	    HAL_GPIO_WritePin(SMBUS_PORT, SMBUS_SCK, GPIO_PIN_RESET);  //置低电平
#define SMBUS_SDA_H()	    HAL_GPIO_WritePin(SMBUS_PORT, SMBUS_SDA, GPIO_PIN_SET);
#define SMBUS_SDA_L()	    HAL_GPIO_WritePin(SMBUS_PORT, SMBUS_SDA, GPIO_PIN_RESET);

#define SMBUS_SMBUS_SDA()	   PCin(7) //读取引脚电平


/**************************************GXY*******************************************/
//IO方向设置
//#define SMBUS_SDA_IN()  {GPIOC->MODER&=~(3<<(7*2));GPIOC->MODER|=0<<7*2;delay_us(1);}	//PB11输入模式
//#define SMBUS_SDA_OUT() {GPIOC->MODER&=~(3<<(7*2));GPIOC->MODER|=1<<7*2;delay_us(1);} //PB11输出模式



/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : SMBus_StartBit
* Description    : Generate START condition on SMBus
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SMBus_StartBit(void)
{
//	SMBUS_SDA_OUT();
    HAL_GPIO_WritePin(SMBUS_SDA_PORT, SMBUS_SDA, 1);		// Set SDA line
    
    SMBus_Delay(35);	    // Wait a few microseconds
    HAL_GPIO_WritePin(SMBUS_SCK_PORT, SMBUS_SCK, 1);		// Set SCL line
    SMBus_Delay(35);	    // Generate bus free time between Stop
    HAL_GPIO_WritePin(SMBUS_SDA_PORT, SMBUS_SDA, 0);		// Clear SDA line
    SMBus_Delay(35);	    // Hold time after (Repeated) Start
    // Condition. After this period, the first clock is generated.
    //(Thd:sta=4.0us min)在SCK=1时，检测到SDA由1到0表示通信开始（下降沿）
    HAL_GPIO_WritePin(SMBUS_SCK_PORT, SMBUS_SCK, 0);	    // Clear SCL line
    SMBus_Delay(35);	    // Wait a few microseconds
}

/*******************************************************************************
* Function Name  : SMBus_StopBit
* Description    : Generate STOP condition on SMBus
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SMBus_StopBit(void)
{
//	SMBUS_SDA_OUT();
    HAL_GPIO_WritePin(SMBUS_SCK_PORT, SMBUS_SCK, 0);		// Clear SCL line
    SMBus_Delay(35);	    // Wait a few microseconds
    HAL_GPIO_WritePin(SMBUS_SDA_PORT, SMBUS_SDA, 0);		// Clear SDA line
    SMBus_Delay(35);	    // Wait a few microseconds
    HAL_GPIO_WritePin(SMBUS_SCK_PORT, SMBUS_SCK, 1);		// Set SCL line
    SMBus_Delay(35);	    // Stop condition setup time(Tsu:sto=4.0us min)
    HAL_GPIO_WritePin(SMBUS_SDA_PORT, SMBUS_SDA, 1);		// Set SDA line在SCK=1时，检测到SDA由0到1表示通信结束（上升沿）
}

/*******************************************************************************
* Function Name  : SMBus_SendByte
* Description    : Send a byte on SMBus
* Input          : Tx_buffer
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t SMBus_SendByte(uint8_t Tx_buffer)
{
    uint8_t	Bit_counter;
    uint8_t	Ack_bit;
    uint8_t	bit_out;

    for(Bit_counter=8; Bit_counter; Bit_counter--)
    {
        if (Tx_buffer&0x80)
        {
            bit_out=1;   // If the current bit of Tx_buffer is 1 set bit_out
        }
        else
        {
            bit_out=0;  // else clear bit_out
        }
        SMBus_SendBit(bit_out);		// Send the current bit on SDA
        Tx_buffer<<=1;				// Get next bit for checking
    }

    Ack_bit=SMBus_ReceiveBit();		// Get acknowledgment bit
    return	Ack_bit;
}

/*******************************************************************************
* Function Name  : SMBus_SendBit
* Description    : Send a bit on SMBus 82.5kHz
* Input          : bit_out
* Output         : None
* Return         : None
*******************************************************************************/
void SMBus_SendBit(uint8_t bit_out)
{
//	SMBUS_SDA_OUT();
    if(bit_out==0)
    {
        HAL_GPIO_WritePin(SMBUS_SDA_PORT, SMBUS_SDA, 0);
    }
    else
    {
        HAL_GPIO_WritePin(SMBUS_SDA_PORT, SMBUS_SDA, 1);
    }
    SMBus_Delay(15);					// Tsu:dat = 250ns minimum
    HAL_GPIO_WritePin(SMBUS_SCK_PORT, SMBUS_SCK, 1);					// Set SCL line
    SMBus_Delay(25);					// High Level of Clock Pulse
    HAL_GPIO_WritePin(SMBUS_SCK_PORT, SMBUS_SCK, 0);					// Clear SCL line
    SMBus_Delay(20);					// Low Level of Clock Pulse
//	HAL_GPIO_WritePin(SMBUS_SDA_PORT, SMBUS_SDA, 1);				    // Master release SDA line ,
    return;
}

/*******************************************************************************
* Function Name  : SMBus_ReceiveBit
* Description    : Receive a bit on SMBus
* Input          : None
* Output         : None
* Return         : Ack_bit
*******************************************************************************/
uint8_t SMBus_ReceiveBit(void)
{
    uint8_t Ack_bit;
//	SMBUS_SDA_OUT();
    HAL_GPIO_WritePin(SMBUS_SDA_PORT, SMBUS_SDA, 1);          //引脚靠外部电阻上拉，当作输入
		SMBus_Delay(15);			// High Level of Clock Pulse
    HAL_GPIO_WritePin(SMBUS_SCK_PORT, SMBUS_SCK, 1);			// Set SCL line
    SMBus_Delay(35);			// High Level of Clock Pulse
//		SMBUS_SDA_IN();
    if (HAL_GPIO_ReadPin(SMBUS_SDA_PORT, SMBUS_SDA))
    {
        Ack_bit=1;
    }
    else
    {
        Ack_bit=0;
    }
    HAL_GPIO_WritePin(SMBUS_SCK_PORT, SMBUS_SCK, 0);			// Clear SCL line
    SMBus_Delay(20);			// Low Level of Clock Pulse

    return	Ack_bit;
}

/*******************************************************************************
* Function Name  : SMBus_ReceiveByte
* Description    : Receive a byte on SMBus
* Input          : ack_nack
* Output         : None
* Return         : RX_buffer
*******************************************************************************/
uint8_t SMBus_ReceiveByte(uint8_t ack_nack)
{
    uint8_t 	RX_buffer;
    uint8_t	Bit_Counter;

    for(Bit_Counter=8; Bit_Counter; Bit_Counter--)
    {
        if(SMBus_ReceiveBit())			// Get a bit from the SDA line
        {
            RX_buffer <<= 1;			// If the bit is HIGH save 1  in RX_buffer
            RX_buffer |=0x01;
        }
        else
        {
            RX_buffer <<= 1;			// If the bit is LOW save 0 in RX_buffer
            RX_buffer &=0xfe;
        }
    }
    SMBus_SendBit(ack_nack);			// Sends acknowledgment bit
    return RX_buffer;
}

/*******************************************************************************
* Function Name  : SMBus_Delay
* Description    : 延时  一次循环约1us
* Input          : time
* Output         : None
* Return         : None
*******************************************************************************/
void SMBus_Delay(uint16_t time)
{
    uint16_t i, j;
    for (i=0; i<4; i++)
    {
        for (j=0; j<time; j++);
    }
}

/*******************************************************************************
* Function Name  : SMBus_Init
* Description    : SMBus初始化
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void SMBus_Init()
//{
//	  GPIO_InitTypeDef  GPIO_InitStructure;
//		/* Enable SMBUS_PORT clocks */
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_SMBUS_PORT,ENABLE);
//    /*配置SMBUS_SCK、SMBUS_SDA为集电极推挽输出*/
//  GPIO_InitStructure.GPIO_Pin = SMBUS_SCK | SMBUS_SDA;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//推挽输出
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//上拉
//  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化
//	   HAL_GPIO_WritePin(SMBUS_SCK_PORT, SMBUS_SCK, 1);
//    HAL_GPIO_WritePin(SMBUS_SDA_PORT, SMBUS_SDA, 1);
//}

/*******************************************************************************
 * Function Name  : SMBus_ReadMemory
 * Description    : READ DATA FROM RAM/EEPROM
 * Input          : slaveAddress, command
 * Output         : None
 * Return         : Data
*******************************************************************************/
    uint16_t data;			// Data storage (DataH:DataL)
    uint8_t DataL=0;			// Low data byte storage
    uint8_t DataH=0;			// High data byte storage
uint16_t SMBus_ReadMemory(uint8_t slaveAddress, uint8_t command)
{

    uint8_t Pec;				// PEC byte storage

    uint8_t arr[6];			// Buffer for the sent bytes
    uint8_t PecReg;			// Calculated PEC byte storage
    uint8_t ErrorCounter;	// Defines the number of the attempts for communication with MLX90614

    ErrorCounter=0x00;				// Initialising of ErrorCounter
	slaveAddress <<= 1;	//2-7位表示从机地址
	
    do
    {
repeat:
        SMBus_StopBit();			    //If slave send NACK stop comunication
        --ErrorCounter;				    //Pre-decrement ErrorCounter
        if(!ErrorCounter) 			    //ErrorCounter=0?
        {
            break;					    //Yes,go out from do-while{}
        }

        SMBus_StartBit();				//Start condition
        if(SMBus_SendByte(slaveAddress))//Send SlaveAddress 最低位Wr=0表示接下来写命令
        {
            goto	repeat;			    //Repeat comunication again
        }
        if(SMBus_SendByte(command))	    //Send command
        {
            goto	repeat;		    	//Repeat comunication again
        }

        SMBus_StartBit();					//Repeated Start condition
        if(SMBus_SendByte(slaveAddress+1))	//Send SlaveAddress 最低位Rd=1表示接下来读数据
        {
            goto	repeat;             	//Repeat comunication again
        }

        DataL = SMBus_ReceiveByte(ACK);	//Read low data,master must send ACK
        DataH = SMBus_ReceiveByte(ACK); //Read high data,master must send ACK
        Pec = SMBus_ReceiveByte(NACK);	//Read PEC byte, master must send NACK
        SMBus_StopBit();				//Stop condition

        arr[5] = slaveAddress;		//
        arr[4] = command;			//
        arr[3] = slaveAddress+1;	//Load array arr
        arr[2] = DataL;				//
        arr[1] = DataH;				//
        arr[0] = 0;					//
        PecReg=PEC_Calculation(arr);//Calculate CRC
    }
    while(PecReg != Pec);		//If received and calculated CRC are equal go out from do-while{}

	data = (DataH<<8) | DataL;	//data=DataH:DataL
    return data;
}

void SMBus_WriteMemory(uint8_t slaveAddress, uint8_t command, uint16_t Emissivity)
{
    uint8_t arr[6];			// Buffer for the sent bytes
    uint8_t PecReg;			// Calculated PEC byte storage
    uint8_t ErrorCounter;	// Defines the number of the attempts for communication with MLX90614

    ErrorCounter=0x00;				// Initialising of ErrorCounter
		slaveAddress <<= 1;	//2-7位表示从机地址
	
		arr[5] = 0;		//
		arr[4] = slaveAddress;			//
		arr[3] = command;	//Load array arr
		arr[2] = Emissivity;				//
		arr[1] = Emissivity>>8;				//
		arr[0] = 0;					//
		PecReg=PEC_Calculation(arr);//Calculate CRC
    do
    {
repeat:
        SMBus_StopBit();			    //If slave send NACK stop comunication
        --ErrorCounter;				    //Pre-decrement ErrorCounter
        if(!ErrorCounter) 			    //ErrorCounter=0?
        {
            break;					    //Yes,go out from do-while{}
        }

        SMBus_StartBit();				//Start condition
        if(SMBus_SendByte(slaveAddress))//Send SlaveAddress 最低位Wr=0表示接下来写命令
        {
            goto	repeat;			    //Repeat comunication again
        }
        if(SMBus_SendByte(command))	    //Send command
        {
            goto	repeat;		    	//Repeat comunication again 
        }
				if(SMBus_SendByte(Emissivity))	    //Send command
        {
            goto	repeat;		    	//Repeat comunication again 
        }
				if(SMBus_SendByte(Emissivity>>8))	    //Send command
        {
            goto	repeat;		    	//Repeat comunication again 
        }
			
				if(SMBus_SendByte(PecReg))	    //Send command
        {
            goto	repeat;		    	//Repeat comunication again 
        }
    }while(1);		//If received and calculated CRC are equal go out from do-while{}
}

/*******************************************************************************
* Function Name  : PEC_calculation
* Description    : Calculates the PEC of received bytes
* Input          : pec[]
* Output         : None
* Return         : pec[0]-this byte contains calculated crc value
*******************************************************************************/
uint8_t PEC_Calculation(uint8_t pec[])
{
    uint8_t 	crc[6];
    uint8_t	BitPosition=47;
    uint8_t	shift;
    uint8_t	i;
    uint8_t	j;
    uint8_t	temp;

    do
    {
        /*Load pattern value 0x000000000107*/
        crc[5]=0;
        crc[4]=0;
        crc[3]=0;
        crc[2]=0;
        crc[1]=0x01;
        crc[0]=0x07;

        /*Set maximum bit position at 47 ( six bytes byte5...byte0,MSbit=47)*/
        BitPosition=47;

        /*Set shift position at 0*/
        shift=0;

        /*Find first "1" in the transmited message beginning from the MSByte byte5*/
        i=5;
        j=0;
        while((pec[i]&(0x80>>j))==0 && i>0)
        {
            BitPosition--;
            if(j<7)
            {
                j++;
            }
            else
            {
                j=0x00;
                i--;
            }
        }/*End of while */

        /*Get shift value for pattern value*/
        shift=BitPosition-8;

        /*Shift pattern value */
        while(shift)
        {
            for(i=5; i<0xFF; i--)
            {
                if((crc[i-1]&0x80) && (i>0))
                {
                    temp=1;
                }
                else
                {
                    temp=0;
                }
                crc[i]<<=1;
                crc[i]+=temp;
            }/*End of for*/
            shift--;
        }/*End of while*/

        /*Exclusive OR between pec and crc*/
        for(i=0; i<=5; i++)
        {
            pec[i] ^=crc[i];
        }/*End of for*/
    }
    while(BitPosition>8); /*End of do-while*/

    return pec[0];
}

 /*******************************************************************************
 * Function Name  : SMBus_ReadTemp
 * Description    : Calculate and return the temperature
 * Input          : None
 * Output         : None
 * Return         : SMBus_ReadMemory(0x00, 0x07)*0.02-273.15
*******************************************************************************/
float SMBus_ReadTemp(void)
{   
	float temp;
	temp = SMBus_ReadMemory(SA, RAM_ACCESS|RAM_TOBJ1)*0.02-273.15;
	return temp;
}

float SMBus_ReadTemp2(void)
{   
	float temp;
	temp = SMBus_ReadMemory(SA, RAM_ACCESS|RAM_TOBJ2)*0.02-273.15;
	return temp;
}

float SMBus_ReadTempA(void)
{   
	float temp;
	temp = SMBus_ReadMemory(SA, RAM_ACCESS|RAM_TA)*0.02-273.15;
	return temp;
}

//parameter init
void Parameter_Init(int ReflectRate)														//参数初始化
{
	SMBus_WriteMemory(SA, EEPROM_ACCESS|0x04, ReflectRate);
	HAL_Delay(100);
}

//T
extern float Tem_Real, Q, Tem_Avg;
extern float Pkn1, Pk, Zkn1, Zk, Xkn1, Xk, XkA, K;
extern float T1, T2, T3, T4, T5;
extern float T11, T22, T33, T44, T55;
void T_Kalmen(char T_Choose)
{
    if(T_Choose == 1){
        T1 = SMBus_ReadTemp();HAL_Delay(10);
        T2 = SMBus_ReadTemp();HAL_Delay(10);
        T3 = SMBus_ReadTemp();HAL_Delay(10);
        T4 = SMBus_ReadTemp();HAL_Delay(10);
        T5 = SMBus_ReadTemp();HAL_Delay(10);
    }
    else if(T_Choose == 0) {
        T1 = SMBus_ReadTempA();HAL_Delay(10);
        T2 = SMBus_ReadTempA();HAL_Delay(10);
        T3 = SMBus_ReadTempA();HAL_Delay(10);
        T4 = SMBus_ReadTempA();HAL_Delay(10);
        T5 = SMBus_ReadTempA();HAL_Delay(10);
    }
    
    Xkn1 = (T1+T2+T3+T4+T5)/5.0;
    
    T1 = (T1 - Xkn1)*(T1 - Xkn1);
    T2 = (T2 - Xkn1)*(T2 - Xkn1);
    T3 = (T3 - Xkn1)*(T3 - Xkn1);
    T4 = (T4 - Xkn1)*(T4 - Xkn1);
    T5 = (T5 - Xkn1)*(T5 - Xkn1);
    
    Tem_Avg = (T1+T2+T3+T4+T5)/5.0;
    
    Tem_Avg = sqrt(Tem_Avg);

    
    Pk = T5 - T4 + Q;

    K = Pk/(Pk + Tem_Avg);

    if(T_Choose == 1){
        Zk = SMBus_ReadTemp();
        Xk = Xkn1 + K*(Zk-Xkn1);
    }
    else if(T_Choose == 0){
        Zk = SMBus_ReadTempA();
        XkA = Xkn1 + K*(Zk-Xkn1);
    }
}
float TemMax=0.0, TemMin=45.0;
extern float TemGet[20], TemSum;
void T_Advance(char T_Choose)
{
    if(T_Choose == 1){
        for(char count=0; count<20; count++){
            TemGet[count] = SMBus_ReadTemp();
            TemSum += TemGet[count];
            if(TemMax<TemGet[count])TemMax=TemGet[count];
            if(TemMin>TemGet[count])TemMin=TemGet[count];
        }
    }
    else if(T_Choose == 0) {
//        T1 = SMBus_ReadTempA();HAL_Delay(10);
//        T2 = SMBus_ReadTempA();HAL_Delay(10);
//        T3 = SMBus_ReadTempA();HAL_Delay(10);
//        T4 = SMBus_ReadTempA();HAL_Delay(10);
//        T5 = SMBus_ReadTempA();HAL_Delay(10);
    }
    
//    Xkn1 = (T1+T2+T3+T4+T5)/5.0;
    Xkn1 = (TemSum - TemMax - TemMin - TemGet[0]\
           - TemGet[19] - TemGet[1] - TemGet[18]) / 14.0;
    
//    T1 = (T1 - Xkn1)*(T1 - Xkn1);
//    T2 = (T2 - Xkn1)*(T2 - Xkn1);
//    T3 = (T3 - Xkn1)*(T3 - Xkn1);
//    T4 = (T4 - Xkn1)*(T4 - Xkn1);
//    T5 = (T5 - Xkn1)*(T5 - Xkn1);
//    
//    Tem_Avg = (T1+T2+T3+T4+T5)/5.0;
//    
//    Tem_Avg = sqrt(Tem_Avg);

    
//    Pk = T5 - T4 + Q;

//    K = Pk/(Pk + Tem_Avg);

    if(T_Choose == 1){
//        Zk = SMBus_ReadTemp();
//        Xk = Xkn1 + K*(Zk-Xkn1);
        Xk = Xkn1;
    }
    else if(T_Choose == 0){
        Zk = SMBus_ReadTempA();
        XkA = Xkn1 + K*(Zk-Xkn1);
    }
}

void Real_Kalmen()
{
    T_Kalmen(1);HAL_Delay(10);
    T_Kalmen(0);HAL_Delay(10);
    T11 = Xk + (Xk - XkA)*0.26;
    T_Kalmen(1);HAL_Delay(10);
    T_Kalmen(0);HAL_Delay(10);
    T22 = Xk + (Xk - XkA)*0.26;
    T_Kalmen(1);HAL_Delay(10);
    T_Kalmen(0);HAL_Delay(10);
    T33 = Xk + (Xk - XkA)*0.26;
    T_Kalmen(1);HAL_Delay(10);
    T_Kalmen(0);HAL_Delay(10);
    T44 = Xk + (Xk - XkA)*0.26;
    T_Kalmen(1);HAL_Delay(10);
    T_Kalmen(0);HAL_Delay(10);
    T55 = Xk + (Xk - XkA)*0.26;
    
    Xkn1 = (T11+T22+T33+T44+T55)/5.0;
    T11 = (T1 - Xkn1)*(T1 - Xkn1);
    T22 = (T2 - Xkn1)*(T2 - Xkn1);
    T33 = (T3 - Xkn1)*(T3 - Xkn1);
    T44 = (T4 - Xkn1)*(T4 - Xkn1);
    T55 = (T5 - Xkn1)*(T5 - Xkn1);
    
    Tem_Avg = (T11+T22+T33+T44+T55)/5.0;
    
    Tem_Avg = sqrt(Tem_Avg);
    
    Pk = T55 - T44 + Q;

    K = Pk/(Pk + Tem_Avg);
    
    Tem_Real = Xkn1 + K*(T55-Xkn1);
}

/*********************************END OF FILE*********************************/
