#include "i2c.h"
	
GPIO_TypeDef* I2C_PORT_SDA;
GPIO_TypeDef* I2C_PORT_SCL;
uint16_t I2C_PIN_SDA;
uint16_t I2C_PIN_SCL;
uint8_t I2C_ADDR;

/*********************************************************************************
* Function: I2C_Start
* Description: I2C protocol, start singal: SCL stay high, then SDA change from high to low
* Input: none
* Output: none
* Return: none
* Call: internal
*/
void I2C_Start(void)
{
	GPIO_SetBits(I2C_PORT_SDA, I2C_PIN_SDA);
	GPIO_SetBits(I2C_PORT_SCL, I2C_PIN_SCL);
	I2C_Delay();
	GPIO_ResetBits(I2C_PORT_SDA, I2C_PIN_SDA);
	I2C_Delay();
	GPIO_ResetBits(I2C_PORT_SCL, I2C_PIN_SCL);
	I2C_Delay();
}

/*********************************************************************************
* Function: I2C_Stop
* Description: I2C protocol, stop singal: SCL stay high, then SDA change from low to high
* Input: none
* Output: none
* Return: none
* Call: internal
*/
void I2C_Stop(void)
{
	GPIO_ResetBits(I2C_PORT_SCL, I2C_PIN_SCL);
	GPIO_ResetBits(I2C_PORT_SDA, I2C_PIN_SDA);
	I2C_Delay();
	GPIO_SetBits(I2C_PORT_SCL, I2C_PIN_SCL);
	I2C_Delay();
	GPIO_SetBits(I2C_PORT_SDA, I2C_PIN_SDA);
	I2C_Delay();
}

/*********************************************************************************
* Function: I2C_Ack
* Description: I2C protocol, ack
* Input: none
* Output: none
* Return: none
* Call: internal
*/
void I2C_Ack(void)
{
	GPIO_ResetBits(I2C_PORT_SDA, I2C_PIN_SDA);
	I2C_Delay();
	GPIO_SetBits(I2C_PORT_SCL, I2C_PIN_SCL);
	I2C_Delay();
	GPIO_ResetBits(I2C_PORT_SCL, I2C_PIN_SCL);
	I2C_Delay();
	GPIO_SetBits(I2C_PORT_SDA, I2C_PIN_SDA);
}

/*********************************************************************************
* Function: I2C_Uak
* Description: I2C protocol, unack
* Input: none
* Output: none
* Return: none
* Call: internal
*/
void I2C_Uak(void)
{
	GPIO_SetBits(I2C_PORT_SDA, I2C_PIN_SDA);
	I2C_Delay();
	GPIO_SetBits(I2C_PORT_SCL, I2C_PIN_SCL);
	I2C_Delay();
	GPIO_ResetBits(I2C_PORT_SCL, I2C_PIN_SCL);
	I2C_Delay();
}

/*********************************************************************************
* Function: I2C_Check_Ack
* Description: I2C protocol, check ack signal for the simulate I2C port
* Input: none
* Output: none
* Return: ack result, 0 is ack, 1 is fail
* Call: internal
*/
FlagStatus I2C_Check_Ack(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	FlagStatus Ack;

	GPIO_ResetBits(I2C_PORT_SCL, I2C_PIN_SCL);
	GPIO_SetBits(I2C_PORT_SDA, I2C_PIN_SDA);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = I2C_PIN_SDA;
	GPIO_Init(I2C_PORT_SDA, &GPIO_InitStructure);

	I2C_Delay();
	GPIO_SetBits(I2C_PORT_SCL, I2C_PIN_SCL);
	I2C_Delay();
	if (GPIO_ReadInputDataBit(I2C_PORT_SDA, I2C_PIN_SDA) == SET)
	{
		Ack = SET;
	}
	else
	{
		Ack = RESET;
	}
	GPIO_ResetBits(I2C_PORT_SCL, I2C_PIN_SCL);
	I2C_Delay();
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = I2C_PIN_SDA;
	GPIO_Init(I2C_PORT_SDA, &GPIO_InitStructure);
	
	I2C_Delay();
	GPIO_SetBits(I2C_PORT_SDA, I2C_PIN_SDA);
	
	return Ack;
}

