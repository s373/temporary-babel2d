/*
 *  MiniChronos.h
 *  TemporaryBabel2D_000
 *
 *  Created by André Sier on 20130327.
 *  Copyright 2013 s373.net/x. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"


class MiniChronos {

public:
		

	MiniChronos(){}		~MiniChronos(){}
	
	
	void setup(){
		
		currentYearIni = UnixToS373Years(ofGetUnixTime());
		currentYear = UnixToS373Years(ofGetUnixTime());
		currentYearEllapsed = currentYear - currentYearIni;
		currentYearMax = UnixToS373Years(ofGetUnixTime()+3600.0);//86400.0);//604800.0);//31536000.0);
		font.loadFont("tb2dmedia/spincycle_3d_tt.ttf",12);
		cyearini = (float) (currentYearIni*1e5);
		cyear = (float)    (currentYear*1e5);
		cyearmax = (float) (currentYearMax*1e5);
		cbegin = ofGetElapsedTimef();
		cellapsed = ofGetElapsedTimef();
		cellapsedmax = ofGetElapsedTimef()+3600.0;//+60
		
		
		secondsThen = ofGetElapsedTimef()+1;
		drawString = "";
		quit = false;
		blink = 0; colorstate = true;
		
	}
	
	
	void update(){
		if(ofGetElapsedTimef()>secondsThen){		
			
			secondsThen = ofGetElapsedTimef();
			
			currentYear = UnixToS373Years(ofGetUnixTime());
			currentYearEllapsed = currentYear - currentYearIni;
			cellapsed = ofGetElapsedTimef();
			
			float longevity = getLongevity();
			if( ofMap(cellapsed, cellapsedmax, cbegin, 0, 1) < 0.21) {
				if(longevity>0.20){
					blink = 120;
				} else if(longevity > 0.16){
					blink = 60;
				} else if(longevity > 0.12){
					blink = 30;
				} else if(longevity > 0.08210){
					blink = (int)ofRandom(2, 16);
				} else if(longevity > 0.07769){
					blink = (int)ofRandom(1, 8);
				} else if(longevity > 0.0){
					blink = 5;
				} else {
					blink = 2;
				}
			}
			
						
			drawString = "      t e m p o r a r y - b a b e l 2 d\n"+ofToHex(currentYear)+"    "+ofToString(currentYear,7)+"    "+ofToString(currentYearEllapsed,7);
			
			if(cellapsed > (cellapsedmax+25.19771969e-1)){
				cout << " time's up " << endl;
				quit = true;
			}
		}
	}
	
	
	void draw( float lx, float ly, float dx, float dy){		
		chronosDrawCartesian1d(lx, ly, dx, dy);
	}
	
	float getLongevity(){
			return ofMap(cellapsed, cellapsedmax, cbegin, 0, 1);
	}
	
	double getCurrentYear(){
		return currentYear;
	}
	double getCurrentYearIni(){
		return currentYearIni;
	}
	double getCurrentYearEllapsed(){
		return currentYearEllapsed;
	}
	
	bool timesup(){
		return quit;
	}

	
private:
	
	double		currentYear;
	double		currentYearIni;
	double		currentYearEllapsed;
	double		currentYearMax;
	float		cyear, cyearini,cyearmax;
	float		cellapsed, cellapsedmax, cbegin;			
	float		secondsThen;	
	string		drawString;
	bool		quit;
	ofTrueTypeFont font;
	int			blink;
	bool		colorstate;
	
	
	void chronosDrawCartesian1d (float lx, float ly, float dx, float dy){
			
		ofNoFill();
		ofSetHexColor(0xcccccc);
		ofRect(lx, ly, dx, dy); 
		
		ofFill();
		if(blink>0){ 
			if(colorstate) ofSetColor(255, 100); else ofSetColor(0,0,255, 100); if(ofGetFrameNum()%blink==0) colorstate^=true; }
		else {
			ofSetColor(255, 100);
		}
		
		ofRect(lx+2, ly+2, ofMap(cellapsed, cellapsedmax, cbegin, 1, dx-2), dy-4);
		if(!colorstate) ofSetColor(1,9,69*77); else //2.61); else
		ofSetColor(255);
//		ofDrawBitmapString(drawString, lx + 5, ly + dy*0.5 + 5 );
		font.drawString(drawString, lx + 5, ly + dy*0.5 + 5 -16);
		
	}
	
	
	double UnixToS373Years(unsigned int unixTime){
		
		
		double s373Years = (double) unixTime / 31536000.0; // dividir pelo #segundos de um ano
		
		s373Years += 1970.0; // adicionar 1970
		
		return s373Years;
		
		
		/*
					 1 minuto = 60 segundos
					 1 hora = 3600 segundos		 
					 1 dia = 86400 segundos
					
				1 semana = 604800 segundos
				30 dias = 2592000 segundos
				1 ano  = 31536000 segundos
		 
		 */
		
		
		
		/*
				 unix time example
				 
				 1365723433
				 
				 / 
				 
				 num segundos num ano 31536000
				 
				 =
				 
				 43.306805968
				 
				 + 1970
				 
				 = 2013.306805968
				 
		 */
		
		
	}
	
	unsigned int S373YearsToUnix(double uunniiTime){
		// 
	}

};