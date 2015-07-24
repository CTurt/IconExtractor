#include <stdio.h>
#include <stdlib.h>

#include "nds.h"
#include "lodepng.h"

#define RAW_RED(colour) (((colour)) & 0x1f)
#define RAW_BLUE(colour) (((colour) >> 5) & 0x1f)
#define RAW_GREEN(colour) (((colour) >> 10) & 0x1f)

#define RED(colour) (((RAW_RED(colour) << 3) + (RAW_RED(colour) >> 2)))
#define BLUE(colour) (((RAW_BLUE(colour) << 3) + (RAW_BLUE(colour) >> 2)))
#define GREEN(colour) (((RAW_GREEN(colour) << 3) + (RAW_GREEN(colour) >> 2)))

int readBanner(char *filename, tNDSBanner *banner) {
	tNDSHeader header;
	
	FILE *romF = fopen(filename, "rb");
	if(!romF) return 1;
	
	fread(&header, sizeof(header), 1, romF);
	fseek(romF, header.bannerOffset, SEEK_SET);
	fread(banner, sizeof(*banner), 1, romF);
	fclose(romF);
	
	return 0;
}

void loadImage(unsigned short (*image)[32], unsigned short *palette, unsigned char *tileData) {
	int tile, pixel;
	for(tile = 0; tile < 16; tile++) {
		for(pixel = 0; pixel < 32; pixel++) {
			unsigned short a = tileData[(tile << 5) + pixel];
			
			int px = ((tile & 3) << 3) + ((pixel << 1) & 7);
			int py = ((tile >> 2) << 3) + (pixel >> 2);
			
			unsigned short upper = (a & 0xf0) >> 4;
			unsigned short lower = (a & 0x0f);
			
			if(upper != 0) image[px + 1][py] = palette[upper];
			else image[px + 1][py] = 0;
			
			if(lower != 0) image[px][py] = palette[lower];
			else image[px][py] = 0;
		}
	}
}

void DStoRGBA(unsigned short (*ds)[32], unsigned char (*rgba)[32][4]) {
	int x, y;
	for(x = 0; x < 32; x++) {
		for(y = 0; y < 32; y++) {
			unsigned short c = ds[y][x];
			
			rgba[x][y][0] = RED(c);
			rgba[x][y][1] = BLUE(c);
			rgba[x][y][2] = GREEN(c);
			rgba[x][y][3] = c ? 255 : 0;
			
			printf("%c", c ? 'O' : ' ');
		}
		printf("\n");
	}
}

int saveRGBA(unsigned char (*rgba)[32][4], char *filename) {
	unsigned char *png;
	size_t size;
	int ret = 0;
	
	unsigned int error = lodepng_encode32(&png, &size, (unsigned char *)rgba, 32, 32);
	
	if(!error) {
		if(lodepng_save_file(png, size, filename)) ret = 1;
	}
	else ret = 1;
	
	free(png);
	
	return ret;
}

int main(int argc, char **argv) {
	tNDSBanner banner;
	
	unsigned short image[32][32];
	unsigned char imageRGBA[32][32][4];
	
	if(argc < 3) {
		printf("NDS Icon Extractor\n\n");
		printf("Usage:\t");
		printf("%s input.nds output.png\n", argv[0]);
		
		return 1;
	}
	
	if(readBanner(argv[1], &banner)) {
		printf("Couldn't read ROM!\n");
		return 1;
	}
	
	loadImage(image, banner.palette, banner.icon);
	
	DStoRGBA(image, imageRGBA);
	
	if(saveRGBA(imageRGBA, argv[2])) {
		printf("Couldn't save PNG!\n");
		return 1;
	}
	
	return 0;
}
