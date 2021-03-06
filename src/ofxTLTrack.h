/**
 * ofxTimeline
 *	
 * Copyright (c) 2011-2012 James George
 * http://jamesgeorge.org + http://flightphase.com
 * http://github.com/obviousjim + http://github.com/flightphase 
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * ----------------------
 *
 * ofxTimeline 
 * Lightweight SDK for creating graphic timeline tools in openFrameworks
 *
 * Developed with support of YCAM InterLab
 */

#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxTween.h"
#include "ofRange.h"
#include "ofxTLEvents.h"
#include <set>
	
class ofxTimeline; 
class ofxTLTrack 
{
  public:
	ofxTLTrack();
	virtual ~ofxTLTrack();
	
	virtual void setup();
    void _draw(); //calls draw() internally plus some universal stuff
    //override this in your sublcass
	virtual void update(){}; //events should be triggered from here
	virtual void draw(){};
    
    //draw modal content is called after the main draw() call
    //override this if you want to draw content that can show up on top of other timelines
	//useful in conjunction with timeline->presentedModalContent(this)
    //for showing big controls, ie the tweener window
	virtual void drawModalContent(){}; 
	
	virtual void enable();
	virtual void disable();
	virtual bool isEnabled();
    
    virtual bool isActive();
	virtual bool hasFocus();
	virtual bool isHovering();
	
    //managed by the page object, do not set yourself
	virtual void setDrawRect(ofRectangle bounds);
//	virtual void offsetDrawRect(ofVec2f offset);
	float getBottomEdge();
    
    //returns the screenspace position of the elements bounds, not including header and footer
	virtual ofRectangle getDrawRect();
	
    //saving and loading
    //Could potentially be moved out of the main track to keyframe
    virtual string getXMLFilePath();
    virtual string getXMLFileName();
    
	virtual void setXMLFileName(string filename);
	
    //parent wrappers that call virtual versions implemented by subclasses
    bool _mousePressed(ofMouseEventArgs& args, long millis);
    void _mouseMoved(ofMouseEventArgs& args, long millis);
    void _mouseDragged(ofMouseEventArgs& args, long millis);
    void _mouseReleased(ofMouseEventArgs& args, long millis);
    
	//standard events to be implement in subclasses
	//mousePressed returns true if the click activated the track in a way that should maintain attention
	//for instance, selecting a keyframe
    virtual bool mousePressed(ofMouseEventArgs& args, long millis){return false;}
	virtual void mouseMoved(ofMouseEventArgs& args, long millis){}
    virtual void mouseDragged(ofMouseEventArgs& args, long millis){}; 
	virtual void mouseReleased(ofMouseEventArgs& args, long mllis){};
	
	virtual void keyPressed(ofKeyEventArgs& args){};
	virtual void nudgeBy(ofVec2f nudgePercent){};
	
    //Triggered by the page object based on user interaction, Only calls when the focus actually changes
    virtual void gainedFocus();
    virtual void lostFocus();
    
    //This is triggered when a shift+click+drag rectangle finish on the timeline
    //it passes normalized 0-1 ranges for time (x) and value (y)
    //It's the responsability of the ofxTLTrack to respond appropriately
    //and select the relevant elements under the track
    virtual void regionSelected(ofLongRange timeRange, ofRange valueRange){};
    
	//copy+paste
	virtual string copyRequest(){return "";};
	virtual string cutRequest(){return "";};
	virtual void pasteSent(string pasteboard){};
	virtual void selectAll(){};
	virtual void unselectAll(){};
	
    //returns the number of selected items
    //this used to determine two things:
    //1 Should an incoming click create a new item or remove a multiple selection? 
    //If the timeline has more than 1 item selected incoming clicks that don't hit anything will deselect all
    //otherwise they'll create a new value on that track
    //2 Can this track be modified by the current event? If there are selected items
    //the this track's state will be stored in the undo buffer.
    virtual int getSelectedItemCount(){ return 0; };
    
    //undo
    virtual string getXMLRepresentation(){return "";};
    virtual void loadFromXMLRepresentation(string rep){};

	//zoom events
	virtual void zoomStarted(ofxTLZoomEventArgs& args);
	virtual void zoomDragged(ofxTLZoomEventArgs& args);
	virtual void zoomEnded(ofxTLZoomEventArgs& args);
    //managed by the zoom call backs and initialized by the Page upon creation
	void setZoomBounds(ofRange zoomBoundsPercent);

	virtual void save(){};
	virtual void load(){};
	virtual void clear(){};
	
    //override this if your track can be used as the timeControl
    //for example video and audio tracks implement this
    virtual bool isPlaying(){ return false; }
    
	//add any points (in screenspace x) that should be snapped to
	virtual void getSnappingPoints(std::set<long>& points){};
	
	ofxTimeline* getTimeline();
	//set by the timeline it's self, no need to call this yourself
	void setTimeline(ofxTimeline* timeline); 
	string getName();
	void setName(string name);
    
    //optional display name that track headers will use
    virtual void setDisplayName(string name);
    virtual string getDisplayName();
    
    virtual string getTrackType();
    
	bool getCreatedByTimeline();
	void setCreatedByTimeline(bool created);
	ofxTLEvents& events(); //convenience wrapper for timeline events;
    
  protected:
	
	ofxTimeline* timeline;
	bool enabled;
	//responsability of subclass to react to this on draw, is cleared by super class each frame
	bool viewIsDirty;
	
	virtual bool pointInScreenBounds(ofVec2f screenpoint);
	virtual float screenXtoNormalizedX(float x);
	virtual float normalizedXtoScreenX(float x);
	virtual float screenXtoNormalizedX(float x, ofRange outputRange);
	virtual float normalizedXtoScreenX(float x, ofRange inputRange);

    virtual long screenXToMillis(float x);    
    virtual float millisToScreenX(long millis);

    //TODO: check if these are used any more
	virtual int indexForScreenX(int mouseX);
	virtual int screenXForIndex(int index);
	virtual int indexForScreenX(int screenX, int durationInFrames);
	virtual int screenXForIndex(int index, int durationInFrames);
	
	virtual float screenXForTime(float time);
	virtual float timeForScreenX(float screenX);
	virtual float screenXForTime(float time, float durationInSeconds);
	virtual float timeForScreenX(float screenX, float durationInSeconds);
		
	virtual void drawRectChanged(){};
	
	virtual bool isOnScreen(float screenX);
	
	bool hover; //mouse is over the element
    bool active; //mouse is clicking on the element
	bool focused; //this element was the last one interacted with
    
	bool createdByTimeline;

	string name;
    string displayName;
	string xmlFileName;
	ofRectangle bounds;
	ofRange zoomBounds;	
};
