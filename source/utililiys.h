#ifndef UTILILIYS_H
#define UTILILIYS_H

#include "colors.h"

#define pi 3.141592653589793

float fsq(float x) {
	return x * x;
}

#define printMtx(mtx) \
	printf("Mtx\r");\
	printf("%f, %f, %f\r", mtx[0][0], mtx[1][0], mtx[2][0]);\
	printf("%f, %f, %f\r", mtx[0][1], mtx[1][1], mtx[2][1]);\
	printf("%f, %f, %f\r", mtx[0][2], mtx[1][2], mtx[2][2]);\
	printf("%f, %f, %f\r", mtx[0][3], mtx[1][3], mtx[2][3]);\


#define printMtx44(mtx) \
	printf("Mtx44\r");\
	printf("%f, %f, %f, %f\r", mtx[0][0], mtx[1][0], mtx[2][0], mtx[3][0]);\
	printf("%f, %f, %f, %f\r", mtx[0][1], mtx[1][1], mtx[2][1], mtx[3][1]);\
	printf("%f, %f, %f, %f\r", mtx[0][2], mtx[1][2], mtx[2][2], mtx[3][2]);\
	printf("%f, %f, %f, %f\r", mtx[0][2], mtx[1][3], mtx[2][3], mtx[3][3]);\

static guVector xAxis = {1,0,0};
static guVector yAxis = {0,1,0};
static guVector zAxis = {0,0,1};

//liy vert
#define LV_VP 1         //0000 0000  0000 0001
#define LV_VN 2         //0000 0000  0000 0010
#define LV_TC 4         //0000 0000  0000 0100
#define LV_VC 8         //0000 0000  0000 1000
#define LV_MX 16        //0000 0000  0001 0000
#define LV_VPIDX8 32    //0000 0000  0010 0000
#define LV_VPIDX16 64   //0000 0000  0100 0000
#define LV_VNIDX8 128   //0000 0000  1000 0000
#define LV_VNIDX16 256  //0000 0001  0000 0000
#define LV_TCIDX8  512  //0000 0010  0000 0000
#define LV_TCIDX16 1024 //0000 0100  0000 0000
#define LV_TC1     2048 //0000 1000  0000 0000

void liy_VtxDescConfig(uint16_t config, int vtxfmt) {
	GX_ClearVtxDesc();
	if(config & LV_MX) { GX_SetVtxDesc(GX_VA_PTNMTXIDX, GX_DIRECT); }
	if(config & LV_VP) { GX_SetVtxDesc(GX_VA_POS, GX_DIRECT); }
	if(config & LV_VPIDX8) { GX_SetVtxDesc(GX_VA_POS, GX_INDEX8); }
	if(config & LV_VPIDX16) { GX_SetVtxDesc(GX_VA_POS, GX_INDEX16); }
	if(config & LV_VN) { GX_SetVtxDesc(GX_VA_NRM, GX_DIRECT); }
	if(config & LV_VNIDX8) { GX_SetVtxDesc(GX_VA_NRM, GX_INDEX8); }
	if(config & LV_VNIDX16) { GX_SetVtxDesc(GX_VA_NRM, GX_INDEX16); }
	if(config & LV_VC) { GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT); }
	if(config & LV_TC) { GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT); }
	if(config & LV_TC1){ GX_SetVtxDesc(GX_VA_TEX1, GX_DIRECT); }
	if(config & LV_TCIDX8) { GX_SetVtxDesc(GX_VA_TEX0, GX_INDEX8); }
	if(config & LV_TCIDX16) { GX_SetVtxDesc(GX_VA_TEX0, GX_INDEX16); }
	//attrfmts
	if(config & LV_MX) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_PTNMTXIDX, GX_INDEX8, GX_U8, 0); }
	if(config & LV_VP) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_POS, GX_POS_XYZ, GX_F32, 0); }
	if(config & LV_VPIDX8) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_POS, GX_POS_XYZ, GX_F32, 0); }
	if(config & LV_VPIDX16) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_POS, GX_POS_XYZ, GX_F32, 0); }
	if(config & LV_VN) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0); }
	if(config & LV_VNIDX8) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0); }
	if(config & LV_VNIDX16) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0); }
	if(config & LV_VC) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_CLR0, GX_CLR_RGB, GX_RGB8, 0); }
	if(config & LV_TC) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TC1){ GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TCIDX8) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TCIDX16) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0); }
}

