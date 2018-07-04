#include<stdio.h>
#include<stdlib.h>

typedef unsigned char BYTE;

struct _JFIFHeader{
	BYTE SOI[2];          /* 00h  Start of Image Marker     */
	BYTE APP0[2];         /* 02h  Application Use Marker    */
	BYTE Length[2];       /* 04h  Length of APP0 Field      */
	BYTE Identifier[5];   /* 06h  "JFIF" (zero terminated) Id String */
	BYTE Version[2];      /* 07h  JFIF Format Revision      */
	BYTE Units;           /* 09h  Units used for Resolution */
	BYTE Xdensity[2];     /* 0Ah  Horizontal Resolution     */
	BYTE Ydensity[2];     /* 0Ch  Vertical Resolution       */
	BYTE XThumbnail;      /* 0Eh  Horizontal Pixel Count    */
	BYTE YThumbnail;      /* 0Fh  Vertical Pixel Count      */
};

int main(int argc, char *argv[]){
	FILE *fpi, *fpt;
	int i = 0, j = 0;
	unsigned char c = 0;
	unsigned short x = 0;
	int f_sz;
	struct _JFIFHeader jh;

	if(argc < 2){
		perror("File not given\n");
		exit(1);
	}
	if((fpi = fopen(argv[1],"rb")) == NULL){
		perror("Input File opening Error!\n");
		exit(1);
	}
	fseek(fpi, 0, SEEK_END);
	f_sz = ftell(fpi);
	fseek(fpi, 0, SEEK_SET);

	if((fpt = fopen("img.txt","w+")) == NULL){
		perror("Output File opening Error!\n");
		exit(1);
	}

	fread(&jh, sizeof(jh), 1, fpi);

	printf("Start of Image:\t%02x %02x\n", jh.SOI[0], jh.SOI[1]);
	printf("Application Use Marker:\t%02x %02x\n", jh.APP0[0], jh.APP0[1]);
	printf("Length(APP0):\t%02x %02x\n", jh.Length[0], jh.Length[1]);
	printf("JFIF Identifier:\t%02x %02x %02x %02x %02x\n", jh.Identifier[0], jh.Identifier[1], jh.Identifier[2], jh.Identifier[3], jh.Identifier[4]);
	printf("JFIF Format Revision:\t%02x %02x\n", jh.Version[0], jh.Version[1]);
	printf("Resolution Units:\t%02x\n", jh.Units);
	printf("Horizontal Resolution:\t%02x %02x\n", jh.Xdensity[0], jh.Xdensity[1]);
	printf("Vertical Resolution:\t%02x %02x\n", jh.Ydensity[0], jh.Ydensity[1]);
	printf("Horizontal Pixel Count:\t%02x\n", jh.XThumbnail);
	printf("Vertical Pixel Count:\t%02x\n", jh.YThumbnail);

	fseek(fpi, 0, SEEK_SET);
	while(i < f_sz){
		fread(&c, 1, 1, fpi);
		++i, ++j;
		x = (x << 8) || c;
		if(c == 0xff)
			fprintf(fpt,"...");
		if(j < 16){
			fprintf(fpt,"%02x ",c);
		}else{
			fprintf(fpt,"%02x\n",c);
			j = 0;
		}
	}

	fclose(fpi);
	fclose(fpt);

	printf("%d\n",f_sz);
	return 0;
}
