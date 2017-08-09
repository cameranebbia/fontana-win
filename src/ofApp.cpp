#include "ofApp.h"

void ofApp::setup() {


	RenderWidth = 1920;
	aspectH = 10;
	RenderHeight = RenderWidth / 16 * aspectH;

	isMousePressed = false;

	setupGui();

	setupParticles();
	/*
	if (useCerchioCvImage)
		setupCv();
	*/
	setupSfondo();


	setupKinect();

	maskTop.loadImage("maskfontana.png");


	suonoBackground.loadSound("background.mp3");
	suonoBackground.setLoop(true);
	suonoBackground.play();

	suonoPresence.loadSound("presence.wav");
	suonoPresence.setLoop(true);
	suonoPresence.play();


}

void ofApp::update() {

	suonoBackground.setVolume(suonoBackVol);

	float presAmountLoc = grayImageKinect.countNonZeroInRegion(0, 0, grayImageKinect.width, grayImageKinect.height);
	float suonoPresVol = ofMap(presAmountLoc, 0, suonoPresMaxArea, 0, suonoPresVolMax, true);
	suonoPresence.setVolume(suonoPresVol);
	ofSoundUpdate();

	updateKinect();
}

void ofApp::draw() {
	ofBackground(0);
	if (whiteBackground) {
		ofBackground(255);
	}

	drawParticles();

	ofSetColor(255, 255, 255, 255);

	if (doMaskTop) {
		ofEnableAlphaBlending();
		maskTop.draw(0, 0, RenderWidth, RenderHeight);
		ofDisableAlphaBlending();
	}

	/*
	if (useCerchioCvImage && showPointsMap) {
		drawCvMap();
	}
	*/
	if (showPointsSfondo) {
		drawSfondo();
	}
	if (showPointsKinect) {
		drawKinect();
	}

	if (bounceShow) {
		ofNoFill();
		ofSetColor(0, 255, 0);

		ofCircle(bounceCenterX, bounceCenterY, bounceRadius);

		ofSetColor(255);
		ofFill();
	}

	/*CvColorImage.draw(50, 20, 160, 120);
	CvGrayImage.draw(220, 20, 160, 120);
	CvContourFinder.draw(220, 20, 160, 120);*/

	sfondoGrayImage.draw(390, 20, 160, 120);
	sfondoContourFinder.draw(390, 20, 160, 120);

	grayImageKinect.draw(560, 20, 160, 120);
	contourFinderKinect.draw(560, 20, 160, 120);



	if (showGui) {
		drawGui();
	}

	ofDrawBitmapString("fps " + ofToString(ofGetFrameRate(), 2), 300, 800);


}

void ofApp::keyPressed(int key) {
	switch (key) {
	case 'g':
		showGui = !showGui;
		break;
	case 's':
		guiGeneral.saveToFile("general.xml");
		guiParticleSystem.saveToFile("particle.xml");
		//guiCvMap.saveToFile("CVMap.xml");
		guiSfondo.saveToFile("sfondo.xml");
		guiKinect.saveToFile("kinect.xml");
		break;
	}

}

void ofApp::mousePressed(int x, int y, int button) {
	isMousePressed = true;
}

void ofApp::mouseReleased(int x, int y, int button) {
	isMousePressed = false;
}


