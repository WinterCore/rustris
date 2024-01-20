
use eframe::{egui, egui_glow::{self, check_for_gl_error}, glow::{self, HasContext}};

use egui::mutex::Mutex;
use std::{sync::Arc, mem};


#[derive(Debug)]
pub struct PlayerBoard {
    program: glow::Program,
    vertex_array: glow::VertexArray,
    angle: f32,
}

impl PlayerBoard {
    pub fn new(gl: &glow::Context) -> Self {
        use glow::HasContext as _;

        let shader_version = if cfg!(target_arch = "wasm32") {
            "#version 300 es"
        } else {
            "#version 330"
        };

        
        unsafe {
            let program = gl.create_program().expect("Cannot create program");

            let (vertex_shader_source, fragment_shader_source) = (
                r#"
                    layout(location = 0) in vec2 vertex;

                    const vec2 verts[3] = vec2[3](
                        vec2(0.0, 1.0),
                        vec2(-1.0, -1.0),
                        vec2(1.0, -1.0)
                    );
                    const vec4 colors[3] = vec4[3](
                        vec4(1.0, 0.0, 0.0, 1.0),
                        vec4(0.0, 1.0, 0.0, 1.0),
                        vec4(0.0, 0.0, 1.0, 1.0)
                    );
                    out vec4 v_color;
                    uniform float u_angle;
                    void main() {
                        v_color = colors[gl_VertexID];
                        gl_Position = vec4(vertex, 0.0, 1.0);
                        gl_Position.x *= sin(u_angle);
                        gl_Position.y *= cos(u_angle);
                    }
                "#,
                r#"
                    precision mediump float;
                    in vec4 v_color;
                    out vec4 out_color;

                    void main() {
                        out_color = v_color;
                    }
                "#,
            );

            let shader_sources = [
                (glow::VERTEX_SHADER, vertex_shader_source),
                (glow::FRAGMENT_SHADER, fragment_shader_source),
            ];

            let shaders: Vec<_> = shader_sources
                .iter()
                .map(|(shader_type, shader_source)| {
                    let shader = gl
                        .create_shader(*shader_type)
                        .expect("Cannot create shader");
                    gl.shader_source(shader, &format!("{shader_version}\n{shader_source}"));
                    gl.compile_shader(shader);
                    assert!(
                        gl.get_shader_compile_status(shader),
                        "Failed to compile {shader_type}: {}",
                        gl.get_shader_info_log(shader)
                    );
                    gl.attach_shader(program, shader);
                    shader
                })
                .collect();

            gl.link_program(program);
            assert!(
                gl.get_program_link_status(program),
                "{}",
                gl.get_program_info_log(program)
            );

            for shader in shaders {
                gl.detach_shader(program, shader);
                gl.delete_shader(shader);
            }

            let vertex_array = gl
                .create_vertex_array()
                .expect("Cannot create vertex array");

            let triangle_vertices = [
                -0.0f32, 1.0f32,
                -1.0f32, -1.0f32,
                1.0f32, -1.0f32,
            ];
            let triangle_vertices_u8: &[u8] = core::slice::from_raw_parts(
                triangle_vertices.as_ptr() as *const u8,
                triangle_vertices.len() * core::mem::size_of::<f32>(),
            );

            let vbo = gl.create_buffer().unwrap();

            gl.bind_vertex_array(Some(vertex_array));
            gl.bind_buffer(glow::ARRAY_BUFFER, Some(vbo));

            gl.buffer_data_u8_slice(glow::ARRAY_BUFFER, triangle_vertices_u8, glow::STATIC_DRAW);
            println!("Buffer triangle vertices {:?}", triangle_vertices_u8);
            gl.vertex_attrib_pointer_f32(0, 2, glow::FLOAT, false, mem::size_of::<f32>() as i32 * 2, 0);
            // [0, 0, 0, 63, 0, 0, 128, 63, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 63, 0, 0, 0, 0]
            
            check_for_gl_error!(gl, "vertex_attrib_pointer_f32");
            gl.enable_vertex_attrib_array(0);
            gl.bind_buffer(glow::ARRAY_BUFFER, None);
            gl.bind_vertex_array(None);

            Self {
                program,
                vertex_array,
                angle: 0f32,
            }
        }
    }
    

    pub fn destroy(&self, gl: &glow::Context) {
        use glow::HasContext as _;
        unsafe {
            gl.delete_program(self.program);
            gl.delete_vertex_array(self.vertex_array);
        }
    }

    pub fn paint(&mut self, gl: &glow::Context) {
        unsafe {

            gl.clear_color(0.2f32, 0.3f32, 0.3f32, 1.0f32);
            gl.clear(glow::COLOR_BUFFER_BIT);

            gl.use_program(Some(self.program));
            gl.bind_vertex_array(Some(self.vertex_array));

            gl.uniform_1_f32(
                gl.get_uniform_location(self.program, "u_angle").as_ref(),
                self.angle,
            );

            self.angle += 0.01f32;


            gl.draw_arrays(glow::TRIANGLES, 0, 3);
    
            // gl.bind_vertex_array(Some(vao));
        }
    }
}
