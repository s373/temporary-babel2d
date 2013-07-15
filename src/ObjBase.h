/*
 *  ObjBase.h
 *  TemporaryBabel2D_000
 *
 *  Created by andré sier on 20130427.
 *  Copyright 2013 s373.net/x. All rights reserved.
 *
 */


#pragma once
#include "ofMain.h"
#include "ofxBox2d.h"


class TemporaryBabel2D;
class PlayerBase;
class LevelBase;


enum OBJTYPES {
	RECT,CIRCLE,TRI,POLYLINE
};


enum OBJMODES {
	STATICINDESTRUCTIBLEOBJ,
	STATICOBJ,	
	STATICAUDIOOBJ,
	DYNAMICOBJ,	
	BOMBOBJ,
	TIMELINE,
	AUDIOLINE,
	LIGHTAUDIOOBJ,
	EXITAUDIOOBJ
};
	




static float mtof(float midi){  
	return  (440.0f * powf(2.0f, ((midi-69.0) / 12.0)) );  
}





class Box2dObjects {
public:	
	
	Box2dObjects(){};
	~Box2dObjects(){};
	
	
	//polylines
	vector <ofPolyline *>			linesPtr;
	vector <ofxBox2dPolygon *>	polyLinesPtr;
	//circles
	vector <ofxBox2dCircle *>	  circlesPtr;
	// adding triangles
	vector <ofxBox2dPolygon *>	trianglepoolPtr;
	// adding bookshelfs
	vector <ofxBox2dRect *>		rectpoolPtr;

	
	void addTriangle(b2World * world, const ofVec2f pos, const float angle, const float radius);
	void addXShape(b2World * world, const ofVec2f pos, const float angle, const float radius, const int sub=3,const float apertureFact=1.0f, const float pdensity=1.0,const  float pbounce=0.3f,const float pfriction=0.3f );
	
	
	
};









class ObjBase {
public:

	 ObjBase(){}
	~ObjBase(){}
	
	TemporaryBabel2D	*tb2dPtr;
	PlayerBase			*playerPtr;
	LevelBase			*levelPtr;

	vector<Box2dObjects> boxobjects;
	
	
	int objmode, objtype;
	float energy, energydec;
	
	bool				doesAudio;
	bool				doesDie;
//	bool				doesAudio;
	
	
	int	  audioom;
	float phase, phaseinc, frequency, volume;
	float source, target, delta;
	
	
	
	
	virtual void setup(TemporaryBabel2D * app, const int objecttype, const int objectmode);
	
	virtual void calcFrequency();
	void calcVolume(const PlayerBase *player);	
	void setPhase(const float newphase){
		phase = newphase;
	}
	virtual void update();
	virtual void draw();
	
	
	
};








// A simple little Data class. This is were
// you can store anything you want.
class Data {
public:
	ofColor color;
	string  name;
	int		id;
};


// A Custom Particle extedning the box2d circle
class CustomParticle : public ofxBox2dCircle {
	
public:
	
	void setupTheCustomData() {
		
		static int colors[] = {0xcae72b, 0xe63b8f, 0x2bb0e7};
		static string abc   = "abcdefghijklmnopqrstuvwxyz";
		
		// we are using a Data pointer because 
		// box2d needs to have a pointer not 
		// a referance
		setData(new Data());
		Data * theData = (Data*)getData();
		
		theData->id = ofRandom(0, 100);
		theData->name += abc[(int)ofRandom(0, abc.size())];
		theData->color.setHex(colors[(int)ofRandom(0, 3)]);
		
		printf("setting the custom data!\n");
		
	}
	
	void draw() {
		Data* theData = (Data*)getData();
		if(theData) {
			
			// Evan though we know the data object lets just 
			// see how we can get the data out from box2d
			// you would use this when using a contact listener
			// or tapping into box2d's solver.
			
			float radius = getRadius();
			ofPushMatrix();
			ofTranslate(getPosition());
			ofRotateZ(getRotation());
			ofSetColor(theData->color);
			ofFill();
			ofCircle(0, 0, radius);	
			
			ofSetColor(255);
			ofDrawBitmapString(theData->name, -5, 5);
			ofPopMatrix();
		}
	}
	
	
};





