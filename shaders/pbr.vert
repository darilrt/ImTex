#version 400
layout (location = 1) in vec3 _normal;
layout (location = 2) in vec2 uv;

out vec2 textcoord;
out vec3 normal;
out vec3 worldPos;

out mat4 modelMatrix;

void main() {
	textcoord = uv.xy;
    worldPos = vec3(gl_ModelViewMatrix * gl_Vertex);
    normal = (gl_ModelViewMatrix * vec4(_normal, 1.0)).xyz;
    
    modelMatrix = gl_ModelViewMatrix;
    
	gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}
