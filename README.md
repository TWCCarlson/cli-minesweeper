This is a quick little commandline interface implementation of minesweeper.

![Pasted image 20250520124357](https://github.com/user-attachments/assets/07a2cf7a-866f-4857-85d7-e5db265a9d17)


You can make moves using the syntax <instruction><row><col>. For example, to place a flag on (6,a) write `f6a`. To mark a tile with a question mark, write `q6a`. To reveal the tile and risk exploding a mine, write `o6a`.

The game will show you how many mines you have left to 'find' by subtracting the number of flags you have placed from the number of mines corresponding to the difficulty level you chose.

There are currently 3 levels of difficulty to choose from:

| Difficulty   | H  | W  | Mine Count | Mines/Tile |
|--------------|----|----|------------|------------|
| Beginner     | 9  | 9  | 10         | 0.12       |
| Intermediate | 16 | 16 | 40         | 0.16       |
| Expert       | 16 | 26 | 84         | 0.20       |

Or you can set your own, custom, difficulty with board sizes from 2x2 to 26x26 and from 1-675 mines.
