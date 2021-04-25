# Listing 2

This is a demo of how powerful threads are:

- Idea is to show the difference of a multi-threaded and single-threaded program regarding the performance

- This shall be done by a simple task of processing two txt files:
  - the content of the files shall be read in by the program and then be putout to another single file
    - one program does this in a singe thread the other in two
- Performance can be evaluated by measuring the execution time of the two programs and comparing them afterwards
  - either UNIX timestamps or ```std::chrono```for measuring the time
    - each file should have around 1000 lines of text
