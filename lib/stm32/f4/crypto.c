/** @defgroup crypto_file CRYPTO
 *
 * @ingroup STM32F4xx
 *
 * @brief <b>unicore-mx STM32F4xx CRYPTO</b>
 *
 * @version 1.0.0
 *
 * @date 18 Jun 2013
 *
 */
/*
 * Copyright (C) 2011 Stephen Caudle <scaudle@doceme.com>
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

#include <unicore-mx/stm32/crypto.h>

/**@{*/

/*---------------------------------------------------------------------------*/

/** @brief Set the MAC algorithm
 */
void crypto_set_mac_algorithm(enum crypto_mode_mac mode)
{
	crypto_set_algorithm((enum crypto_mode) mode);
}

/**
 * @brief Swap context
 *
 *@param[in] buf uint32_t Memory space for swap (16 items length)
 */
void crypto_context_swap(uint32_t *buf)
{
	int i;
	/* Apply exact order of ? */
	for (i = 0; i < 8; i++) {
		uint32_t save = *buf;
		*buf++ = CRYP_CSGCMCCMR(i);
		CRYP_CSGCMCCMR(i) = save;
	};

	for (i = 0; i < 8; i++) {
		uint32_t save = *buf;
		*buf++ = CRYP_CSGCMR(i);
		CRYP_CSGCMCCMR(i) = save;
	};
}

/**@}*/
