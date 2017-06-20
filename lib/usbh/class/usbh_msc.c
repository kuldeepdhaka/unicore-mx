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

#include <unicore-mx/usbh/class/msc.h>
#include <unicore-mx/usbh/helper/ctrlreq.h>
#include "../usbh-private.h"

#define BBB_CALLBACK(arg, dev, status)		\
	if (arg->callback != NULL) {			\
		arg->callback(dev, status, arg);	\
	}

/**
 * CSW (Command Status Wrapper) transfer callback
 */
static void bbb_csw_callback(const usbh_transfer *transfer,
				usbh_transfer_status status, usbh_urb_id urb_id)
{
	(void) urb_id;
	usbh_device *dev = transfer->device;
	struct usbh_msc_bbb_arg *arg = transfer->user_data;

	if (status != USBH_SUCCESS) {
		BBB_CALLBACK(arg, dev, status);
		return;
	}

	/* Signature check */
	if (arg->csw.dCSWSignature != USB_MSC_CSW_SIGNATURE) {
		LOGF_LN("CSW signature invalid (got 0x%"PRIx32", expecting 0x%"PRIx32")",
			arg->csw.dCSWSignature, USB_MSC_CSW_SIGNATURE);
		BBB_CALLBACK(arg, dev, USBH_ERR_IO);
		return;
	}

	/* Tag check */
	if (arg->csw.dCSWTag != arg->cbw.dCBWTag) {
		LOGF_LN("CSW tag mismatch (got %"PRIu32", expecting %"PRIu32")",
			arg->csw.dCSWTag, arg->cbw.dCBWTag);
		BBB_CALLBACK(arg, dev, USBH_ERR_IO);
		return;
	}

	BBB_CALLBACK(arg, dev, status);
}

/**
 * Place CSW (Command Status Wrapper) transfer to device
 * @param dev USBH device
 * @param arg MSC BBB Argument
 */
static void bbb_csw_submit(usbh_device *dev, struct usbh_msc_bbb_arg *arg)
{
	const struct usbh_transfer transfer = {
		.device = dev,
		.ep_type = USBH_EP_BULK,
		.ep_addr = arg->ep.in.addr | 0x80,
		.ep_size = arg->ep.in.size,
		.data = &arg->csw,
		.length = sizeof(arg->csw),
		.flags = USBH_FLAG_ZERO_PACKET,
		.interval = 0,
		.timeout = arg->timeout.csw,
		.callback = bbb_csw_callback,
		.user_data = arg
	};

	usbh_transfer_submit(&transfer);
}

/**
 * Data In transfer callback
 * @note If the device do not want to send as much data that host want,
 *  1. device may "end the transfer with a short packet." (that should cause status = USBH_ERR_SHORT_PACKET)
 *  2. device may "fill data to pad up to a total of dCBWDataTransferLength"
 */
static void bbb_data_in_callback(const usbh_transfer *transfer,
			usbh_transfer_status status, usbh_urb_id urb_id)
{
	(void) urb_id;

	usbh_device *dev = transfer->device;
	struct usbh_msc_bbb_arg *arg = transfer->user_data;

	if (status == USBH_ERR_SHORT_PACKET) {
		/* Ignore, goto CSW stage */
	} else if (status != USBH_SUCCESS) {
		BBB_CALLBACK(arg, dev, status);
		return;
	}

	bbb_csw_submit(dev, arg);
}

/**
 * Place Data In transfer to device
 * @param dev USBH device
 * @param arg MSC BBB Argument
 */
static void bbb_data_in_submit(usbh_device *dev,
				struct usbh_msc_bbb_arg *arg)
{
	const struct usbh_transfer transfer = {
		.device = dev,
		.ep_type = USBH_EP_BULK,
		.ep_addr = arg->ep.in.addr | 0x80,
		.ep_size = arg->ep.in.size,
		.data = arg->data,
		.length = arg->cbw.dCBWDataTransferLength,
		.flags = USBH_FLAG_NO_SHORT_PACKET,
		.interval = 0,
		.timeout = arg->timeout.data_in,
		.callback = bbb_data_in_callback,
		.user_data = arg
	};

	usbh_transfer_submit(&transfer);
}

/**
 * Data Out transfer callback
 */