/*********************************************************************************
* Function: I2C_Tx
* Description: I2C protocol, send byte, high bit first, data change when SCL is low
* Input: -Data, send data
* Output: none
* Return: none
* Call: internal
*/
 void I2C_Tx(uint8_t Data)
{
	uint8_t Bit_Pos = 0x80;
	
	while (Bit_Pos)
	{  	
		GPIO_ResetBits(I2C_PORT_SCL, I2C_PIN_SCL);	
		I2C_Delay();	
		if (Data & Bit_Pos)
		{
			GPIO_SetBits(I2C_PORT_SDA, I2C_PIN_SDA);
		}
		else
		{
			GPIO_ResetBits(I2C_PORT_SDA, I2C_PIN_SDA);
		}
		I2C_Delay();
		GPIO_SetBits(I2C_PORT_SCL, I2C_PIN_SCL);
		I2C_Delay();				
		Bit_Pos >>= 1;
	}
	GPIO_ResetBits(I2C_PORT_SCL, I2C_PIN_SCL);
}

/*********************************************************************************
* Function: I2C_Rx
* Description: I2C protocol, receive byte, high bit first, data effective when SCL is high
* Input: none
* Output: none
* Return: receive data
* Call: internal
*/
 uint8_t I2C_Rx(void)
{
	uint8_t Bit_Pos = 0x80;
	uint8_t Data = 0x00;
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = I2C_PIN_SDA;
	GPIO_Init(I2C_PORT_SDA, &GPIO_InitStructure);

	GPIO_ResetBits(I2C_PORT_SCL, I2C_PIN_SCL);  
	I2C_Delay();  
	while (Bit_Pos)
	{
		GPIO_SetBits(I2C_PORT_SCL, I2C_PIN_SCL);
		I2C_Delay();   
    if (GPIO_ReadInputDataBit(I2C_PORT_SDA, I2C_PIN_SDA) == SET)
		{
			Data |= Bit_Pos;	
		}
		I2C_Delay(); 	      
    GPIO_ResetBits(I2C_PORT_SCL, I2C_PIN_SCL);  
		I2C_Delay();  	  
		Bit_Pos >>= 1;			 
	} 
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = I2C_PIN_SDA;
	GPIO_Init(I2C_PORT_SDA, &GPIO_InitStructure);

	GPIO_SetBits(I2C_PORT_SDA, I2C_PIN_SDA);
	
	return Data;
}

/*********************************************************************************
* Function: I2C_Write
* Description: I2C protocol, write circle
* Input: -Data, write data
* Output: none
* Return: ack result
* Call: internal
*/
 FlagStatus I2C_Write(uint8_t Data)
{
	FlagStatus Ack = RESET;

	I2C_Tx(Data);
	if (I2C_Check_Ack() == SET)
	{
		Ack = SET;
	}

	return Ack;
}

/*********************************************************************************
* Function: I2C_Read_Ack
* Description: I2C protocol, ack read circle
* Input: none
* Output: none
* Return: read data
* Call: internal
*/
 uint8_t I2C_Read_Ack(void)
{
	uint8_t Data;

	Data = I2C_Rx();
	I2C_Ack();

	return Data;
}

/*********************************************************************************
* Function: I2C_Read_Uak
* Description: I2C protocol, unack read circle
* Input: none
* Output: none
* Return: read data
* Call: internal
*/
 uint8_t I2C_Read_Uak(void)
{
	uint8_t Data;

	Data = I2C_Rx();
	I2C_Uak();

	return Data;
}

/*********************************************************************************
* Function: I2C_Write_Byte
* Description: I2C protocol, single byte write circle
* Input: -Addr, write address
* Input: -Data, write data
* Output: none
* Return: write result, 0 is success, 1 is fail
* Call: external
*/
 FlagStatus I2C_Write_Byte(uint8_t Addr, uint8_t Data)
{
	uint8_t Try_Count = 0;
	FlagStatus Ack;
	
	while (Try_Count++ < 5)
	{
		I2C_Start();
		
		if (I2C_Write((I2C_ADDR) & 0xFE) == SET) //device addressing
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		if (I2C_Write(Addr) == SET) //write address
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		if (I2C_Write(Data) == SET) //write data
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		I2C_Stop();
		Ack = RESET;
		break;
	}
	
	return Ack;
}

