/*
 * w25qxx_diskio.c
 *
 *  Created on: Mar 17, 2022
 *      Author: lth
 */

#include "main.h"
#include "string.h"
#include "ff_gen_drv.h"
#include "w25qxx.h"
#include "w25qxx_diskio.h"

/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;
extern W25QXX_HandleTypeDef w25qxx;


//Diskio_drvTypeDef  w25qxx_Driver =
//{
//	w25qxx_diskio_initialize,
//	w25qxx_diskio_status,
//	w25qxx_diskio_read,
//#if  _USE_WRITE
//	w25qxx_diskio_write,
//#endif  /* _USE_WRITE == 1 */
//#if  _USE_IOCTL == 1
//	w25qxx_diskio_ioctl,
//#endif /* _USE_IOCTL == 1 */
//};

//DSTATUS w25qxx_diskio_sethandler(W25QXX_HandleTypeDef *init_w25qxx) {
//	w25qxx = init_w25qxx;
//}

/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS w25qxx_diskio_initialize (
	BYTE pdrv           /* Physical drive nmuber to identify the drive */
)
{
	DBG("w25qxx_diskio_initialize - pdrv = %d", pdrv);
	Stat = 0;
    return Stat;
}

/**
  * @brief  Gets Disk Status
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS w25qxx_diskio_status (
	BYTE pdrv       /* Physical drive number to identify the drive */
)
{
	DBG("w25qxx_diskio_status - pdrv = %d", pdrv);
    return Stat;
}

/**
  * @brief  Reads Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT w25qxx_diskio_read (
	BYTE pdrv,      /* Physical drive nmuber to identify the drive */
	BYTE *buff,     /* Data buffer to store read data */
	DWORD sector,   /* Sector address in LBA */
	UINT count      /* Number of sectors to read */
)
{
	DBG("w25qxx_diskio_read - pdrv = %d, sector = %lu, count = %u", pdrv, sector, count);
	w25qxx_read(&w25qxx, sector * w25qxx.sector_size, buff, count * w25qxx.sector_size);
    return RES_OK;
}

/**
  * @brief  Writes Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT w25qxx_diskio_write (
	BYTE pdrv,          /* Physical drive nmuber to identify the drive */
	const BYTE *buff,   /* Data to be written */
	DWORD sector,       /* Sector address in LBA */
	UINT count          /* Number of sectors to write */
)
{
	W25QXX_result_t res;
	DBG("w25qxx_diskio_write - pdrv = %d, sector = 0x%lx, count = %u", pdrv, sector, count);
	res = w25qxx_erase(&w25qxx, sector * w25qxx.sector_size, count * w25qxx.sector_size);
	DBG("erase returned %lu", res);
	res = w25qxx_write(&w25qxx, sector * w25qxx.sector_size, buff, count * w25qxx.sector_size);
    return RES_OK;
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT w25qxx_diskio_ioctl (
	BYTE pdrv,      /* Physical drive nmuber (0..) */
	BYTE cmd,       /* Control code */
	void *buff      /* Buffer to send/receive control data */
)
{
	DBG("w25qxx_diskio_ioctl - pdrv = %d, cmd = %d", pdrv, cmd);

	DRESULT res = RES_OK;

	switch (cmd) {
	case CTRL_SYNC:

		break;
	case GET_SECTOR_SIZE:
		DBG("GET_SECTOR_SIZE, returning %lu", w25qxx.sector_size);
		*(DWORD*)buff = w25qxx.sector_size;
		break;
	case GET_SECTOR_COUNT:
		DBG("GET_BLOCK_COUNT, returning %lu", w25qxx.block_count);
		*(DWORD*)buff = w25qxx.sectors_in_block * w25qxx.block_count;
		break;
	case GET_BLOCK_SIZE:
		DBG("GET_BLOCK_SIZE, returning %lu", w25qxx.block_size);
		*(DWORD*)buff = w25qxx.block_size;
		break;
	default:
		res = RES_ERROR;
	}

    return res;
}
#endif /* _USE_IOCTL == 1 */
