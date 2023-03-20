<p align="center">
<img width="300" alt="Screenshot 2023-03-21 at 00 48 01" src="https://user-images.githubusercontent.com/49457798/226489433-2cbf2e37-0525-43e6-8039-e4b717ebb879.png">
</p>

# Chess Engine ♟️

Chess engine implemented from scratch in C++. After a bit of testing againt the [Chess.com](chess.com) AI, I think its current level is around 1800 ELO points, more than enough to beat me! >.<

## Techniques and Algorithms
A quick reference to the [Chess Programming Wiki](https://www.chessprogramming.org/Main_Page) of the approaches I have implemented.

### Board representation
- [Bitboards](https://www.chessprogramming.org/Bitboards)
- [Bitscan](https://www.chessprogramming.org/BitScan)
- [Flipping and Rotating](https://www.chessprogramming.org/Flipping_Mirroring_and_Rotating)
- [Setwise Operations and Masking](https://www.chessprogramming.org/General_Setwise_Operations)

### Search
- [Move Encoding](https://www.chessprogramming.org/Encoding_Moves)
- [Negamax](https://www.chessprogramming.org/Negamax)
- [Alpha-Beta Pruning](https://www.chessprogramming.org/Alpha-Beta)
- [Move Ordering](https://www.chessprogramming.org/Move_Ordering)
- [Zobrist Hashing](https://www.chessprogramming.org/Zobrist_Hashing)
- [Transposition Table](https://www.chessprogramming.org/Transposition_Table)

### Evaluation
- [Material balance](https://www.chessprogramming.org/Material)
- [Quiescence search](https://www.chessprogramming.org/Quiescence_Search)
- [Piece-Square tables](https://www.chessprogramming.org/Piece-Square_Tables)
