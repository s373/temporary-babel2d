/*
 *  TemporaryBabel2D.h
 *  TemporaryBabel2D_000
 *
 *  Created by andré sier on 20130427.
 *  Copyright 2013 s373.net/x. All rights reserved.
 *
 */

#pragma once

#include <cstddef>
#include "ofMain.h"
#include "ofxBox2d.h"

#include "PlayerBase.h"
#include "LevelBase.h"
#include "ObjBase.h"
#include "MiniChronos.h"

class TemporaryBabel2D : public ofBaseApp {
public:			
	typedef enum {	
		INTRO			=  0,
		SELECTPLAYER	= 10,
		LEVELMAP		= 20,
		LEVELPLAY		= 30,
		LEVELLOOSE		= 40,
		LEVELWIN		= 50,
		GAMEOVER		= 60,
		GAMEWIN			= 70	
	} GAMESTATE;
	
	
	
	int		mainLevel;
	float	ellapsedseconds;
	
	PlayerBase *				player;
	
	vector <LevelBase *>		levels;
	LevelBase *					level;
	float						leveleasy;
			

	MiniChronos			chronos;
	bool				bInited, bDrawStats;
	unsigned long long	seed;
	GAMESTATE			gamestate;
	
	
	ofImage			images[5];
	ofSoundPlayer	sounds[7], soundtime;
	
	// audio
	void audioIn(float * input, int bufferSize, int nChannels); 
	void audioOut(float * output, int bufferSize, int nChannels);
	ofSoundStream    soundStream;	
	vector<float>   audiosamplesoutput;
	vector<float> audiosamplesinput;
	float 	     audioenergyI;
	float	   audioenergyO;
	float   outtervolume;
	float  innervolume;
	ofMutex			audiomutex;
	int				bufferSize, bufferCounter;
	float			drawdioXRange, drawdioHeadPos, 
					drawdioHeadSpeed, headPos, headPosY; 
	float			drawdioBaseSpeed, drawdioCurrentSpeed, drawdioDir;
	ofColor			drawdioColor;
	
	ofTrueTypeFont fontTitle, font, fontMedium;
	int blink, colorstate;
	ofColor	lettersColor, bgColor;
	float	colorHue, colorHueDst, colorGray, colorGrayDst;
	
	
	
	
	TemporaryBabel2D(){
		mainLevel = 0;
		ellapsedseconds = 0.0f;
		levels.clear();
		level = NULL;
		leveleasy = 1.0f;
		player = NULL;
		levels.clear();
		level = NULL;
		leveleasy = 1.0f;
		seed = ofGetSystemTime();
		ofSeedRandom((int)seed);
		bInited = false;
		bDrawStats = false;//true;
		bufferSize = 256;
		bufferCounter = 0;
		innervolume = outtervolume = 
		audioenergyI = audioenergyO = 0.0f;
		blink = 60; colorstate = 0; 
		colorHue = colorHueDst = ofRandom(360);
		colorGray = colorGrayDst = headPos = 0;
		gamestate = INTRO;
		drawdioXRange = 34; drawdioHeadPos = -0.5; // 34 234
		drawdioHeadSpeed = leveleasy*0.01;
		drawdioBaseSpeed = 250.0e-7; drawdioDir = 1; drawdioCurrentSpeed = 0.0f;
		headPos = headPosY = 0;
	}
	~TemporaryBabel2D(){
		
	}
	
	
	void init();
	void setup();
	void update();
	void draw();
	void drawIntro();
	void drawPlay();
	void drawGameOver();
	void drawdio(); void drawstats();
	
	void restartGame();
	void levelUp();
	void playTime();
	void playReaders();

	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	
	void contactStart(ofxBox2dContactArgs &e);
	void contactEnd(ofxBox2dContactArgs &e);


};