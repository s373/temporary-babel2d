/*
 *  TemporaryBabel2D.cpp
 *  TemporaryBabel2D_000
 *
 *  Created by andré sier on 20130427.
 *  Copyright 2013 s373.net/x. All rights reserved.
 *
 */




#include "TemporaryBabel2D.h"


static string audioFiles[7] = {
	"tb2dmedia/0.5seconds2x.wav",
	"tb2dmedia/0.5seconds2xPhase.wav",
	"tb2dmedia/1second.wav",
	"tb2dmedia/1secondBeat.wav",
	"tb2dmedia/1secondPhase.wav",
	"tb2dmedia/2secondsphase.wav",
	"tb2dmedia/4x1secondPhase2.wav"
};

static string imageFiles[5] = {
	"tb2dmedia/flare01.png",
	"tb2dmedia/flare01a.png",
	"tb2dmedia/flare03.png",
	"tb2dmedia/flare03a.png",
	"tb2dmedia/flare04a.png"
};


void TemporaryBabel2D::init(){
	chronos.setup();
	for(int i=0; i<5; i++){
		images[i].loadImage(imageFiles[i]);
	}
	for(int i=0; i<7; i++){
		sounds[i].loadSound(audioFiles[i]);
		sounds[i].setLoop(false);
		sounds[i].setMultiPlay(true);
		sounds[i].setVolume(0.7f);
		sounds[i].setSpeed(ofRandom(0.97, 1.03));
		sounds[i].play();
	}
	soundtime.loadSound("tb2dmedia/1secondPhase.wav");
	soundtime.setLoop(false);
	soundtime.setMultiPlay(true);
	soundtime.setVolume(0.37f);
	soundtime.setSpeed(ofRandom(0.97, 1.03));
	soundtime.play();
	fontTitle.loadFont("tb2dmedia/spincycle_3d_tt.ttf",52);
	fontMedium.loadFont("tb2dmedia/spincycle_3d_tt.ttf",37);
	font.loadFont("tb2dmedia/spincycle_3d_tt.ttf",12);
	ofSetWindowTitle("Temporary-Babel2D");
	ofSetCircleResolution(7);
}



void TemporaryBabel2D::setup(){
	
	if(!bInited){
		init();
		bInited = true;
	}
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);
	ofEnableAlphaBlending();
	
	
	level = new LevelBase();
	level->setup(this);
	
	player = new PlayerBase();
	player->setup(this);
	
	
	
	
	soundStream.listDevices();
	audiosamplesoutput.assign(bufferSize,0.0f);
	audiosamplesinput .assign(bufferSize,0.0f);
	audioenergyI = audioenergyO = 0.0f;
	outtervolume = 0.5f;
	innervolume = 0.5f;
	bufferSize = 256;
	
	// ptr, output, input, buffersize, nbuffers
	soundStream.setup(this, 0, 2, 44100, bufferSize, 4);
	
}


void TemporaryBabel2D::restartGame(){
	chronos.setup();
	for(int i=0; i<5; i++){
		images[i].loadImage(imageFiles[i]);
	}
	for(int i=0; i<7; i++){
		sounds[i].loadSound(audioFiles[i]);
		sounds[i].setLoop(false);
		sounds[i].setMultiPlay(true);
		sounds[i].setVolume(0.7f);
		sounds[i].setSpeed(ofRandom(0.97, 1.03));
		sounds[i].play();
	}
	fontTitle.loadFont("tb2dmedia/spincycle_3d_tt.ttf",52);
	fontMedium.loadFont("tb2dmedia/spincycle_3d_tt.ttf",37);
	font.loadFont("tb2dmedia/spincycle_3d_tt.ttf",12);
	ofSetWindowTitle("Temporary-Babel2D");
}


void TemporaryBabel2D::levelUp(){ // *

	mainLevel++;
	leveleasy = ofMap(mainLevel, 0, 99, 1, 0.01);
	drawdioXRange = ofMap(mainLevel, 0, 99, 34, 234, true); 
	drawdioHeadSpeed = ofMap(leveleasy,1,0, 0.01, 0.25);
	player->headanchor.setPosition( ofRandom(20, 200), ofRandom( 500,700  ) );
	level->levelup();

}






