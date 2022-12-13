/**
* @file control_law.h
* @brief Functions and data definitions for the control law
* @anchor header_claw
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
#ifndef CONTROL_LAW
#define CONTROL_LAW

/************************** Function Prototypes *****************************/
/**
* @brief Control law for the GNC code.
*
* @details Dummy control law. Takes input data, sums a random quantity and returns.
*     Can be easily swappable with different implementations that respect this interface.
*
* @param[in]  data_in  input data
* @param[out] data_out output data
*
* @return none
*/
void control_law(int* data_in, int* data_out);

#endif /*CONTROL LAW*/
