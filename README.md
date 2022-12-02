# Game of Life
## Description  
The Game of Life is a zero-player game. It consists of a two-dimensional rectangular world in which each square is inhabited by a living or dead cell. A cell lives or dies at the next moment depending on the number of living or dead cells in the adjacent eight squares. If there are too many living cells in the adjacent square, this cell will die at the next moment due to lack of resources; on the contrary, if there are too few living cells around, this cell will die because of being too lonely. In practice, the player can set the number of surrounding living cells that is suitable for the survival of the cell. If the number is set too high, most of the cells in the world will die because they can't find too many living neighbors until the whole world is lifeless; if the number is set too low, the world will be filled with life again And nothing changed. See detail [Conway's Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)

## Rules  
In the game of life, for any cell, the rules are as follows:

+ Each cell has two states - alive or dead , each cell interacts with the surrounding eight cells centered on itself (as shown, black is alive, white is dead)
+ When the current cell is in a viable state, when the surrounding viable cells are less than 2 (excluding 2), the cell becomes a dead state. (The number of simulated lives is scarce)
+ When the current cell is alive, when there are 2 or 3 living cells around, the cell remains as it is.
+ When the current cell is in a viable state, when there are more than 3 viable cells around, the cell becomes a dead state. (too many simulated lives)
+ When the current cell is in a dead state, when there are 3 surviving cells around, the cell becomes a viable state. (simulated breeding)

## Play  
Here is a sample input file `input.txt` which run command
```shell
make
```
