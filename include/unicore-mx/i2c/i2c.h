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

#ifndef UNICOREMX_I2C_H
#define UNICOREMX_I2C_H

#include <stdint.h>
#include <stdbool.h>

__BEGIN_DECLS

typedef struct i2c_backend i2c_backend;
typedef struct i2c_frontend i2c_frontend;

/**
 * Initalize I2C
 * default:
 *  speed = standard mode (ie least possible speed by the peripherial)
 *  slave visible = no
 *  slave general call = disable
 */
i2c_frontend *i2c_init(const i2c_backend *backend);

/**
 * I2C speeds
 * @internal
 * Encoded as MAJOR.MINOR
 * MAJOR takes the upper 8bit, MINOR take the lower 8bit
 * Only 0-9 hexadecimal values are used
 * @endinternal
 */
enum i2c_speed {
	I2C_STANDARD_MODE = 0x0000,
	I2C_FAST_MODE = 0x0100,
	I2C_HIGH_SPEED_MODE = 0x0200,
	I2C_FAST_MODE_PLUS = 0x0300,
	I2C_ULTRA_FAST_MODE = 0x0400,

	I2C_SM = I2C_STANDARD_MODE,
	I2C_FM = I2C_FAST_MODE,
	I2C_HSM = I2C_HIGH_SPEED_MODE,
	I2C_FMP = I2C_FAST_MODE_PLUS,
	I2C_UFM = I2C_ULTRA_FAST_MODE
};

typedef enum i2c_speed i2c_speed;

/**
 * Set I2C speed
 * @param i2c I2C
 * @param speed Speed
 */
void i2c_speed_set(i2c_frontend *i2c, i2c_speed speed);

/**
 * I2C slave/master is allowed to strech clock
 * @param i2c I2C
 * @param allow Allow
 */
void i2c_clock_streching_set(i2c_frontend *i2c, bool allow);

/* Master */

/**
 * Read @a count number of bytes from slave at @a addr and store to @a data
 * @param i2c I2C
 * @param addr Slave address (7bit)
 * @param data buffer to store
 * @param count Number of bytes to transfer
 * @return the number of bytes actually transferred
 */
uint16_t i2c_read(i2c_frontend *i2c, uint8_t addr, void *data,
	uint16_t count);

/**
 * Read @a count number of bytes from slave at @a addr10 and store to @a data
 * @param i2c I2C
 * @param addr Slave address (10bit)
 * @param data buffer to store
 * @param count Number of bytes to transfer
 * @return the number of bytes actually transferred
 */
uint16_t i2c_read10(i2c_frontend *i2c, uint16_t addr10, void *data,
	uint16_t count);

/**
 * Write @a count number of bytes from @a data to slave at @a addr
 * @param i2c I2C
 * @param addr Slave address (7bit)
 * @param data buffer to store
 * @param count Number of bytes to transfer
 * @return the number of bytes actually transferred
 */
uint16_t i2c_write(i2c_frontend *i2c, uint8_t addr, const void *data,
	uint16_t count);

/**
 * Write @a count number of bytes from @a data to slave at @a addr10
 * @param i2c I2C
 * @param addr Slave address (10bit)
 * @param data buffer to store
 * @param count Number of bytes to transfer
 * @return the number of bytes actually transferred
 */
uint16_t i2c_write10(i2c_frontend *i2c, uint16_t addr10, const void *data,
	uint16_t count);

/**
 * First write to slave and then read from the slave at @a addr (7bit)
 * RESTART will be performed after write and before read.
 * @param i2c I2C
 * @param addr Slave address (7bit)
 * @param write_data Write data pointer
 * @param write_count Write data count
 * @param read_data Read data pointer
 * @param read_count Read data count
 * @return true if everything was successful
 * @return false on something failed
 */
bool i2c_write_read(i2c_frontend *i2c, uint8_t addr,
	const void *write_data, uint16_t *write_count,
	void *read_data, uint16_t *read_count);

/**
 * First write to slave and then read from the slave at @a addr (10bit)
 * RESTART will be performed after write and before read.
 * @param i2c I2C
 * @param addr Slave address (10bit)
 * @param write_data Write data pointer
 * @param write_count Write data count
 * @param read_data Read data pointer
 * @param read_count Read data count
 * @return true if everything was successful
 * @return false on something failed
 */
bool i2c_write_read10(i2c_frontend *i2c, uint16_t addr,
	const void *write_data, uint16_t *write_count,
	void *read_data, uint16_t *read_count);

