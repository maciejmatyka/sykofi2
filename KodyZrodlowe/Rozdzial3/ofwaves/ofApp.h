/**
 *
 * OFDevCon Example Code Sprint
 * 
 * This example shows building a mesh, texturing it with a webcam, and extruding the vertices based on the pixel brightness
 * Moving the mouse also rotates the mesh to see it at different angles
 *
 * Created by Tim Gfrerer and James George for openFrameworks workshop at Waves Festival Vienna sponsored by Lichterloh and Pratersauna
 * Adapted during ofDevCon on 2/23/2012
 */

#pragma once

#include "ofMain.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void keyPressed  (int key);
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
	void clearButtonPressed();

	// Waves
	float ofApp::itoscreen(float i);
	float ofApp::countE(int i, int j);

	
	ofCamera cam; // add mouse controls for camera movement
	float extrusionAmount;
	ofVboMesh mainMesh;

	// computational mesh

	#define NX 512// 256
	#define NY 512 //256

	double U[NX + 1][NY + 1];
	double E[NX + 1][NY + 1];
	int Flag[NX + 1][NY + 1];

	// numerical constants

	//const float DT = 0.045;    // time dt
	const float DX = 0.1;       // spatial dx
	const float EPS = 0.333;    // laplace operator
	const int C_SRC = 2;        // pulsating source cell flag
	const int C_BND = 1;        // bundary cell flag


	bool gHide=0;
	ofParameter<float> DT;
	ofParameter<float> rotateAmount;
	// ofMap(ofGetMouseY(), 0, ofGetHeight(), 20.01, 160);
	ofParameter<float> rotateAmountX;// ofMap(ofGetMouseX(), 0, ofGetWidth(), -88, 88);
	ofParameter<float> Zoom;
	ofParameter<bool> sources;
	ofParameter<float> FREQ;    // pulsating source frequency
	ofParameter<float> AMP;    // pulsating source frequency
	ofParameter<float> VEL;
	ofxButton Clear;
									 
									 /*ofParameter<ofColor> color;
	ofParameter<ofVec2f> center;
	ofParameter<int> circleResolution;
	ofParameter<bool> filled;
	ofxButton twoCircles;
	ofxButton ringButton;
	ofParameter<string> screenSize;*/


	ofParameter<float> radius;
	ofParameter<ofColor> color;
	ofParameter<ofVec2f> center;
	ofParameter<int> circleResolution;
	ofParameter<bool> filled;
	ofxButton twoCircles;
	ofxButton ringButton;
	ofParameter<string> screenSize;

	// physical timing

	double t = 0; 			// actual time in idle loop
	double realdt; 			// time elapsed in idle loop

	ofxPanel gui;

};
