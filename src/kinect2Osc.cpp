#include "kinect2Osc.h"

kinectTransmitter::kinectTransmitter(){
	//host = DEFAULT_HOST;
	//port = DEFAULT_PORT;
	//setup();
}

kinectTransmitter::~kinectTransmitter(){
	
}

kinectTransmitter::kinectTransmitter(ofxKinectCommonBridge *k, string h, int p){



}

void kinectTransmitter::setup( ofxKinectCommonBridge *k, string h, int p){
	kinect = k;
	setHost(h);
	setPort(p);
	setupJoints();
	setupGui();

	oscSend.setup(host,port);
	ofxOscMessage hello;
	hello.setAddress("/hiya");
	hello.addFloatArg(1.0);
	oscSend.sendMessage(hello);
	trackedSkeletons = DEFAULT_SKELETONS;

}

void kinectTransmitter::resetOsc(){
}

void kinectTransmitter::update(){

	
	if(kinect->isFrameNew()){
		skeletons = kinect->getSkeletons();
		
		for (int i=0;i<skeletons.size();i++){
			if(skeletons.size()>=i && skeletons[i].tracked){
				
				for(pair<JointType, Kv2Joint> j : skeletons[i].joints){ 
					int foundJoint = ofFind(trackedJoints,(int)j.first);
					//cout << foundJoint <<endl;
					if ( ofFind(trackedJoints,(int)j.first) != trackedJoints.size() || bSendAll){ //are we tracking this joint?
						
						ofxOscMessage message;
						
						int jointInt = j.first;
						string jointName = jointList[jointInt];
						
						message.setAddress("/kinect/"+ ofToString (i) + ofToString("/") + ofToString(jointName) + ofToString("/pos"));
						message.addFloatArg(j.second.getPosition().x);
						message.addFloatArg(j.second.getPosition().y);
						message.addFloatArg(j.second.getPosition().z);
						cout<< "/kinect/"+ ofToString (i) + ofToString("/") + ofToString(jointName) + ofToString("/pos ")<< j.second.getPosition().x  << endl;
							
						oscSend.sendMessage(message);
						
						message.clear();
						message.setAddress("/kinect/"+ ofToString (i) + ofToString("/") + ofToString(jointName) + ofToString("/ori"));
						message.addFloatArg(j.second.getOrientation().asVec3().x);
						message.addFloatArg(j.second.getOrientation().asVec3().y);
						message.addFloatArg(j.second.getOrientation().asVec3().z);

						oscSend.sendMessage(message);

				
					}else{

					}
					


				}
				
			}

		}

		if (bSendMask){
			kinect->getDepthTexture().readToPixels(maskPixels);
			
			ofxOscMessage message;
			message.setAddress("/kinect/mask");
			//message.addBlobArg( ofBuffer(maskPixels));
			oscSend.sendMessage(message);
		}

	}
}

void kinectTransmitter::trackJoint(int j){
	if(find(trackedJoints.begin(),trackedJoints.end(),j) != trackedJoints.end()){ //does trackedjoints contain this joint?
		cout<< "We're already tracking "<< j<< endl;
	} else{
		trackedJoints.push_back(j);
	}
}

void kinectTransmitter::forgetJoint(int j){
	if(find(trackedJoints.begin(),trackedJoints.end(),j) != trackedJoints.end()){ //does trackedjoints contain this joint?
		trackedJoints.erase(find(trackedJoints.begin(),trackedJoints.end(),j));
	} else{
		
		cout<<"We're not tracking "<< j<< endl;;
	}
}

void kinectTransmitter::setupJoints(){
	string jointArray[] = {
		"JointType_SpineBase",
        "JointType_SpineMid",
        "JointType_Neck",
        "JointType_Head",
        "JointType_ShoulderLeft",
        "JointType_ElbowLeft",
        "JointType_WristLeft",
        "JointType_HandLeft",
        "JointType_ShoulderRight",
        "JointType_ElbowRight",
        "JointType_WristRight",
        "JointType_HandRight",
        "JointType_HipLeft",
        "JointType_KneeLeft",
        "JointType_AnkleLeft"	,
        "JointType_FootLeft"	,
        "JointType_HipRight"	,
        "JointType_KneeRight"	,
        "JointType_AnkleRight"	,
        "JointType_FootRight"	,
        "JointType_SpineShoulder",
        "JointType_HandTipLeft"	,
        "JointType_ThumbLeft"	,
        "JointType_HandTipRight",
        "JointType_ThumbRight"	,
	
	
	};
	jointList.assign(begin(jointArray),end(jointArray));

}

