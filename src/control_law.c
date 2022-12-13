/**
* @file control_law.c
* @brief Functions implementation of @ref header_claw "control_law.h"
* @author: Antonio Riccio
* @copyright
* Copyright 2022 Antonio Riccio <ariccio@protonmail.com>.
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 3 of
* the License, or any later version. This program is distributed in
* the hope that it will be useful, but WITHOUT ANY WARRANTY; without
* even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details. You should
* have received a copy of the GNU General Public License along with
* this program; if not, write to the Free Software Foundation, Inc.,
* 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
*/
/***************************** Include Files ********************************/
#include <stdlib.h>

#include "control_law.h"

void control_law(int* data_in, int* data_out)
{
   *data_out = (*data_in) + (rand() % 100);
}
