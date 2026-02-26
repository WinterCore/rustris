# Rustris

<h4 align="center">Simple Tetris game written in <s>Rust</s> C</h4>

This project was supposed to be written in Rust but I changed my mind because apparently OpenGL is full of unsafe code and there's no advantage in using Rust so I'll be writing it in C, and NO I will not change the name.


## How to run locally
- Make sure you have any C compiler and [make](https://www.gnu.org/software/make/) installed
- Clone the project `git clone https://github.com/WinterCore/rustris.git && cd rustris`
- Build the game by running `make`
- Run the game executable `./Rustris`

### TODO

- [X] Implement game over condition (board collision at spawn)
- [X] Refactor rendering code
- [X] Implement 7-bag randomizer (cycle shuffled sets of all 7 pieces instead of pure random)
- [X] Implement counterclockwise rotation
- [X] Implement hard drop (instant drop on key press)
- [ ] Implement lock delay (brief window to move/rotate before a piece locks)
- [ ] Add ghost piece (shows where the active piece will land)
- [ ] Implement pause
- [ ] UI
  - [ ] Implement text rendering (bitmap font or freetype)
  - [ ] Constrain board to center of screen, leaving room for side panels
  - [ ] Left panel: score, level, lines cleared
  - [ ] Right panel: next piece preview
  - [ ] Game over screen with final score and restart prompt
  - [ ] Main menu screen
- [ ] Add sound effects (piece lock, line clear, level up)
- [ ] Implement hold piece
- [ ] Style improvements
  - [ ] Add borders to pieces
  - [ ] Add simple animations (line clear flash, piece spawn animation)
- [ ] Add multiplayer


### References
- [https://learnopengl.com/Getting-started/Hello-Triangle](https://learnopengl.com/Getting-started/Hello-Triangle)

`bear -- make` to generate `compile_commands.json` for `coc-clangd`