void kinectTransmitter::setupGui(){
	gui.setup();
	trackSimpleHands.addListener(this, &kinectTransmitter::toggleSimpleHands);
	trackDetailedHands.addListener(this, &kinectTransmitter::toggleDetailedHands);
	trackHead.addListener(this, &kinectTransmitter::toggleHead);
	trackFeet.addListener(this, &kinectTransmitter::toggleFeet);
	sendMask.addListener(this, &kinectTransmitter::toggleMask);
	sendAll.addListener(this, &kinectTransmitter::toggleAll);
	gui.add(hostLabel.setup("Host: "+ host));
	gui.add(portLabel.setup("Port: "+ ofToString(port)));
	gui.add(sendAll.setup("Send All Joints", false));
	gui.add(trackSimpleHands.setup("Track Simple Hands", false));
	gui.add(trackDetailedHands.setup("Track Detailed Hands", false));
	gui.add(trackHead.setup("Track Head", false));
	gui.add(trackFeet.setup("Track Feet", false));
	gui.add(sendMask.setup("Send Mask", false));
	/*
	for(int i = 0; i< jointList.size();i++){
		ofxToggle toggle;
		toggle.setName(jointList[i]);
		toggle.addListener(this, &kinectTransmitter::toggleJoint);	
		jointToggles.push_back(toggle);	
	}
	for(int i = 0; i< jointToggles.size();i++){
		gui.add(jointToggles[i].setup(jointList[i],false));
	}
	*/
}

void kinectTransmitter::drawGui(){
	gui.draw();
}

void kinectTransmitter::toggleSimpleHands(bool& clicked){
	

	if(clicked){	
		trackDetailedHands = false;
		sendAll = false;
		trackJoint((JointType)JointType_HandLeft);
		trackJoint((JointType)JointType_HandRight);

	}else{
		forgetJoint((JointType)JointType_HandLeft);
		forgetJoint((JointType)JointType_HandRight);
	}
	
}

void kinectTransmitter::toggleDetailedHands(bool& clicked){
	

	if(clicked){	
		trackSimpleHands = false;
		sendAll = false;
		trackJoint((JointType)JointType_HandTipLeft);
		trackJoint((JointType)JointType_HandTipRight);
		trackJoint((JointType)JointType_HandRight);
		trackJoint((JointType)JointType_HandLeft);
		trackJoint((JointType)JointType_WristRight);
		trackJoint((JointType)JointType_WristLeft);
		trackJoint((JointType)JointType_ThumbRight);
		trackJoint((JointType)JointType_ThumbLeft);
	}else{
		forgetJoint((JointType)JointType_HandTipLeft);
		forgetJoint((JointType)JointType_HandTipRight);
		forgetJoint((JointType)JointType_HandRight);
		forgetJoint((JointType)JointType_HandLeft);
		forgetJoint((JointType)JointType_WristRight);
		forgetJoint((JointType)JointType_WristLeft);
		forgetJoint((JointType)JointType_ThumbRight);
		forgetJoint((JointType)JointType_ThumbLeft);
	}
}


void kinectTransmitter::toggleFeet(bool& clicked){
	
	if(clicked){	
		sendAll = false;
		trackJoint((JointType)JointType_FootRight);
		trackJoint((JointType)JointType_FootLeft);
	}else{
		forgetJoint((JointType)JointType_FootRight);
		forgetJoint((JointType)JointType_FootLeft);
	}
}

void kinectTransmitter::toggleHead(bool& clicked){
	
	if(clicked){	
		sendAll = false;
		trackJoint((JointType)JointType_Head);
	}else{
		forgetJoint((JointType)JointType_Head);
	}
}

void kinectTransmitter::toggleMask(bool& clicked){
	
	bSendMask = clicked;
}

void kinectTransmitter::toggleAll(bool& clicked){
	bSendAll = clicked;
	if(clicked){
		trackSimpleHands = false;
		trackDetailedHands = false;
		trackHead = false;
		trackFeet = false;
	}

}

/*
void kinectTransmitter::toggleJoint(const void * sender, bool & pressed){
	ofxToggle * toggle = (ofxToggle *) sender;
	string name = toggle->getName();
	
	int jointInt = ofFind(jointList, name) ;

	cout<<name<<" "<<jointInt<< endl;
	if(jointInt != jointList.size()){
		if(pressed){
			trackJoint(jointInt);
		} else if (!pressed){
			forgetJoint(jointInt);
		}
	}
		//trackedJoints.push_back(jointInt);
}*/