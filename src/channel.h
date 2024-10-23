/**
* @file channel.h
* @brief Functions and data definitions for the channel ADT
* @anchor header_ch
* @author: Antonio Riccio
* @copyright
* Copyright 2022 Antonio Riccio <hi@ariccio.me>.
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
#ifndef CHANNEL_H
#define CHANNEL_H

/**************************** Type Definitions ******************************/
/**
 * @brief Abstract representation of data exchanged in a channel.
 *
 * @details The user shall use this format to exchange data on a channel.
 *
 */
typedef struct
{
   long mtype;             /**< header of a message */
   int mvalue;             /**< data value of a message */
} message_t;

/**
 * @brief Abstract representation of a channel.
 *
 * @details The user shall initialise a structure of this type before using a channel.
 *       Process wanting to share a channel shall use same seed, which is then used to
 *       derive an identical ch_key.
 *
 */
typedef struct
{
   int ch_key;              /**< system-wide channel identifier */
   int ch_id;               /**< process-wide channel identifier */
   char seed;               /**< parameter for connecting to an aleardy existing channel */
} channel_t;

/************************** Function Prototypes *****************************/

/**
 * @name Init functions
 * @{
 */
void channel_create(channel_t* channel_ptr, char seed);
void channel_delete(channel_t* channel_ptr);
void channel_connect(channel_t* channel_ptr);
/* @} */

/**
 * @name Blocking I/O operations
 * @{
 */
void channel_retrieve_nonblock(channel_t* channel_ptr, message_t* data);
void channel_retrieve_cat_nonblock(channel_t* channel_ptr, message_t* data, long category);
void channel_push_nonblock(channel_t* channel_ptr, message_t* data);
/* @} */

/**
 * @name Non-blocking I/O operations
 * @{
 */
void channel_retrieve_block(channel_t* channel_ptr, message_t* data);
void channel_retrieve_cat_block(channel_t* channel_ptr, message_t* data, long category);
void channel_push_block(channel_t* channel_ptr, message_t* data);
/* @} */

#endif /*CHANNEL_H*/
