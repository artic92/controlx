# the_exploration_company_challenge

This project represent the challenge from the Exploration Company for the hiring campaign in 2022.

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

## Running the tests

In order to run the tests:

```text
make -C src/ && ./src/driver
```
