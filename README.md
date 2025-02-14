# Chess Engine in C

This is a simple, header-only chess engine library written in C, designed in the [stb-style](https://github.com/nothings/stb). It consists of a single header file that you can easily integrate into your C projects. The library provides core functionality for managing chess boards, generating valid moves, and more, making it a great starting point for building chess applications or engines.

## Key Features

- **No bitboards**: The chess engine avoids the complexity of [bitboards](https://en.wikipedia.org/wiki/Bitboard) for move generation and board representation.
- **16-bit move encoding**: Each move is encoded in a simple, 16-bit structure, making it easy to work with.

### Move Encoding Format

Each move is represented by a 16-bit encoding with the following structure:

- **From square (6 bits)**: The starting square of the move (64 possible squares).
- **To square (6 bits)**: The destination square of the move (64 possible squares).
- **Move type (2 bits)**: Defines the type of move:
    - `00`: Normal move
    - `01`: Promotion
    - `10`: Castle
    - `11`: En-passant
- **Promotion piece (2 bits)**: Defines the piece to promote to in case of a promotion move:
    - `00`: Knight
    - `01`: Bishop
    - `10`: Rook
    - `11`: Queen

## How to Use

To integrate the chess engine into your project, follow these steps:

### 1. Download the Header File

Download or copy the `chess.h` file into your project directory.

### 2. Include the Header in Your Code

In any C source file where you wish to use the chess engine, include the `chess.h` file as follows:

```c
#define CHESS_IMPLEMENTATION
#include "chess.h"
```

For practical examples, refer to the examples/ folder. It contains code snippets that demonstrate how to use the chess engine in various scenarios. These examples will help you get started quickly with different use cases.