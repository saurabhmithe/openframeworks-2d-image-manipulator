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
    float angle = 0;
    
    float xScale = 1;
    float yScale = 1;
};
