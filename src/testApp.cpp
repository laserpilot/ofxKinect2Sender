#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){


	kinect.initSensor();
	//kinect.initColorStream(true);
	kinect.initIRStream();
	kinect.initDepthStream(true);
	kinect.initBodyIndexStream();
	kinect.initSkeletonStream(true);
	kinect.setDepthClipping(0, 10000);
	//simple start
	kinect.start();
	ofDisableAlphaBlending(); //Kinect alpha channel is default 0;
	mShader.load("shaders/chromadepth");

	mImg.allocate(512, 424, OF_IMAGE_GRAYSCALE);
	mRawImg.allocate(512, 424, OF_IMAGE_GRAYSCALE);

	//sender.setup("172.16.1.216", 1234);

	k2Osc.setup(&kinect, "172.16.1.135", 1235);
}

//--------------------------------------------------------------
void testApp::update(){
	kinect.update();
	if(kinect.isFrameNew()){
		//mImg.setFromPixels(kinect.getDepthPixelsRef());
		mRawImg.setFromPixels(kinect.getRawDepthPixelsRef());
	//	//mImg.update();
	//	//mRawImg.update();
	//	
	//	unsigned short * foo = mRawImg.getPixels();
	//	float foobar = 0;
	//	
	//	pointCloud.clear();

	//	pointCloud.enableColors();
	//	pointCloud.enableIndices();

	//	//if (r.size() > 0) {
	//	pointCloud.clear();
	//	for (int y = 0; y < 424; y++) {
	//		for(int x = 0; x < 512; x++){
	//			int i = y * 512 + x;
	//			//                    if (r[i] > 0.0) {
	//			pointCloud.addVertex(ofVec3f(x,y, ofMap(foo[i], 0, 65535, 0, 8000, true)));
	//			//                    ofFloatColor vertexCol;
	//			//                    vertexCol.setHsb((r[i]-0.5) * 2, 1.0, 0.7);
	//			pointCloud.addColor(ofFloatColor(ofMap(foo[i], 0, 65535, 0, 8000, true)));
	//			//                    }
	//		}
	//	}
	//	//
	}
	k2Osc.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofBackground(0, 255);


	mRawImg.draw(0, 0);
	mImg.draw(512, 0);

	//kinect v2 outputs depth/ir resolution of 512x424
	//	kinect.drawDepth(0, ofGetHeight() - 424);
		kinect.drawBodyIndex(512, ofGetHeight() - 424);

		ofPushMatrix();
		{
		ofTranslate(ofGetWidth()/2, ofGetHeight()/3);
		kinect.drawAllSkeletons(ofVec2f(320,240));
		}
		ofPopMatrix();
	//glEnable(GL_DEPTH_TEST);
	//cam.begin();
	////mShader.begin();
	////mShader.setUniform1f("near", 0);
	////mShader.setUniform1f("far", 1000);
	////mShader.setUniformTexture("diffusemap", kinect.getDepthTexture(), 0);
	//pointCloud.drawVertices();
	////mShader.end();
	//cam.end();
	//glDisable(GL_DEPTH_TEST);

		k2Osc.drawGui();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

	k2Osc.trackJoint(key-48);


	ofxOscMessage m;
	m.setAddress("/hey");
	m.addFloatArg(30.0);
	//sender.sendMessage(m);

	//kinect.getSkeletons[0].
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}