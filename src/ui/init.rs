
use eframe::{egui::{self, Label, RichText, Style, Button}, epaint::{text::LayoutJob, Vec2}};

pub fn init() -> Result<(), eframe::Error> {
    let options = eframe::NativeOptions {
        initial_window_size: Some(egui::vec2(720.0, 480.0)),
        ..Default::default()
    };

    eframe::run_native(
        "Rustris",
        options,
        Box::new(|cc| {
            // This gives us image support:
            // egui_extras::install_image_loaders(&cc.egui_ctx);

            Box::<App>::default()
        }),
    )
}

struct App {

}

impl App {
    fn new(cc: &eframe::CreationContext<'_>) -> Self {
        let gl = cc
            .gl
            .as_ref()
            .expect("You need to run eframe with the glow backend");
        Self {
        }
    }
}

impl Default for App {
    fn default() -> Self {
        Self {
        }
    }
}

impl eframe::App for App {
    fn on_exit(&mut self, gl: Option<&eframe::glow::Context>) {
    }

    fn on_close_event(&mut self) -> bool {
        println!("onClose");
        return true;
    }

    fn update(&mut self, ctx: &egui::Context, frame: &mut eframe::Frame) {
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

                let play_button = Button::new(RichText::new("Play").size(30.0));
                ui.add(play_button);

                let quit_button = ui.add(Button::new(RichText::new("Quit").size(30.0)));

                if quit_button.clicked() {
                    frame.close();
                }
            });
        });
    }
}
