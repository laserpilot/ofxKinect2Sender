#pragma once

#include "ofMain.h"
#include "ofxKinectCommonBridge.h"
#include "ofxGui.h"
#include "ofxOSC.h"
#include "kinect2Osc.h"


class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofEasyCam cam;
		ofMesh pointCloud;
		ofShader mShader;
		ofxKinectCommonBridge kinect;
		ofShader yuvRGBShader;
		ofPlanePrimitive plane;
		ofFloatImage mImg;
		ofShortImage mRawImg;

		//ofxOscSender sender;
		kinectTransmitter k2Osc;
};
