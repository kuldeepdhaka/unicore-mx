/*
 * This file is part of the unicore-mx project.
 *
 * Copyright (C) 2016 Kuldeep Singh Dhaka <kuldeepdhaka9@gmail.com>
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

#include "i2c-private.h"

uint16_t i2c_generic_read(i2c_frontend *i2c, uint8_t addr,
	void *data, uint16_t count)
{
	const i2c_task tasks[] = {{
		.addr = addr,
		.flags = I2C_TASK_READ | I2C_TASK_LAST,
		.data = data,
		.count = count,
	}};

	i2c_sync_exec(i2c, tasks, NULL, &count);

	return count;
}

uint16_t i2c_generic_read10(i2c_frontend *i2c, uint16_t addr10,
	void *data, uint16_t count)
{
	uint8_t addr = (0b11110 << 2) | ((addr10 >> 8) & 0b11);
	uint8_t write = addr10 & 0xFF;
	uint16_t write_count = 1;
	i2c_write_read(i2c, addr, &write, &write_count, data, &count);
	return count;
}

uint16_t i2c_generic_write(i2c_frontend *i2c, uint8_t addr,
	const void *data, uint16_t count)
{
	const i2c_task tasks[] = {{
		.data = data,
		.count = count,
		.addr = addr,
		.flags = I2C_TASK_WRITE | I2C_TASK_LAST
	}};

	i2c_sync_exec(i2c, tasks, NULL, &count);

	return count;
}

uint16_t i2c_generic_write10(i2c_frontend *i2c, uint16_t addr10,
	const void *data, uint16_t count)
{
	uint16_t _count = count + 1;
	void *_data = alloca(_count);
	uint8_t _addr = (0b11110 << 2) | ((addr10 >> 8) & 0b11);

	_data[0] = addr10 & 0xFF;
	memcpy(&_data[1], data, count);

	const i2c_task tasks[] = {{
		.data = _data,
		.count = _count,
		.addr = _addr,
		.flags = I2C_TASK_WRITE | I2C_TASK_LAST
	}};

	i2c_sync_exec(i2c, tasks, NULL, &count);

	return count;
}

bool i2c_generic_write_read(i2c_frontend *i2c, uint8_t addr,
	const void *write_data, uint16_t *write_count,
	void *read_data, uint16_t *read_count)
{
	const i2c_task tasks[] = {{
		.data = write_data,
		.count = *write_count,
		.addr = addr,
		.flags = I2C_TASK_WRITE
	}, {
		.data = read_data,
		.count = *read_count,
		.addr = addr,
		.flags = I2C_TASK_READ | I2C_TASK_LAST
	}};

	unsigned index;
	uint16_t count;

	if (i2c_sync_exec(i2c, tasks, &index, &count) != I2C_OK) {
		if (index == 0) {
			*write_count = count;
			*read_count = 0;
		} else if (index == 1) {
			/* write_count is already ok */
			*read_count = count;
		} else {
			/* Not possible */
		}

		return false;
	}

	return true;
}

bool i2c_generic_write_read10(i2c_frontend *i2c, uint8_t addr,
	const void *write_data, uint16_t *write_count,
	void *read_data, uint16_t *read_count)
{
	/* FIXME: more complex version of i2c_generic_write_read with memcpy for
	 *   the remaining 10bit address (like i2c_generic_write10) */
}

bool i2c_generic_detect(i2c_frontend *i2c, uint8_t addr)
{
	return i2c_read(i2c, addr, NULL, 0) == B0_OK;
}

bool i2c_generic_detect10(i2c_frontend *i2c, uint16_t addr10)
{
	uint8_t addr = (0b11110 << 2) | ((addr10 >> 8) & 0b11);
	uint8_t data = addr10 & 0xFF;

	const i2c_task tasks[] = {{
		.data = &data,
		.count = 1,
		.addr = addr,
		.flags = I2C_TASK_WRITE | I2C_TASK_LAST
	}};

	return i2c_sync_exec(i2c, tasks, NULL, NULL) == B0_OK;
}