void ofApp::setupGui() {
	showGui = false;

	guiGeneral.setup();
	guiGeneral.setName("GENERAL");
	guiGeneral.add(doMaskTop.setup("doMaskTop", true));
	guiGeneral.add(whiteBackground.setup("whiteBackground", false));

	guiGeneral.add(alphaAmpLine.setup("alphaAmpLine", 200, 0, 255));
	guiGeneral.add(redLine.setup("redLine", 200, 0, 255));
	guiGeneral.add(greenLine.setup("greenLine", 200, 0, 255));
	guiGeneral.add(blueLine.setup("blueLine", 200, 0, 255));

	guiGeneral.add(alphaAmpPart.setup("alphaAmpPart", 200, 0, 255));
	guiGeneral.add(redPart.setup("redPart", 200, 0, 255));
	guiGeneral.add(greenPart.setup("greenPart", 200, 0, 255));
	guiGeneral.add(bluePart.setup("bluePart", 200, 0, 255));

	guiGeneral.add(suonoBackVol.setup("suonoBackVol", 0, 0, 1));
	guiGeneral.add(suonoPresVolMax.setup("suonoPresVolMax", 0, 0, 1));
	guiGeneral.add(suonoPresMaxArea.setup("suonoPresMaxArea", 1, 1, 100000));


	guiGeneral.add(bounceShow.setup("bounceShow", true));
	guiGeneral.add(bounceCenterX.setup("bounceCenterX", 1000, 500, 1420));
	guiGeneral.add(bounceCenterY.setup("bounceCenterY", 540, 0, 1080));
	guiGeneral.add(bounceRadius.setup("bounceRadius", 540, 100, 1000));
	guiGeneral.add(bounceAmount.setup("bounceAmount", 0.2, 0, 1));
	guiGeneral.add(bounceMin.setup("bounceMin", 10, 0, 100));




	guiGeneral.setPosition(1700, 40);

	guiGeneral.loadFromFile("general.xml");


	guiParticleSystem.setup();
	guiParticleSystem.setName("PARTICLE_SYSTEM");

	guiParticleSystem.add(timeStep.setup("timeStep", 1, 0, 100));
	guiParticleSystem.add(particleNeighborhood.setup("particleNeighborhood", 32, 0, 64));
	guiParticleSystem.add(particleRepulsion.setup("particleRepulsion", 0.01, 0, 2));

	guiParticleSystem.add(lineSize.setup("linesize", 1, 0.1, 20));
	guiParticleSystem.add(pointSize.setup("particlesize", 3, 0.1, 20));

	guiParticleSystem.add(wallX.setup("wallX", 0, 0, ofGetWidth()));
	guiParticleSystem.add(wallY.setup("wallY", 0, 0, ofGetHeight()));
	guiParticleSystem.add(wallW.setup("wallW", RenderWidth, 0, RenderWidth));
	guiParticleSystem.add(wallH.setup("wallH", RenderHeight, 0, RenderHeight));

	guiParticleSystem.setPosition(10, 660);

	guiParticleSystem.loadFromFile("particle.xml");
	/*
	guiCvMap.setup();
	guiCvMap.setName("CV_MAPPING");
	guiCvMap.add(useCerchioCvImage.setup("useCerchioCvImage", false));
	guiCvMap.add(stepCVMap.setup("CVMapStep", 5, 1, 20));
	guiCvMap.add(CvForceMap.setup("CvForceMap", 1, 0, 1000));
	guiCvMap.add(CvRadiusMap.setup("CvRadiusMap", 10, 0, 100));
	guiCvMap.add(showPointsMap.setup("showPointsMap", true));

	guiCvMap.setPosition(300, 660);

	guiCvMap.loadFromFile("CVMap.xml");
	*/

	guiSfondo.setup();
	guiSfondo.setName("SFONDO");

	guiSfondo.add(stepSfondo.setup("stepSfondo", 5, 1, 20));
	guiSfondo.add(sfondoForce.setup("sfondoForce", 1, 0, 1000));
	guiSfondo.add(sfondoRadius.setup("sfondoRadius", 10, 0, 100));
	guiSfondo.add(showPointsSfondo.setup("showPointsSfondo", true));

	guiSfondo.setPosition(600, 660);

	guiSfondo.loadFromFile("sfondo.xml");


	guiKinect.setup();
	guiKinect.setName("KINECT_SETTINGS");
	guiKinect.add(stepKinect.setup("steppoint", 5, 1, 20));
	guiKinect.add(mirror.setup("mirror", false));
	guiKinect.add(farThreshold.setup("farThreshold", 70, 0, 255));
	guiKinect.add(showPointsKinect.setup("showPointsKinect", true));
	guiKinect.add(forceKinect.setup("forceKinect", 1, 0, 100));
	guiKinect.add(radiusKinect.setup("radiusKinect", 10, 0, 1000));
	guiKinect.add(erode.setup("erode", 0, 0, 30));
	guiKinect.add(dilate.setup("dilate", 0, 0, 30));

	guiKinect.setPosition(900, 660);

	guiKinect.loadFromFile("kinect.xml");


}

