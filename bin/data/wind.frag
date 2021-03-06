#version 120




//precision highp float;

uniform float time;
uniform float wind;
uniform vec2 resolution;

//#define time iGlobalTime
//#define resolution iResolution

#define MAX_ITER 20

float remap(float value, float l1, float h1, float l2, float h2) {
  return l2 + (value - l1) * (h2 - l2) / (h1 - l1);
}

float mapRed(float r) {
//  r = remap(r, 0.0, 1.0, 0.0, 1.0);
  return r;
}

float mapGreen(float g) {
//  g = remap(g, 0.0, 1.0, 0.0, 1.0);
  return g;
}

float mapBlue(float b) {
 //b = remap(b, 0.0, 1.0, 0.0, 0.9);
  return b;
}

void main() {
	vec2 p = (gl_FragCoord.xy / resolution * 4.0) - vec2(-10.0, 0.0);
	vec2 i = p;
	float scaleTime = time * 0.005 + 1000.0;

	float c = 1.0;
	float inten = .05;

	for (int n = 0; n < MAX_ITER; n++){
		float t = -scaleTime * (1.5 - (10.0 / float(n+1)));
    //i[1] = i[1] + (100.0 * sin(scaleTime * 0.1 + 1000.0));
    i[1] = i[1] + scaleTime * 10.0;
		i = p + vec2(
      cos(t - i.x) + cos(t - i.y),
      sin(t - i.y) + cos(t + i.x)
    );
		c += 1.0/length(vec2(p.x / (cos(i.y + t)/inten)));
	}
	c /= float(MAX_ITER);
	//c = 1.2-sqrt(pow(c,3.0));
	float col = c*c*c*c*c;
	gl_FragColor = vec4(mapRed(col)*wind, mapGreen(col)*wind, mapBlue(col)*wind, 1.0);
}