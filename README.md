# ControlX

ControlX is a C-based framework for sensor data processing and control in spacecraft systems. It simulates interactions between sensors and actuators, including fault tolerance mechanisms such as Triple Modular Redundancy (TMR).

## Overview

ControlX processes data from three sensors (IMU, GNSS, Star Trackers) and controls six thrusters based on this data. It demonstrates real-time sensor data processing, fault detection, and system extensibility through modular architecture.

## Features

* Sensor Data Handling: Process data from multiple sensor sources, including IMU, GNSS, and Star Trackers.
* Thruster Control: Output commands to actuators (thrusters) based on sensor inputs.
* Fault Tolerance: Support for TMR, with two-out-of-three voting logic to ensure data reliability.
* Error Injection: Simulate faulty sensors to test system robustness with stuck-at-N sensor errors.

## Dependencies

The project requires the following dependencies to be compiled and executed:

 * gcc
 * doxygen
 * make
 * graphviz

You can install the above dependencies by issuing the command (for Ubuntu platforms):

```text
sudo apt-get install -y make gcc doxygen doxygen-gui graphviz
```

## Documentation

To generate the documentation:

```text
doxygen doc/generate_doc
```

Open the HTML version with any type of browser. In case of firefox:

```text
firefox doc/output/index.html
```

## CLI Options

The project includes several command-line options:

* -h: Display the help menu.
* -t: Enable TMR mode, introducing sensor redundancy and voting logic.
* -i: Inject stuck-at-N sensor errors for fault tolerance testing.
* -f <path>: Set the path to a log file to store output.

Example usage:

```bash
./driver -t -i -f /path/to/log.txt
```

## Running the tests

In order to run the tests:

```text
make -C src/ && ./src/driver
```
