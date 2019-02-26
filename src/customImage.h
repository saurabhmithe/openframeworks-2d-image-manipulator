//
//  customImage.h
//  ImageDraw
//
//  Created by Saurabh Mithe on 2/22/19.
//

#include <stdio.h>
#include "ofApp.h"

class customImage {
public:
    ofImage image;
    glm::vec3 dropLocation;
    glm::vec3 imagePosition;
    ofPath border;
    int width;
    int height;
    bool isSelected;
};