static void bbb_data_out_callback(const usbh_transfer *transfer,
					usbh_transfer_status status, usbh_urb_id urb_id)
{
	(void) urb_id;

	usbh_device *dev = transfer->device;
	struct usbh_msc_bbb_arg *arg = transfer->user_data;

	if (status != USBH_SUCCESS) {
		BBB_CALLBACK(arg, dev, status);
		return;
	}

	bbb_csw_submit(dev, arg);
}

/**
 * Place Data Out transfer to device
 * @param dev USBH device
 * @param arg MSC BBB Argument
 */
static void bbb_data_out_submit(usbh_device *dev,
					struct usbh_msc_bbb_arg *arg)
{
	const struct usbh_transfer transfer = {
		.device = dev,
		.ep_type = USBH_EP_BULK,
		.ep_addr = arg->ep.out.addr & 0x7F,
		.ep_size = arg->ep.out.size,
		.data = arg->data,
		.length = arg->cbw.dCBWDataTransferLength,
		.flags = USBH_FLAG_NONE,
		.interval = 0,
		.timeout = arg->timeout.data_out,
		.callback = bbb_data_out_callback,
		.user_data = arg
	};

	usbh_transfer_submit(&transfer);
}

/**
 * CBW (Command Block Wrapper) transfer callback
 */
static void bbb_cbw_callback(const usbh_transfer *transfer,
				usbh_transfer_status status, usbh_urb_id urb_id)
{
	(void) urb_id;
	usbh_device *dev = transfer->device;
	struct usbh_msc_bbb_arg *arg = transfer->user_data;

	if (status != USBH_SUCCESS) {
		BBB_CALLBACK(arg, dev, status);
		return;
	}

	/* No data stage? */
	if (!arg->cbw.dCBWDataTransferLength) {
		bbb_csw_submit(dev, arg);
		return;
	}

	if (arg->cbw.bmCBWFlags & (1 << 7)) {
		bbb_data_in_submit(dev, arg);
	} else {
		bbb_data_out_submit(dev, arg);
	}
}

/**
 * Place CBW (Command Block Wrapper) transfer to device
 * @param dev USBH device
 * @param arg MSC BBB Argument
 */
static void bbb_cbw_submit(usbh_device *dev,
					struct usbh_msc_bbb_arg *arg)
{
	const struct usbh_transfer transfer = {
		.device = dev,
		.ep_type = USBH_EP_BULK,
		.ep_addr = arg->ep.out.addr & 0x7F,
		.ep_size = arg->ep.out.size,
		.data = &arg->cbw,
		.length = sizeof(arg->cbw),
		.flags = USBH_FLAG_ZERO_PACKET,
		.interval = 0,
		.timeout = arg->timeout.cbw,
		.callback = bbb_cbw_callback,
		.user_data = arg
	};

	usbh_transfer_submit(&transfer);
}

void usbh_msc_bbb(usbh_device *dev, struct usbh_msc_bbb_arg *arg)
{
	/* Signature check */
	if (arg->cbw.dCBWSignature != USB_MSC_CBW_SIGNATURE) {
		LOGF_LN("CBW signature invalid (got 0x%"PRIx32", expecting 0x%"PRIx32")",
			arg->cbw.dCBWSignature, USB_MSC_CBW_SIGNATURE);
		BBB_CALLBACK(arg, dev, USBH_ERR_INVALID);
		return;
	}

	bbb_cbw_submit(dev, arg);
}

usbh_urb_id usbh_msc_bbb_reset(usbh_device *dev, uint8_t bInterface,
			usbh_transfer_callback callback)
{
	return usbh_ctrlreq_ep0(dev,
		USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,
		USB_MSC_REQ_BULK_ONLY_RESET, 0, bInterface, NULL, 0,
		callback);
}

usbh_urb_id usbh_msc_get_max_lun(usbh_device *dev, uint8_t bInterface,
	uint8_t *max_lun, usbh_transfer_callback callback)
{
	return usbh_ctrlreq_ep0(dev,
		USB_REQ_TYPE_IN | USB_REQ_TYPE_CLASS | USB_REQ_TYPE_INTERFACE,
		USB_MSC_REQ_GET_MAX_LUN, 0, bInterface, max_lun, 1,
		callback);
}
