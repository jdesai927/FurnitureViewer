#version 150

//there's no rules about the prefixes but they do help
//keep track of things
uniform mat4 u_modelMatrix;
uniform mat4 u_projMatrix;
uniform vec3 u_lightPos;

in vec4 vs_normal;
in vec4 vs_position;
in vec3 vs_color;

out vec3 fs_normal;
out vec3 fs_light_vector;
out vec3 fs_color;

void main() {
    
    // Set the color and normal as just the input from the VBOs,
	// they only get interesting after they've been interpolated by the rasterizer
    fs_color = vs_color;
    fs_normal = (transpose(inverse(u_modelMatrix)) * vs_normal).xyz;
    
    // Set up our vector for the light
    fs_light_vector = vec3(u_lightPos - (u_modelMatrix * vs_position).xyz);

	//fs_color = fs_light_vector;

	//built-in things to pass down the pipeline
    gl_Position = u_projMatrix * u_modelMatrix * vs_position;
}