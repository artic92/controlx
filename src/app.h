/**
* @file app.h
* @brief Functions and data definitions for the demo app
* @anchor header_app
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
#ifndef HEADER_H
#define HEADER_H

/*! \mainpage
 *
 * \section intro_sec Introduction
 *
 * This project represent the challenge from the Exploration Company for the hiring campaign in 2022.
 *
 * \section intro_overview Overview
 *
 * On a spacecraft, usually, sensor data is consumed from certain sensors. Most of the time, this data requires some
 * preprocessing before it can be consumed by, e.g. a Guidance, Navigation, and Control (GNC) algorithm. Such an
 * algorithm will calculate commands for the actuators (e.g. thrusters). The software framework will use the
 * commands to send it to the actuator. Often such connections are realized with a serial bus. Often the GNC
 * algorithms are developed by other teams, so it must be possible to easily exchange them for fast iteration.
 *
 * In this very simplified use case, we assume the following:
 *
 * * We have three input sensors:
 *  * IMU
 *  * GNSS data
 *  * Star Trackers
 * * We have six actuators (thrusters)
 * * The GNC algorithms are supplied by the GNC team in form of a C function that accepts the sensor data and
 * returns the thruster commands. For the use case, you can just stub a function doing some dummy
 * calculations with the inputs and returning some outputs for the actuators. Input data can be null, i.e. not all
 * sensors use the same speed.
 *
 * \section doc_arch Architecture
 *
 * The project is implemetented by means of Unix processes. Their interconnection is done through an
 * abract object of type channel_t. The latter can be updated to change the underlying communication mechanism.
 * So far, the implementation of a channel_t is done through Unix message queues.
 *
 * Following is a diagram of the architecture:
 *
 * \anchor img_basic_arch
 * \image html basic_architecture.png "architecture with no TMR"
 *
 * \anchor sec_tmr_arch
 * \subsection doc_arch_tmr TMR architecture
 *
 * The solution provides TMR configuration by specifying the option '-t' when calling the mail executable. The voter
 * executes a 2-out-of-3 voting mechanism. The architecture uses one message queue for the messages between sensors
 * and control and another queue between control and actuators. The selection of specific sources is done through usage
 * of differen message types. Please refer to the code documentation for the IDs used.
 *
 * \anchor img_tmr_arch
 * \image html tmr_architecture.png "architecture with TMR"
 *
 * \section install_deps Dependencies
 *
 * The project requires the following dependencies to be compiled and executed:
 *  * gcc
 *  * doxygen
 *  * make
 *  * graphviz
 *
 * You can install the above dependencies by issuing the command (for Ubuntu platforms):
 *
 * sudo apt-get install -y make gcc doxygen doxygen-gui graphviz
 *
 * \section install_doc Documentation
 *
 * To generate the documentation:
 *
 * 1. cd doc/
 * 2. doxygen gen_doc_cfg
 *
 * Open the HTML version with any type of browser. In case of firefox:
 *
 * firefox output/index.html
 *
 * \section install_run Running the tests
 *
 * In order to run the tests:
 *
 * 1. cd src
 * 2. make
 * 3. ./driver
 */

/***************************** Include Files ********************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "channel.h"
#include "control_law.h"

/************************** Constant Definitions *****************************/
/**
 * @name Processes configuration
 * @brief Number of processes composing the demo
 * @{
 */
#define TOT_IMU         1
#define TOT_GNSS        1
#define TOT_STRTRK      1
#define TOT_VOTERS      3
#define TOT_ACTUATORS   1
/* @} */

/**
 * @name IDs
 * @anchor def_ids
 * @brief Identifiers of each process needed for message exchange
 * @{
 */
#define ID_IMU          1
#define ID_GNSS         2
#define ID_STRTRK       3
#define ID_ACT          4
#define ID_CTR          5
/* @} */

/**
 * @name Channel association
 * @brief Needed to couple each process to the right channel
 * @{
 */
#define CH1             '1'
#define CH2             '2'
#define CHIMUTMR        '3'
#define CHGNSSTMR       '4'
#define CHSTRTRKTMR     '5'
#define CHCMD           '6'
/* @} */

/**
 * @brief Termination message sent to the stand-alone processes
 */
#define TERMINATE       10000

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
void sense(channel_t* data_ch_tx, int id_sens, int id_replica, bool inject_errors);

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
void actuate(channel_t* data_ch_rx, int id_replica);

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

# endif /*HEADER_H*/
