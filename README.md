# DataMiner
A simple data mining software which is easily extensible for new algorithms.

## Adding a custom Algorithm
To add a custom algorithm, all you must do is inherit from the processor class and implement all virtual methods. There is one additional step however, which is to add the processor's name and a function which newly heap allocates the processor and returns it in `Processors.hpp` located in `src/Processor/Processors.hpp` inside of the map.