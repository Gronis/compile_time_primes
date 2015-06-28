Compiletime Prime Table Calculation
===================================

This repo calculates a boolean array with prime numbers. The index represents the number and value represent whenever the number is a prime or not.
The bool array is calculated compiletime which by using c++ meta programming. This is done by using c++11 variadic templates, and thus requires a 
c++11 comilator.


Currently, compiling with more than 500 primes takes forever, so optimization might be needed. Calculating primes realtime is probably preferrable instead.
This project was done because I wanted to learn meta programming in c++, and calculating whenever a number is a prime was the first assignment I was introduced to 
when I started at Chalmers University. Feel free to use the code however you like (if you can make any since of it).