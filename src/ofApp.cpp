#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){


	//serial.setup(settingsXML.getValue("microPort", "COM8"), settingsXML.getValue("microPort", 115200));
	serial.setup("COM3", 115200);

	gui.setup();

	gui.add(windSpeed.setup("wind", 0, 0, 255));
	gui.add(useTexture.setup("use texture", true));
	gui.add(showTexture.setup("show texture", false));
	gui.add(useBrightness.setup("use brightness", true));
	gui.add(showSunCalc.setup("show sun calculation", false));

	gui.add(sunBrightness.setup("sun Brightness" , 0, 0, 255));


	for (int i = 0;i < MOTOR_NUMBERS;i++) {
	gui.add(motorSpeed[i].setup("motor" + ofToString(i), 100, 0, 255));
	}
	
	fbo.allocate(1000, 1000,GL_RGBA);
	tempImage.allocate(1000, 1000, OF_IMAGE_COLOR_ALPHA);

    model.loadModel("pinwheel.obj");

	shader.load("wind");

	svg.load("fandesign.svg");
	

	for (int i = 0; i < MOTOR_NUMBERS; i++) {

		motorCurrentAngle[i] = 0;

	}

	//SUN CALCULATION
	Poco::LocalDateTime now;
	date_str = Poco::DateTimeFormatter::format(now, "%Y-%m-%d %H:%M:%S");
	lat = 51.547492; // Note southern degrees need to be - (not like those from google maps)
	lon = -0.0107809; //
	latlon_str = "lat:" + ofToString(lat) + ", lon:" + ofToString(lon);
	SunCalcPosition sunpos = sun_calc.getSunPosition(now, lat, lon);
	pos_str = "altitude=" + ofToString(sunpos.altitude) + ", azimuth=" + ofToString(sunpos.azimuth * RAD_TO_DEG);
	todayInfo = sun_calc.getDayInfo(now, lat, lon, true);
	min_info_str = sun_calc.infoToString(todayInfo, true);
	max_info_str = sun_calc.infoToString(todayInfo, false);
	ofLogNotice() << min_info_str << endl << endl << max_info_str;
	labels.push_back("Today\n" + ofxSunCalc::dateToDateString(now));
	vector<SunCalcDayInfo> sun_infos;
	sun_infos.push_back(todayInfo);
	for (int i = 0; i<1; i++) {
		timelines.push_back(ofFbo());
		timelines[i].allocate(ofGetWidth() - 20 - 110, 32,GL_RGBA);
		ofxSunCalc::drawSimpleDayInfoTimeline(timelines[i], sun_infos[i]);
	}

}

//--------------------------------------------------------------
void ofApp::update(){

	shader.begin();
	shader.setUniform2f("resolution", 1000, 1000);
	float windSpeedMapped = ofMap((float)windSpeed, 0, 255, 0, 1);
	shader.setUniform1f("time", ofGetElapsedTimeMillis()*0.001 *(1.0+ (windSpeedMapped*12)));
	shader.setUniform1f("wind", (float)windSpeedMapped);
	shader.end();

	if (ofGetFrameNum() % 4 == 0) {
		if (serial.isInitialized()) {
			//cout << "6" << endl;

			vector<long>tMotorVal;

			//TODO Replace 2 with MOTOR_NUM
			for (int i = 0;i < 2;i++) {
				tMotorVal.push_back((long)motorSpeed[i]);
			}

			string serialRead = getSerialString(serial, '\n');

			cout << "serial read" << serialRead<< endl;

			if (serialRead == "end") {
			//	serial.flush();
			//	sendArduino(serial, tMotorVal);
			}
			else if (serialRead == ""|| serialRead == "\n" || serialRead == " " ) {
			//	sendArduino(serial, tMotorVal);
			}
			else {
			//	cout << "wind speed" << endl;
		if(ofToInt(serialRead)!=0)
				windSpeed = ofMap(ofToInt(serialRead), 78, 130, 0, 255, true);
		serial.flush();
		sendArduino(serial, tMotorVal);

			}
			//	cout << "send" << endl;
		}
		else {
			//	serialMega.flush();
		}
		//cout << "7" << endl;
	}

}

