/**
* @file control.h
* @brief Functions and data definitions for the control functions
* @anchor header_ctrl
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
#ifndef CONTROL_H
#define CONTROL_H

/***************************** Include Files ********************************/
#include "channel.h"
#include "control_law.h"
#include "app.h"

/************************** Function Prototypes *****************************/
/**
* @brief GNC code.
*
* @details Gets data from sensors or voter (for @ref sec_tmr_arch "TMR" configuration),
*     elaborate then by applying the control law and sends them to actuators.
*
* @param[in] cmd_ch     service channel where commands are exchanged
* @param[in] data_ch_rx channel where data is received
* @param[in] data_ch_tx channel where data is transmitted
*
* @return none
*/
void control(channel_t* cmd_ch, channel_t* data_ch_rx, channel_t* data_ch_tx);

/**
* @brief Voter code.
*
* @details Implement 2-ou-of-3 voting when @ref sec_tmr_arch "TMR" is enabled.
* @sa @ref driver_details "main()"
* @note when no consensus cannot be reached, i.e. all three values are different, a default
*     value of 0 is sent.
*
* @param[in] cmd_ch     service channel where commands are exchanged
* @param[in] data_ch_rx channel where data is received
* @param[in] data_ch_tx channel where data is transmitted
* @param[in] id_sens    class identifier according to @ref def_ids "this" classification
*
* @return none
*/
void vote(channel_t* cmd_ch, channel_t* data_ch_rx, channel_t* data_ch_tx, int id_sens);

# endif /*CONTROL_H*/