/*********************************************************************************
* Function: I2C_Write_Byte16
* Description: I2C protocol, single byte write circle
* Input: -Addr, write address
* Input: -Data, write data
* Output: none
* Return: write result, 0 is success, 1 is fail
* Call: external
*/
 FlagStatus I2C_Write_Byte16(uint16_t Addr, uint8_t Data)
{
	uint8_t Try_Count = 0;
	FlagStatus Ack;
	
	while (Try_Count++ < 5)
	{
		I2C_Start();
		
		if (I2C_Write((I2C_ADDR) & 0xFE) == SET) //device addressing
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		if (I2C_Write(Addr >> 8) == SET) //write address
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		if (I2C_Write(Addr) == SET) //write address
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		if (I2C_Write(Data) == SET) //write data
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		I2C_Stop();
		Ack = RESET;
		break;
	}
	
	return Ack;
}

/*********************************************************************************
* Function: I2C_Read_Byte
* Description: I2C protocol, single byte read circle
* Input: -Addr, read address
* Output: -Data, read data
* Return: read result, 0 is success, 1 is fail
* Call: external
*/
 FlagStatus I2C_Read_Byte(uint8_t Addr, uint8_t Data)
{
	uint8_t Try_Count = 0;
	FlagStatus Ack;
	
	while (Try_Count++ < 5)
	{
		I2C_Start();
		
		if (I2C_Write((I2C_ADDR) & 0xFE) == SET) //device addressing
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		if (I2C_Write(Addr) == SET) //write address
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		I2C_Start();
		
		if (I2C_Write((I2C_ADDR) | 0x01) == SET) //device addressing
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		Data = I2C_Read_Uak();
		
		I2C_Stop();
		Ack = RESET;
		break;
	}
	
	return Ack;
}

/*********************************************************************************
* Function: I2C_Read_Byte16
* Description: I2C protocol, single byte read circle
* Input: -Addr, read address
* Output: -Data, read data
* Return: read result, 0 is success, 1 is fail
* Call: external
*/
 FlagStatus I2C_Read_Byte16(uint16_t Addr, uint8_t Data)
{
	uint8_t Try_Count = 0;
	FlagStatus Ack;
	
	while (Try_Count++ < 5)
	{
		I2C_Start();
		
		if (I2C_Write((I2C_ADDR) & 0xFE) == SET) //device addressing
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		if (I2C_Write(Addr >> 8) == SET) //write address
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		if (I2C_Write(Addr) == SET) //write address
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		I2C_Start();
		
		if (I2C_Write((I2C_ADDR) | 0x01) == SET) //device addressing
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		Data = I2C_Read_Uak();
		
		I2C_Stop();
		Ack = RESET;
		break;
	}
	
	return Ack;
}

/*********************************************************************************
* Function: I2C_Sequential_Write
* Description: I2C protocol, several bytes write
* Input: -Addr, start address to be write
* Input: -Data, point to the data to be write
* Input: -Data_Len, how many data to be write
* Output: none
* Return: write result, 0 is success, 1 is fail
* Call: external
*/
 FlagStatus I2C_Sequential_Write(uint8_t Addr, uint8_t* Data, uint8_t Data_Len)
{
	uint8_t Try_Count = 0;
	FlagStatus Ack = RESET;
	uint8_t* Curr_Data = Data;
	uint8_t Curr_Len = Data_Len;
	
	while (Try_Count++ < 5)
	{
		Ack = RESET;
		I2C_Start();
		
		if (I2C_Write((I2C_ADDR) & 0xFE) == SET) //device addressing
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		if (I2C_Write(Addr) == SET) //write address
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		while (Curr_Len--)
		{
			if (I2C_Write(*Curr_Data++) == SET) //write data
			{
				I2C_Stop();
				Ack = SET;
				Curr_Len = Data_Len;
				Curr_Data = Data;
				break;
			}
		}
		if (Ack == SET) continue;
		
		I2C_Stop();
		break;
	}
	
	return Ack;
}

