#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
}

//--------------------------------------------------------------
void ofApp::update(){
    // used for animation
    // allows to change the state of the program on a frequent basis
    // gets called once every frame update - 60 times for a 60Hz frame rate
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if (listOfImages.size() > 0) {
        
        // Draw all the images from the list of images
        for (int i = 0; i < listOfImages.size(); i++) {
            ofPushMatrix();
            ofTranslate(listOfImages[i].imagePosition); // draw where dragged
            ofRotateDeg(listOfImages[i].angle, 0, 0, 1);
            ofScale(listOfImages[i].xScale, listOfImages[i].yScale);
//            ofTranslate(listOfImages[i].width/2, listOfImages[i].height/2, 0);
//            ofRotateDeg(listOfImages[i].rotation, 0, 0, 1);
//            ofTranslate(-listOfImages[i].width/2, -listOfImages[i].height/2, 0);
            
            listOfImages[i].image.draw(glm::vec3(0, 0, 0));
            if(isAnySelected) {
                if(currentSelectedImageIndex == i) {
                    ofPath path;
                    int borderWidth = 5; // selected image border width
                    path.setColor(ofColor::black);
                    path.setFilled(false);
                    path.setStrokeWidth(5);
                    path.rectangle(0, 0, listOfImages[currentSelectedImageIndex].width, listOfImages[currentSelectedImageIndex].height);
                   // path.rectangle(selectedImage.imagePosition.x + borderWidth, selectedImage.imagePosition.y + borderWidth, selectedImage.width - 2*borderWidth, selectedImage.height - 2*borderWidth);
                    path.draw();
                }
            }
            
            ofPopMatrix();
//            } else {
//                listOfImages[i].image.draw(glm::vec3(0, 0, 0));
//            }
        }
        
        // Draw a border around the selected image
        // Do not draw any border if none selected
        
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    if (key == 'S' || key == 's') {
        if (listOfImages.size() > 0) {
            saveImageToDisk();
        }
    }
    
    if (key == 'F' || key == 'f') {
        ofToggleFullscreen();
    }
    
    if (key == 'R' || key == 'r') {
        rotateMode = true;
        uniformScaleMode = false;
        nonUniformScaleMode = false;
    }
    
    if (key == 'U' || key == 'u') {
        uniformScaleMode = true;
        rotateMode = false;
        nonUniformScaleMode = false;
    }
    
    if (key == 'N' || key == 'n') {
        uniformScaleMode = false;
        rotateMode = false;
        nonUniformScaleMode = true;
    }
    
    if (key == 'Q' || key == 'q') {
        rotateMode = false;
        uniformScaleMode = false;
        nonUniformScaleMode = false;
    }
    
    if( key == OF_KEY_BACKSPACE || key == OF_KEY_DEL ) {
        if (listOfImages.size() > 0) {
            listOfImages.erase(listOfImages.begin() + currentSelectedImageIndex);
            
            currentSelectedImageIndex--;
            if (currentSelectedImageIndex < 0) {
                currentSelectedImageIndex = 0;
            }
        }
    }
    
    customImage temp;
    if (key == OF_KEY_DOWN) {
        if (listOfImages.size() > 0 && currentSelectedImageIndex > 0) {
            temp = listOfImages[currentSelectedImageIndex];
            listOfImages[currentSelectedImageIndex] = listOfImages[currentSelectedImageIndex - 1];
            listOfImages[currentSelectedImageIndex - 1] = temp;
            currentSelectedImageIndex--;
        }
    }
    
    if (key == OF_KEY_UP) {
        if (listOfImages.size() > 0 && currentSelectedImageIndex < listOfImages.size() - 1) {
            temp = listOfImages[currentSelectedImageIndex];
            listOfImages[currentSelectedImageIndex] = listOfImages[currentSelectedImageIndex + 1];
            listOfImages[currentSelectedImageIndex + 1] = temp;
            currentSelectedImageIndex++;
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    if (!bDrag) return;
    
    mousePos = glm :: vec3(x, y , 0); // new mouse position
    glm::vec3 delta = mousePos - lastMouse;
//    customImage img = listOfImages[currentSelectedImageIndex];
//    float centX = img.imagePosition.x + img.width / 2;
//    float centY = img.imagePosition.y + img.height / 2;
//
//    float refX = (img.imagePosition.x + img.width);
//    float refY =  (img.imagePosition.y + img.height);
//
//    glm::vec3 nPos = glm::normalize(glm::vec3(x, y, 0) - glm::vec3(centX, centY, 0));
//    glm::vec3 nNewPos = glm::normalize(glm::vec3(refX, refY, 0) - glm::vec3(centX, centY, 0));
//
    
    if(rotateMode) {
        // rotate image
        listOfImages[currentSelectedImageIndex].angle += -delta.x;
    } else if (uniformScaleMode) {
        // scale image uniformly
        float scaleFactor = (mousePos.x/lastMouse.x);
        listOfImages[currentSelectedImageIndex].xScale = listOfImages[currentSelectedImageIndex].xScale * scaleFactor;
        listOfImages[currentSelectedImageIndex].yScale =  listOfImages[currentSelectedImageIndex].yScale * scaleFactor;
    } else if (nonUniformScaleMode) {
        // scale image non-uniformly
        listOfImages[currentSelectedImageIndex].xScale = listOfImages[currentSelectedImageIndex].xScale * (mousePos.x/lastMouse.x);
        listOfImages[currentSelectedImageIndex].yScale = listOfImages[currentSelectedImageIndex].yScale * (mousePos.y/lastMouse.y);
    } else {
        // translate image
        listOfImages[currentSelectedImageIndex].imagePosition += delta;
    }
    lastMouse = mousePos;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    lastMouse = glm::vec3 (x, y, 0);
    glm:: vec3 pos(x, y, 0); // 3d vector
    isAnySelected = false;
    for (int i = 0; i < listOfImages.size(); i++) {
        listOfImages[i].isSelected = false;
        customImage myImage = listOfImages[i];
        
        glm::mat4 m = glm::translate(glm::mat4(1.0), glm::vec3(listOfImages[i].imagePosition));
        glm::mat4 M = glm::rotate(m, glm::radians(listOfImages[i].angle), glm::vec3(0, 0, 1));
        
        glm::vec3 p2 = glm::inverse(M) *  glm::vec4(pos, 1);
        
        if (insideRectangle(p2, glm::vec3(0, 0, 0), myImage.width*myImage.xScale, myImage.height*myImage.yScale)) {
            currentSelectedImageIndex = i;
            isAnySelected = true;
            bDrag = true;
            listOfImages[i].isSelected = true;
        }
    }
}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    bDrag = false;
    lastMouse = glm::vec3 (x, y, 0);
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    if (image.load(dragInfo.files[0])) {
        bImageLoaded = true;
        imageWidth = image.getWidth();
        imageHeight = image.getHeight();
        dropLocation = glm::vec3(dragInfo.position,0);
        imagePosition = dropLocation;
        
        customImage myImage;
        myImage.width = image.getWidth();
        myImage.height = image.getHeight();
        myImage.dropLocation = glm::vec3(dragInfo.position,0);
        myImage.imagePosition = dropLocation;
        myImage.image = image;
        
        listOfImages.push_back(myImage);
    }
}

bool ofApp::insideImage(const glm::vec3 &p) {
    
    glm::mat4 m = glm::translate(glm::mat4(1.0), glm::vec3(imagePosition));
    glm::mat4 M = glm::rotate(m, glm::radians(angle), glm::vec3(0, 0, 1));
    
    glm::vec3 p2 = glm::inverse(M) *  glm::vec4(p, 1);
    if (insideRect(p2, glm::vec3(0,0,0), imageWidth, imageHeight)) return true;
    else return false;
    
}


