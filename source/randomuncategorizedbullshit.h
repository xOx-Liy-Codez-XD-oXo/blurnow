#ifndef RANDOMUNCATEGORIZEDBULLSHIT_H
#define RANDOMUNCATEGORIZEDBULLSHIT_H

int liy_bufPos(int x, int y, int bufwidth) {
	return x+(y*bufwidth);
}

void alphabeltCalcRightbound() {
	for (int i = 0; i < NUM_LETTERS; i++) {
		for (int j = 0; j < alphabetTricounts[i] * 3; j++) {
			alphabetRightbounds[i] = liym_max(alphabetRightbounds[i], alphabetVertpos[i][(j*3)]);
		}
		printf("%f\r", alphabetRightbounds[i]);
	}
	alphabetRightbounds[29] = 0.25f; // space width
}

int alphTableIndexFromChar(char c) {
	switch(c) {
		case 'a':
			return 0;
		case 'b':
			return 1;
		case 'c':
			return 2;
		case 'd':
			return 3;
		case 'e':
			return 4;
		case 'f':
			return 5;
		case 'g':
			return 6;
		case 'h':
			return 7;
		case 'i':
			return 8;
		case 'j':
			return 9;
		case 'k':
			return 10;
		case 'l':
			return 11;
		case 'm':
			return 12;
		case 'n':
			return 13;
		case 'o':
			return 14;
		case 'p':
			return 15;
		case 'q':
			return 16;
		case 'r':
			return 17;
		case 's':
			return 18;
		case 't':
			return 19;
		case 'u':
			return 20;
		case 'v':
			return 21;
		case 'w':
			return 22;
		case 'x':
			return 23;
		case 'y':
			return 24;
		case 'z':
			return 25;
		case '(':
			return 26;
		case ')':
			return 27;
		case '?':
			return 28;
		case ' ':
			return 29;
		case '\'':
			return 30;
		case ',':
			return 31;
		case '!':
			return 32;
		case '.':
			return 33;
		default:
			return 28;
	
	}
}

int basicStringsFrame;

//liy button
   #define LIYB_UP 0b10000000
#define LIYB_RIGHT 0b01000000
 #define LIYB_DOWN 0b00100000
 #define LIYB_LEFT 0b00010000
    #define LIYB_A 0b00001000
    #define LIYB_B 0b00000100
    #define LIYB_1 0b00000010
    #define LIYB_2 0b00000001

void dbgRefresh() {
	GX_SetCopyClear(background, 0x00ffffff);

	GX_CopyDisp(frameBuffer[fb],GX_TRUE);

	GX_DrawDone();

	VIDEO_SetBlack(false);

	VIDEO_SetNextFramebuffer(frameBuffer[fb]);
	VIDEO_Flush();
	VIDEO_WaitVSync();

	fb ^= 1;
}

void liyrub_drawScreenquad(f32 scale) {
	liy_VtxDescConfig(LV_VP | LV_TC, 0);
	GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
		GX_Position3f32(-1.0f, -1.0f, 0.0f);
		GX_TexCoord2f32(0.0f, scale);
		GX_Position3f32(1.0f, -1.0f, 0.0f);
		GX_TexCoord2f32(scale, scale);
		GX_Position3f32(1.0f, 1.0f, 0.0f);
		GX_TexCoord2f32(scale, 0.0f);
		GX_Position3f32(-1.0f, 1.0f, 0.0f);
		GX_TexCoord2f32(0.0f, 0.0f);
}

void liyrub_drawRotScreenquad(f32 scale) {
	liy_VtxDescConfig(LV_VP | LV_TC, 0);
	GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
		GX_Position3f32(-1.0f, -1.0f, 0.0f);

		GX_TexCoord2f32(scale, 1.0f);

		GX_Position3f32(1.0f, -1.0f, 0.0f);

		GX_TexCoord2f32(scale, 1.0f - scale);

		GX_Position3f32(1.0f, 1.0f, 0.0f);

		GX_TexCoord2f32(0.0f, 1.0f - scale);

		GX_Position3f32(-1.0f, 1.0f, 0.0f);

		GX_TexCoord2f32(0.0f, 1.0f);
}

#endif