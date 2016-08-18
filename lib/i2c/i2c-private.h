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

#ifndef UNICOREMX_I2C_PRIVATE_H
#define UNICOREMX_I2C_PRIVATE_H

#include <unicore-mx/i2c/i2c.h>

/*
 * Fallback functions that can be implemented via other functions.
 * The custom function provided by backend could be optimized for periph.
 */
/** @copydoc i2c_read */
uint16_t i2c_generic_read(i2c_frontend *i2c, uint8_t addr,
	void *data, uint16_t count);

/** @copydoc i2c_read10 */
uint16_t i2c_generic_read10(i2c_frontend *i2c, uint16_t addr10,
	void *data, uint16_t count);

/** @copydoc i2c_write */
uint16_t i2c_generic_write(i2c_frontend *i2c, uint8_t addr,
	const void *data, uint16_t count);

/** @copydoc i2c_write10 */
uint16_t i2c_generic_write10(i2c_frontend *i2c, uint16_t addr10,
	const void *data, uint16_t count);

/** @copydoc i2c_write_read */
bool i2c_generic_write_read(i2c_frontend *i2c, uint8_t addr,
	const void *write_data, uint16_t *write_count,
	void *read_data, uint16_t *read_count);

/** @copydoc i2c_write_read10 */
bool i2c_generic_write_read10(i2c_frontend *i2c, uint8_t addr,
	const void *write_data, uint16_t *write_count,
	void *read_data, uint16_t *read_count);

/** @copydoc i2c_detect */
bool i2c_generic_detect(i2c_frontend *i2c, uint8_t addr);

/** @copydoc i2c_detect10 */
bool i2c_generic_detect10(i2c_frontend *i2c, uint16_t addr10);

struct i2c_backend {
	i2c_frontend *(*init)(void);

	/** @copydoc i2c_speed_set */
	void (*speed_set)(i2c_frontend *i2c, i2c_speed speed);

	/** @copydoc i2c_clock_streching_set */
	void (*clock_streching_set)(i2c_frontend *i2c, bool allow);

	/** @copydoc i2c_read */
	uint16_t (*read)(i2c_frontend *i2c, uint8_t addr, void *data,
		uint16_t count);

	/** @copydoc i2c_read10 */
	uint16_t (*read10)(i2c_frontend *i2c, uint16_t addr10, void *data,
		uint16_t count);

	/** @copydoc i2c_write */
	uint16_t (*write)(i2c_frontend *i2c, uint8_t addr, const void *data,
		uint16_t count);

	/** @copydoc i2c_write10 */
	uint16_t (*write10)(i2c_frontend *i2c, uint16_t addr10, const void *data,
		uint16_t count);

	/** @copydoc i2c_write_read */
	bool (*write_read)(i2c_frontend *i2c, uint8_t addr,
		const void *write_data, uint16_t *write_count,
		void *read_data, uint16_t *read_count);

	/** @copydoc i2c_write_read */
	bool (*write_read10)(i2c_frontend *i2c, uint16_t addr,
		const void *write_data, uint16_t *write_count,
		void *read_data, uint16_t *read_count);

	/** @copydoc i2c_detect */
	bool (*detect)(i2c_frontend *i2c, uint8_t addr);

	/** @copydoc i2c_detect10 */
	bool (*detect10)(i2c_frontend *i2c, uint16_t addr10);

	/* This function is mandatory, "generic" function use it to implement. */
	/** @copydoc i2c_sync_exec */
	i2c_task_status (*sync_exec)(i2c_frontend *i2c, const i2c_task *tasks,
		unsigned *failed_task_index, uint16_t *failed_task_count);

	/** @copydoc i2c_async_exec */
	i2c_task_id (*async_exec)(i2c_frontend *i2c, const i2c_task *tasks,
		i2c_task_callback callback);

	/** @copydoc i2c_async_cancel */
	void (*async_cancel)(i2c_frontend *i2c, i2c_task_id task_id);

	/** @copydoc i2c_visible_set */
	void (*visible_set)(i2c_frontend *i2c, bool visible);

	/** @copydoc i2c_gencall_set */
	void (*gencall_set)(i2c_frontend *i2c, bool enable);

	/** @copydoc i2c_address_set */
	void (*address_set)(i2c_frontend *i2c, uint8_t addr);

	/** @copydoc i2c_address10_set */
	void (*address10_set)(i2c_frontend *i2c, uint16_t addr10);
};

struct i2c_frontend {
	const struct i2c_backend *backend;
};


#endif
