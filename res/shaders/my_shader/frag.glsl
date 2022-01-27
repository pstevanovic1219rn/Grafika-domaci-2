#version 330

in vec3 pass_color;
in vec2 pass_uv;
in vec3 pass_normal;
in vec3 pass_pos;

out vec4 final_color;

uniform sampler2D albedo;

uniform vec3 uni_light_color;
uniform vec3 uni_light_direction;
uniform vec3 uni_light_source;

void main()
{
	vec3 light_source_1 = vec3(-20.0, 1.0, 0.0);
	vec3 light_color_1 = vec3(1.0, 0.7, 0.4);
	
	vec3 light_source_2 = vec3(0.0, 1.0, -20.0);
	vec3 light_color_2 = vec3(0.4, 0.4, 0.4);
	
	vec3 light_source_3 = vec3(0.0, 1.0, 20.0);
	vec3 light_color_3 = vec3(0.7, 1.0, 0.4);
	
	vec3 light_source_4 = vec3(20.0, 1.0, 0.0);
	vec3 light_color_4 = vec3(0.4, 0.7, 1.0);

	float light_factor = clamp(dot(normalize(pass_normal), normalize(uni_light_direction)), 0, 1);
	
	light_factor = 1.0;
	
	float distance_from_camera = clamp(4.0 - distance(pass_pos, uni_light_source), 0, 1);
	float distance_from_1 = clamp(3.0 - distance(pass_pos, light_source_1)/5.0, 0, 1);
	float distance_from_2 = clamp(3.0 - distance(pass_pos, light_source_2)/5.0, 0, 1);
	float distance_from_3 = clamp(3.0 - distance(pass_pos, light_source_3)/5.0, 0, 1);
	float distance_from_4 = clamp(3.0 - distance(pass_pos, light_source_4)/5.0, 0, 1);
	
	vec4 albedo_color = texture(albedo, pass_uv);
	
	vec3 refined_color = albedo_color.xyz * (uni_light_color * light_factor) * (clamp(distance_from_camera + distance_from_1 * light_color_1 + distance_from_2 * light_color_2 + distance_from_3 * light_color_3 + distance_from_4 * light_color_4, 0, 1));
	
	final_color = vec4(refined_color, 1.0);
}