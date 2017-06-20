/*
 * This file is part of the unicore-mx project.
 *
 * Copyright (C) 2017 Kuldeep Singh Dhaka <kuldeepdhaka9@gmail.com>
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef UNICOREMX_USBH_CLASS_MSC_H
#define UNICOREMX_USBH_CLASS_MSC_H

#include <unicore-mx/usbh/usbh.h>
#include <unicore-mx/usb/class/msc.h>

struct usbh_msc_bbb_arg {
	/**
	 * This function is called with the argument and status on success
	 * or falure.
	 *
	 * @param arg USBH MSC Argument
	 * @param status Last transfer status
	 * @param readed Actual number of @a data bytes readed after CSW
	 * @note @a readed is only valid for Data In
	 */
	void (*callback)(usbh_device *dev, usbh_transfer_status status,
					struct usbh_msc_bbb_arg *arg);

	/** Command Block Wrapper */
	struct usb_msc_cbw cbw;

	/** Command Status Wrapper */
	struct usb_msc_csw csw;

	/** Data to send after CBW or recv after CSW */
	void *data;

	/**
	 * In and Out endpoint address and size
	 *
	 * @warning Application code need to initalize
	 *  these value before any BBB API use */
	struct {
		struct {
			uint8_t addr;
			uint16_t size;
		} in, out;
	} ep;

	/** Various stage of IO timeout values.
	 * @warning Application code need to initalize
	 *  these value before any BBB API use
	 */
	struct {
		uint32_t cbw;
		uint32_t data_out;
		uint32_t csw;
		uint32_t data_in;
	} timeout;

	/** User specific data */
	void *user_data;
};

typedef struct usbh_msc_bbb_arg usbh_msc_bbb_arg;

/**
 * Perform MSC Bulk only IO
 * This will perform the full state timeline of BBB
 * CBW [-> Data OUT] -> CSW [-> CSW]
 * @warning @a arg should be a valid pointer
 *  (and all pointers inside it) till callback is not performed.
 *
 * @warning This API is used by
 *  - usbh_msc_bbb_read10()
 *  - usbh_msc_bbb_write10()
 *  - usbh_msc_bbb_test_unit_ready()
 */
void usbh_msc_bbb(usbh_device *dev, struct usbh_msc_bbb_arg *arg);

/**
 * Initalize READ10 CBW
 * @param cbw Command Block Wrapper
 * @param lba_start Logical Block Address start address
 * @param lba_count Number of Logical Block Address to read
 * @param block_size Number of bytes in a block
 *
 * @note Increment dCBWTag
 * @note dCBWDataTransferLength = @a lba_count * @a block_size
 * @note bmCBWFlags = 1 << 7
 * @note bCBWLUN = 0
 * @note [CBWCB] DPO = 0
 * @note [CBWCB] FUA = 0
 * @note [CBWCB] RELADR = 0
 * @note [CBWCB] LBA = @a lba_start
 * @note [CBWCB] TRANSFER LENGTH = @a lba_count
 * @note [CBWCB] CONTROL = 0
 */
static inline void usbh_msc_bbb_read10_cbw(struct usb_msc_cbw *cbw,
		uint32_t lba_start, uint16_t lba_count, uint16_t block_size)
{
	cbw->dCBWSignature = USB_MSC_CBW_SIGNATURE;
	cbw->dCBWTag++; /* Next tag */
	cbw->dCBWDataTransferLength = lba_count * block_size;
	cbw->bmCBWFlags = 1 << 7;
	cbw->bCBWLUN = 0;
	cbw->bCBWCBLength = 10;
	cbw->CBWCB[0] = USB_MSC_SCSI_READ_10;
	cbw->CBWCB[1] = 0x00; /* DPO=0, FUA=0, RELADR=0 */
	cbw->CBWCB[2] = lba_start >> 24;
	cbw->CBWCB[3] = lba_start >> 16;
	cbw->CBWCB[4] = lba_start >> 8;
	cbw->CBWCB[5] = lba_start >> 0;
	cbw->CBWCB[6] = 0x00;
	cbw->CBWCB[7] = lba_count >> 8;
	cbw->CBWCB[8] = lba_count >> 0;
	cbw->CBWCB[9] = 0x00; /* CONTROL=0 */
	cbw->CBWCB[10] = 0x00; /* PAD */
	cbw->CBWCB[11] = 0x00; /* PAD */
	cbw->CBWCB[12] = 0x00; /* PAD */
	cbw->CBWCB[13] = 0x00; /* PAD */
	cbw->CBWCB[14] = 0x00; /* PAD */
	cbw->CBWCB[15] = 0x00; /* PAD */
}

