/* This provides unification of code over STM32 subfamilies */

/*
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

#include <unicore-mx/cm3/common.h>
#include <unicore-mx/stm32/memorymap.h>

#if defined(STM32F0)
#       include <unicore-mx/stm32/common/crs_common_all.h>
#elif defined(STM32L0)
#       include <unicore-mx/stm32/common/crs_common_all.h>
#elif defined(STM32L4)
#       include <unicore-mx/stm32/common/crs_common_all.h>
#else
#       error "stm32 family not defined or not supported for this peripheral"
#endif

