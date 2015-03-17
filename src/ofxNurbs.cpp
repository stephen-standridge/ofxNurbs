#include "ofxNurbs.h"
#include "homography.h"
//http://www.glprogramming.com/red/chapter12.html

/*
GLfloat texpts[2][2][2] = {{{-1.0, -1.0}, {-1.0, 1.0}},
    {{1.0, -1.0}, {1.0, 1.0}}};
*/

GLfloat texpts[2][2][2] = {{{0.0, 0.0}, {0.0, 1.0}},
    {{1.0, 0.0}, {1.0, 1.0}}};



//--------------------------------------------------------------
void ofxNurbs::setup(ofPixels image, int u_points, int v_points){
    
    _u_points = ofClamp(u_points,2,10);
    _v_points = ofClamp(v_points,2,10);
    
    controlPoints.resize(_u_points);
    for(int u=0; u<controlPoints.size(); u++){
        controlPoints[u].resize(_v_points);
    }
    
    
    resetMesh();
    
    src[0]=ofPoint(0,0); src[1]=ofPoint(1,0);
    src[2]=ofPoint(0,1); src[3]=ofPoint(1,1);
    resetQuad();
    
    drawMode=GL_FILL;
    
    editMode=false;
    editCorners=true;
    editMeshSelection=0;
    editQuadSelection=0;
    
    ofUnregisterMouseEvents(this);
    
    width = image.getWidth();
    height = image.getHeight();
    
    //Check weather it's a video or a photo... it might need to be updated every time
    glGenTextures(1, &textures[0]);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width , height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.getPixels());
    
    
    setPixelsRef( ofPtr<ofPixels>(new ofPixels) );
}

//---------------------------------------------------------------------------
void ofxNurbs::setPixelsRef(ofPtr<ofPixels> pixels){
    if(pix != NULL){
        //Unload texture
    }
    pix = pixels;
}

//--------------------------------------------------------------
void ofxNurbs::updateImage(ofPixels image){
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width , height, 0, GL_RGB, GL_UNSIGNED_BYTE, image.getPixels());
    
}

//--------------------------------------------------------------
void ofxNurbs::enableEditMode(){
	ofRegisterMouseEvents(this);
	editMode=true;
}
//--------------------------------------------------------------
void ofxNurbs::disableEditMode(){
	ofUnregisterMouseEvents(this);
	editMode=false;
}
//--------------------------------------------------------------
void ofxNurbs::toggleEditMode(){
	editMode=!editMode;
	if(editMode)
		enableEditMode();
	else
		disableEditMode();
}
//--------------------------------------------------------------
bool ofxNurbs::getEditMode(){
	return editMode;
}
//--------------------------------------------------------------
void ofxNurbs::end(){
	glPopMatrix();
}
//--------------------------------------------------------------
void ofxNurbs::save(string saveFile){
	ofxXmlSettings XML;
	XML.clear();
	XML.addTag("corners");
	XML.pushTag("corners");
	
	
	for(int i =0; i<4; i++){
		int t = XML.addTag("corner");
		XML.setValue("corner:x",corners[i].x, t);
		XML.setValue("corner:y",corners[i].y, t);
	}
	XML.saveFile(saveFile);
}
//--------------------------------------------------------------
void ofxNurbs::load(string loadFile){
	ofxXmlSettings XML;
	if( !XML.loadFile(loadFile) ){
		ofLog(OF_LOG_ERROR, "ofxGLWarper : xml file not loaded. Check file path.");
	}
	
	if(!XML.tagExists("corners")){
		ofLog(OF_LOG_ERROR, "ofxGLWarper : incorrrect xml formating. No \"corners\" tag found");
		return;
	}
	XML.pushTag("corners");
	if (XML.getNumTags("corner")<4 ) {
		ofLog(OF_LOG_ERROR, "ofxGLWarper : incorrrect xml formating. less than 4 \"corner\" tags found");
		return;	
	}
	for(int i =0; i<4; i++){
		int t = XML.addTag("corner");
		XML.pushTag("corner", i);
		if (XML.tagExists("x") && XML.tagExists("y")){
			corners[i].x = XML.getValue("x", double(1.0));
			corners[i].y = XML.getValue("y", double(1.0));
		}
		XML.popTag();
	}

	//UPDATE MESH
	ofLog(OF_LOG_WARNING, "ofxGLWarper : xml file loaded OK!.");
}
//--------------------------------------------------------------
void ofxNurbs::saveToXml(ofxXmlSettings &XML){
	XML.clear();
	XML.addTag("corners");
	XML.pushTag("corners");
	
	
	for(int i =0; i<4; i++){
		int t = XML.addTag("corner");
		XML.setValue("corner:x",corners[i].x, t);
		XML.setValue("corner:y",corners[i].y, t);
	}
	XML.popTag();
}
//--------------------------------------------------------------
void ofxNurbs::loadFromXml(ofxXmlSettings &XML){	
	if(!XML.tagExists("corners")){
		ofLog(OF_LOG_ERROR, "ofxGLWarper : incorrrect xml formating. No \"corners\" tag found");
		return;
	}
	XML.pushTag("corners");
	if (XML.getNumTags("corner")<4 ) {
		ofLog(OF_LOG_ERROR, "ofxGLWarper : incorrrect xml formating. less than 4 \"corner\" tags found");
		return;
	}
	for(int i =0; i<4; i++){
		int t = XML.addTag("corner");
		XML.pushTag("corner", i);
		if (XML.tagExists("x") && XML.tagExists("y")){
			corners[i].x = XML.getValue("x", double(1.0));
			corners[i].y = XML.getValue("y", double(1.0));
		}
		XML.popTag();
	}
	XML.popTag();

	//UPDATE MESH
	ofLog(OF_LOG_WARNING, "ofxGLWarper : xml object loaded OK!.");
	
}









