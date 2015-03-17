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
	void begin();
	void end();
    
    void setPixelsRef(ofPtr<ofPixels> pixels);

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
    
    void setControlPoint(int index, ofPoint position);
    ofPoint getControlPoint(int index);
        
    void setCornerSensibility(float sensibility);
    float getCornerSensibility();

private:
    int _u_points;
    int _v_points;
    
	void drawCtlPoints(ofMatrix4x4& mat);

	void updateMesh();
    void resetMesh();
    void updateQuad();
    void resetQuad();
    
	ofPoint mouseToMesh(int x, int y);
    ofPoint meshToScreen(ofPoint meshPoint);

	int x, y;
	int  width; //width of the quad to work with
	int	 height; // height of the quad to work with

    /*
    ofPoint controlPoints[OFXNURBS_U_POINTS][OFXNURBS_V_POINTS];
    ofPoint controlPointsQW[OFXNURBS_U_POINTS][OFXNURBS_V_POINTS];
	*/
    vector< vector<ofPoint> >  controlPoints;
    ofPoint corners[4];
		
	GLenum drawMode;
    ofPtr<ofPixels> pix;

	GLuint textures[1];

	bool editMode;
	int editSelection;
    bool editCorners;
    int editMeshSelection;
    int editQuadSelection;
    
    
    ofPoint src[4];
    
    ofMatrix4x4 quadWarping;
    ofMatrix4x4 quadWarpingT;
    ofMatrix4x4 quadWarpingI;
    
    
};