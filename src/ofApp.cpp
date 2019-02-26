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
            listOfImages[i].image.draw(glm::vec3(0, 0, 0));
            if (listOfImages[i].isSelected) currentSelectedImageIndex = i;
            ofPopMatrix();
        }
        
        // Draw a border around the selected image
        // Do not draw any border if none selected
        if(isAnySelected) {
            customImage selectedImage = listOfImages[currentSelectedImageIndex];
            ofPath path;
            int borderWidth = 5; // selected image border width
            path.setFillColor(ofColor::black);
            path.rectangle(selectedImage.imagePosition.x, selectedImage.imagePosition.y, selectedImage.width, selectedImage.height);
            path.rectangle(selectedImage.imagePosition.x + borderWidth, selectedImage.imagePosition.y + borderWidth, selectedImage.width - 2*borderWidth, selectedImage.height - 2*borderWidth);
            path.draw();
        }
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
    glm :: vec3 mousePos(x, y , 0); // new mouse position
    if (!bDrag) return;
    glm :: vec3 delta = mousePos - lastMouse;
    listOfImages[currentSelectedImageIndex].imagePosition += delta;
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
        if (insideRectangle(pos, myImage.imagePosition, myImage.width, myImage.height)) {
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