void ofApp::drawGui() {
	guiGeneral.draw();
	guiParticleSystem.draw();
	//guiCvMap.draw();
	guiSfondo.draw();
	guiKinect.draw();


}

void ofApp::setupParticles() {
	// this number describes how many bins are used
	// on my machine, 2 is the ideal number (2^2 = 4x4 pixel bins)
	// if this number is too high, binning is not effective
	// because the screen is not subdivided enough. if
	// it's too low, the bins take up so much memory as to
	// become inefficient.
	int binPower = 4;

	particleSystem.setup(RenderWidth, RenderHeight, binPower);

	kParticles = 3;
	float padding = 0;
	float maxVelocity = .05;
	for (int i = 0; i < kParticles * 1024; i++) {
		//float x = ofRandom(padding, ofGetWidth() - padding);
		//float y = ofRandom(padding, ofGetHeight() - padding);
		//		float x = ofRandom(RenderWidth / 2 - 200, RenderWidth / 2 + 200);
		//		float y = ofRandom(RenderHeight / 2 - 200, RenderHeight / 2 + 200);
		float randAng = ofRandom(2 * PI);
		float x = cos(randAng)*ofRandom(bounceRadius - 50) + bounceCenterX;
		float y = sin(randAng)*ofRandom(bounceRadius - 50) + bounceCenterY;

		float xv = ofRandom(-maxVelocity, maxVelocity);
		float yv = ofRandom(-maxVelocity, maxVelocity);
		BinnedParticle particle(x, y, xv, yv);
		particleSystem.add(particle);
	}

	timeStep = 1;
	particleNeighborhood = 32;
	particleRepulsion = 1;
	// lineOpacity = 80;
	//pointOpacity = 100;
}


void ofApp::drawParticles() {

	particleSystem.setLineCol(ofColor(redLine, greenLine, blueLine));
	particleSystem.setAlphaAmpLine(alphaAmpLine);

	particleSystem.setPartCol(ofColor(redPart, greenPart, bluePart));
	particleSystem.setAlphaAmpPart(alphaAmpPart);

	particleSystem.setTimeStep(timeStep);


	//ofEnableBlendMode(OF_BLENDMODE_MULTIPLY);

	//ofBlendMode(OF_BLENDMODE_MULTIPLY);
	//ofSetColor(Red, green, blu, lineOpacity);

	particleSystem.setupForces();
	// apply per-particle forces
	/*
	if (useCerchioCvImage) {
		for (int i = 0; i < CvContourFinder.nBlobs; i++) {
			for (int j = 0; j < CvContourFinder.blobs[i].nPts; j += stepCVMap) {
				float xpos = ofMap(CvContourFinder.blobs[i].pts[j].x, 0, CvImage.getWidth(), 0, RenderWidth);
				float ypos = ofMap(CvContourFinder.blobs[i].pts[j].y, 0, CvImage.getHeight(), 0, RenderHeight);
				//ofVertex( contourFinder.blobs[i].pts[j].x, contourFinder.blobs[i].pts[j].y );
				//particleSystem.addRepulsionForce(contourFinder.blobs[i].pts[j].x, contourFinder.blobs[i].pts[j].y, radius, power);
				particleSystem.addRepulsionForce(xpos, ypos, CvRadiusMap, CvForceMap);
			}
		}
	}
	*/
	for (int i = 0; i<sfondoContourFinder.nBlobs; i++) {
		for (int j = 0; j<sfondoContourFinder.blobs[i].nPts; j += stepSfondo) {
			float xpos = ofMap(sfondoContourFinder.blobs[i].pts[j].x, 0, sfondoImage.getWidth(), 0, RenderWidth);
			float ypos = ofMap(sfondoContourFinder.blobs[i].pts[j].y, 0, sfondoImage.getHeight(), 0, RenderHeight);
			//ofVertex( contourFinder.blobs[i].pts[j].x, contourFinder.blobs[i].pts[j].y );
			//particleSystem.addRepulsionForce(contourFinder.blobs[i].pts[j].x, contourFinder.blobs[i].pts[j].y, radius, power);
			particleSystem.addRepulsionForce(xpos, ypos, sfondoRadius, sfondoForce);
		}
	}

	ofVec2f center = ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2);

	for (int i = 0; i < contourFinderKinect.nBlobs; i++) {

		ofVec2f nearest;
		float prevDist = 999999;

		for (int j = 0; j < contourFinderKinect.blobs[i].nPts; j += stepKinect) {
			float xpos = ofMap(contourFinderKinect.blobs[i].pts[j].x, 0, kinect.width, 0, RenderWidth);
			float ypos = ofMap(contourFinderKinect.blobs[i].pts[j].y, 0, kinect.height, 0, RenderHeight);

			ofVec2f pos = ofVec2f(xpos, ypos);
			float dist = pos.distance(center);
			if (dist < prevDist) {
				prevDist = dist;
				nearest = pos;
			}
		}
		float xpos = nearest.x;
		float ypos = nearest.y;

		particleSystem.addRepulsionForce(xpos, ypos, radiusKinect, forceKinect);

	}


	glLineWidth(lineSize);


	glBegin(GL_LINES);


	for (int i = 0; i < particleSystem.size(); i++) {
		BinnedParticle& cur = particleSystem[i];
		particleSystem.addRepulsionForce(cur, particleNeighborhood, particleRepulsion);

		//cur.bounceOffWalls(wallX, wallY, wallW, wallH);
		cur.bounceOffCircle(ofVec2f(bounceCenterX, bounceCenterY), bounceRadius, bounceAmount, bounceMin);

		cur.addDampingForce();
	}
	glEnd();

	// single global forces
	//particleSystem.addAttractionForce(ofGetWidth() / 2, ofGetHeight() / 2, ofGetWidth(), centerAttraction);
	ofDisableAlphaBlending();


	if (isMousePressed) {
		particleSystem.addRepulsionForce(mouseX, mouseY, 100, 100);
	}


	glPointSize(pointSize);




	ofEnableAlphaBlending();
	particleSystem.update(ofGetLastFrameTime());
	//	ofSetColor(RedP, greenP, bluP, pointOpacity);

	//	particleSystem.draw(ofColor(RedP, greenP, bluP, pointOpacity), pointSize);
	particleSystem.draw();

	ofDisableAlphaBlending();

	ofSetColor(0, 0, 0, 255);

	ofSetColor(255);
}


