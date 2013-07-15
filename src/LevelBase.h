/*
 *  LevelBase.h
 *  BOX2DtriangleWishSimplePRIMER_Ptr
 *
 *  Created by andré sier on 20130427.
 *  Copyright 2013 s373.net/x. All rights reserved.
 *
 */

#pragma once


#include "ofMain.h"
#include "ofxBox2d.h"

#include "ObjBase.h"

class TemporaryBabel2D;

class LevelBase {
public:
	
	class PlayerSoundData {
	public:
		int	 soundID;
		bool bHit;
		float mult;
	};
	
	class ShadowLightData {
	public:
		int	 textureID;
		int	 soundID;
		bool bHit;
		float mult;
		float phase;
	};
	
	
	
	TemporaryBabel2D			*tb2d;
	
	ofxBox2d					box2dShadowWorld;
	ofxBox2d					box2dFrontWorld;
	
	float leveltime;
	float leveltimelimit;
	float leveltimebegin;
	
	int itemstocollect;
	int itemscollected;
	
	bool exitopen, leveldone;
	float donedistance;
	
	float readersprob, booksprob, trisprob, readersshadowprob,wallbookshelvesshadowprob,trisshadowprob;
	
	vector<ObjBase *>		levelobjects;
	vector<ObjBase *>		shadowlevelobjects;
	ofxBox2dCircle					librarian;	
	vector <ofxBox2dCircle *>		readers;
	vector <ofxBox2dRect *>			wallbookshelves;
	vector <ofxBox2dPolygon *>		trianglepool;
	vector <ofxBox2dCircle *>		readersshadow;
	vector <ofxBox2dRect *>			wallbookshelvesshadow;
	vector <ofxBox2dPolygon *>		trianglepoolshadow;

	
	virtual void setup(TemporaryBabel2D * app);
	virtual void update();
	virtual void draw();
	virtual void addReaders();
	virtual void addWallBookShelves();
	virtual void addTriangles();
	virtual void addReadersShadow();
	virtual void addWallBookShelvesShadow();	
	
	virtual void buildlevel();
	virtual void levelup();
	virtual void enterlevel();
	virtual void exitlevel();
	virtual void setFps(float fps);

	void contactStart(ofxBox2dContactArgs &e);
	void contactEnd(ofxBox2dContactArgs &e);

	

};