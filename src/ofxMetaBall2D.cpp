//
//  ofxMetaBall2D.cpp
//
//  Created by azuremous on 8/20/14.
//
//

#include "ofxMetaBall2D.h"

//--------------------------------------------------------------
ofxMetaBall2D::ofxMetaBall2D():threshold(0.30){}

//--------------------------------------------------------------
bool ofxMetaBall2D::setup(float w, float h){
    renderTex.allocate(w ,h,GL_RGBA);
    metaballTex.allocate(w, h, GL_RGBA);
    realMetaballTex.allocate(w, h, GL_RGBA, 8);
    
    renderTex.begin();
    ofClear(255);
    renderTex.end();
    
    metaballTex.begin();
    ofClear(255);
    metaballTex.end();
    
    realMetaballTex.begin();
    ofClear(255);
    realMetaballTex.end();
    
    string fragmentShader;
    string  vertexShader;
    
    fragmentShader = "";
    vertexShader = "";
    
    fragmentShader += STRINGIFY(
                                uniform sampler2DRect texture;
                                uniform float red;
                                uniform float green;
                                uniform float blue;
                                
                                void main() {
                                    
                                    vec4 tex = texture2DRect ( texture, gl_TexCoord[0].st );
                                    if (tex.r + tex.g + tex.b != 3.0) {
                                        tex.r = red;
                                        tex.g = green;
                                        tex.b = blue;
                                    }
                                    
                                    //tex = 1.0 - tex;
                                    
                                    gl_FragColor = vec4(tex.r , tex.g, tex.b, 1.0);
                                }
                                );
    
    vertexShader += STRINGIFY(
                              
                              void main(){
                                  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
                                  gl_PointSize = 0.0;
                                  gl_TexCoord[0] = gl_MultiTexCoord0;
                                  gl_FrontColor = gl_Color;
                              }
                              );
    
    shader.unload();
    shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader);
    shader.setupShaderFromSource(GL_VERTEX_SHADER, vertexShader);
    if(ofIsGLProgrammableRenderer()){
		shader.bindDefaults();
	}
    return shader.linkProgram();
}

//--------------------------------------------------------------
void ofxMetaBall2D::setThreshold(float t){
    threshold = t;
}

//--------------------------------------------------------------
void ofxMetaBall2D::setColor(float _r, float _g, float _b){
    
    if (_r > 1.0) { r = ofMap(_r, 0, 255, 0.0, 1.0); }
    else{ r = _r; }
    if (_g > 1.0) { g = ofMap(_g, 0, 255, 0.0, 1.0); }
    else{ _g = _g; }
    if (_b > 1.0) { b = ofMap(_b, 0, 255, 0.0, 1.0); }
    else{ b = _b; }
    
}

//--------------------------------------------------------------
void ofxMetaBall2D::begin(){
    renderTex.begin();
    ofClear(0,0,0);
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    glEnable(GL_BLEND);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
}

//--------------------------------------------------------------
void ofxMetaBall2D::end(){
    glPopAttrib();
    renderTex.end();
    
    metaballTex.begin();
    
    ofClear(255);
    ofPushStyle();
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, threshold);
    ofSetColor(0);
    renderTex.draw(0,0);
    glDisable(GL_ALPHA_TEST);
    ofPopStyle();
    
    metaballTex.end();
    
    realMetaballTex.begin();
    ofTexture &t = metaballTex.getTextureReference();
    
    t.bind();
    shader.begin();
    
    shader.setUniform1f("red", r);
    shader.setUniform1f("green", g);
    shader.setUniform1f("blue", b);
    
    t.draw(0, 0);
    shader.end();
    t.unbind();
    realMetaballTex.end();
}

//--------------------------------------------------------------
void ofxMetaBall2D::draw(float x, float y){
    realMetaballTex.draw(x, y);
}

//--------------------------------------------------------------
ofTexture ofxMetaBall2D::createBall(int r, int g, int b, float radius){
    
    ofImage ptrImg;
    ptrImg.allocate(radius*2, radius*2, OF_IMAGE_COLOR_ALPHA);
    
    for (float y=0;  y < radius*2; y++) {
        for (float x=0; x < radius*2; x++) {
            
            float dist = ofDist(x, y, radius, radius);
            float alpha = ofMap(dist, radius / 2.0, radius, 255, 0,true);
            ofColor c;
            c.set(r, g, b, alpha);
            ptrImg.setColor(x,y,c);
        }
    }
    ptrImg.reloadTexture();
    
    return ptrImg.getTextureReference();
    
}

//--------------------------------------------------------------
ofTexture ofxMetaBall2D::createRect(int r, int g, int b,int dt, int h){
    
    int w = 255 * dt;
    ofTexture t;
    t.allocate(w,h,GL_RGBA);
    
    unsigned char 	* colorAlphaPixels;
    colorAlphaPixels	= new unsigned char [w*h*4];
    
    // color alpha pixels, use w and h to control red and green
	for (int i = 0; i < w; i++){
		for (int j = 0; j < h; j++){
			colorAlphaPixels[(j*w+i)*4 + 0] = r;	// r
			colorAlphaPixels[(j*w+i)*4 + 1] = g;	// g
			colorAlphaPixels[(j*w+i)*4 + 2] = b; 	// b
            int a;
            float t = ofMap(i, 0, w, 0.0, 1.0);
            float tx = (1.0-t);
            if (i <= (w-1) /2) a = ofMap(i, 0, (w-1)/2, 0, 255) * (1.0-(tx * tx * tx * tx)) ;
            if (i > (w-1) /2) a = ofMap(i, (w-1)/2, w, 255, 0, true) * (1.0-(tx * tx * tx * tx));
			colorAlphaPixels[(j*w+i)*4 + 3] = a; 	// alpha
		}
	}
    
    t.loadData(colorAlphaPixels, w,h, GL_RGBA);
    
    return t;
};