//--------------------------------------------------------------
void ofApp::draw(){

	ofClear(255);
	ofEnableAlphaBlending();
	ofBackgroundGradient( ofColor(sunBrightness, sunBrightness, sunBrightness),ofColor(0, 0, 0));
	ofSetColor(255, 255, 255);

	drawWeatherDebug();

	fbo.begin();
	shader.begin();
	ofDrawRectangle(0, 0, 1000, 1000);
	shader.end();
	fbo.end();
	ofPixels pixels;
	fbo.readToPixels(pixels);
	tempImage.setFromPixels(pixels, 1000, 1000, OF_IMAGE_COLOR_ALPHA);

	for (int i = 0; i < MOTOR_NUMBERS; i++) {
		motorCurrentAngle[i] += ofMap(motorSpeed[i], 0.0, 255.0, 0.0, 10.0, true);
	}


	cam.begin();

	if (useTexture && showTexture) {
		fbo.draw(0, 0);
	}

	ofLight light;
	ofMaterial material;
	ofEnableLighting();
	light.enable();
	light.setAmbientColor(ofColor(255, 255, 255, 255));

	

	light.setPosition(500, 500, ofMap(sunBrightness,0,255,100,-500));
	material.begin();
	ofSetColor(255, 255, 255);
	for (int i = 0;i < svg.getNumPath();i++) {
		ofPath p= svg.getPathAt(i);
		vector<ofPolyline>poly=p.getOutline();
		for (int j = 0; j<poly.size() ; j++) {
			vector<ofPoint>pts= poly[j].getVertices();
			for (int k = 0; k < pts.size(); k++) {
				ofDrawCircle(pts[k], 10);

				ofPushMatrix();
			

				ofTranslate(pts[k]);
			
				ofTranslate(model.getMesh(0).getCentroid() / 2);
				ofRotate(90, 0, 1, 0);
				ofTranslate(-model.getMesh(0).getCentroid() / 2);

				//cout << "pts" << pts.size() << endl;
				if (pts.size() == MOTOR_NUMBERS) {
					if (useTexture)
					{
						ofColor col =	tempImage.getColor(pts[k].x, pts[k].y);
						motorSpeed[k] = col.getBrightness();
					}

					ofTranslate(model.getMesh(0).getCentroid() / 2);
					ofRotate(motorCurrentAngle[k], 1, 0, 0);
					ofTranslate(-model.getMesh(0).getCentroid() / 2);
				}

				ofScale(0.3, 0.3, 0.3);
				ofSetColor(255,255,255,255);
				model.drawFaces();
				//ofSetColor(0, 0, 0, 255);
				//model.drawWireframe();
				
				
				ofPopMatrix();
			}
		}

	
	}
	material.end();
	ofDisableLighting();
	light.disable();


	cam.end();


	gui.draw();
}

