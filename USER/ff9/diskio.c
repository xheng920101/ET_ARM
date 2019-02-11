/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "stm32f2xx.h"
#include "sdcard.h" 
#include "usart.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM		0	/* Example: Map Ramdisk to physical drive 0 */
#define DEV_MMC		1	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

#define BLOCK_SIZE            512 /* Block Size in Bytes */
/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
//	DSTATUS stat;
//	int result;

//	switch (pdrv) {
//	case DEV_RAM :
//		result = RAM_disk_status();

//		// translate the reslut code here

//		return stat;

//	case DEV_MMC :
//		result = MMC_disk_status();

//		// translate the reslut code here

//		return stat;

//	case DEV_USB :
//		result = USB_disk_status();

//		// translate the reslut code here

//		return stat;
//	}
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	 SD_Error  Status;
    /* Supports only single drive */
    if (pdrv)
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
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
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
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
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



/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
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
