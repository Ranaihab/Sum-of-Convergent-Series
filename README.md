# Sum-of-Convergent-Series
This program calculates the sum of convergent series using mpi.
<br/>
- Process 0 (master) broadcast n to all processes and reduce to get the sum from all other
processes<br/>
- All processes (including master process) will do their part of calculation
