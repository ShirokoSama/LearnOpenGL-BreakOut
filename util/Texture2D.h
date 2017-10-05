//
// Created by Srf on 2017/10/5.
//

#ifndef BREAKOUT_TEXTURE2D_H
#define BREAKOUT_TEXTURE2D_H

#include <glad/glad.h>

class Texture2D {
public:
    GLuint ID;
    GLuint width, height; // Width and height of loaded image in pixels
    GLuint internal_format; // Format of texture object
    GLuint image_format; // Format of loaded image
    GLuint wrapS; // Wrapping mode on S axis
    GLuint wrapT; // Wrapping mode on T axis
    GLuint filterMin; // Filtering mode if texture pixels < screen pixels
    GLuint filterMax; // Filtering mode if texture pixels > screen pixels
    // Constructor (sets default texture modes)
    Texture2D();
    // Generates texture from image data
    void generate(GLuint width, GLuint height, unsigned char* data);
    // Binds the texture as the current active GL_TEXTURE_2D texture object
    void bind() const;
};


#endif //BREAKOUT_TEXTURE2D_H
