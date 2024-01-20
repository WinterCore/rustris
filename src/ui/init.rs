use super::player_board::PlayerBoard;

use std::sync::Arc;

use eframe::{egui::{self, RichText, Button}, egui_glow::CallbackFn, epaint::Vec2};


use egui::mutex::Mutex;


pub fn init() -> Result<(), eframe::Error> {
    let options = eframe::NativeOptions {
        ..Default::default()
    };

    eframe::run_native(
        "Rustris",
        options,
        Box::new(|cc| {
            // This gives us image support:
            // egui_extras::install_image_loaders(&cc.egui_ctx);

            Box::new(App::new(cc))
        }),
    )
}



#[derive(PartialEq, Debug)]
enum AppView {
    Home,
    Game,
}

struct App {
    view: AppView,
    player_board: Option<Arc<Mutex<PlayerBoard>>>
}

impl App {
    fn new(cc: &eframe::CreationContext<'_>) -> Self {
        let gl = cc
            .gl
            .as_ref()
            .expect("You need to run eframe with the glow backend");

        Self {
            view: AppView::Game,
            player_board: Option::Some(Arc::new(Mutex::new(PlayerBoard::new(&*gl)))),
        }
    }

    fn paint(&mut self, ui: &mut egui::Ui) {
        let (rect, response) =
            ui.allocate_exact_size(egui::Vec2::splat(300.0), egui::Sense::drag());


        // Clone locals so we can move them into the paint callback:
        if let Some(board) = self.player_board.clone() {

            let callback = egui::PaintCallback {
                rect,
                callback: std::sync::Arc::new(CallbackFn::new(move |_info, painter| {
                    board.lock().paint(painter.gl());
                })),
            };
            ui.painter().add(callback);
        }
    }
}

impl eframe::App for App {
    fn on_exit(&mut self, gl: Option<&eframe::glow::Context>) {
        if let Some(gl) = gl {
            if let Some(ref player_board) = self.player_board {
                player_board.lock().destroy(gl);
            }
        }
    }

    fn update(&mut self, ctx: &egui::Context, frame: &mut eframe::Frame) {
        match self.view {
            AppView::Home => {
                egui::CentralPanel::default().show(ctx, |ui| {
                    ui.vertical_centered(|ui| {
                        ui.add_space(30.0);
                        ui.style_mut().spacing.button_padding = Vec2 {
                            x: 40.0,
                            y: 10.0,
                        };
                        ui.style_mut().spacing.item_spacing = Vec2 {
                            x: 10.0,
                            y: 20.0,
                        };
                        ui.label(RichText::new("RUSTRIS").size(60.0).strong());


                        ui.add_space(60.0);

                        let play_button = ui.add(Button::new(RichText::new("Play").size(30.0)));
                        let quit_button = ui.add(Button::new(RichText::new("Quit").size(30.0)));
                        if play_button.clicked() {
                            self.view = AppView::Game;
                            println!("Clicked");

                            if let Some(ref pb) = self.player_board {
                                println!("debug {:?}", pb.lock());
                            }
                        }

                        if quit_button.clicked() {
                            ctx.send_viewport_cmd(egui::ViewportCommand::Close);
                        }
                    });
                });
            },
            AppView::Game => {
                egui::CentralPanel::default().show(ctx, |ui| {
                    egui::Frame::canvas(ui.style()).show(ui, |ui| {
                        self.paint(ui);
                    });
                });
            }
        }
    }
}
