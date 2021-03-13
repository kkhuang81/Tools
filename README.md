# Tools
Some useful tools to process graph files.



### Description

**pw_ic.cpp**: generate the realizations under the IC model.

**pw_lt.cpp**: generate the realizations under the LT model.



### Compile requirement

**dependency**: sftm folder

**command**: g++ pw_ic.cpp -O3 sfmt/SFMT.c -o pwic



### Input requirement

Dataset file



### How to run

./pw dataset_name number

--number: the number of realizations to be  generated