void TemporaryBabel2D::update(){
	chronos.update();
	if(chronos.timesup()){
		sleep(10);
		ofExit(0);
	}
	if(blink>0){
		if(ofGetFrameNum()%blink==0){
			colorstate^=1;
		}
	}
	
	float ccdif = colorHueDst - colorHue;
	if(ABS(ccdif)<1e-1){
		colorHueDst = colorHue<100?ofRandom(200,360):ofRandom(0,25);//ofRandom(360);
	} else {
		colorHue += ccdif * 0.00219771969;
		float hue = colorHue + ofRandom(-audioenergyI*100,audioenergyI*100);
		lettersColor.setHsb(hue, 255, 255, 201);
//		drawdioColor.setHsb((360-colorHue), 255, 255, 157);
//		drawdioColor.setHsb((colorHue + ofRandom(-audioenergyI*100,audioenergyI*100)), 255, 255, 157);
		drawdioColor.setHsb((hue + ofRandom(-audioenergyI*100,audioenergyI*100)), 255, 255, 157);
	}
	
	float longevity = chronos.getLongevity()*101.0f;
	colorGray +=(longevity-colorGray)*0.000512;
	bgColor.set(colorGray);
	
	drawdioCurrentSpeed = drawdioDir*(drawdioBaseSpeed + audioenergyI * (0.1f/leveleasy));
//	float drawdioHeadPosDst = drawdioDir*(drawdioBaseSpeed + audioenergyI * (0.1f/leveleasy)) + drawdioHeadPos;
	drawdioHeadPos = drawdioCurrentSpeed + drawdioHeadPos;//drawdioHeadPosDst;
	if(drawdioHeadPos>1){
		drawdioHeadPos=1; drawdioDir = -1; playTime();
	} else if(drawdioHeadPos<-1){
		drawdioHeadPos=-1; drawdioDir = 1; playTime();
	}
										  
	//cosf(ofGetFrameNum()*drawdioHeadSpeed) * drawdioXRange + ofGetWidth()/2;
//	drawdioHeadPos += ( drawdioHeadPosDst - drawdioHeadPos ) * 0.025;
//	drawdioHeadSpeed += ( (audioenergyI*leveleasy*100.1)  -drawdioHeadSpeed) * 0.05;
//	headPos += (drawdioHeadPos*drawdioXRange - headPos) * 0.05; //* drawdioXRange + ofGetWidth()/2;
	headPos = (drawdioHeadPos*drawdioXRange + ofGetWidth()/2) ;
	
	
	level->setFps(ofMap(audioenergyI, 0, 1, 30, 1000));
	level->update();
	if(gamestate!=INTRO){
		player->update();
		
		if(ofGetFrameNum()%300 == 0){
			
			if(level->leveldone){			
				levelUp();						
			}
			
			
		}
		
		
	}
	
	
	
	
}






void TemporaryBabel2D::draw(){
	
	ofBackground(bgColor);
	
	level->draw();
	player->draw();
	
	drawdio();
	
	
	
	ofSetColor(lettersColor);
	
	if(gamestate==INTRO){
		fontTitle.drawString("temporary\n -babel2d", 25, ofGetHeight()/2.71);
		font.drawString("s373.net/x 201304", 169, ofGetHeight()/2.021);
		font.drawString("a 0 game by\n  Andre Sier\n  s373.net/x", 181, ofGetHeight()/1.25);
		if(colorstate>0){
			fontMedium.drawString("press space\n  to start", 61, ofGetHeight()/1.69);
		}	
	}	
	
	if(level->leveldone){
		if(colorstate>0){
			fontMedium.drawString("level done\n ready?", 61, ofGetHeight()/1.69);
		}			
		
	}
	
	ofSetColor(lettersColor, ofRandom(ofMap(level->donedistance, 1000, 20, 7,217, true)));
	ofLine(ofRandom(mainLevel), ofGetHeight(), level->librarian.getPosition().x, level->librarian.getPosition().y);
	ofLine(ofGetWidth()-ofRandom(mainLevel), ofGetHeight(), level->librarian.getPosition().x, level->librarian.getPosition().y);
	
	chronos.draw(5, ofGetHeight()-25, ofGetWidth()-10, 22);
	

	if(bDrawStats){
		drawstats();
	}
	
	
}







void TemporaryBabel2D::drawIntro(){
	
}




void TemporaryBabel2D::drawPlay(){
	

}


void TemporaryBabel2D::drawGameOver(){
	
}


void TemporaryBabel2D::playTime() {	
	soundtime.setSpeed(ofRandom(0.37, 1.69));
	soundtime.play();
}
void TemporaryBabel2D::playReaders() {	
	int idx = (int) ofRandom(7);
	sounds[idx].setSpeed(ofRandom(0.37, 1.69));
	sounds[idx].play();
}



