use std::{thread::sleep, time::Duration};

use ncurses::*;

const WIDTH: i32 = 12;
const HEIGHT: i32 = 18;

fn main() {
    
    initscr();
    // raw();
    use_default_colors();
    start_color();
    
    let win = newwin(HEIGHT, WIDTH, 0, 0);

    wclear(win);
    mvwaddstr(win, 0, 0, "Hello world");

    wrefresh(win);

    // sleep(Duration::from_millis(3000));
    wgetch(win);

    endwin();
}
