/*
 *  PlayerBase.h
 *  TemporaryBabel2D_000
 *
 *  Created by andré sier on 20130427.
 *  Copyright 2013 s373.net/x. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxBox2d.h"
#include "Utils.h"

class TemporaryBabel2D; class LevelBase;
class PlayerBase {
public:
	enum PLAYERMODES {
		NEARZERO,
		WALKING,
		HOVERING,
		SKATING	
	};
	enum PLAYERTYPES {
		HUMAN,
		ZOMBIE,
		LEECH,
		SUN	
	};	
	
	TemporaryBabel2D *tb2d;
	LevelBase		*levelPtr;
	ofVec2f		pos, force;	
	int			playertype;	
	float		playerDtinfluence, playerforcemagnitude;
	float		playerenergy;
	int			playermode;
	bool		keyup, keydown, keyleft, keyright, keyspace, lives, applyforce;
	float		forcetimethen;
	
	PlayerBase(){
		tb2d = NULL; levelPtr = NULL; pos.set(0,0); force.set(0,-2);
		playermode = NEARZERO; playertype = HUMAN;
		playerDtinfluence = 1e2; playerenergy = 1.0f; playerforcemagnitude = 0.0f;
		keyup = keydown = keyleft = keyright = keyspace = false;
		lives = true; applyforce = true; forcetimethen = ofGetElapsedTimef() + 1;
	}
	
	~PlayerBase(){}
	
	// player shapes
	
	ofxBox2dCircle					headanchor;
	ofxBox2dRect					bodyanchor[5];
	ofxBox2dPolygon					skateanchor;		  
	vector<ofxBox2dJoint>			bodywojoints;			 

	ofxBox2dCircle					headanchorshadow;
	ofxBox2dRect					bodyanchorshadow[5];
	ofxBox2dPolygon					skateanchorshadow;		  
	vector<ofxBox2dJoint>			bodywojointsshadow;			 
	
	
	virtual void setup(TemporaryBabel2D *app);
	virtual void buildplayer();
	virtual void update();
	virtual void draw();
	virtual void keyPressed(int key);
	virtual void keyReleased(int key);
	
	
	
	bool getLives(){
		return lives;
	}
	
	void setDtInfluence(float dt) {
		playerDtinfluence = dt;	
	}
	float getDtInfluence( ) {
		return playerDtinfluence;	
	}
	int getPlayermode(){
		return playermode;
	}
	void damagePlayer(float amt){
		playerenergy-=amt;
		boundPlayerEnergy();
	}
	void healPlayer(float amt){
		playerenergy+=amt;
		boundPlayerEnergy();
	}
	
	void boundPlayerEnergy(){
		if(playerenergy < 0){
			playerenergy = 0;
			lives = false;
		}
		if(playerenergy > 1){
			playerenergy = 1;
			lives = true;
		}
	}
	
};




