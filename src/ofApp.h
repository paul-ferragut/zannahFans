#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxSvg.h"
#include "ofxSunCalc.h"
#include "ofxGui.h"

#define MOTOR_NUMBERS 15

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofShader shader;
		ofxPanel gui;
		ofxFloatSlider motorSpeed[MOTOR_NUMBERS];
		float motorCurrentAngle[MOTOR_NUMBERS];
		
		ofxToggle useTexture;
		ofxToggle showTexture;
		ofxToggle useBrightness;
		ofxToggle showSunCalc;

		ofxFloatSlider sunBrightness;


		ofxFloatSlider windSpeed;
		ofxAssimpModelLoader model;

		ofxSVG svg;

		ofEasyCam cam;

		ofFbo fbo;

		ofImage tempImage;

		void drawWeatherDebug();
		ofxSunCalc sun_calc;
		SunCalcDayInfo todayInfo;
		string min_info_str;
		string max_info_str;
		string pos_str;
		string date_str;
		string latlon_str;
		vector<ofFbo> timelines;
		vector<string> labels;
		float lat;
		float lon;

		
};