void TemporaryBabel2D::drawstats() {
	int bodycount = level->box2dFrontWorld.getBodyCount() + level->box2dShadowWorld.getBodyCount();
	string stats = "fps: "+ofToString(ofGetFrameRate())+"\n"+
	"audio: "+ofToString(audioenergyI)+"\n"+
	"audiospeed: "+ofToString(drawdioBaseSpeed)+" "+ ofToString(drawdioCurrentSpeed) +"\n"+
//	"mouse: "+ofToString(ofGetMouseX())+" "+ofToString(ofGetMouseY())+" "+ofToString(ofGetMousePressed(0))+     "\n"+
	"level: "+ofToString(mainLevel)+"\n"+
	"leveleasy: "+ofToString(leveleasy)+"\n"+
	"leveldist: "+ofToString(level->donedistance)+"\n"+
	"headPos: "+ofToString(headPos)    + "\n"+ 
	"bodycount: " + ofToString(bodycount) + "\n"+
	"playerforce: "+ofToString(player->force)+" "+ ofToString(player->playerforcemagnitude) +"\n";
	//			"headPos: "+ofToString(headPos)    + "\n"+;
	
	font.drawString(stats, 5, 52);
	
}



//drawdio
void TemporaryBabel2D::drawdio() {				
	ofSetColor(drawdioColor);
	float xleft = -drawdioXRange-5+ ofGetWidth()/2;
	float xright = drawdioXRange+ ofGetWidth()/2;
	float yhead = 25.0f; int yidx = ofMap(drawdioHeadPos, -1, 1, 0, (bufferSize-1), true);
//	float xhead = drawdioHeadPos * drawdioXRange;
	for(int i=0; i<(bufferSize-1); i++){
		float x = ofMap(i, 0, bufferSize, -drawdioXRange, drawdioXRange) + ofGetWidth()/2;
		float x1 =ofMap((i+1), 0, bufferSize, -drawdioXRange, drawdioXRange) + ofGetWidth()/2;
		float y  =ofMap(audiosamplesinput[i], -1, 1, -25, 25) + 27;
		float y1 =ofMap(audiosamplesinput[i+1], -1, 1, -25, 25) + 27;
		if(i==yidx){
			headPosY = yhead = (y-2.5);
		}
		ofLine(x,y,x1,y1);
	}
	ofRect(xleft, 25, 5, 5); ofRect(xright, 25, 5, 5); ofRect(headPos, yhead, 5, 5);
	font.drawString("level "+ofToString(mainLevel,2), ofGetWidth()/2-27, 50);
}	






void TemporaryBabel2D::keyPressed(int key){

	player->keyPressed(key);
	
	if(key=='S'){
		ofSaveFrame(true);
	}
	if(key=='h'){
		bDrawStats^=true;
	}
	
	switch( gamestate ){
		case INTRO:
			if(key==' '){
				gamestate = GAMEWIN;
			}
			break;
			
			
	}
	
}


void TemporaryBabel2D::keyReleased(int key){
	
	player->keyReleased(key);
	
	switch( gamestate ){
		case INTRO:
			if(key==' '){
				gamestate = LEVELPLAY;
			}
			break;
			
			
	}
	
}




void TemporaryBabel2D::mouseMoved(int x, int y ){
	
}

void TemporaryBabel2D::mouseDragged(int x, int y, int button){
	
}

void TemporaryBabel2D::mousePressed(int x, int y, int button){
	
}
void TemporaryBabel2D::mouseReleased(int x, int y, int button){
	
}
void TemporaryBabel2D::windowResized(int w, int h){
	
}




void TemporaryBabel2D::contactStart(ofxBox2dContactArgs &e){
	
}

void TemporaryBabel2D::contactEnd(ofxBox2dContactArgs &e){
	
}



void TemporaryBabel2D::audioOut(float * output, int bufferSize, int nChannels){
	
	
}







void TemporaryBabel2D::audioIn(float * input, int bufferSize, int nChannels){	
	
	float curVol = 0.0;	
	int numCounted = 0;	

	for (int i = 0; i < bufferSize; i++){
		audiosamplesinput[i]	= input[i*2]*0.5;
		audiosamplesinput[i]	+= input[i*2+1]*0.5;
		
		curVol += audiosamplesinput[i] * audiosamplesinput[i];
		numCounted+=1;
	}
	
	//this is how we get the mean of rms :) 
	curVol /= (float)numCounted;
	
	// this is how we get the root of rms :) 
	curVol = sqrtf( curVol );
	
	audioenergyI *= 0.93;
	audioenergyI += 0.07 * curVol;
	
	bufferCounter++;
	
}

