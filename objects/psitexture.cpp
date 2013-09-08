// psitexture.cpp

#include <cstdio>
#include "psitexture.h"

PsiTexture::PsiTexture()
{
	texture_number = -1;
}

PsiTexture::PsiTexture(const char *input, int min_filter, int mag_filter)
{
	texture_number = -1;
	OpenPsi(input, min_filter, mag_filter);
}

PsiTexture::~PsiTexture()
{
	if(texture_number != -1)
	DeleteTexture();
}

bool PsiTexture::OpenPsi(const char *input, int min_filter, int mag_filter)
{
	const char check_string[] = "Piano's Single Alpha Image Format 1.0";
	FILE *psifile = NULL;
	int i;

	if(texture_number != -1)
	DeleteTexture();

	// open psi file
	psifile = fopen(input, "rb");
	if(psifile == NULL) return false;

	// check whether file header is valid or not
	int inchar = fgetc(psifile);
	if(inchar != sizeof(check_string)-1) return false;
	char *compare_string = new char[inchar];
	fread(compare_string, 1, inchar, psifile);
	for(i = 0; i < inchar; i++)
		if(check_string[i] != compare_string[i]) return false;
	delete compare_string;

	// get image sizes
	original_size_x = 0;
	original_size_y = 0;
	extended_size_x = 0;
	extended_size_y = 0;
	inchar = fgetc(psifile);
	inchar /= 4;
	for(i = 0; i < inchar; i++)
		extended_size_x |= fgetc(psifile) << (8*i);
	for(i = 0; i < inchar; i++)
		extended_size_y |= fgetc(psifile) << (8*i);
	for(i = 0; i < inchar; i++)
		original_size_x |= fgetc(psifile) << (8*i);
	for(i = 0; i < inchar; i++)
		original_size_y |= fgetc(psifile) << (8*i);

	// copying colors to memory
	unsigned char *data = 
		new unsigned char[extended_size_x * extended_size_y * 4];
	int buffersize = original_size_x * 4;
	int skipsize = extended_size_x * 4;
	for(i = 0; i < original_size_y; i++)
		fread(data + (i*skipsize), 1, buffersize, psifile);
	fclose(psifile);

	// generating texture
	glGenTextures(1, &texture_number);
	glBindTexture(GL_TEXTURE_2D, texture_number);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, extended_size_x, extended_size_y, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, data);
	delete data;

	return true;
}

bool PsiTexture::isOpen()
{
	return texture_number != -1;
}

void PsiTexture::DeleteTexture()
{
	if(texture_number == -1) return;

	// delete texture
	glDeleteTextures(1, &texture_number);
	texture_number = -1;
}

unsigned PsiTexture::GetTextureNumber()
{
	return texture_number;
}

float PsiTexture::GetMaxXf()
{
	return float(original_size_x) / float(extended_size_x);
}

float PsiTexture::GetMaxYf()
{
	return float(original_size_y) / float(extended_size_y);
}

double PsiTexture::GetMaxXd()
{
	return double(original_size_x) / double(extended_size_x);
}

double PsiTexture::GetMaxYd()
{
	return double(original_size_y) / double(extended_size_y);
}