/**
 * Initalize READ10 CBW
 * @param cbw Command Block Wrapper
 * @param lba_start Logical Block Address start address
 * @param lba_count Number of Logical Block Address to read
 * @param block_size Number of bytes in a block
 *
 * @note Increment dCBWTag
 * @note dCBWDataTransferLength = @a lba_count * @a block_size
 * @note bmCBWFlags = 0
 * @note bCBWLUN = 0
 * @note [CBWCB] DPO = 0
 * @note [CBWCB] FUA = 0
 * @note [CBWCB] RELADR = 0
 * @note [CBWCB] LBA = @a lba_start
 * @note [CBWCB] TRANSFER LENGTH = @a lba_count
 * @note [CBWCB] CONTROL = 0
 */
static inline void usbh_msc_bbb_write10_cbw(struct usb_msc_cbw *cbw,
		uint32_t lba_start, uint16_t lba_count, uint16_t block_size)
{
	cbw->dCBWSignature = USB_MSC_CBW_SIGNATURE;
	cbw->dCBWTag++; /* Next tag */
	cbw->dCBWDataTransferLength = lba_count * block_size;
	cbw->bmCBWFlags = 0;
	cbw->bCBWLUN = 0;
	cbw->bCBWCBLength = 10;
	cbw->CBWCB[0] = USB_MSC_SCSI_WRITE_10;
	cbw->CBWCB[1] = 0x00; /* DPO=0, FUA=0, RELADR=0 */
	cbw->CBWCB[2] = lba_start >> 24;
	cbw->CBWCB[3] = lba_start >> 16;
	cbw->CBWCB[4] = lba_start >> 8;
	cbw->CBWCB[5] = lba_start >> 0;
	cbw->CBWCB[6] = 0x00;
	cbw->CBWCB[7] = lba_count >> 8;
	cbw->CBWCB[8] = lba_count >> 0;
	cbw->CBWCB[9] = 0x00; /* CONTROL=0 */
	cbw->CBWCB[10] = 0x00; /* PAD */
	cbw->CBWCB[11] = 0x00; /* PAD */
	cbw->CBWCB[12] = 0x00; /* PAD */
	cbw->CBWCB[13] = 0x00; /* PAD */
	cbw->CBWCB[14] = 0x00; /* PAD */
	cbw->CBWCB[15] = 0x00; /* PAD */
}

/**
 * Initalize TEST UNIT READY CBW
 * @param cbw Command Block Wrapper
 *
 * @note Increment dCBWTag
 * @note dCBWDataTransferLength = 0
 * @note bmCBWFlags = 0
 * @note bCBWLUN = 0
 * @note CONTROL = 0
 */
