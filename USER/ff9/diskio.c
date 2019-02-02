/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2007        */
/*-----------------------------------------------------------------------*/
/* This is a stub disk I/O module that acts as front end of the existing */
/* disk I/O modules and attach it to FatFs module with common interface. */
/*-----------------------------------------------------------------------*/

#include "diskio.h"
#include "stm32f2xx.h"
#include "sdcard.h" 
#include "usart.h"

#define BLOCK_SIZE            512 /* Block Size in Bytes */



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */

DSTATUS disk_initialize (
    BYTE drv                /* Physical drive nmuber (0..) */
)
{
    SD_Error  Status;
    /* Supports only single drive */
    if (drv)
    {
        return STA_NOINIT;
    }
#if SDDEBUG
    printf("\r\n SDCard Initializing!");
#endif
    Status = SD_Init();
	
    if (Status!=SD_OK )
    {
#if SDDEBUG
		printf("\r\n SDCard Initializing Fail!");
#endif    			
        return STA_NOINIT;
    }
    else
    {
#if SDDEBUG	
		printf("\r\n SDCard Initialized!");
#endif
        return RES_OK;
    }

}



/*-----------------------------------------------------------------------*/
/* Return Disk Status                                                    */

DSTATUS disk_status (
    BYTE drv        /* Physical drive nmuber (0..) */
)
{
    return RES_OK;
}


								   
/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */

DRESULT disk_read (
    BYTE drv,        /* Physical drive nmuber (0..) */
    BYTE *buff,        /* Data buffer to store read data */
    DWORD sector,    /* Sector address (LBA) */
    BYTE count        /* Number of sectors to read (1..255) */
)
{
    if(((uint32_t)buff&(uint32_t)0x1)!=0)
        printf("\r\n disk_read error:data is not aligned!buff is %x,count is %d",buff,count);
    
    if (count > 1)
    {
        SD_ReadMultiBlocks(buff, sector*BLOCK_SIZE, BLOCK_SIZE, count);
        /* Check if the Transfer is finished */
        SD_WaitReadOperation();  //循环查询dma传输是否结束
        /* Wait until end of DMA transfer */
        while(SD_GetStatus() != SD_TRANSFER_OK);
    }
    else
    {
        SD_ReadBlock(buff, sector*BLOCK_SIZE, BLOCK_SIZE);
        /* Check if the Transfer is finished */
        SD_WaitReadOperation();  //循环查询dma传输是否结束
        /* Wait until end of DMA transfer */
        while(SD_GetStatus() != SD_TRANSFER_OK);

    }
    return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */

#if _READONLY == 0
DRESULT disk_write (
    BYTE drv,            /* Physical drive nmuber (0..) */
    const BYTE *buff,    /* Data to be written */
    DWORD sector,        /* Sector address (LBA) */
    BYTE count            /* Number of sectors to write (1..255) */
)
{
    if(((uint32_t)buff&(uint32_t)0x1)!=0)
        printf("\r\n disk_write error:data is not aligned!buff is %x,count is %d",buff,count);

    if (count > 1)
    {
        SD_WriteMultiBlocks((uint8_t *)buff, sector*BLOCK_SIZE, BLOCK_SIZE, count);
        /* Check if the Transfer is finished */
          SD_WaitWriteOperation();      
          
        while(SD_GetStatus() != SD_TRANSFER_OK);
    }
    else
    {
        SD_WriteBlock((uint8_t *)buff,sector*BLOCK_SIZE, BLOCK_SIZE);
        /* Check if the Transfer is finished */
           SD_WaitWriteOperation();       
        
        while(SD_GetStatus() != SD_TRANSFER_OK);
    }
    
    return RES_OK;
}
#endif /* _READONLY */

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */

DRESULT disk_ioctl (
    BYTE drv,        /* Physical drive nmuber (0..) */
    BYTE ctrl,        /* Control code */
    void *buff        /* Buffer to send/receive control data */
)
{
    return RES_OK;
}
                             
/*-----------------------------------------------------------------------*/
/* Get current time                                                      */
/*-----------------------------------------------------------------------*/ 
DWORD get_fattime(void)
{
     return 0;
} 
