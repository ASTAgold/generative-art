#ifdef GL_ES
precision highp float;
#endif

uniform float u_time;
uniform vec2 u_resolution;
uniform vec2 u_mouse;

uniform sampler2D u_current;
uniform vec2 u_currentSize;

void main() {
    // vec2 mouseCoord = u_mouse/u_resolution;
    vec2 screenCoord = gl_FragCoord.xy/u_resolution;

	vec2 distVec = (u_mouse - gl_FragCoord.xy)/u_resolution;
	float distSquared = distVec.x * distVec.x + distVec.y * distVec.y;

	vec3 color = texture2D(u_current, screenCoord).rgb * vec3(sqrt(distSquared));

	gl_FragColor = vec4(color, 1.0);
}