/*
void ofApp::setupCv() {
	CvImage.loadImage("mapCV.jpg"); //load image mapping parete iniziale
	CvImage.setImageType(OF_IMAGE_COLOR);

	CvColorImage.allocate(CvImage.getWidth(), CvImage.getHeight());
	CvGrayImage.allocate(CvImage.getWidth(), CvImage.getHeight());
	//	CVgrayBg.allocate(CvImage.getWidth(), CvImage.getHeight());
	//	CVgrayDiff.allocate(CvImage.getWidth(), CvImage.getHeight());

	CvThreshold = 80;

	CvColorImage.setFromPixels(CvImage.getPixelsRef());
	CvGrayImage = CvColorImage;

	CvGrayImage.threshold(CvThreshold);



	CvContourFinder.findContours(CvGrayImage, 20, (CvImage.getWidth()*CvImage.getHeight()) / 3, 20, false, true);

}

void ofApp::drawCvMap() {
	ofPushStyle();
	ofSetColor(255, 0, 0);
	//info = "RENDER LAST STATE :: USE CV MAPPING";
	for (int i = 0; i<CvContourFinder.nBlobs; i++) {
		for (int j = 0; j<CvContourFinder.blobs[i].nPts; j += stepCVMap) {

			float xpos = ofMap(CvContourFinder.blobs[i].pts[j].x, 0, CvImage.getWidth(), 0, RenderWidth);
			float ypos = ofMap(CvContourFinder.blobs[i].pts[j].y, 0, CvImage.getHeight(), 0, RenderHeight);
			ofCircle(xpos, ypos, 2);
		}
	}
	ofPopStyle();
}
*/


