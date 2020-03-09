#pragma once
#include <Windows.h>
#include <iostream>

using namespace std;



int JGN_bmpWidth;
int JGN_bmpHeight;
char* JGN_bmpData;

#define JGN_RGB 0
#define JGN_RGBA 1

void JGNbmpLoad(char filename[],int A)
{
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;
	FILE* file;
	file = fopen(filename, "rb");

	if(A != 1 && A !=0)
	{ 
		cout << "Second argument must be JGN_RGB or JGN_RGBA";
	}
	else
	{
		if (file == NULL)
		{
			cout << "Cant open bmp";

		}
		else
		{
			fread(&bfh, sizeof(BITMAPFILEHEADER), 1, file);
			if (bfh.bfType != 0x4D42)
			{
				cout << "Not a valid bmp";
			}
			else
			{
				fread(&bih, sizeof(BITMAPINFOHEADER), 1, file);

				if (bih.biSizeImage == 0)
				{
					bih.biSizeImage = bih.biHeight*bih.biWidth * (3 + A);
				}


				JGN_bmpData = (char*)malloc(sizeof(char)*bih.biSizeImage);
				fseek(file, bfh.bfOffBits, SEEK_SET);


				fread(JGN_bmpData, 1, bih.biSizeImage, file);

				JGN_bmpWidth = bih.biWidth;
				JGN_bmpHeight = bih.biHeight;
				
		
				char temp;
				for (int i = 0; i < bih.biSizeImage; i += (3 + A))
				{
					temp = JGN_bmpData[i];
					JGN_bmpData[i] = JGN_bmpData[i + 2];
					JGN_bmpData[i + 2] = temp;

				}
			

			
				
				fclose(file);
			}
		}
	}
}