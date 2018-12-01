#ifndef PingPongBuffer_H
#define PingPongBuffer_H

#include "ofMain.h"

struct PingPongBuffer {
public:
    void allocate(int _width, int _height, int _internalformat = GL_RGBA, int _numColorBuffers = 1){
        // Allocate
        ofFbo::Settings fboSettings;
        fboSettings.width  = _width;
        fboSettings.height = _height;
        fboSettings.numColorbuffers = _numColorBuffers;
        fboSettings.useDepth = false;
        fboSettings.internalformat = _internalformat;
        fboSettings.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
        fboSettings.wrapModeHorizontal = GL_CLAMP_TO_EDGE;
        fboSettings.wrapModeVertical = GL_CLAMP_TO_EDGE;
        fboSettings.minFilter = GL_NEAREST;
        fboSettings.maxFilter = GL_NEAREST;
        for(int i = 0; i < 2; i++){
            FBOs[i].allocate(fboSettings);
        }
        
        // Clean
        clear();
        
        // Set everything to 0
        flag = 0;
        swap();
        flag = 0;
    }
    
    void swap() {
        src = &(FBOs[flag % 2]);
        dst = &(FBOs[(++flag) % 2]);
    }
    
    void clear() {
        for(int i = 0; i < 2; ++i){
            FBOs[i].begin();
            ofClear(0, 255);
            FBOs[i].end();
        }
    }
    
    ofFbo& operator[]( int n ) { return FBOs[n]; }
    
    ofFbo*   src;       // Source       ->  Ping
    ofFbo*   dst;       // Destination  ->  Pong
private:
    ofFbo   FBOs[2];    // Real addresses of ping/pong FBO´s
    int     flag;       // Integer for making a quick swap
};

#endif PingPongBuffer_H
