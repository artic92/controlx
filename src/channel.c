/**
* @file channel.c
* @brief Functions implementation of @ref header_ch "channel.h"
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
/***************************** Include Files ********************************/
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include "channel.h"

/************************** Constant Definitions *****************************/
// support for ftok()
#define PATH      "."

// support for msgrcv()
#define FCFS       0

/**
* @brief Creates a channel.
*
* @param[inout] channel_ptr pointer to a struct channel_t with channel configuration
* @param[in]     seed        identifier used to connect to a shared channel
*
* @return none
*/
void channel_create(channel_t* channel_ptr, char seed)
{
   channel_ptr->ch_key = ftok(PATH, seed);
   channel_ptr->seed = seed;

   if(channel_ptr->ch_id = msgget(channel_ptr->ch_key, IPC_CREAT | IPC_EXCL | 0664) == -1)
   {
      if(errno == EEXIST)
      {
         channel_ptr->ch_id = msgget(channel_ptr->ch_key, 0);
      }
   }
}

/**
* @brief Deletes a channel.
*
* @param[inout] channel_ptr pointer to a struct channel_t with channel configuration
*
* @return none
*/
void channel_delete(channel_t* channel_ptr)
{
   msgctl(channel_ptr->ch_id, IPC_RMID, NULL);
}

/**
* @brief Connects to an existing channel.
*
* @param[inout] channel_ptr pointer to a struct channel_t with channel configuration
*
* @return none
*/
void channel_connect(channel_t* channel_ptr)
{
   return;
}

/**
* @brief Retrieves the first message from a channel. The calling process is not blocked
*     if there are no messages available on the channel.
*
* @param[inout] channel_ptr pointer to a struct channel_t with channel configuration
* @param[out]    data        pointer to a user-allocated message_t structure
*
* @return none
*/
void channel_retrieve_nonblock(channel_t* channel_ptr, message_t* data)
{
   msgrcv(channel_ptr->ch_id, (void*)data, sizeof(message_t)-sizeof(long), FCFS, IPC_NOWAIT);
}

/**
* @brief Retrieves the first message from a channel. The calling process is blocked
*     until a message is delivered to the channel.
*
* @param[inout] channel_ptr pointer to a struct channel_t with channel configuration
* @param[out]    data        pointer to a user-allocated message_t structure
*
* @return none
*/
void channel_retrieve_block(channel_t* channel_ptr, message_t* data)
{
   msgrcv(channel_ptr->ch_id, (void*)data, sizeof(message_t)-sizeof(long), FCFS, 0);
}

/**
* @brief Retrieves the first message with the specified category. The calling process is not blocked
*     if there are no messages available on the channel.
*
* @param[inout] channel_ptr pointer to a struct channel_t with channel configuration
* @param[out]    data        pointer to a user-allocated message_t structure
* @param[in]     category    message category to retrieve
*
* @return none
*/
void channel_retrieve_cat_nonblock(channel_t* channel_ptr, message_t* data, long category)
{
   msgrcv(channel_ptr->ch_id, (void*)data, sizeof(message_t)-sizeof(long), category, IPC_NOWAIT);
}

/**
* @brief Retrieves the first message with the specified category. The calling process is blocked
*     until a message is delivered to the channel.
*
* @param[inout] channel_ptr pointer to a struct channel_t with channel configuration
* @param[out]    data        pointer to a user-allocated message_t structure
* @param[in]     category    message category to retrieve
*
* @return none
*/
void channel_retrieve_cat_block(channel_t* channel_ptr, message_t* data, long category)
{
   msgrcv(channel_ptr->ch_id, (void*)data, sizeof(message_t)-sizeof(long), category, 0);
}

/**
* @brief Pushes data to a channel. The calling process is not blocked
*     if there are no messages available on the channel.
*
* @param[inout] channel_ptr pointer to a struct channel_t with channel configuration
* @param[in ]    data        pointer to a user-allocated message_t structure
*
* @return none
*/
void channel_push_nonblock(channel_t* channel_ptr, message_t* data)
{
   msgsnd(channel_ptr->ch_id, (void*)data, sizeof(message_t)-sizeof(long), 0);
}

/**
* @brief Retrieves the first message with the specified category. The calling process is blocked
*     until a message is delivered to the channel.
*
* @param[inout] channel_ptr pointer to a struct channel_t with channel configuration
* @param[in ]    data        pointer to a user-allocated message_t structure
*
* @return none
*/
void channel_push_block(channel_t* channel_ptr, message_t* data)
{
   msgsnd(channel_ptr->ch_id, (void*)data, sizeof(message_t)-sizeof(long), IPC_NOWAIT);
}
