CS 776 Assignment 2
===================
Ryan C. Gorman
2023-09-27

## Steps to Run
1. Run `cmake .`
2. Run `make`
3. Run `./assignment2 <parameter_search|ga_performance|chc_performance>`

## Parameter Search
The parameter search will run the genetic algorithm with a variety of
parameters and output the results to files called `dejong#.csv`, where `#` is
the number of the De Jong function being optimized.

To run the parameter search, run `./assignment2 parameter_search`.

## GA Performance
The GA performance will run the genetic algorithm with the parameters that
performed the best in the parameter search and output the results to files
called `chc_performance_dejong#.csv`, where `#` is the number of the De Jong 
function being optimized.

To run the GA performance, run `./assignment2 ga_performance`.

## CHC Performance
The CHC performance will run the CHC algorithm with the parameters given
(50, 75, 0.95, 0.05) and output the results to files called
`chc_performance_dejong#.csv`, where `#` is the number of the De Jong
function being optimized.

To run the CHC performance, run `./assignment2 chc_performance`.