static inline void usbh_msc_bbb_test_unit_ready_cbw(struct usb_msc_cbw *cbw)
{
	cbw->dCBWSignature = USB_MSC_CBW_SIGNATURE;
	cbw->dCBWTag++; /* Next tag */
	cbw->dCBWDataTransferLength = 0;
	cbw->bmCBWFlags = 0;
	cbw->bCBWLUN = 0;
	cbw->bCBWCBLength = 6;
	cbw->CBWCB[0] = USB_MSC_SCSI_TEST_UNIT_READY;
	cbw->CBWCB[1] = 0x00;
	cbw->CBWCB[2] = 0x00;
	cbw->CBWCB[3] = 0x00;
	cbw->CBWCB[4] = 0x00;
	cbw->CBWCB[5] = 0x00; /* CONTROL=0 */
	cbw->CBWCB[6] = 0x00; /* PAD */
	cbw->CBWCB[7] = 0x00; /* PAD */
	cbw->CBWCB[8] = 0x00; /* PAD */
	cbw->CBWCB[9] = 0x00; /* PAD */
	cbw->CBWCB[10] = 0x00; /* PAD */
	cbw->CBWCB[11] = 0x00; /* PAD */
	cbw->CBWCB[12] = 0x00; /* PAD */
	cbw->CBWCB[13] = 0x00; /* PAD */
	cbw->CBWCB[14] = 0x00; /* PAD */
	cbw->CBWCB[15] = 0x00; /* PAD */
}

/**
 * Initalize READ CAPACITY CBW
 * @param cbw Command Block Wrapper
 *
 * @note Increment dCBWTag
 * @note dCBWDataTransferLength = 6
 * @note bmCBWFlags = 1 << 7
 * @note bCBWLUN = 0
 * @note [CBWCB] RELADR = 0
 * @note [CBWCB] LBA = 0x00000000
 * @note [CBWCB] PMI = 0
 * @note [CBWCB] CONTROL = 0
 */
static inline void usbh_msc_bbb_read_capacity_cbw(struct usb_msc_cbw *cbw)
{
	cbw->dCBWSignature = USB_MSC_CBW_SIGNATURE;
	cbw->dCBWTag++; /* Next tag */
	cbw->dCBWDataTransferLength = 6;
	cbw->bmCBWFlags = 1 << 7;
	cbw->bCBWLUN = 0;
	cbw->bCBWCBLength = 10;
	cbw->CBWCB[0] = USB_MSC_SCSI_READ_CAPACITY;
	cbw->CBWCB[1] = 0x00; /* RELADR=0 */
	cbw->CBWCB[2] = 0x00; /* LBA[MSB] = 0 */
	cbw->CBWCB[3] = 0x00; /* LBA[..] = 0  */
	cbw->CBWCB[4] = 0x00; /* LBA[..] = 0  */
	cbw->CBWCB[5] = 0x00; /* LBA[LSB] = 0 */
	cbw->CBWCB[6] = 0x00;
	cbw->CBWCB[7] = 0x00;
	cbw->CBWCB[8] = 0x00; /* PMI=0 */
	cbw->CBWCB[9] = 0x00; /* CONTROL=0 */
	cbw->CBWCB[10] = 0x00; /* PAD */
	cbw->CBWCB[11] = 0x00; /* PAD */
	cbw->CBWCB[12] = 0x00; /* PAD */
	cbw->CBWCB[13] = 0x00; /* PAD */
	cbw->CBWCB[14] = 0x00; /* PAD */
	cbw->CBWCB[15] = 0x00; /* PAD */
}

/**
 * Initalize [standard] INQUIRY CBW
 * @param cbw Command Block Wrapper
 *
 * @note Increment dCBWTag
 * @note dCBWDataTransferLength = 36
 * @note bmCBWFlags = 1 << 7
 * @note bCBWLUN = 0
 * @note [CBWCB] CMDDT = 0
 * @note [CBWCB] EVPD = 0
 * @note [CBWCB] ALLOCATION LENGTH = 36
 * @note [CBWCB] CONTROL = 0
 */
