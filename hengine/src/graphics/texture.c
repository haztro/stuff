#include "graphics/texture.h"
#include "graphics/lodepng.h"

#include <stdlib.h>
#include <stdio.h>

int TEXTURE_Init(Texture *texture, const char *filename);

Texture* TEXTURE_Create(const char *filename)
{
	Texture *texture = (Texture *) malloc(sizeof(Texture));
	TEXTURE_Init(texture, filename);

	return texture;
}

int TEXTURE_Init(Texture *texture, const char *filename)
{
	glGenTextures(1, &(texture->id));

	// Bind the texture object
	glBindTexture(GL_TEXTURE_2D, texture->id);


	unsigned int error;
	unsigned char *image;
	unsigned int width, height;

  	error = lodepng_decode32_file(&image, &width, &height, filename);
  	if (error) 
  		printf("Error loading image: %s, %u: %s\n", filename, error, lodepng_error_text(error));

  	// Flip image
  	unsigned char *image_flipped = (unsigned char *) malloc(width * 4 * height);

  	for (int y = 0; y < height; y++)
  	{
  		for (int x = 0; x < width; x++)
  		{
  			int dst_i = (y * width + x) * 4;
  			int src_i = ((height - y - 1) * width + x) * 4;
  			image_flipped[dst_i] = image[src_i];
  			image_flipped[dst_i + 1] = image[src_i + 1];
  			image_flipped[dst_i + 2] = image[src_i + 2];
  			image_flipped[dst_i + 3] = image[src_i + 3];
  		}
  	}

	// Upload the pixels to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_flipped);

  	free(image);
  	free(image_flipped);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Generate the mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);

	texture->width = width;
	texture->height = height;

	return 0;
}