void liy_VtxDescConfigMultitex(uint16_t config, uint8_t texcount, int vtxfmt) {
	GX_ClearVtxDesc();
	if(config & LV_MX) { GX_SetVtxDesc(GX_VA_PTNMTXIDX, GX_DIRECT); }
	if(config & LV_VP) { GX_SetVtxDesc(GX_VA_POS, GX_DIRECT); }
	if(config & LV_VPIDX8) { GX_SetVtxDesc(GX_VA_POS, GX_INDEX8); }
	if(config & LV_VPIDX16) { GX_SetVtxDesc(GX_VA_POS, GX_INDEX16); }
	if(config & LV_VN) { GX_SetVtxDesc(GX_VA_NRM, GX_DIRECT); }
	if(config & LV_VNIDX8) { GX_SetVtxDesc(GX_VA_NRM, GX_INDEX8); }
	if(config & LV_VNIDX16) { GX_SetVtxDesc(GX_VA_NRM, GX_INDEX16); }
	if(config & LV_VC) { GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT); }

	if(config & LV_TC) { GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT); }
	if(config & LV_TC && texcount >= 2) { GX_SetVtxDesc(GX_VA_TEX1, GX_DIRECT); }
	if(config & LV_TC && texcount >= 3) { GX_SetVtxDesc(GX_VA_TEX2, GX_DIRECT); }
	if(config & LV_TC && texcount >= 4) { GX_SetVtxDesc(GX_VA_TEX3, GX_DIRECT); }
	if(config & LV_TC && texcount >= 5) { GX_SetVtxDesc(GX_VA_TEX4, GX_DIRECT); }
	if(config & LV_TC && texcount >= 6) { GX_SetVtxDesc(GX_VA_TEX5, GX_DIRECT); }
	if(config & LV_TC && texcount >= 7) { GX_SetVtxDesc(GX_VA_TEX6, GX_DIRECT); }
	if(config & LV_TC && texcount >= 8) { GX_SetVtxDesc(GX_VA_TEX7, GX_DIRECT); }

	if(config & LV_TCIDX8) { GX_SetVtxDesc(GX_VA_TEX0, GX_INDEX8); }
	if(config & LV_TCIDX8 && texcount >= 2) { GX_SetVtxDesc(GX_VA_TEX1, GX_INDEX8); }
	if(config & LV_TCIDX8 && texcount >= 3) { GX_SetVtxDesc(GX_VA_TEX2, GX_INDEX8); }
	if(config & LV_TCIDX8 && texcount >= 4) { GX_SetVtxDesc(GX_VA_TEX3, GX_INDEX8); }
	if(config & LV_TCIDX8 && texcount >= 5) { GX_SetVtxDesc(GX_VA_TEX4, GX_INDEX8); }
	if(config & LV_TCIDX8 && texcount >= 6) { GX_SetVtxDesc(GX_VA_TEX5, GX_INDEX8); }
	if(config & LV_TCIDX8 && texcount >= 7) { GX_SetVtxDesc(GX_VA_TEX6, GX_INDEX8); }
	if(config & LV_TCIDX8 && texcount >= 8) { GX_SetVtxDesc(GX_VA_TEX7, GX_INDEX8); }
	
	if(config & LV_TCIDX16) { GX_SetVtxDesc(GX_VA_TEX0, GX_INDEX16); }
	if(config & LV_TCIDX16 && texcount >= 2) { GX_SetVtxDesc(GX_VA_TEX1, GX_INDEX16); }
	if(config & LV_TCIDX16 && texcount >= 3) { GX_SetVtxDesc(GX_VA_TEX2, GX_INDEX16); }
	if(config & LV_TCIDX16 && texcount >= 4) { GX_SetVtxDesc(GX_VA_TEX3, GX_INDEX16); }
	if(config & LV_TCIDX16 && texcount >= 5) { GX_SetVtxDesc(GX_VA_TEX4, GX_INDEX16); }
	if(config & LV_TCIDX16 && texcount >= 6) { GX_SetVtxDesc(GX_VA_TEX5, GX_INDEX16); }
	if(config & LV_TCIDX16 && texcount >= 7) { GX_SetVtxDesc(GX_VA_TEX6, GX_INDEX16); }
	if(config & LV_TCIDX16 && texcount >= 8) { GX_SetVtxDesc(GX_VA_TEX7, GX_INDEX16); }
	
	//attrfmts
	if(config & LV_MX) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_PTNMTXIDX, GX_INDEX8, GX_U8, 0); }
	if(config & LV_VP) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_POS, GX_POS_XYZ, GX_F32, 0); }
	if(config & LV_VPIDX8) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_POS, GX_POS_XYZ, GX_F32, 0); }
	if(config & LV_VPIDX16) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_POS, GX_POS_XYZ, GX_F32, 0); }
	if(config & LV_VN) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0); }
	if(config & LV_VNIDX8) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0); }
	if(config & LV_VNIDX16) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0); }
	if(config & LV_VC) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_CLR0, GX_CLR_RGB, GX_RGB8, 0); }

	if(config & LV_TC) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TC && texcount >= 2) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TC && texcount >= 3) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX2, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TC && texcount >= 4) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX3, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TC && texcount >= 5) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX4, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TC && texcount >= 6) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX5, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TC && texcount >= 7) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX6, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TC && texcount >= 8) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX7, GX_TEX_ST, GX_F32, 0); }
	
	if(config & LV_TCIDX8) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TCIDX8 && texcount >= 2) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TCIDX8 && texcount >= 3) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX2, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TCIDX8 && texcount >= 4) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX3, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TCIDX8 && texcount >= 5) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX4, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TCIDX8 && texcount >= 6) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX5, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TCIDX8 && texcount >= 7) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX6, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TCIDX8 && texcount >= 8) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX7, GX_TEX_ST, GX_F32, 0); }
	
	if(config & LV_TCIDX16) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TCIDX16 && texcount >= 2) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TCIDX16 && texcount >= 3) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX2, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TCIDX16 && texcount >= 4) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX3, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TCIDX16 && texcount >= 5) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX4, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TCIDX16 && texcount >= 6) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX5, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TCIDX16 && texcount >= 7) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX6, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TCIDX16 && texcount >= 8) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX7, GX_TEX_ST, GX_F32, 0); }
}

