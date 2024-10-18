# Rustris

<h4 align="center">Simple Tetris game written in <s>Rust</s> C</h4>

This project was supposed to be written in Rust but I changed my mind because apparently OpenGL is full of unsafe code and there's no advantage in using Rust so I'll be writing it in C, and NO I will not change the name.


## How to run locally
- Make sure you have any C compiler and [make](https://www.gnu.org/software/make/) installed
- Clone the project `git clone https://github.com/WinterCore/rustris.git && cd rustris`
- Build the game by running `make`
- Run the game executable `./Rustris`

### TODO

- Implement the basic game
- Improve the UI
- Add a better RNG
- Add multiplayer


### References
- [https://learnopengl.com/Getting-started/Hello-Triangle](https://learnopengl.com/Getting-started/Hello-Triangle)
- [https://tetris.fandom.com/wiki/SRS](https://tetris.fandom.com/wiki/SRS)

`bear -- make` to generate `compile_commands.json` for `coc-clangd`
