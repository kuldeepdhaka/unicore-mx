##
## Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
## Copyright (C) 2012 Michael Ossmann <mike@ossmann.com>
## Copyright (C) 2012/2013 Benjamin Vernoux <titanmkd@gmail.com>
##
## This library is free software: you can redistribute it and/or modify
## it under the terms of the GNU Lesser General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with this library.  If not, see <http://www.gnu.org/licenses/>.
##

LIBNAME		= libucmx_lpc43xx_m0

PREFIX		?= arm-none-eabi
#PREFIX		?= arm-elf
CC		= $(PREFIX)-gcc
AR		= $(PREFIX)-ar
CFLAGS		= -O2 -g3 -Wall -Wextra -I../../include -fno-common \
		  -mcpu=cortex-m0 -mthumb -Wstrict-prototypes \
		  -ffunction-sections -fdata-sections -MD
# ARFLAGS	= rcsv
ARFLAGS		= rcs

# LPC43xx common files for M4 / M0
OBJ_LPC43XX = gpio.o scu.o i2c.o ssp.o nvic.o uart.o

#LPC43xx M0 specific file + Generic LPC43xx M4/M0 files
OBJS		= vector.o $(OBJ_LPC43XX)

# VPATH += ../usb

# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
Q := @
endif

all: $(LIBNAME).a

$(LIBNAME).a: $(OBJS)
	@printf "  AR      $(subst $(shell pwd)/,,$(@))\n"
	$(Q)$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	@printf "  CC      $(subst $(shell pwd)/,,$(@))\n"
	$(Q)$(CC) $(CFLAGS) -o $@ -c $<

%.o: ../lpc43xx/%.c
	@printf "  CC      $(subst $(shell pwd)/,,$(@))\n"
	$(Q)$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@printf "  CLEAN   lib/lpc43xx_m0\n"
	$(Q)rm -f *.o *.d
	$(Q)rm -f $(LIBNAME).a

.PHONY: clean

-include $(OBJS:.o=.d)

