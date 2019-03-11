#pragma once

#include "ofMain.h"
#include <customImage.h>
#include <vector>

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    bool bImageLoaded = false;
    float imageWidth, imageHeight;
    glm :: vec3 position;
    ofImage image;
    bool insideRectangle(glm::vec3 p, glm::vec3 rp, float width, float height) {
        return (p.x > rp.x && p.x < rp.x + width && p.y > rp.y && p.y < rp.y + height);
    };
    bool bDrag = false;
    glm :: vec3 lastMouse;
    float angle = 30; // angle to rotate by
    glm :: vec3 dropLocation;
    glm :: vec3 imagePosition;
    glm :: vec3 mousePos;
    std::vector<customImage> listOfImages;
    customImage cImage;
    customImage* selectedImage;
    bool isAnySelected = false;
    int currentSelectedImageIndex;
    void saveImageToDisk() {
        ofImage saveImage;
        saveImage.grabScreen(0, 0, 1024, 768);
        saveImage.save("myPic.jpg");
    }
    bool rotateMode = false;
    bool uniformScaleMode = false;
    bool nonUniformScaleMode = false;
    
    float deltaX;
    float deltaY;
};
