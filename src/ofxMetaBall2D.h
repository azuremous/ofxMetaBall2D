//
//  ofxMetaBall2D.h
//
//  Created by azuremous on 8/20/14.
//
//

#pragma once

#include "ofMain.h"
#define STRINGIFY(A) #A

class ofxMetaBall2D {
private:
    ofFbo renderTex;
    ofFbo metaballTex;
    ofFbo realMetaballTex;
    ofShader shader;
    
    float threshold;
    float r,g,b;
    
public:
    explicit ofxMetaBall2D();
    
    bool setup(float w, float h);
    void setThreshold(float t);
    void setColor(float _r, float _g, float _b);
    void begin();
    void end();
    void draw(float x, float y);
    void draw(float x, float y, float w, float h);
    
    ofTexture createBall(int r, int g, int b, float radius = 64.0);
    ofTexture createRect(int r, int g, int b,int dt = 4, int h = 200);
    ofTexture rawTexture() { return metaballTex.getTextureReference(); }
    ofFbo getFbo() const { return realMetaballTex; }
};