void ofxNurbs::draw(){
    
    ofVec3f translation((ofGetWidth()-width)*0.5,(ofGetHeight()-height)*0.5,0);
    ofVec3f scale(width,height,1);
    
    ofPushMatrix();
    ofTranslate(translation);
    ofScale(scale.x, scale.y, scale.z);
    
    
    ofPushMatrix();
    
    glMultMatrixf(quadWarpingT.getPtr());
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glEvalMesh2(drawMode, 0, OFXNURBS_GRID_RES, 0, OFXNURBS_GRID_RES);
    glFlush();
    glDisable(GL_TEXTURE_2D);
    
    ofPopMatrix();
    ofPopMatrix();
    
    
    if(editMode){
        
        ofMatrix4x4 mat;
        mat.scale(scale.x, scale.y, scale.z);
        mat.translate(translation);
        
        
        /*
        ofMatrix4x4 mat;
        mat.translate(ofVec3f(10,10,0));
        mat.scale(2, 1, 1);
        ofPoint v(1,1,1);
        v = v*mat;
        */
        drawCtlPoints(mat);
    }
    
    
    
}

//--------------------------------------------------------------
void ofxNurbs::keyPressed(int key){

    if(key=='e'){
        editMode=!editMode;
    }
    else if(key=='c'){
        editCorners=!editCorners;
    }
    else if(key=='r'){
        resetQuad();
        resetMesh();
    }
    else if(key==OF_KEY_RIGHT){
        editMeshSelection=((editMeshSelection+1)%(_u_points*_v_points));
        int selU=editMeshSelection%_u_points;
        int selV=editMeshSelection/_u_points;
        cout<<"Selected "<<editMeshSelection<<":"<<controlPoints[selU][selV]<<endl;
    }
}

//--------------------------------------------------------------
void ofxNurbs::mouseMoved(ofMouseEventArgs &args){
    float x = args.x;
    float y = args.y;
    
    if(editMode){
        if(editCorners){
            ofPoint pos(x,y);
            for (int i=0; i<OFXNURBS_TEX_CORNERS; i++) {
                if(pos.distance(meshToScreen(corners[i]))<CTLPOINT_RADIUS){
                    editQuadSelection=i;
                    return;
                }
            }
        }
        else{
            //ofPoint pos(quadWarpingI*mouseToMesh(x,y));
            ofPoint pos(x,y);
            for (int u = 0; u < _u_points; u++) {
                for (int v = 0; v < _v_points; v++) {
                    if(pos.distance(quadWarping*meshToScreen(controlPoints[u][v]))<CTLPOINT_RADIUS){
                        editMeshSelection=u+v*_u_points;
                        return;
                    }
                }
            }
        }
    }
}

//--------------------------------------------------------------
void ofxNurbs::mousePressed(ofMouseEventArgs &args){
}

//--------------------------------------------------------------
void ofxNurbs::mouseDragged(ofMouseEventArgs &args){
    int x = args.x;
    int y = args.y;
    
    if(editMode){
        if(editCorners){
            ofPoint pos(mouseToMesh(x,y));
            corners[editQuadSelection].set(pos);
            updateQuad();
        }
        else{
            ofPoint pos(quadWarpingI*mouseToMesh(x,y));
            int selU=editMeshSelection%_u_points;
            int selV=editMeshSelection/_u_points;
            controlPoints[selU][selV].set(pos);
            updateMesh();
        }
    }
}

//--------------------------------------------------------------
void ofxNurbs::mouseReleased(ofMouseEventArgs &args){
}