//--------------------------------------------------------------
//--------------------------------------------------------------
void ofApp::drawWeatherDebug() {

	Poco::LocalDateTime now; //(2015,5,29,17,38);

	if (ofGetKeyPressed(OF_KEY_ALT)) {
		// auto step the time of day to proof changes
		int total_min = fabs(sin(ofGetElapsedTimef()*.05)) * 1440; // 1440 == mins per day  60 * 24
		int hr = floor(total_min / 60.0);
		int mn = total_min - (hr * 60); //now.minute();
		now.assign(now.year(), now.month(), now.day(), hr, mn);
	}

	float sun_brightness = ofxSunCalc::getSunBrightness(todayInfo, now);

	if (ofGetKeyPressed(OF_KEY_COMMAND)) {
		sun_brightness = fabs(sin(ofGetElapsedTimef()*.1));
	}

	// draw background gradient based on sun_brightness

	ofColor nightBG(ofColor::black);
	ofColor nightFG(64);

	ofColor dayBG(ofColor::skyBlue);
	ofColor dayFG(ofColor::paleGoldenRod);

	ofColor background = nightBG.lerp(dayBG, sun_brightness);
	ofColor foreground = nightFG.lerp(dayFG, sun_brightness);

	//	ofBackgroundGradient(foreground, background);
	if (showSunCalc) {
		/*
		ofDrawBitmapStringHighlight(date_str, 15, 20, ofColor::paleGoldenRod, ofColor::black);

		ofDrawBitmapStringHighlight(min_info_str, 15, 45, ofColor::salmon, ofColor::white);

		ofDrawBitmapStringHighlight(max_info_str, 15, 125, ofColor::darkorange, ofColor::white);

		ofDrawBitmapStringHighlight(latlon_str, 180, 20, ofColor::gold, ofColor::black);

		ofDrawBitmapStringHighlight(pos_str, 180, 45, ofColor::cornsilk, ofColor::black);
		*/
		ofDrawBitmapStringHighlight("Current Brightness in London" + ofToString(sun_brightness, 3), 20, ofGetHeight()-160, ofColor::goldenRod, ofColor::white);

		float tx = 10 + 110;
		float ty = ofGetHeight() - 140;
		for (int i = 0; i<timelines.size(); i++) {

			ofSetColor(255);
			timelines[i].draw(tx, ty);

			ofDrawBitmapStringHighlight(labels[i], 10, ty + 13);

			if (i == 0) { // today
				ofNoFill();
				ofSetLineWidth(1.0);
				ofSetColor(255);
				ofRect(tx, ty, timelines[i].getWidth(), timelines[i].getHeight());

				// Draw a current time mark
				float pixels_per_min = (timelines[i].getWidth() / 24) / 60.0;
				float nx = tx + pixels_per_min * (now.hour() * 60 + now.minute());
				ofSetColor(255, 0, 0);
				ofSetLineWidth(3.0);
			//	cout << "now" << nx << endl;
				ofLine(nx, ty, nx, ty + timelines[i].getHeight());
			}

			ty += timelines[i].getHeight() + 25;
		}
	
		ofFill();
		ofSetColor(255, 255, 255);
	}
	if (useBrightness) {
		sunBrightness = ofMap(ofxSunCalc::getSunBrightness(todayInfo, now),0,1,0,255);
	}
	
}
//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}



void ofApp::sendArduino(ofSerial &serial, vector<long>val) {


	string endOfLineCharacter = "n";
	//string send = "";

	string stringVal = "";
	for (int i = 0;i < val.size();i++) {
		stringVal += ofToString(val[i]) + ",";;
	}


	stringVal = stringVal.substr(0, stringVal.size() - 1);//delete the last char

	stringVal += endOfLineCharacter;

	cout << "sent to arduino buffer: " << stringVal << endl;
	char buffer[1024];
	strncpy(buffer, stringVal.c_str(), sizeof(buffer));
	buffer[sizeof(buffer) - 1] = 0;

	serial.writeBytes((unsigned char*)&buffer[0], stringVal.size());

}




string ofApp::getSerialString(ofSerial &serial, char until) {

	if (serial.isInitialized() && serial.available()) {
		static string str;
		stringstream ss;
		char ch;
		int ttl = 1000;

		while ((ch = serial.readByte())>0 && ttl-->0 && ch != until) {
			ss << ch;
		}

		str += ss.str();
		if (ch == until) {
			string tmp = str;
			str = "";
			return ofxTrimString(tmp);
		}
		else {
			return "";
		}
	}
	else {
		return "";
	}
}

// trim trailing spaces
string ofApp::ofxTrimStringRight(string str) {
	size_t endpos = str.find_last_not_of(" \t\r\n");
	return (string::npos != endpos) ? str.substr(0, endpos + 1) : str;
}

// trim trailing spaces
string ofApp::ofxTrimStringLeft(string str) {
	size_t startpos = str.find_first_not_of(" \t\r\n");
	return (string::npos != startpos) ? str.substr(startpos) : str;
}

string ofApp::ofxTrimString(string str) {
	return ofxTrimStringLeft(ofxTrimStringRight(str));;
}

