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
    
    // No need to do anything if no image is present in the canvas
    if (listOfImages.size() > 0) {
        
        // This loop performs operations on each image present in the list of images
        for (int i = 0; i < listOfImages.size(); i++) {
            
            ofPushMatrix();
            
            // Perform manipulations on the current image
            ofTranslate(listOfImages[i].imagePosition); // translate to new position
            ofRotateDeg(listOfImages[i].angle, 0, 0, 1); // rotate by new angle
            ofScale(listOfImages[i].xScale, listOfImages[i].yScale); // scale my new factors
            listOfImages[i].image.draw(glm::vec3(0, 0, 0)); // draw the image on the canvas
            
            // Drawing a border around the selected image
            if(isAnySelected && currentSelectedImageIndex == i) {
                ofPath path;
                int borderWidth = 5; // border width for selected image
                path.setColor(ofColor::black); // border color
                path.setFilled(false); // making hollow square
                path.setStrokeWidth(5);
                path.rectangle(0, 0, listOfImages[currentSelectedImageIndex].width, listOfImages[currentSelectedImageIndex].height);
                path.draw();
            }

            ofPopMatrix();
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    // Save the image to the disk
    if (key == 'S' || key == 's') {
        if (listOfImages.size() > 0) {
            saveImageToDisk();
        }
    }
    
    // Toggle full-screen mode
    if (key == 'F' || key == 'f') {
        ofToggleFullscreen();
    }
    
    // Go into rotate mode
    if (key == 'R' || key == 'r') {
        rotateMode = true;
        uniformScaleMode = false;
        nonUniformScaleMode = false;
    }
    
    // Go into uniform scaling mode
    if (key == 'U' || key == 'u') {
        uniformScaleMode = true;
        rotateMode = false;
        nonUniformScaleMode = false;
    }
    
    // Go into non-uniform scaling mode
    if (key == 'N' || key == 'n') {
        uniformScaleMode = false;
        rotateMode = false;
        nonUniformScaleMode = true;
    }
    
    // Quit other modes and go back to translate mode
    if (key == 'Q' || key == 'q') {
        rotateMode = false;
        uniformScaleMode = false;
        nonUniformScaleMode = false;
    }
    
    // Delete selected image
    if( key == OF_KEY_BACKSPACE || key == OF_KEY_DEL ) {
        if (listOfImages.size() > 0) {
            listOfImages.erase(listOfImages.begin() + currentSelectedImageIndex);
            
            currentSelectedImageIndex--;
            if (currentSelectedImageIndex < 0) {
                currentSelectedImageIndex = 0;
            }
        }
    }
    
    // Cycle selected image down the image stack
    customImage temp;
    if (key == OF_KEY_DOWN) {
        if (listOfImages.size() > 0 && currentSelectedImageIndex > 0) {
            temp = listOfImages[currentSelectedImageIndex];
            listOfImages[currentSelectedImageIndex] = listOfImages[currentSelectedImageIndex - 1];
            listOfImages[currentSelectedImageIndex - 1] = temp;
            currentSelectedImageIndex--;
        }
    }
    
    // Cycle selected image up the image stack
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
    
    mousePos = glm :: vec3(x, y , 0);
    glm::vec3 delta = mousePos - lastMouse; // difference between the old mouse position and the new mouse position
    
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
    glm:: vec3 pos(x, y, 0);
    isAnySelected = false;
    for (int i = 0; i < listOfImages.size(); i++) {
        listOfImages[i].isSelected = false;
        customImage myImage = listOfImages[i];
        
        glm::mat4 m = glm::translate(glm::mat4(1.0), glm::vec3(listOfImages[i].imagePosition));
        glm::mat4 M = glm::rotate(m, glm::radians(listOfImages[i].angle), glm::vec3(0, 0, 1));
        
        glm::vec3 p2 = glm::inverse(M) *  glm::vec4(pos, 1);
        
        // check if the current mouse pointer lies inside any image
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