static inline void usbh_msc_bbb_inquiry_cbw(struct usb_msc_cbw *cbw)
{
	cbw->dCBWSignature = USB_MSC_CBW_SIGNATURE;
	cbw->dCBWTag++; /* Next tag */
	cbw->dCBWDataTransferLength = 36;
	cbw->bmCBWFlags = 1 << 7;
	cbw->bCBWLUN = 0;
	cbw->bCBWCBLength = 6;
	cbw->CBWCB[0] = USB_MSC_SCSI_INQUIRY;
	cbw->CBWCB[1] = 0x00; /* CMDDT=0, EVPD=0 */
	cbw->CBWCB[2] = 0x00; /* PAGE OR OPERATION CODE = 0 */
	cbw->CBWCB[3] = 0x00;
	cbw->CBWCB[4] = 36;   /* ALLOCATION LENGTH = 36 */
	cbw->CBWCB[5] = 0x00; /* CONTROL=0 */
	cbw->CBWCB[6] = 0x00; /* PAD */
	cbw->CBWCB[7] = 0x00; /* PAD */
	cbw->CBWCB[8] = 0x00; /* PAD */
	cbw->CBWCB[9] = 0x00; /* PAD */
	cbw->CBWCB[10] = 0x00; /* PAD */
	cbw->CBWCB[11] = 0x00; /* PAD */
	cbw->CBWCB[12] = 0x00; /* PAD */
	cbw->CBWCB[13] = 0x00; /* PAD */
	cbw->CBWCB[14] = 0x00; /* PAD */
	cbw->CBWCB[15] = 0x00; /* PAD */
}

/**
 * Initalize REQUEST SENSE CBW
 * @param cbw Command Block Wrapper
 *
 * @note Increment dCBWTag
 * @note dCBWDataTransferLength = 18
 * @note bmCBWFlags = 1 << 7
 * @note bCBWLUN = 0
 * @note [CBWCB] ALLOCATION LENGTH = 18
 * @note [CBWCB] CONTROL = 0
 */
static inline void usbh_msc_bbb_request_sense_cbw(struct usb_msc_cbw *cbw)
{
	cbw->dCBWSignature = USB_MSC_CBW_SIGNATURE;
	cbw->dCBWTag++; /* Next tag */
	cbw->dCBWDataTransferLength = 18;
	cbw->bmCBWFlags = 1 << 7;
	cbw->bCBWLUN = 0;
	cbw->bCBWCBLength = 6;
	cbw->CBWCB[0] = USB_MSC_SCSI_REQUEST_SENSE;
	cbw->CBWCB[1] = 0x00;
	cbw->CBWCB[2] = 0x00;
	cbw->CBWCB[3] = 0x00;
	cbw->CBWCB[4] = 18;   /* ALLOCATION LENGTH = 18 */
	cbw->CBWCB[5] = 0x00; /* CONTROL=0 */
	cbw->CBWCB[6] = 0x00; /* PAD */
	cbw->CBWCB[7] = 0x00; /* PAD */
	cbw->CBWCB[8] = 0x00; /* PAD */
	cbw->CBWCB[9] = 0x00; /* PAD */
	cbw->CBWCB[10] = 0x00; /* PAD */
	cbw->CBWCB[11] = 0x00; /* PAD */
	cbw->CBWCB[12] = 0x00; /* PAD */
	cbw->CBWCB[13] = 0x00; /* PAD */
	cbw->CBWCB[14] = 0x00; /* PAD */
	cbw->CBWCB[15] = 0x00; /* PAD */
}

/**
 * Reset the mass storage device and its associated interface.
 * @param dev USBH Device
 * @param bInterface Interface number
 * @param callback Callback to perform on transfer complete
 * @return URB ID
 */
usbh_urb_id usbh_msc_bbb_reset(usbh_device *dev, uint8_t bInterface,
		usbh_transfer_callback callback);

/**
 * Get Max LUN
 * @param dev USBH Device
 * @param bInterface Interface number
 * @param[out] Max LUN
 * @param callback Callback to perform on transfer complete
 * @return URB ID
 */
usbh_urb_id usbh_msc_get_max_lun(usbh_device *dev, uint8_t bInterface,
		uint8_t *max_lun, usbh_transfer_callback callback);

#endif
