/*
 * This file is part of the unicore-mx project.
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

#ifndef NRF51_UART_H
#define NRF51_UART_H

#include <unicore-mx/cm3/common.h>
#include <unicore-mx/nrf/memorymap.h>
#include <unicore-mx/nrf/periph.h>

/* Universal Asynchronous Receiver/Transmitter */

#define UART0			UART0_BASE

/* Tasks */

#define UART_TASK_STARTRX(uart)			MMIO32((uart) + 0x000)
#define UART_TASK_STOPRX(uart)			MMIO32((uart) + 0x004)
#define UART_TASK_STARTTX(uart)			MMIO32((uart) + 0x008)
#define UART_TASK_STOPTX(uart)			MMIO32((uart) + 0x00C)
#define UART_TASK_SUSPEND(uart)			MMIO32((uart) + 0x01C)

#define UART0_TASK_STARTRX			UART_TASK_STARTRX(UART0)
#define UART0_TASK_STOPRX			UART_TASK_STOPRX(UART0)
#define UART0_TASK_STARTTX			UART_TASK_STARTTX(UART0)
#define UART0_TASK_STOPTX			UART_TASK_STOPTX(UART0)
#define UART0_TASK_SUSPEND			UART_TASK_SUSPEND(UART0)

/* Events */

#define UART_EVENT_CTS(uart)			MMIO32((uart) + 0x100)
#define UART_EVENT_NCTS(uart)			MMIO32((uart) + 0x104)
#define UART_EVENT_RXDRDY(uart)			MMIO32((uart) + 0x108)
#define UART_EVENT_TXDRDY(uart)			MMIO32((uart) + 0x11C)
#define UART_EVENT_ERROR(uart)			MMIO32((uart) + 0x124)
#define UART_EVENT_RXTO(uart)			MMIO32((uart) + 0x144)

#define UART0_EVENT_CTS			UART_EVENT_CTS(UART0)
#define UART0_EVENT_NCTS			UART_EVENT_NCTS(UART0)
#define UART0_EVENT_RXDRDY			UART_EVENT_RXDRDY(UART0)
#define UART0_EVENT_TXDRDY			UART_EVENT_TXDRDY(UART0)
#define UART0_EVENT_ERROR			UART_EVENT_ERROR(UART0)
#define UART0_EVENT_RXTO			UART_EVENT_RXTO(UART0)

/* Registers */

#define UART_INTEN(uart)			periph_inten(uart)
#define UART_INTENSET(uart)			periph_intenset(uart)
#define UART_INTENCLR(uart)			periph_intenclr(uart)
#define UART_ERRORSRC(uart)			MMIO32((uart) + 0x480)
#define UART_ENABLE(uart)			MMIO32((uart) + 0x500)
#define UART_PSELRTS(uart)			MMIO32((uart) + 0x508)
#define UART_PSELTXD(uart)			MMIO32((uart) + 0x50C)
#define UART_PSELCTS(uart)			MMIO32((uart) + 0x510)
#define UART_PSELRXD(uart)			MMIO32((uart) + 0x514)
#define UART_RXD(uart)			MMIO32((uart) + 0x518)
#define UART_TXD(uart)			MMIO32((uart) + 0x51C)
#define UART_BAUDRATE(uart)			MMIO32((uart) + 0x524)
#define UART_CONFIG(uart)			MMIO32((uart) + 0x56C)

#define UART0_INTEN			UART_INTEN(UART0)
#define UART0_INTENSET			UART_INTENSET(UART0)
#define UART0_INTENCLR			UART_INTENCLR(UART0)
#define UART0_ERRORSRC			UART_ERRORSRC(UART0)
#define UART0_ENABLE			UART_ENABLE(UART0)
#define UART0_PSELRTS			UART_PSELRTS(UART0)
#define UART0_PSELTXD			UART_PSELTXD(UART0)
#define UART0_PSELCTS			UART_PSELCTS(UART0)
#define UART0_PSELRXD			UART_PSELRXD(UART0)
#define UART0_RXD			UART_RXD(UART0)
#define UART0_TXD			UART_TXD(UART0)
#define UART0_BAUDRATE			UART_BAUDRATE(UART0)
#define UART0_CONFIG			UART_CONFIG(UART0)

/* Register Contents */

#define UART_INTEN_CTS			(1 << 0)
#define UART_INTEN_NCTS			(1 << 1)
#define UART_INTEN_RXDRDY			(1 << 2)
#define UART_INTEN_TXDRDY			(1 << 7)
#define UART_INTEN_ERROR			(1 << 9)
#define UART_INTEN_RXTO			(1 << 17)

#define UART_ERRORSRC_OVERRUN			(1 << 0)
#define UART_ERRORSRC_PARITY			(1 << 1)
#define UART_ERRORSRC_FRAMING			(1 << 2)
#define UART_ERRORSRC_BREAK			(1 << 3)

#define UART_ENABLE_ENABLED			(4)
#define UART_ENABLE_DISABLED			(0)

enum uart_baud {
	UART_BAUD_1200 = 0x0004F000,
	UART_BAUD_2400 = 0x0009D000,
	UART_BAUD_4800 = 0x0013B000,
	UART_BAUD_9600 = 0x00275000,
	UART_BAUD_14400 = 0x003B0000,
	UART_BAUD_19200 = 0x004EA000,
	UART_BAUD_28800 = 0x0075F000,
	UART_BAUD_38400 = 0x009D5000,
	UART_BAUD_57600 = 0x00EBF000,
	UART_BAUD_76800 = 0x013A9000,
	UART_BAUD_115200 = 0x01D7E000,
	UART_BAUD_230400 = 0x03AFB000,
	UART_BAUD_250000 = 0x04000000,
	UART_BAUD_460800 = 0x075F7000,
	UART_BAUD_921600 = 0x0EBEDFA4,
	UART_BAUD_1M = 0x10000000,
};

#define UART_CONFIG_HWFC			(1)
#define UART_CONFIG_PARITY			(7 << 1)

#define UART_PSEL_OFF			(0xff)
#define UART_MAX_PIN			(31)
#define UART_PSEL_VAL(p)		((p) <= UART_MAX_PIN ? (p) : 0xffffffff)


BEGIN_DECLS

#define uart_enable_interrupts			periph_enable_interrupts
#define uart_disable_interrupts			periph_disable_interrupts
#define uart_clear_interrupts			periph_clear_interrupts

void uart_enable(uint32_t uart);
void uart_disable(uint32_t uart);
void uart_configure(uint32_t uart,
		uint8_t txd, uint8_t rxd, uint8_t rts, uint8_t cts,
		enum uart_baud br, bool enable_parity);

void uart_send_buffer_blocking(uint32_t uart, const uint8_t *buffer, uint16_t len);
void uart_send_string_blocking(uint32_t uart, const char *str);
void uart_send(uint32_t uart, uint16_t byte);

END_DECLS

#endif  /* NRF51_UART_H */
