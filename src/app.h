/**
* @file app.h
* @brief Definitions for the demo app
* @anchor header_app
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
#ifndef APP_H
#define APP_H

/*! \mainpage
 *
 * \section intro_sec Introduction
 *
 * ControlX is a C-based framework for sensor data processing and control in spacecraft systems. It simulates interactions between sensors and actuators, including fault tolerance mechanisms such as Triple Modular Redundancy (TMR).
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
 * 2. doxygen generate_doc
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
#include "control.h"

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
#define TOT_ACTUATORS   6
/* @} */

/**
 * @name Dummy processes configuration
 * @brief Configuration of the total number of acquisition and actuation
 * @{
 */
#define TOT_SENSING      20
#define TOT_ACTUATING    10
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

/**
 * @brief Mark the procedure with file visibility
 */
#define PRIVATE         static

# endif /*APP_H*/