/**
 * Check if a slave is visible on bus or not
 * @param i2c I2C
 * @param addr Slave address (7bit)
 * @return true if detected
 * @return false if not detected
 */
bool i2c_detect(i2c_frontend *i2c, uint8_t addr);

/**
 * Check if a slave is visible on bus or not
 * @param i2c I2C
 * @param addr10 Slave address (10bit)
 * @return true if detected
 * @return false if not detected
 */
bool i2c_detect10(i2c_frontend *i2c, uint16_t addr10);

/**
 * I2C task status
 */
enum i2c_task_status {
	I2C_OK = 0, /**< Task executed successfully */
	I2C_ERR_NO_SLAVE = -1, /** Slave not detected */
	I2C_ERR_PREMAT = -2, /** Premature end of transfer */
	I2C_ERR_INVALID = -3 /** Invalid task */
};

typedef enum i2c_task_status i2c_task_status;

enum i2c_task_flags {
	I2C_TASK_LAST = (1 << 0), /**< Last task to execute */

	I2C_TASK_WRITE = (0 << 1), /**< Perform write */
	I2C_TASK_READ = (1 << 1), /**< Perform read */

	I2C_TASK_DIR_MASK = (1 << 1)
};

typedef enum i2c_task_flags i2c_task_flags;

struct i2c_task {
	i2c_task_flags flags; /**< Task flags */
	void *data; /**< Data pointer, if NULL means end of list */
	uint16_t count; /** Number of bytes to transfer */
	uint8_t addr; /**< Slave 7bit address */
};

typedef struct i2c_task i2c_task;

/* Master Task Sync */

/**
 * Perform task in synchronous mode
 * If any of the task fail out of @a tasks, no further executed take place.
 *
 * All the @a tasks will be executed in one go
 *  (i.e. RESTART when switching direction)
 *
 * @param i2c I2C
 * @param failed_task_index Failed executed task index (can be NULL).
 * @param failed_task_count Number of bytes transferred for the
 *   failed task (can be NULL)
 * @return task status
 *
 * @note if the @a tasks execute successfully, @a failed_task_index and
 *  @a failed_task_count are not touched
 */
i2c_task_status i2c_sync_exec(i2c_frontend *i2c, const i2c_task *tasks,
	unsigned *failed_task_index, uint16_t *failed_task_count);

/* Master Task Async */

typedef uint64_t i2c_task_id;

struct i2c_task_callback_arg {
	i2c_task_id task_id;
	i2c_task_status status;
	i2c_task *tasks;

	/**
	 * @note for successful execution, @a index and @a count will
	 *  have undefined values
	 */
	struct {
		unsigned index;
		uint16_t count;
	} failed_task;
};

typedef struct i2c_task_callback_arg i2c_task_callback_arg;

/**
 *
 */
typedef void (*i2c_task_callback)(i2c_frontend *i2c,
	const i2c_task_callback_arg *arg);

/**
 * Place the @a task in queue for execution
 *
 * If any of the task fail out of @a tasks, no further executed take place.
 *
 * All the @a tasks will be executed in one go
 *  (i.e. RESTART when switching direction)
 *
 * @param i2c I2C
 * @param tasks List of tasks to executed in one go
 * @param callback Callback to called when done
 */
i2c_task_id i2c_async_exec(i2c_frontend *i2c, const i2c_task *tasks,
	i2c_task_callback callback);

/**
 * Cancel a async task
 * @param i2c I2C
 * @param task_id Task ID returned by @a i2c_async_exec
 */
void i2c_async_cancel(i2c_frontend *i2c, i2c_task_id task_id);

__END_DECLS

/* Slave */

/**
 * Enable general call.
 * @param i2c I2C
 * @param enable Enable
 *
 * @note
 * If general call is enabled, \n
 * Registered "begin" callback is called with \n
 *     {address = 0x0, flags=I2C_SLAVE_READ} and
 *  when done, registered "end" callback is called with
 *   {address = 0x0, flags=I2C_SLAVE_READ}
 */
void i2c_gencall_set(i2c_frontend *i2c, bool enable);

/**
 * Set slave mode 7bit address to @a addr
 * @param i2c I2C
 * @param addr 7bit address
 */
void i2c_address_set(i2c_frontend *i2c, uint8_t addr);

