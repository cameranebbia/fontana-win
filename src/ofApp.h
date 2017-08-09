#pragma once

#include "BinnedParticleSystem.h"
#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	bool isMousePressed;



	int RenderWidth;
	int RenderHeight;

	int aspectH;





	void setupGui();
	void drawGui();
	bool showGui;

	ofImage maskTop;


	ofxPanel guiGeneral;
	ofxToggle doMaskTop;
	ofxToggle whiteBackground;
	ofxFloatSlider alphaAmpLine;
	ofxFloatSlider redLine;
	ofxFloatSlider greenLine;
	ofxFloatSlider blueLine;
	ofxFloatSlider alphaAmpPart;
	ofxFloatSlider redPart;
	ofxFloatSlider greenPart;
	ofxFloatSlider bluePart;




	//// particles
	BinnedParticleSystem particleSystem;
	int kParticles;
	void setupParticles();
	void drawParticles();

	ofxPanel guiParticleSystem;

	ofxFloatSlider timeStep;
	ofxIntSlider particleNeighborhood;
	ofxFloatSlider particleRepulsion;
	//float centerAttraction;

	ofxFloatSlider wallX;
	ofxFloatSlider wallY;
	ofxFloatSlider wallW;
	ofxFloatSlider wallH;


	ofxFloatSlider pointSize;
	ofxFloatSlider lineSize;

	/*
	// CV cerchio 
	ofxToggle useCerchioCvImage;
	ofImage CvImage; // CV IMAGE
	ofxCvColorImage	CvColorImage;
	ofxCvGrayscaleImage CvGrayImage;
	int CvThreshold;
	ofxCvContourFinder 	CvContourFinder;
	*/
	ofxToggle bounceShow;
	ofxFloatSlider bounceCenterX;
	ofxFloatSlider bounceCenterY;
	ofxFloatSlider bounceRadius;
	ofxFloatSlider bounceAmount;
	ofxFloatSlider bounceMin;


	/*
	void setupCv();
	void drawCvMap();

	ofxPanel guiCvMap;
	ofxIntSlider stepCVMap;
	ofxFloatSlider CvForceMap;
	ofxFloatSlider CvRadiusMap;
	ofxToggle showPointsMap;
	*/


	/// sfondo muovi acqua
	ofImage sfondoImage;
	ofxCvColorImage sfondoColorImage;
	ofxCvGrayscaleImage sfondoGrayImage;
	ofxCvGrayscaleImage sfondoGrayImageMoving;
	int sfondoThreshold;
	ofxCvContourFinder 	sfondoContourFinder;

	void setupSfondo();
	void drawSfondo();

	ofxPanel guiSfondo;
	ofxIntSlider stepSfondo;
	ofxFloatSlider sfondoForce;
	ofxFloatSlider sfondoRadius;
	ofxToggle showPointsSfondo;

	// kinect

	ofxKinect kinect;
	ofxCvGrayscaleImage grayImageKinect; // grayscale depth image

	ofxCvGrayscaleImage grayThreshNearKinect; // the near thresholded image
	ofxCvGrayscaleImage grayThreshFarKinect; // the far thresholded image
	ofxCvContourFinder contourFinderKinect;

	ofxCvColorImage kinectMask;
	ofxCvGrayscaleImage kinectMaskBN;
	ofImage kinectMaskImg;


	void setupKinect();
	void updateKinect();
	void drawKinect();

	ofxPanel guiKinect;
	ofxToggle mirror;
	ofxFloatSlider farThreshold;
	ofxFloatSlider forceKinect;
	ofxFloatSlider radiusKinect;
	ofxToggle showPointsKinect;
	ofxIntSlider stepKinect;
	ofxIntSlider erode, dilate;


	/// sounds

	ofSoundPlayer suonoBackground;
	ofSoundPlayer suonoPresence;

	ofxFloatSlider suonoBackVol;

	ofxFloatSlider suonoPresVolMax;

	ofxFloatSlider suonoPresMaxArea;

};
