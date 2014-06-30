#include "ofMain.h"
#include "ofxKinectCommonBridge.h"
#include "ofxOsc.h"
#include "ofxGui.h"


#define DEFAULT_HOST "172.16.1.135"
#define DEFAULT_PORT 8000
#define DEFAULT_SKELETONS 1

/*
OSC Format
/kinect/skeleton#/jointdesc/(position/orientation) value

Example:
/kinect/0/left_arm/pos -0.232424 0.9393 0.003

*/

class kinectTransmitter {
	
public:
	kinectTransmitter();
	~kinectTransmitter();
	kinectTransmitter(ofxKinectCommonBridge *k, string h, int p);
	//inline void setKinect(ofxKinectCommonBridge k){kinect=k;};
	inline void setHost(string h){host=h; resetOsc();};
	inline void setPort(int p){port=p; resetOsc();};

	void setup(ofxKinectCommonBridge *k, string h, int p);
	void update();
	void drawGui();

	inline void trackSkeletons(int i){trackedSkeletons=i;};
	void trackJoint(int j);
	void forgetJoint(int j);



	
private:
	ofxKinectCommonBridge *kinect;
	string host;
	int port;

	ofxOscSender oscSend;
	ofxPanel gui;

	void resetOsc();

	vector<Kv2Skeleton> skeletons;
	
	int trackedSkeletons; //How many skeletons to track?
	vector<int> trackedJoints;

	vector<string> jointList;
	void setupJoints();

	void setupGui();

	ofBuffer maskBuffer;
	ofPixels maskPixels;
	bool bSendMask;

	bool bSendAll;

	ofxToggle trackSimpleHands, trackDetailedHands, trackHead, trackFeet, sendMask, sendAll;
	ofxLabel hostLabel, portLabel;


	void toggleSimpleHands( bool &);
	void toggleDetailedHands( bool &);
	void toggleHead( bool &);
	void toggleFeet( bool &);
	void toggleMask(bool &);
	void toggleAll(bool &);
};