/**
 * Set slave mode 10bit address to @a addr10
 * @param i2c I2C
 * @param addr 10bit address
 *
 * @note application can only expect 7bit/10bit address on bus.
 *  **NOT** both, It depend on the backend.
 *
 * @internal
 * Some hardware support multiple address.
 * User may write some hardware specific code that could
 *  utilize multiple address feature.
 * The stack should ensure that the address which is actually
 *  send by the master is passed to callback.
 * even it do not match the address provided in i2c_slave*_address function.
 * AIM: make thing less headache for user while being flexible
 * Also application to i2c_address_set()
 * @endinternal
 */
void i2c_address10_set(i2c_frontend *i2c, uint16_t addr10);

/**
 * Set if slave is visible on bus
 * @param i2c I2C
 * @param visible true for visible on bus else false
 */
void i2c_visible_set(i2c_frontend *i2c, bool visible);

enum i2c_slave_callback_flags {
	I2C_SLAVE_READ = (1 << 0), /**< Master want to peform read */

	/**
	 * If application code do not distingush between a START-STOP and RESTART.
	 * application code can ignore the flag and "begin" and "end" callback become
	 *  same as "start" and "stop".
	 */
	I2C_SLAVE_RESTART = (1 << 1), /**< Restart was done by master */

	I2C_SLAVE_DIR_MASK = (1 << 0), /**< Direction mask */
	I2C_SLAVE_RESTART_MASK = (1 << 1), /**< Restart mask */
};

typedef enum i2c_slave_callback_flags i2c_slave_callback_flags;

struct i2c_slave_callback_arg {
	void **data; /**< Data pointer */
	uint16_t *count; /**< Number of bytes valid/store in @a data */
	i2c_slave_callback_flags flags; /**< Callback flags */
	uint8_t addr; /**< Slave Address (7bit) */
};

typedef struct i2c_slave_callback_arg i2c_slave_callback_arg;

/**
 * Called when a START or RESTART is performed on bus (7bit slave address)
 * @param i2c I2C
 * @param arg Callback argument
 * @note @a arg data and @a count need to be provided by application
 */
typedef void (*i2c_begin_callback)(i2c_frontend *i2c,
	i2c_slave_callback_arg *arg);

/**
 * Called when a STOP or RESTART is performed on bus (7bit slave address)
 * @param i2c I2C
 * @param arg Callback argument
 */
typedef void (*i2c_end_callback)(i2c_frontend *i2c,
	const i2c_slave_callback_arg *arg);

/**
 * Register the begin callback (7bit slave address)
 * @param i2c I2C
 * @param begin Begin callback
 */
void i2c_register_begin_callback(i2c_frontend *i2c, i2c_begin_callback begin);

/**
 * Register the end callback
 * @param i2c I2C
 * @param end End callback
 */
void i2c_register_end_callback(i2c_frontend *i2c, i2c_end_callback end);

struct i2c_callback10_arg {
	void **data; /**< Data pointer */
	uint16_t *count; /**< Number of bytes valid/store in @a data */
	i2c_slave_callback_flags flags; /**< Callback flags */
	uint8_t addr; /**< Slave Address (10bit) */
};

typedef struct i2c_callback10_arg i2c_callback10_arg;

/**
 * Called when a START or RESTART is performed on bus (10bit slave address)
 * @param i2c I2C
 * @param arg Callback argument
 * @note @a arg data and @a count need to be provided by application
 */
typedef void (*i2c_begin_callback10)(i2c_frontend *i2c,
	i2c_callback10_arg *arg);

/**
 * Called when a STOP or RESTART is performed on bus (10bit slave address)
 * @param i2c I2C
 * @param arg Callback argument
 */
typedef void (*i2c_end_callback10)(i2c_frontend *i2c,
	const i2c_callback10_arg *arg);

/**
 * Register the begin callback (10bit slave address)
 * @param i2c I2C
 * @param begin10 Begin callback
 *
 * @internal
 * 10bit slave address state machine isn't completly same as 7bit address.
 *
 * Example:
 *  START 1111 0XX [W] [remaining-address] RESTART 1111 0XX [R] <read data>
 *  The above is a RESTART for 7bit address but
 *  for 10bit slave address, it isn't!
 *  so, the callbacks are different.
 *
 * Comment also applicable to:
 *  - i2c_register_end_callback()
 *  - i2c_register_start_callback10()
 *  - i2c_register_end_callback10()
 * @endinternal
 */
void i2c_register_begin_callback(i2c_frontend *i2c,
	i2c_begin_callback10 begin10);

/**
 * Register the end callback (10bit slave address)
 * @param i2c I2C
 * @param end10 End callback
 */
void i2c_register_end_callback(i2c_frontend *i2c,
	i2c_begin_callback10 end10);

#endif
