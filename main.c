#include <stdio.h>
#include <math.h>

#include "model.h"
#include "tga.h"
#define _CRT_SECURE_NO_WARNINGS
void swap(int *a, int *b);
int iabs(int a);

/*
* Using Digital Differential Analyzer algorihm
* to draw interval connecting (x0, y0) with (x1, y1)
* on image using color
*/
void line (tgaImage *image, 
           int x0, int y0,
           int x1, int y1,
           tgaColor color);

void meshgrid(tgaImage *image, Model *model) {
	 int i, j; tgaColor white = tgaRGB(255, 255, 255);
	 for (i = 0; i < model->nface; ++i) {
		 int screen_coords[3][2]; // ��������� � �������� ����������
		 for (j = 0; j < 3; ++j) {
			 Vec3 *v = &(model->vertices[model->faces[i][3 * j]]);
				 screen_coords[j][0] = ((*v)[0] + 1) * image->width / 2;
			 screen_coords[j][1] = (1 - (*v)[1]) * image->height / 2;
		}
		 // ������ 3 �����
			for (j = 0; j < 3; ++j) {
			line(image, screen_coords[j][0], screen_coords[j][1],
				screen_coords[(j + 1) % 3][0], screen_coords[(j + 1) % 3][1],
				white);
		
		}
	}	
}

int main()
{
	Model *model = loadFromObj("obj\\cat.obj");
  
	int height = 800;
    int width = 800;
    tgaImage * image = tgaNewImage(height, width, RGB);

	meshgrid(image, model);
	tgaSaveToFile(image, "out.tga");
	//tgaFreeImage(image);
	//freeModel(model);
    int i;
    //tgaColor white = tgaRGB(255, 255, 255);
    //tgaColor red = tgaRGB(255, 0, 0);
    //tgaColor blue = tgaRGB(0, 0, 255);

    tgaFreeImage(image);    
	freeModel(model);
}



void line(tgaImage *image, int x0, int y0, int x1, int y1, tgaColor color) {
	char steep = 0;
	if (abs(x0 - x1) < abs(y0 - y1)) {
		swap(&x0, &y0);
		swap(&x1, &y1);
		steep = 1;
	}
	if (x0 > x1) {
		swap(&x0, &x1);
		swap(&y0, &y1);
	}

	int errorAccumulation = 0;
	int deltaError = 2 * abs(y1 - y0);
	int y = y0;
	for (int x = x0; x <= x1; x++) {
		if (steep == 1) {
			tgaSetPixel(image, (unsigned int)y, (unsigned int)x, color);
		}
		else {
			tgaSetPixel(image, (unsigned int)x, (unsigned int)y, color);
		}
		
		errorAccumulation += deltaError;

		if (errorAccumulation > (x1 - x0)) {
			y += (y1 > y0 ? 1 : -1);
			errorAccumulation -= 2 * (x1 - x0);
		}
	}
}

void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

int iabs(int a) {
    return (a >= 0) ? a : -a;
}
