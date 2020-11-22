# CELLULAR AUTOMATON AND UNIVERSAL COMPUTER

In this module, cellular automaton will be explored as a Universal Computer.

## Universal Computer

A universal computer in a cellular automaton is a system that can compute anything that a Turing machine can compute. A cellular automaton in which such a system exists is called universal. A universal computer may be either infinite or finite, but when combined with a universal constructor, it is assumed to be finite. 

In 1982, John Conway proved in Winning Ways that the Game of Life has a (finite) universal computer, as well as a universal constructor.

> Constructing finite-universal computer using conway's game of life is relatively simple. It uses glider's logic along with sliding memory implementation. But practical realization of such logic is difficult.

## Life as a Universal Turing Machine

Like any other CA, Life can be considered a computational device: an initial configuration of the automaton can encode an input string. One can let the system run and, at some point, read the current configuration as the result of the computation performed so far, decoding it into an output string. But exactly what can Life compute? It turns out that Life can compute everything a universal Turing machine can and therefore, taking on board Turing’s Thesis, function as a general purpose computer: a suitable selection of initial conditions can ensure that the system carry out arbitrary algorithmic procedures. 