void calcTableDelta(float *sources, float *differents, float *outs, int itemcount) {
	for(int i = 0; i < itemcount; i++) {
		outs[i] = differents[i] - sources[i];
	}
}

#define LIY_HASVERTPOS 0b00000001
#define LIY_HASFACENOR 0b00000010
#define LIY_HASVERTNOR 0b00000100
#define LIY_HASTEXCORD 0b00001000
#define LIY_HASVCINDEX 0b00010000
#define LIY_HASVERTCOL 0b00100000
#define LIY_HASMTXIDX 0b01000000

int calcSizeDisplaylistLegacy(uint8_t arrfeatures, int facecost, int tricount) {
	int cost = 3; //cost is the cost in bytes. GX_Begin() is 3 bytes

	if (arrfeatures & LIY_HASVERTPOS) { cost += facecost * 4 * tricount * 3; } //f32 is 4 big
	//we do not support sending face normal to gx
	if (arrfeatures & LIY_HASVERTNOR) { cost += facecost * 4 * tricount * 3; }
	if (arrfeatures & LIY_HASTEXCORD) { cost += facecost * 4 * tricount * 2; } //we send texcords in 2f32 instead
	if (arrfeatures & LIY_HASVCINDEX) { cost += facecost * 1 * tricount * 1; } //just 1 byte per vertex
	if (arrfeatures & LIY_HASVERTCOL) { cost += facecost * 1 * tricount * 3; } //3 bytes per vert
	if (arrfeatures & LIY_HASMTXIDX) { cost += facecost * 1 * tricount * 1; }	

	cost += 32; //throw in an extra 32. this gets shaven off later but things fail w/o this

	cost += 32 - (cost % 32); //round size up to nearest 32nd

	return cost;
}

