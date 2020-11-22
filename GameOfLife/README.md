# Cellular Automaton

Cellular Automaton is a concept that implements complex operation using elementary rules. The rules are even more basic than the bitwise operation, and are in complexity around the level of boolean algebra.

## Use of Cellular Automaton

Cellular Automaton is useful in the following but not limited to these domains : 

1. Simulation of Biological Systems
2. Simulation of Dynamical Fluid Systems
3. Multi-Agent process simulation
4. Data Encryption 
5. Data Compression
6. Digital Image, Digital Audio and Video  processing
7. Hashing Algorithms Optimization
8. Implementation as a Universal Computer

## Conway's Game of Life

Conway's Game of Life is probably one of the earliest attempts to simulate a real-life looking process using cellular Automaton. 
It is based on observing an `3x3` grid to decide the next state of a particular cell located centrally to the grid.
Now consider, these basic rules : 
1. If a cell has only 0 or 1 alive neighbour, it dies out of `ISOLATION`.
2. If a cell has 2 alive neighbour it survives (if alive) `SURVIVAL`.
3. If a cell has 3 neighbours then it becomes `ALIVE` if dead, `REPRODUCTION`.
4. If a cell has 4 or more neighbours alive, it dies of `CONGESTION`.

These rules looks easy but produce complex systems : 

1. `OSCILLATOR` : Oscillator oscillates between 2 or more state. It has multiple variants. Some of them are shown below : 

> Period 2 OSCILLATOR   
![OSCILLATOR1](https://upload.wikimedia.org/wikipedia/commons/9/95/Game_of_life_blinker.gif)    
![OSCILLATOR2](https://upload.wikimedia.org/wikipedia/commons/1/12/Game_of_life_toad.gif)   
![OSCILLATOR3](https://upload.wikimedia.org/wikipedia/commons/1/1c/Game_of_life_beacon.gif) 

> Period 3 OSCILLATOR   
![OSCILLATOR4](https://upload.wikimedia.org/wikipedia/commons/0/07/Game_of_life_pulsar.gif)

> Period 15 OSCILLATOR  
![OSCILLATOR5](https://upload.wikimedia.org/wikipedia/commons/f/fb/I-Column.gif)

2. `Glider` : Glider is an agent capable of motion. It falls under variant of space-ships.  
![GLIDER](https://upload.wikimedia.org/wikipedia/commons/f/f2/Game_of_life_animated_glider.gif)

3. `Glider Gun` ; Glider Gun is an agent capable of producing Gliders for infinite time and survive itself as well.     
![GLIDERGUN](https://upload.wikimedia.org/wikipedia/commons/e/e5/Gospers_glider_gun.gif)    

