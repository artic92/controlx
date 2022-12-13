/**
* @file driver.c
* @brief Entry point for the demo app.
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
#include "app.h"

/************************** Function Prototypes *****************************/
/**
* @brief Sensor code.
*
* @details Sends data to the GNC or to the voter (when in TMR more) at intervals
*     that varies randomly between 0-10 seconds.
*
* @param[in] data_ch_tx   channel where the data is sent
* @param[in] id_sens      class identifier according to @ref def_ids "this" classification
* @param[in] id_replica   identifier of the replica in @ref sec_tmr_arch "TMR" configuration
* @param[in] inject_errors when true the sensor injects faulty data
*
* @return none
*/
PRIVATE void sense(channel_t *data_ch_tx, int id_sens, int id_replica, bool inject_errors);

/**
* @brief Actuator code.
*
* @details Gets data from the GNC every time there is one available and simulates
*     a random delay between 0-10 seconds.
*
* @param[in] data_ch_rx channel where the data is received
* @param[in] id_replica identifier of the replica
*
* @return none
*/
PRIVATE void actuate(channel_t *data_ch_rx, int id_replica);

/**
*
* @brief Creates the infrastructure showed in the \ref img_basic_arch "architecture" section
*
* @anchor driver_details
* @details  This code creates all necessary processes and IPC needed to simulate
*   the various parts of the data-gathering system (sensors, actuators, control).
*
*   If enable_tmr = true the code creates the TMR configuration as shown \ref img_tmr_arch "here"
*
*   If inject_errors = true the sensors simulate a stuck-at-N error condition
*/
int main (int argc, char* argv[])
{
   int i;
   pid_t pid;
   int status;
   int opt;
   int log_fd;

   // log file configuration
   FILE* actual_log_file = stdout;
   char log_file_path[100];

   // CLI flags configuration
   bool change_log_file = false;
   bool enable_tmr = false;
   bool inject_errors = false;

   // number of sensors and voters for the non-TMR configuration
   int tot_imu = TOT_IMU;
   int tot_gnss = TOT_GNSS;
   int tot_strtrk = TOT_STRTRK;
   int tot_voters = 0;

   channel_t* ch_imu = NULL;
   channel_t* ch_gnss = NULL;
   channel_t* ch_strtrk = NULL;
   channel_t* ch_sens = NULL;
   channel_t* ch_act = NULL;
   channel_t* ch_cmd = NULL;
   message_t exit_msg;

   // CLI arguments parsing
   while ((opt = getopt(argc, argv, "hf:ti")) != -1)
   {
      switch (opt)
      {
      case 'f':
         strcpy(log_file_path, optarg);
         change_log_file = true;
         break;
      case 't':
         enable_tmr = true;
         break;
      case 'i':
         inject_errors = true;
         break;
      case 'h':
      default:
         fprintf(stderr, "Usage %s [-h] [-t] [-i] [-f PATH]\n", argv[0]);
         fprintf(stderr, "............ -h help\n");
         fprintf(stderr, "............ -t enable TMR example\n");
         fprintf(stderr, "............ -i inject errors from sensors\n");
         fprintf(stderr, "............ -f set the path pf the log file\n");
         exit(EXIT_FAILURE);
      }
   }

   // Change output from stout to a user-defined file
   if(change_log_file)
   {
      fprintf(actual_log_file, "[%i] log to %s\n", getpid(), log_file_path);

      if ((log_fd = open(log_file_path, O_CREAT | O_WRONLY | O_APPEND)) == -1)
      {
         perror("open failed with code");
         exit(EXIT_FAILURE);
      }

      dprintf(log_fd, "open a file with descriptor %i\n", log_fd);
   }

   // Increase total number of processes in TMR configuration
   if(enable_tmr)
   {
      fprintf(actual_log_file, "[%i] TMR configuration enabled\n", getpid());

      tot_imu = tot_imu * 3;
      tot_gnss = tot_gnss * 3;
      tot_strtrk = tot_strtrk * 3;
      tot_voters = TOT_VOTERS;

      ch_imu = malloc(sizeof(channel_t));
      ch_gnss = malloc(sizeof(channel_t));
      ch_strtrk = malloc(sizeof(channel_t));

      channel_create(ch_imu, CHIMUTMR);
      channel_create(ch_gnss, CHGNSSTMR);
      channel_create(ch_strtrk, CHSTRTRKTMR);
   }

   ch_sens = malloc(sizeof(channel_t));
   ch_act = malloc(sizeof(channel_t));
   ch_cmd = malloc(sizeof(channel_t));

   channel_create(ch_sens, CH1);
   channel_create(ch_act, CH2);
   channel_create(ch_cmd, CHCMD);

   // generate tot_imu IMU processes
   for (i = 0; i < tot_imu; i++)
   {
      pid = fork();
      if(pid == 0)
      {
         if(enable_tmr){
            sense(ch_imu, ID_IMU, i, inject_errors);
         }else{
            sense(ch_sens, ID_IMU, i, inject_errors);
         }
         exit(EXIT_SUCCESS);
      }
   }

   // generate tot_gnss GNSS processes
   for (i = 0; i < tot_gnss; i++)
   {
      pid = fork();
      if(pid == 0)
      {
         if(enable_tmr){
            sense(ch_gnss, ID_GNSS, i, inject_errors);
         }else{
            sense(ch_sens, ID_GNSS, i, inject_errors);
         }
         exit(EXIT_SUCCESS);
      }
   }

   // generate tot_strtrk star tracker processes
   for (i = 0; i < tot_strtrk; i++)
   {
      pid = fork();
      if(pid == 0)
      {
         if(enable_tmr){
            sense(ch_strtrk, ID_STRTRK, i, inject_errors);
         }else{
            sense(ch_sens, ID_STRTRK, i, inject_errors);
         }
         exit(EXIT_SUCCESS);
      }
   }

   if(enable_tmr)
   {
      // generate the voter process for IMU TMR
      pid = fork();
      if (pid == 0)
      {
         vote(ch_cmd, ch_imu, ch_sens, ID_IMU);
         exit(EXIT_SUCCESS);
      }

      // generate the voter process for GNSS TMR
      pid = fork();
      if (pid == 0)
      {
         sleep(30);
         vote(ch_cmd, ch_gnss, ch_sens, ID_GNSS);
         exit(EXIT_SUCCESS);
      }

      // generate the voter process for star tracker TMR
      pid = fork();
      if (pid == 0)
      {
         sleep(50);
         vote(ch_cmd, ch_strtrk, ch_sens, ID_STRTRK);
         exit(EXIT_SUCCESS);
      }
   }

   // generate the actuator processes
   pid = fork();
   if (pid == 0)
   {
      actuate(ch_act, ID_ACT);
      exit(EXIT_SUCCESS);
   }

   // generate the control process
   pid = fork();
   if (pid == 0)
   {
      control(ch_cmd, ch_sens, ch_act);
      exit(EXIT_SUCCESS);
   }

   fprintf(actual_log_file, "[%i] driver: waiting for childs termination....\n", getpid());

   for (i = 0; i < (tot_imu + tot_gnss + tot_strtrk + TOT_ACTUATORS); i++)
   {
      if ((pid = wait(&status)) == -1)
      {
         perror("wait");
      }
   }

   fprintf(actual_log_file, "[%i] driver: all processes terminated, terminating voters and command...\n", getpid());

   // send the termination message to all stand-alone processes (control and voters)
   exit_msg.mtype = TERMINATE;
   exit_msg.mvalue = TERMINATE;
   channel_push_nonblock(ch_cmd, &exit_msg);

   // TODO: fix this part
   // if(wait(&status) == -1)
   // {
   //    perror("wait");
   // }

   // do the same for the voters in case of TMR configuration
   if(enable_tmr)
   {
      for (i = 0; i < (tot_voters); i++)
      {
         channel_push_nonblock(ch_cmd, &exit_msg);
         // TODO: fix this part
         // if(wait(&status) == -1)
         // {
         //    perror("wait");
         // }
      }
   }

   channel_delete(ch_sens);
   channel_delete(ch_act);
   channel_delete(ch_cmd);
   free(ch_sens);
   free(ch_act);
   free(ch_cmd);
   close(log_fd);

   return EXIT_SUCCESS;
}

PRIVATE void sense(channel_t* data_ch_tx, int id_sens, int id_replica, bool inject_errors)
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

PRIVATE void actuate(channel_t* data_ch_rx, int id_replica)
{
   message_t data_msg;

   channel_create(data_ch_rx, CH2);

   channel_retrieve_block(data_ch_rx, &data_msg);
   fprintf(stdout, "[%i] actuator: received data: type %li, value %i \n", getpid(), data_msg.mtype, data_msg.mvalue);
   sleep(rand() % 10);
}
