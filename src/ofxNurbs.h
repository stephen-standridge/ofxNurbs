#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

#define OFXNURBS_POINT_DIM      3
#define OFXNURBS_TEX_DIM		2
#define OFXNURBS_TEX_POINTS     2
#define OFXNURBS_TEX_CORNERS	4
#define OFXNURBS_GRID_RES       30

#define CTLPOINT_RADIUS     15.0f

class ofxNurbs{
public:  
    
	//void setup(int u_points, int v_points);
    void setup(ofPixels image, int u_points = 4, int v_points = 4);
    void updateImage(ofPixels image);
	void draw();
	void begin();   //Not implemented yet
	void end();     //Not implemented yet
    
    void setPixelsRef(ofPtr<ofPixels> pixels);  //Not implemented yet

	void reset();
    
	void keyPressed(int key);
	void mouseMoved(ofMouseEventArgs &args);
	void mousePressed(ofMouseEventArgs &args);
	void mouseDragged(ofMouseEventArgs &args);
	void mouseReleased(ofMouseEventArgs &args);
	
	void save(string saveFile = "nurbs.xml");
	void load(string loadFile = "nurbs.xml");
	void saveToXml(ofxXmlSettings& XML);
	void loadFromXml(ofxXmlSettings& XML);
	
	void enableEditMode();
	void disableEditMode();
	void toggleEditMode();
	bool getEditMode();
    
    void setControlPoint(int index, ofPoint position);  //Not implemented yet
    ofPoint getControlPoint(int index);                 //Not implemented yet
        
    void setCornerSensibility(float sensibility);       //Not implemented yet
    float getCornerSensibility();                       //Not implemented yet

private:
    int x, y;
    int  width; //width of the quad to work with
    int	 height; // height of the quad to work with
    
    ofPoint corners[4];
    ofPoint src[4];
    ofMatrix4x4 quadWarping;
    ofMatrix4x4 quadWarpingT;
    ofMatrix4x4 quadWarpingI;
    
    int _u_points;
    int _v_points;
    vector< vector<ofPoint> >  controlPoints;

    GLuint textures[1];
    
    GLenum drawMode;
    
    bool editMode;
    int editSelection;
    bool editCorners;
    int editMeshSelection;
    int editQuadSelection;
    
    
	void drawCtlPoints(ofMatrix4x4& mat);

	void updateMesh();
    void resetMesh();
    void updateQuad();
    void resetQuad();
    
	ofPoint mouseToMesh(int x, int y);
    ofPoint meshToScreen(ofPoint meshPoint);

    ofPtr<ofPixels> pix;
};