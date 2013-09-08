// psitexture.h
//
// Use Piano's Single Alpha Image Format 1.0
// to texture binding

#ifndef __pi_piano_PSITEXTURE_H__
#define __pi_piano_PSITEXTURE_H__

#include <GL/glut.h>

class PsiTexture {
public:
    PsiTexture();
    PsiTexture(const char *input, int min_filter = GL_LINEAR, int mag_filter = GL_LINEAR);
    ~PsiTexture();
    bool OpenPsi(const char *input, int min_filter = GL_LINEAR, int mag_filter = GL_LINEAR);
	bool isOpen();
	void DeleteTexture();
	float GetMaxXf();
	float GetMaxYf();
	double GetMaxXd();
	double GetMaxYd();
	unsigned GetTextureNumber();
private:
	int original_size_x;
	int original_size_y;
	int extended_size_x;
	int extended_size_y;
	unsigned texture_number;
};

#endif