int calcSizeDisplaylist(uint16_t arrfeatures, int facecost, int tricount) {
	int cost = 3; //cost is the cost in bytes. GX_Begin() is 3 bytes

	if (arrfeatures & LV_VP) { cost += facecost * 4 * tricount * 3; } //f32 is 4 big
	if (arrfeatures & LV_VN) { cost += facecost * 4 * tricount * 3; }
	if (arrfeatures & LV_TC) { cost += facecost * 4 * tricount * 2; } //we send texcords in 2f32 instead
	if (arrfeatures & LV_VPIDX8) { cost += facecost * 1 * tricount * 1; } //just 1 byte per vertex
	if (arrfeatures & LV_VNIDX8) { cost += facecost * 1 * tricount * 1; }
	if (arrfeatures & LV_TCIDX8) { cost += facecost * 1 * tricount * 1; }
	if (arrfeatures & LV_VPIDX16) { cost += facecost * 2 * tricount * 1; }
	if (arrfeatures & LV_VNIDX16) { cost += facecost * 2 * tricount * 1; }
	if (arrfeatures & LV_TCIDX16) { cost += facecost * 2 * tricount * 1; }
	if (arrfeatures & LV_VC) { cost += facecost * 1 * tricount * 3; } //3 bytes per vert
	if (arrfeatures & LV_MX) { cost += facecost * 1 * tricount * 1; }	

	cost += 32; //throw in an extra 32. this gets shaven off later but things fail w/o this

	cost += 32 - (cost % 32); //round size up to nearest 32nd

	return cost;
}

void liy_batchLoadMtx(Mtx *mtxarr, int *indexArr, uint16_t whichones) { //1023 for all of them
	int i = 0;
	if(whichones & 1)  { GX_LoadPosMtxImm(mtxarr[indexArr[i]], GX_PNMTX0); 
                             GX_LoadNrmMtxImm(mtxarr[indexArr[i]], GX_PNMTX0); i++;}
	if(whichones & 2)  { GX_LoadPosMtxImm(mtxarr[indexArr[i]], GX_PNMTX1); 
                             GX_LoadNrmMtxImm(mtxarr[indexArr[i]], GX_PNMTX1); i++;}
	if(whichones & 4)  { GX_LoadPosMtxImm(mtxarr[indexArr[i]], GX_PNMTX2); 
                             GX_LoadNrmMtxImm(mtxarr[indexArr[i]], GX_PNMTX2); i++;}
	if(whichones & 8)  { GX_LoadPosMtxImm(mtxarr[indexArr[i]], GX_PNMTX3); 
                             GX_LoadNrmMtxImm(mtxarr[indexArr[i]], GX_PNMTX3); i++;}
	if(whichones & 16) { GX_LoadPosMtxImm(mtxarr[indexArr[i]], GX_PNMTX4); 
                             GX_LoadNrmMtxImm(mtxarr[indexArr[i]], GX_PNMTX4); i++;}
	if(whichones & 32) { GX_LoadPosMtxImm(mtxarr[indexArr[i]], GX_PNMTX5); 
                             GX_LoadNrmMtxImm(mtxarr[indexArr[i]], GX_PNMTX5); i++;}
	if(whichones & 64) { GX_LoadPosMtxImm(mtxarr[indexArr[i]], GX_PNMTX6); 
                             GX_LoadNrmMtxImm(mtxarr[indexArr[i]], GX_PNMTX6); i++;}
	if(whichones & 128){ GX_LoadPosMtxImm(mtxarr[indexArr[i]], GX_PNMTX7); 
                             GX_LoadNrmMtxImm(mtxarr[indexArr[i]], GX_PNMTX7); i++;}
	if(whichones & 256){ GX_LoadPosMtxImm(mtxarr[indexArr[i]], GX_PNMTX8); 
                             GX_LoadNrmMtxImm(mtxarr[indexArr[i]], GX_PNMTX8); i++;}
	if(whichones & 512){ GX_LoadPosMtxImm(mtxarr[indexArr[i]], GX_PNMTX9); 
                             GX_LoadNrmMtxImm(mtxarr[indexArr[i]], GX_PNMTX9);}
}

void LX_LoadMtxImm(Mtx mtx, int whichone) {
	GX_LoadPosMtxImm(mtx, whichone);
	GX_LoadNrmMtxImm(mtx, whichone);
}

void drawDial(uint8_t r, uint8_t g, uint8_t b) {
	GX_Begin(GX_TRIANGLES, GX_VTXFMT0, 3);
		GX_Position3f32(-0.1f, 0.0f, 0.0f);
		GX_Color3u8(r, g, b);
		GX_Position3f32( 0.1f, 0.0f, 0.0f);
		GX_Color3u8(r, g, b);
		GX_Position3f32( 0.0f, 1.0f, 0.0f);
		GX_Color3u8(r, g, b);
}

#define LM_0 1 //liy mtx
#define LM_1 2
#define LM_2 4
#define LM_3 8
#define LM_4 16
#define LM_5 32
#define LM_6 64
#define LM_7 128
#define LM_8 256
#define LM_9 512
#define LM_ALL 1023

#endif