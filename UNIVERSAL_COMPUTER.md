# CELLULAR AUTOMATON AND UNIVERSAL COMPUTER

In this module, cellular automaton will be explored as a Universal Computer.

## Universal Computer

A universal computer in a cellular automaton is a system that can compute anything that a Turing machine can compute. A cellular automaton in which such a system exists is called universal. A universal computer may be either infinite or finite, but when combined with a universal constructor, it is assumed to be finite. 

In 1982, John Conway proved in Winning Ways that the Game of Life has a (finite) universal computer, as well as a universal constructor.

> Constructing finite-universal computer using conway's game of life is relatively simple. It uses glider's logic along with sliding memory implementation. But practical realization of such logic is difficult.

## Life as a Universal Turing Machine

Like any other CA, Life can be considered a computational device: an initial configuration of the automaton can encode an input string. One can let the system run and, at some point, read the current configuration as the result of the computation performed so far, decoding it into an output string. But exactly what can Life compute? It turns out that Life can compute everything a universal Turing machine can and therefore, taking on board Turing’s Thesis, function as a general purpose computer: a suitable selection of initial conditions can ensure that the system carry out arbitrary algorithmic procedures. 

## Cellular Automaton Analogy to Halting Problem

The Conway's Game of life is analogus to the halting problem. The Game of Life is undecidable, which means that given an initial pattern and a later pattern, no algorithm exists that can tell whether the later pattern is ever going to appear. 
This is a corollary of the halting problem: the problem of determining whether a given program will finish running or continue to run forever from an initial input.

## Solving the Halting Problem

`HYPOTHESIS` : The Game of Life had a deciding algorithm D that can predict its future states.
Since the Game of Life consists a pattern equivalent to universal turing machine, this deciding algorithm D can be used to solve the halting problem by taking `initial pattern` as one UTM and an input, and decide if halting will occour or not. 
`RESULTS` :     
1. It thus follows that any Deciding algorithm D doesnot exist.
2. It also follows that some patterns exist that remain chaotic forever. If this were not the case, one could progress the game sequentially until a non-chaotic pattern emerged, then compute whether a later pattern was going to appear. 