/*********************************************************************************
* Function: I2C_Sequential_Write16
* Description: I2C protocol, several bytes write
* Input: -Addr, start address to be write
* Input: -Data, point to the data to be write
* Input: -Data_Len, how many data to be write
* Output: none
* Return: write result, 0 is success, 1 is fail
* Call: external
*/
 FlagStatus I2C_Sequential_Write16(uint16_t Addr, uint8_t* Data, uint8_t Data_Len)
{
	uint8_t Try_Count = 0;
	FlagStatus Ack = RESET;
	uint8_t* Curr_Data = Data;
	uint8_t Curr_Len = Data_Len;
	
	while (Try_Count++ < 5)
	{
		Ack = RESET;
		I2C_Start();
		
		if (I2C_Write((I2C_ADDR) & 0xFE) == SET) //device addressing
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		if (I2C_Write(Addr >> 8) == SET) //write address
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		if (I2C_Write(Addr) == SET) //write address
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		while (Curr_Len--)
		{
			if (I2C_Write(*Curr_Data++) == SET) //write data
			{
				I2C_Stop();
				Ack = SET;
				Curr_Len = Data_Len;
				Curr_Data = Data;
				break;
			}
		}	
		if (Ack == SET) continue;
		
		I2C_Stop();
		break;
	}
	
	return Ack;
}

/*********************************************************************************
* Function: I2C_Sequential_Read
* Description: I2C protocol, several bytes write
* Input: -Addr, start address to be read
* Input: -Data, buffer pointer to save read data
* Input: -Data_Len, how many data to be read
* Output: none
* Return: read result, 0 is success, 1 is fail
* Call: external
*/
 FlagStatus I2C_Sequential_Read(uint8_t Addr, uint8_t* Data, uint8_t Data_Len)
{
	uint8_t Try_Count = 0;
	FlagStatus Ack;
	
	while (Try_Count++ < 5)
	{
		I2C_Start();
		
		if (I2C_Write((I2C_ADDR) & 0xFE) == SET) //device addressing
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		if (I2C_Write(Addr) == SET) //write address
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		I2C_Start();

		if (I2C_Write((I2C_ADDR)| 0x01) == SET) //device addressing
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		while (Data_Len > 1)
		{
			*Data = I2C_Read_Ack();
			Data_Len--;
			Data++;
		}	
		*Data = I2C_Read_Uak();

		I2C_Stop();
		Ack = RESET;
		break;
	}
	
	return Ack;
}

/*********************************************************************************
* Function: I2C_Sequential_Read16
* Description: I2C protocol, several bytes write
* Input: -Addr, start address to be read
* Input: -Data, buffer pointer to save read data
* Input: -Data_Len, how many data to be read
* Output: none
* Return: read result, 0 is success, 1 is fail
* Call: external
*/
 FlagStatus I2C_Sequential_Read16(uint16_t Addr, uint8_t* Data, uint8_t Data_Len)
{
	uint8_t Try_Count = 0;
	FlagStatus Ack;
	
	while (Try_Count++ < 5)
	{
		I2C_Start();
		
		if (I2C_Write((I2C_ADDR) & 0xFE) == SET) //device addressing
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		if (I2C_Write(Addr >> 8) == SET) //write address
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		if (I2C_Write(Addr) == SET) //write address
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		I2C_Start();

		if (I2C_Write((I2C_ADDR)| 0x01) == SET) //device addressing
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		while (Data_Len > 1)
		{
			*Data = I2C_Read_Ack();
			Data_Len--;
			Data++;
		}	
		*Data = I2C_Read_Uak();

		I2C_Stop();
		Ack = RESET;
		break;
	}
	
	return Ack;
}

/*********************************************************************************
* Function: I2C_Check_Addr
* Description: check if slave device is available
* Input: -Addr, slave device address to be check
* Output: none
* Return: result, 0 is available, 1 is unavailable
* Call: external
*/
FlagStatus I2C_Check_Addr(uint8_t Addr)
{
	uint8_t Try_Count = 0;
	FlagStatus Ack;
	
	while (Try_Count++ < 16)
	{
		I2C_Start();

		if (I2C_Write((Addr) & 0xFE) == SET)
		{
			I2C_Stop();
			Ack = SET;
			continue;
		}
		
		I2C_Stop();
		Ack = RESET;
		break;
	}

	return Ack;
}