void ofApp::setupSfondo() {
	sfondoImage.loadImage("sfondo.jpg"); //load image mapping parete iniziale
	sfondoImage.setImageType(OF_IMAGE_COLOR);

	sfondoColorImage.allocate(sfondoImage.getWidth(), sfondoImage.getHeight());
	sfondoGrayImage.allocate(sfondoImage.getWidth(), sfondoImage.getHeight());
	sfondoGrayImageMoving.allocate(sfondoImage.getWidth(), sfondoImage.getHeight());

	sfondoThreshold = 80;

	sfondoColorImage.setFromPixels(sfondoImage.getPixelsRef());
	sfondoGrayImage = sfondoColorImage;
	sfondoGrayImage.threshold(sfondoThreshold);



	//sfondoContourFinder.findContours(sfondoGrayImage, 20, (sfondoImage.getWidth()*sfondoImage.getHeight()) / 3, 20, true, true);
}

void ofApp::drawSfondo() {
	sfondoGrayImageMoving = sfondoGrayImage;
	sfondoGrayImageMoving.translate(cos(ofGetElapsedTimef()) * 200, sin(ofGetElapsedTimef()) * 130);

	sfondoContourFinder.findContours(sfondoGrayImageMoving, 20, (sfondoImage.getWidth()*sfondoImage.getHeight()) / 3, 20, true, true);

	ofPushStyle();
	ofSetColor(255, 0, 0);
	for (int i = 0; i<sfondoContourFinder.nBlobs; i++) {
		for (int j = 0; j<sfondoContourFinder.blobs[i].nPts; j += stepSfondo) {

			float xpos = ofMap(sfondoContourFinder.blobs[i].pts[j].x, 0, sfondoImage.getWidth(), 0, RenderWidth);
			float ypos = ofMap(sfondoContourFinder.blobs[i].pts[j].y, 0, sfondoImage.getHeight(), 0, RenderHeight);
			ofCircle(xpos, ypos, 2);
		}
	}
	ofPopStyle();
}

/// kinect
void ofApp::setupKinect() {
	//kinect.setRegistration(true);
	//kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	kinect.init(false, false); // disable video image (faster fps)
	kinect.open();		// opens first available kinect

	grayImageKinect.allocate(kinect.width, kinect.height);
	grayThreshNearKinect.allocate(kinect.width, kinect.height);
	grayThreshFarKinect.allocate(kinect.width, kinect.height);


	kinectMaskImg.loadImage("maskKinect.png");
	kinectMask.allocate(640, 480);
	kinectMaskBN.allocate(640, 480);
	kinectMask.setFromPixels(kinectMaskImg);
	kinectMaskBN = kinectMask;


}



void ofApp::updateKinect() {


	kinect.update();
	if (kinect.isFrameNew()) {

		// load grayscale depth image from the kinect source
		grayImageKinect.setFromPixels(kinect.getDepthPixels());
		if (mirror) {
			grayImageKinect.mirror(false, true);
		}

		grayImageKinect *= kinectMaskBN;

		grayImageKinect.threshold(farThreshold);


		for (int i = 0; i<dilate; i++) {
			grayImageKinect.dilate();
		}
		for (int i = 0; i<erode; i++) {
			grayImageKinect.erode();
		}

		contourFinderKinect.findContours(grayImageKinect, 10, (kinect.width*kinect.height) / 2, 20, false);

	}

}




void ofApp::drawKinect() {
	ofPushStyle();
	ofVec2f center = ofVec2f(ofGetWidth() / 2, ofGetHeight() / 2);

	for (int i = 0; i < contourFinderKinect.nBlobs; i++) {

		ofVec2f nearest;
		float prevDist = 999999;

		for (int j = 0; j < contourFinderKinect.blobs[i].nPts; j += stepKinect) {
			float xpos = ofMap(contourFinderKinect.blobs[i].pts[j].x, 0, kinect.width, 0, RenderWidth);
			float ypos = ofMap(contourFinderKinect.blobs[i].pts[j].y, 0, kinect.height, 0, RenderHeight);

			ofVec2f pos = ofVec2f(xpos, ypos);
			float dist = pos.distance(center);
			if (dist < prevDist) {
				prevDist = dist;
				nearest = pos;
			}
		}
		float xpos = nearest.x;
		float ypos = nearest.y;

		ofSetColor(200, 200, 0);
		ofCircle(xpos, ypos, 20);

	}

	ofPopStyle();

}




