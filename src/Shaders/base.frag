#version 150
precision highp float;
void main(void) {
	// Pass through our original color with full opacity.
	gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
}