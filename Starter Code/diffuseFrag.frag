#version 150 

//these are the interpolated values out of the rasterizer, so you can't know
//their specific values without knowing the vertices that contributed to them
in vec3 fs_normal;
in vec3 fs_view;
in vec3 fs_light_vector;
in vec3 fs_color;
in vec3 fs_light_color;
in vec3 fs_ambient;

out vec4 out_Color;

void main() {
    //base colors for materials
    vec4 surfaceColor = vec4(fs_color, 1.0);
	vec4 incidentColor = vec4(fs_light_color, 1.0);
	vec4 ambientColor = vec4(fs_ambient, 1.0f);
    
	vec3 N = normalize(fs_normal);
	vec3 L = normalize(fs_light_vector);
	vec3 V = normalize(fs_view);
	vec3 R = reflect(-1.0f * L, N);

    //calculate diffuse term and clamp to the range [0, 1]
    float diffuseTerm = clamp(dot(N, L), 0.0, 1.0);
    
	//ambient term
	float ambientTerm = 1.0f;

	//specular term
	float specularTerm = clamp(pow(dot(R, V), 50.0f), 0.0f, 1.0f);

	//used for color
	vec4 dottedD = clamp(surfaceColor * incidentColor, 0.0f, 1.0f);

	//ambient color
	vec4 dottedA = clamp(surfaceColor * ambientColor, 0.0f, 1.0f);

    out_Color = (clamp(dottedD * diffuseTerm, 0.0f, 1.0f) * 0.5) + (clamp(incidentColor * specularTerm, 0.0f, 1.0f) * 0.3) + (clamp (dottedA * ambientTerm, 0.0f, 1.0f) * 0.2);
}