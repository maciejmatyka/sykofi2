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


#include "ofApp.h"


float ofApp::itoscreen(float i)
{
	return ((i/(float)NX));
}

float ofApp::countE(int i,int j)
{
	const float AMPL = 13.0;
	return E[i][j] * AMPL ;
}

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	Clear.addListener(this, &ofApp::clearButtonPressed);
	gui.setup("GUI"); // most of the time you don't need a name but don't forget to call setup
	gui.add(DT.set("DT", 0.045, 0.01, 0.085));
	gui.add(rotateAmount.set("Rot X", 28.1, 18, 162));
	gui.add(rotateAmountX.set("Rot Y", 0, -90, 90));
	gui.add(Zoom.set("Zoom", 0, -400, 1000));
	gui.add(sources.set("Sources", true));
	//gui.setBackgroundColor(ofColor::white);
	gui.add(FREQ.set("FREQ", 0.5, 0.001, 4.5));;    // pulsating source frequency
	gui.add(AMP.set("AMP", 0.1, 0.01, 5.0));;    // pulsating source frequency
	gui.add(VEL.set("VEL", 1.0, 0.01, 5.0));;
	gui.add(Clear.setup("Clear"));

	

	//gui.set

	/*gui.add(filled.set("bFill", true));
	gui.add(radius.set("radius", 140, 10, 300));
	gui.add(center.set("center", ofVec2f(ofGetWidth() * .5, ofGetHeight() * .5), ofVec2f(0, 0), ofVec2f(ofGetWidth(), ofGetHeight())));
	gui.add(color.set("color", ofColor(100, 100, 140), ofColor(0, 0), ofColor(255, 255)));
	gui.add(circleResolution.set("circleRes", 5, 3, 90));
	gui.add(twoCircles.setup("twoCircles"));
	gui.add(ringButton.setup("ring"));
	gui.add(screenSize.set("screenSize", ""));*/


	//ofBackground(66,66,66);
	
	//initialize the video grabber
	//vidGrabber.setVerbose(true);
	//vidGrabber.setup(320,240);

	//store the width and height for convenience
	int width = NX;// vidGrabber.getWidth();
	int height = NY;// vidGrabber.getHeight();
	
	//add one vertex to the mesh for each pixel
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x<width; x++)
		{
			mainMesh.addVertex(ofPoint(x,y,0));	// mesh index = x + y*width
												// this replicates the pixel array within the camera bitmap...
//			mainMesh.addColor(ofFloatColor(0,0,0));  // placeholder for colour data, we'll get this from the camera
		}
	}
	
	for (int y = 0; y<height-1; y++){
		for (int x=0; x<width-1; x++){
			mainMesh.addIndex(x+y*width);				// 0
			mainMesh.addIndex((x+1)+y*width);			// 1
			mainMesh.addIndex(x+(y+1)*width);			// 10
			
			mainMesh.addIndex((x+1)+y*width);			// 1
			mainMesh.addIndex((x+1)+(y+1)*width);		// 11
			mainMesh.addIndex(x+(y+1)*width);			// 10
		}
	}
	
	//this is an annoying thing that is used to flip the camera
	cam.setScale(1,-1,1);
	
	
	//this determines how much we push the meshes out
	extrusionAmount = 80.0;



	// Waves

	// clear flags & fields
	for (int i = 0; i < NX; i++)
	for (int j = 0; j < NY; j++)
	{
		Flag[i][j] = 0;
		U[i][j] = E[i][j] = 0;
	}

	// init bnd

/*	// place wall with small gap
	for (int i = 0; i < NX; i++)
		if (fabs(i - NX / 2) >= 2)
			Flag[i][NY / 4] = C_BND;
			*/
	// place 'sources'
/*	for (int i = 0; i < NX; i++)
		if (fabs(i - NX / 2) <= 1)
			Flag[i][NY / 8] = C_SRC;
			*/

/*	for (int i = 0; i < NX; i++)
		if (fabs(i - NX / 2) <= 1)
			Flag[NX/2+i][NY-NY / 8] = C_SRC;
			*/
	for (int j = 0; j <= 1; j++)
		for (int i = 0; i <= 1; i++)
		{
		Flag[NX / 4+i][NY / 8+j] = C_SRC;
		Flag[NX / 2+i][NY - NY / 8+j] = C_SRC;
		Flag[NX / 3+i][NY / 2+j] = C_SRC;
	}
}



