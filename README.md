# Simulation of SJF and SRT in C

## To Compile
	use `make` command
	
	or compile with `gcc simulation.c -o project2 -std=c99`

## To Run
	Using rediection of a file. i.e.
	```
	project2 < testfile
	```

## Input File Format
	```
	P0 0 18 0
	P1 1 16 0
	P2 2 14 0
	P3 3 12 0
	P5 4 10 0
	P6 5 8  0
	P7 6 6  0
	P8 7 4  0
	```
Where the information for the processes are in the following order

``` name arrivalTime runTime priority ```

seperated by a space.