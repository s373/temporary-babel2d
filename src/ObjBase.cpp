/*
 *  ObjBase.cpp
 *  TemporaryBabel2D_000
 *
 *  Created by andré sier on 20130427.
 *  Copyright 2013 s373.net/x. All rights reserved.
 *
 */

#include "ObjBase.h"
#include "TemporaryBabel2D.h"
#include "PlayerBase.h"
#include "LevelBase.h"


/// OBJBase

void ObjBase::setup( TemporaryBabel2D * app, const int objecttype, const int objectmode){
	
	tb2dPtr = app;
	objtype = objecttype;
	objmode = objectmode;
	
	playerPtr = tb2dPtr->player;
	levelPtr = tb2dPtr->level;
	
	if( objmode == STATICAUDIOOBJ || objmode == LIGHTAUDIOOBJ || objectmode == EXITAUDIOOBJ ){
		doesAudio = true;
	} else {
		doesAudio = false;
	}
	
	
	
}


void ObjBase::calcFrequency(){
	
}
void ObjBase::calcVolume(const PlayerBase *player){
	
}


void ObjBase::update(){
	
}

void ObjBase::draw(){
	
}








//// objects setup


	
	
void Box2dObjects::addTriangle(b2World * world, const ofVec2f pos, const float angle, const float radius){
		
}

void Box2dObjects::addXShape(b2World * world, const ofVec2f pos, const float angle, const float radius, const int sub,const float apertureFact, const float pdensity,const  float pbounce,const float pfriction ){

	
	
}
	
	
