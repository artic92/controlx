/**
* @file app.c
* @brief Functions implementation of @ref header_app "app.h".
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
#include "app.h"

void sense(channel_t* data_ch_tx, int id_sens, int id_replica, bool inject_errors)
{
   u_int8_t i;
   message_t data_msg;
   data_msg.mtype = id_sens;

   channel_create(data_ch_tx, data_ch_tx->seed);

   if(inject_errors)
   {
      switch (id_replica)
      {
      case 0:
         srand('c');
         data_msg.mvalue = (rand() % 1000);
         fprintf(stdout, "[%i] sensor %i/%i: stuck-at-N simulation\n", getpid(), id_sens, id_replica);
         break;
      case 1:
         data_msg.mvalue = 999;
         fprintf(stdout, "[%i] sensor %i/%i: stuck-at-N simulation\n", getpid(), id_sens, id_replica);
         break;
      case 2:
      default:
         data_msg.mvalue = (rand() % 100);
         break;
      }
   }
   else
   {
      data_msg.mvalue = (rand() % 100);
   }
   sleep(rand() % 10);

   fprintf(stdout, "[%i] sensor %i/%i: generated data: type %li, value %i\n", getpid(), id_sens, id_replica, data_msg.mtype, data_msg.mvalue);
   channel_push_nonblock(data_ch_tx, &data_msg);
}

void actuate(channel_t* data_ch_rx, int id_replica)
{
   message_t data_msg;

   channel_create(data_ch_rx, CH2);

   channel_retrieve_block(data_ch_rx, &data_msg);
   fprintf(stdout, "[%i] actuator: received data: type %li, value %i \n", getpid(), data_msg.mtype, data_msg.mvalue);
   sleep(rand() % 10);
}

void control(channel_t* cmd_ch, channel_t* data_ch_rx, channel_t* data_ch_tx)
{
   message_t mex_rx;
   message_t mex_tx;
   int i;

   channel_create(data_ch_rx, CH1);
   channel_create(data_ch_tx, CH2);
   channel_create(cmd_ch, CHCMD);

   mex_tx.mtype = ID_CTR;

   fprintf(stdout, "[%i] control: waiting for messages...\n", getpid());

   while (true)
   {
      channel_retrieve_nonblock(cmd_ch, &mex_rx);
      if(mex_rx.mtype == TERMINATE)
      {
         fprintf(stdout, "[%i] control: received termination command, SHUTTING DOWN...\n", getpid());
         sleep(5);
         exit(EXIT_SUCCESS);
      }

      channel_retrieve_block(data_ch_rx, &mex_rx);
      fprintf(stdout, "[%i] control: received data: type %li, value %i \n", getpid(), mex_rx.mtype, mex_rx.mvalue);

      control_law(&mex_rx.mvalue, &mex_tx.mvalue);

      channel_push_nonblock(data_ch_tx, &mex_tx);
      fprintf(stdout, "[%i] control: sent data to actuators: type %li, value %i\n", getpid(), mex_tx.mtype, mex_tx.mvalue);

      channel_retrieve_block(data_ch_rx, &mex_rx);
      fprintf(stdout, "[%i] control: received data: type %li, value %i \n", getpid(), mex_rx.mtype, mex_rx.mvalue);

      control_law(&mex_rx.mvalue, &mex_tx.mvalue);

      channel_push_nonblock(data_ch_tx, &mex_tx);
      fprintf(stdout, "[%i] control: sent data to actuators: type %li, value %i\n", getpid(), mex_tx.mtype, mex_tx.mvalue);

      channel_retrieve_block(data_ch_rx, &mex_rx);
      fprintf(stdout, "[%i] control: received data: type %li, value %i \n", getpid(), mex_rx.mtype, mex_rx.mvalue);

      control_law(&mex_rx.mvalue, &mex_tx.mvalue);

      channel_push_nonblock(data_ch_tx, &mex_tx);
      fprintf(stdout, "[%i] control: sent data to actuators: type %li, value %i\n", getpid(), mex_tx.mtype, mex_tx.mvalue);
   }
}

void vote(channel_t* cmd_ch, channel_t* data_ch_rx, channel_t* data_ch_tx, int id_sens)
{
   message_t mex_rx1, mex_rx2, mex_rx3;
   message_t mex_tx;
   int i;

   mex_tx.mtype = id_sens;

   channel_create(data_ch_rx, data_ch_rx->seed);
   channel_create(data_ch_tx, CH1);
   channel_create(cmd_ch, CHCMD);

   fprintf(stdout, "[%i] vote: waiting for messages...\n", getpid());

   while (true)
   {
      channel_retrieve_nonblock(cmd_ch, &mex_rx1);
      if(mex_rx1.mtype == TERMINATE)
      {
         fprintf(stdout, "[%i] voter: received termination command, SHUTTING DOWN...\n", getpid());
         exit(EXIT_SUCCESS);
      }

      channel_retrieve_block(data_ch_rx, &mex_rx1);
      fprintf(stdout, "[%i] voter: received data: type %li, value %i \n", getpid(), mex_rx1.mtype, mex_rx1.mvalue);

      channel_retrieve_block(data_ch_rx, &mex_rx2);
      fprintf(stdout, "[%i] voter: received data: type %li, value %i \n", getpid(), mex_rx2.mtype, mex_rx2.mvalue);

      if(mex_rx1.mvalue != mex_rx2.mvalue)
      {
         channel_retrieve_block(data_ch_rx, &mex_rx3);
         fprintf(stdout, "[%i] voter: received data: type %li, value %i \n", getpid(), mex_rx3.mtype, mex_rx3.mvalue);

         if(mex_rx2.mvalue == mex_rx3.mvalue)
         {
            fprintf(stdout, "[%i] voter: 2-out-3 consensus reached, values 1:%i, 2:%i, 3:%i\n", getpid(), mex_rx1.mvalue, mex_rx2.mvalue, mex_rx3.mvalue);
            mex_tx.mvalue = mex_rx3.mvalue;
         }
         else
         {
            fprintf(stdout, "[%i] voter: NO consensus reached, sending 0. values 1:%i, 2:%i, 3:%i\n", getpid(), mex_rx1.mvalue, mex_rx2.mvalue, mex_rx3.mvalue);
            mex_tx.mtype = 0;
         }
      }
      else
      {
         fprintf(stdout, "[%i] voter: 3-out-3 consensus reached, values 1:%i, 2:%i\n", getpid(), mex_rx1.mvalue, mex_rx2.mvalue);
         mex_tx.mvalue = mex_rx1.mvalue;
      }

      channel_push_nonblock(data_ch_tx, &mex_tx);
      fprintf(stdout, "[%i] voter: sent data to control: type %li, value %i\n", getpid(), mex_tx.mtype, mex_tx.mvalue);
   }
}