//--------------------------------------------------------------
void ofApp::update(){

/*	//grab a new frame
	vidGrabber.update();
	
	//update the mesh if we have a new frame
	if (vidGrabber.isFrameNew()){
		//this determines how far we extrude the mesh
		for (int i=0; i<vidGrabber.getWidth()*vidGrabber.getHeight(); i++){

			ofFloatColor sampleColor(vidGrabber.getPixels()[i*3]/255.f,				// r
									 vidGrabber.getPixels()[i*3+1]/255.f,			// g
									 vidGrabber.getPixels()[i*3+2]/255.f);			// b
			
			//now we get the vertex aat this position
			//we extrude the mesh based on it's brightness
			ofVec3f tmpVec = mainMesh.getVertex(i);
			tmpVec.z = sampleColor.getBrightness() * extrusionAmount;
			mainMesh.setVertex(i, tmpVec);

			mainMesh.setColor(i, sampleColor);
		}
	}
	*/




	//let's move the camera when you move the mouse
//	float rotateAmount = 0;// ofMap(ofGetMouseY(), 0, ofGetHeight(), 20.01, 160);
//	float rotateAmountX = 0;// ofMap(ofGetMouseX(), 0, ofGetWidth(), -88, 88);

	
	//move the camera around the mesh
	ofVec3f camDirection(0,0,1);
	ofVec3f centre(NX/2,NY/2+ Zoom,0);
	ofVec3f camDirectionRotated = camDirection.getRotated(rotateAmount-110, ofVec3f(1,0,0));
	ofVec3f camDirectionRotated2 = camDirectionRotated.getRotated(rotateAmountX, ofVec3f(0, 1, 0));
	ofVec3f camPosition = centre + camDirectionRotated2 * extrusionAmount ;

	cam.setPosition(camPosition);
	cam.lookAt(centre);

	//
	// Waves
	//

	//realdt = glutGet(GLUT_ELAPSED_TIME) / 100.0 - t;
	//t = glutGet(GLUT_ELAPSED_TIME) / 100.0;

	t = t + 0.1;

	int i, j;
	float x, y;

	// solve equation for U
	// dU/dt = nabla^2 E

	for (j = 1; j < NY; j++)
		for (i = 1; i < NX; i++)
		{
			U[i][j] = U[i][j] +
				VEL*(
					(1 - EPS) * (E[i + 1][j] + E[i - 1][j] + E[i][j - 1] + E[i][j + 1] - 4 * E[i][j])
					+ EPS * (E[i + 1][j + 1] + E[i + 1][j - 1] + E[i - 1][j - 1] + E[i - 1][j + 1] - 4 * E[i][j])
					) * ( DT) / (DX * DX);
		}

	// solve equation for E
	// dE/dt = U

	for (j = 0; j <= NY; j++)
		for (i = 0; i <= NX; i++)
		{
			E[i][j] = E[i][j] + U[i][j] * ( DT);
		}

	// start boundaries: pulsating sources

	const float AMPLITUDE = 0.01;

	for (j = 0; j <= NY; j++)
		for (i = 0; i <= NX; i++)
			if ((Flag[i][j] & C_SRC) && sources)
				E[i][j] = AMP * sin(t * FREQ) * AMPLITUDE;

	// boundaries
	for (i = 0; i <= NX; i++)
		for (j = 0; j <= NX; j++)
			if (Flag[i][j] & C_BND)
				E[i][j] = U[i][j] = 0.0f;

	// rigid wall 1

	for (i = 0; i <= NX; i++)
	{
		U[i][NY] = U[i][NY - 1] = 0;
		E[i][NY] = E[i][NY - 1] = 0;

		U[i][0] = U[i][1] = 0;
		E[i][0] = E[i][1] = 0;

	}

	// rigid wall 2
	for (j = 0; j <= NY; j++)
	{
		U[0][j] = U[1][j] = 0;
		E[0][j] = E[1][j] = 0;

		U[NX][j] = U[NX - 1][j] = 0;
		E[NX][j] = E[NX - 1][j] = 0;
	}

	for (int j = 0; j < NY; j++)
	for (int i = 0; i < NX; i++)
	{
		int k = i + j * NX;
		ofVec3f tmpVec = mainMesh.getVertex(k);
		tmpVec.z = E[i][j] * 3000;
		mainMesh.setVertex(k, tmpVec);
/*		ofFloatColor sampleColor(E[i][j]*3800,				// r
									E[i][j]*3200.0,			// g
								E[i][j]*3223.0);			// b
		mainMesh.setColor(k, sampleColor);*/
	}

}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackgroundGradient(ofColor::white, ofColor::gray);

	//we have to disable depth testing to draw the video frame
//	ofDisableDepthTest();
//	vidGrabber.draw(20,20);
	
	//but we want to enable it to show the mesh
	ofEnableDepthTest();
	cam.begin();		
	//You can either draw the mesh or the wireframe
	


	ofSetColor(0);
	mainMesh.drawFaces();

	ofSetColor(255);
	/*ofTranslate(0, .1, 0);
	mainMesh.drawWireframe();
	ofTranslate(0.1, -.1, 0);
	mainMesh.drawWireframe();*/
	//ofTranslate(-0.1, 0, 0.1);
	ofTranslate(0, 0, 0.8);
	mainMesh.drawWireframe();

	cam.end();
	
	//draw framerate for fun
	ofSetColor(0);
	string msg = "fps: " + ofToString(ofGetFrameRate(), 2);
	ofDrawBitmapString(msg, 10, 20);	


	ofDisableDepthTest();
	ofSetColor(ofColor::red);
	if(!gHide)
	gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	switch(key) {
		case 'f':
			ofToggleFullscreen();
			break;
	}

	if (key == 'h') {
		gHide = !gHide;
	}
	if (key == 's') {
		gui.saveToFile("settings.xml");
	}
	if (key == 'l') {
		gui.loadFromFile("settings.xml");
	}

}

void ofApp::clearButtonPressed()
{
	for (int j = 0; j < NY; j++)
		for (int i = 0; i < NX; i++)
			E[i][j] = U[i][j] = 0;
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