//--------------------------------------------------------------
void ofxNurbs::drawCtlPoints(ofMatrix4x4& mat){
    
    ofPushStyle();
    if(editCorners){
        for (int i=0; i<OFXNURBS_TEX_CORNERS; i++) {
            for(int j=i; j<OFXNURBS_TEX_CORNERS; j++){
                ofSetColor(255,200);
                ofLine(corners[i]*mat,corners[j]*mat);
            }
            ofSetColor(255);
            ofFill();
            ofCircle(corners[i]*mat,0.4f*CTLPOINT_RADIUS);
            ofSetColor(255,150);
            ofNoFill();
            ofCircle(corners[i]*mat,CTLPOINT_RADIUS);
        }
        ofSetColor(0,0,255);
        ofFill();
        ofCircle(corners[editQuadSelection]*mat,0.6f*CTLPOINT_RADIUS);
    }
    else{
        for (int u=0; u<_u_points; u++) {
            for (int v=0; v<_v_points; v++) {
                ofSetColor(255,200);
                if((u+1)<_u_points){
                    ofLine(quadWarping*controlPoints[u][v]*mat,quadWarping*controlPoints[u+1][v]*mat);
                }
                if((v+1)<_v_points)
                    ofLine(quadWarping*controlPoints[u][v]*mat,quadWarping*controlPoints[u][v+1]*mat);
                
                ofSetColor(255);
                ofFill();
                ofCircle(quadWarping*controlPoints[u][v]*mat,0.2f*CTLPOINT_RADIUS);
                ofSetColor(255,150);
                ofNoFill();
                ofCircle(quadWarping*controlPoints[u][v]*mat,CTLPOINT_RADIUS);
            }
        }
        int selU=editMeshSelection%_u_points;
        int selV=editMeshSelection/_u_points;
        ofSetColor(255,0,0);
        ofFill();
        ofCircle(quadWarping*controlPoints[selU][selV]*mat,0.4f*CTLPOINT_RADIUS);
    }
    ofPopStyle();
}

//--------------------------------------------------------------
void ofxNurbs::updateMesh(){

    GLfloat* ctlpoints = new GLfloat[_u_points*_v_points*OFXNURBS_POINT_DIM];
    for (int u = 0; u < _u_points; u++) {
        for (int v = 0; v < _v_points; v++) {
            ctlpoints[(v*_u_points+u)*OFXNURBS_POINT_DIM + 0] = controlPoints[u][v].x;
            ctlpoints[(v*_u_points+u)*OFXNURBS_POINT_DIM + 1] = controlPoints[u][v].y;
            ctlpoints[(v*_u_points+u)*OFXNURBS_POINT_DIM + 2] = controlPoints[u][v].z;
        }
    }

 
    //MESH
    glMap2f(GL_MAP2_VERTEX_3,
            0, 1, _u_points*OFXNURBS_POINT_DIM, _v_points,
            0, 1, OFXNURBS_POINT_DIM, _u_points,
            ctlpoints);
 
    glMap2f(GL_MAP2_TEXTURE_COORD_2,
            0, 1, OFXNURBS_TEX_DIM, OFXNURBS_TEX_POINTS,
            0, 1, OFXNURBS_TEX_POINTS*OFXNURBS_TEX_DIM, OFXNURBS_TEX_POINTS,
            &texpts[0][0][0]);
    
    glEnable(GL_MAP2_TEXTURE_COORD_2);
    glEnable(GL_MAP2_VERTEX_3);
    glMapGrid2f(OFXNURBS_GRID_RES, 0.0, 1.0, OFXNURBS_GRID_RES, 0.0, 1.0);
    glShadeModel(GL_FLAT);
}

//--------------------------------------------------------------
void ofxNurbs::resetMesh(){
    for (int u = 0; u < _u_points; u++) {
        for (int v = 0; v < _v_points; v++) {
            //controlPoints[u][v].set(ofMap(u,0,_u_points-1,-1.0f,1.0f),ofMap(v,0,_v_points-1,-1.0f,1.0f),0.0f);
            controlPoints[u][v].set(ofMap(u,0,_u_points-1,0.0f,1.0f),ofMap(v,0,_v_points-1,0.0f,1.0f),0.0f);
        }
    }
    updateMesh();
}

//--------------------------------------------------------------
void ofxNurbs::updateQuad(){
    quadWarping=findHomography(src,corners);
    quadWarpingT=ofMatrix4x4::getTransposedOf(quadWarping);
    quadWarpingI=ofMatrix4x4::getInverseOf(quadWarping);
}

//--------------------------------------------------------------
void ofxNurbs::resetQuad(){
    corners[0]=ofPoint(0,0); corners[1]=ofPoint(1,0);
    corners[2]=ofPoint(0,1); corners[3]=ofPoint(1,1);
    
    quadWarping.makeIdentityMatrix();
    quadWarpingT.makeIdentityMatrix();
    quadWarpingI.makeIdentityMatrix();
}

//--------------------------------------------------------------
ofPoint ofxNurbs::mouseToMesh(int x, int y){
    /*
    float xRatio=0.5*ofGetWidth()/width;
    float yRatio=0.5*ofGetHeight()/height;
    
    return ofPoint(ofMap(x,0,ofGetWidth(),-xRatio,xRatio),ofMap(y,0,ofGetHeight(),-yRatio,yRatio));
     */
    
    x = (float)x - (float)(ofGetWidth()-width)*0.5;
    y = (float)y - (float)(ofGetHeight()-height)*0.5;
    
    
    return ofPoint((float)x/width, (float)y/height);
}

//--------------------------------------------------------------
ofPoint ofxNurbs::meshToScreen(ofPoint meshPoint){
    return ofPoint(meshPoint.x*width+(ofGetWidth()-width)*0.5, meshPoint.y*height+(ofGetHeight()-height)*0.5);
}