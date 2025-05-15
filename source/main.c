//#define GUY_RELEASE
#define GUY_DEV

//#define MINUSCONSOLE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <asndlib.h>
#include "ogc/lwp_watchdog.h"

#include <fat.h>
#include "oggplayer.h"

#include "liym/liym.h"
#include "utililiys.h"
#include "light.h"
#include "primitives.h"
#include "loadings.h"
#include "liyt.h"

#include "randomuncategorizeddata/lakeMtxTables.h"
#include "randomuncategorizeddata/focuslut.h"

#include "data/mdl/lake/lake.h"

#include "data/blurmouth_liya.h"
#include "data/blurnow_liya.h"
#include "data/toothnow_liya.h"

#include "data/camanim.h"
#include "data/photoAnim.h"
#include "data/focusAnim.h"

//files
#include "laketex_tpl.h"
#include "laketex.h"

#include "blurnow_ogg.h"

#include "secret.h"

//required globals for draws
Mtx view,mv,mr,mvi; // view and perspective matrices
Mtx model, modelview;
Mtx *lakeSkeletonMtx, *lakeSkeletonMv;
GXTexObj laketexTexObj;

#include "draws/lake.h"

//#define TEST_GRID_TEX 1
#ifdef TEST_GRID_TEX
#include "cgrid_tpl.h"
#include "cgrid.h"
#endif

#ifdef GUY_DEV
//Keyboard support is only in DEV
#include <unistd.h>

#include <wiikeyboard/keyboard.h>

void keyboard_callback(char sym) {
	if (sym == 0x1b) exit(0); //esc
}

#endif

#define DEFAULT_FIFO_SIZE	(256*1024)

typedef struct tagtexdef
{
	void *pal_data;
	void *tex_data;
	u32 sz_x;
	u32 sz_y;
	u32 fmt;
	u32 min_lod;
	u32 max_lod;
	u32 min;
	u32 mag;
	u32 wrap_s;
	u32 wrap_t;
	void *nextdef;
} texdef;

static GXRModeObj *rmode = NULL;
static void *frameBuffer[2] = { NULL, NULL};

u32 fb = 0;

int widescreen = 0;

guVector overlayCam = {0.0F, 0.0F, 1.0F},
	 overlayUp = {0.0F, 1.0F, 0.0F},
	 overlaylook = {0.0F, 0.0F, 0.0F};
Mtx overlayView;
Mtx44 overlayPersp;

FILE *infile;
FILE *mouthfile;

//GXColor background = {90, 105, 120, 0xff};
//GXColor background = {45, 53, 60, 0xff};
//GXColor background = {0, 0, 0, 0xff};
GXColor background = {255, 255, 255, 255}; 

#define NUM_LETTERS 40
float *alphabetVertpos[NUM_LETTERS];
int alphabetTricounts[NUM_LETTERS];
float alphabetRightbounds[NUM_LETTERS];

#include "randomuncategorizedbullshit.h"

int main(int argc,char **argv) {
	for(long i = 0; i < 1; i++) {
		printf("", secret[i]);
	}

	#ifndef MINUSCONSOLE
	SYS_STDIO_Report(true);
	#endif

	f32 yscale,zt = 0; f32 lala = 0;
	u32 xfbHeight;
	f32 rquad = 0.0f;
	u32 frame = 0;
	GXTexObj skyboxTexObj;
	GXTexObj foliage1TexObj;
	#ifdef TEST_GRID_TEX
	GXTexObj cgridTexObj;
	#endif

	int rframe = 1;

	Mtx44 perspective;

	Mtx permIdentity;
	Mtx44 permIdentity44;
	guMtxIdentity(permIdentity);
	guMtx44Identity(permIdentity44);

	Mtx fullQuadMv;
	Mtx44 fullQuadPersp;

	fullQuadMv[0][0] = 1.0f;
	fullQuadMv[1][0] = 0.0f;
	fullQuadMv[2][0] = 0.0f;

	fullQuadMv[0][1] = 0.0f;
	fullQuadMv[1][1] = 1.0f;
	fullQuadMv[2][1] = 0.0f;
	
	fullQuadMv[0][2] = 0.0f;
	fullQuadMv[1][2] = 0.0f;
	fullQuadMv[2][2] = 1.0f;

	fullQuadMv[0][3] = 0.0f;
	fullQuadMv[1][3] = 0.0f;
	fullQuadMv[2][3] = -1.0f;


	void *gpfifo = NULL;

	guVector cam = {-.5F, -1.0F, 1.0F},
			up = {0.0F, 0.0F, 1.0F},
		  look = {0.0F, -0.15F, 1.3F};
	//guVector camrot = {1.54f, 0.0f, 0.0f};
	guVector camrot = {1.75f, 0.0f, 0.0f};

	float camFov = 70;


	TPLFile laketexTPL;
	#ifdef TEST_GRID_TEX
	TPLFile cgridTPL;
	#endif

	VIDEO_Init();
	WPAD_Init();
	ASND_Init();

	rmode = VIDEO_GetPreferredMode(NULL);

	// allocate the fifo buffer
	gpfifo = memalign(32,DEFAULT_FIFO_SIZE);
	memset(gpfifo,0,DEFAULT_FIFO_SIZE);

	#ifdef MINUSCONSOLE
	void *consoleTex = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
	CON_Init(consoleTex, 20, 20, 640, 480, 640*2);
	#endif

	// allocate 2 framebuffers for double buffering
	frameBuffer[0] = SYS_AllocateFramebuffer(rmode);
	frameBuffer[1] = SYS_AllocateFramebuffer(rmode);

	// configure video
	VIDEO_Configure(rmode);
	rmode->aa = 0; //no way aa
	rmode->viWidth = 670;
	rmode->viXOrigin = 25; // widen display to remove columns
	VIDEO_SetNextFramebuffer(frameBuffer[fb]);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

	fb ^= 1;

	// init the flipper
	GX_Init(gpfifo,DEFAULT_FIFO_SIZE);

	// clears the bg to color and clears the z buffer
	GX_SetCopyClear(LC_BLACK, 0x00ffffff);

	// other gx setup
	GX_SetViewport(0,0,rmode->fbWidth,rmode->efbHeight,0,1);
	yscale = GX_GetYScaleFactor(rmode->efbHeight,rmode->xfbHeight);
	xfbHeight = GX_SetDispCopyYScale(yscale);
	GX_SetScissor(0,0,rmode->fbWidth,rmode->efbHeight);
	GX_SetDispCopySrc(0,0,rmode->fbWidth,rmode->efbHeight);
	GX_SetDispCopyDst(rmode->fbWidth,xfbHeight);

	// Allocate rendertextures for postprocess fx

	int ltw = rmode->fbWidth;
	int lth = rmode->efbHeight;

	int dsw = 160; //dof small width
	int dsh = 120;

	int texsize = GX_GetTexBufferSize(ltw, lth, GX_TF_RGBA8, GX_FALSE, 0);
	void *DofDefClr = memalign(32, GX_GetTexBufferSize(ltw, lth, GX_TF_RGBA8, GX_FALSE, 0));
	void *DofDefBufr = memalign(32, GX_GetTexBufferSize(ltw, lth, GX_TF_Z24X8, GX_FALSE, 0));
	void *DofDefQtrsize = memalign(32, GX_GetTexBufferSize(dsw, dsh, GX_TF_RGBA8, GX_FALSE, 0));

	void *Picture = memalign(32, GX_GetTexBufferSize(ltw, lth, GX_TF_RGBA8, GX_FALSE, 0));

	// Tex objects for textures
	GXTexObj DofDefBufrTexObj, DofDefQtrsizeTexObj, DofDefClrTexObj, screenQtrTexObj, bloomFullTexObj, PictureTexObj; 
	

	  u8 perfectfilter[7] = { 0, 0, 21,21, 21,0, 0 };
	//                           v center line
	//u8 blurfiltr[7]={ 6, 7, 9, 10, 9, 7, 6 };
	//u8 vfilter[7] = { 0, 0, 0, 63, 0, 0, 0 };
	  u8 vfilter[7] = { 0, 0, 21,21, 21,0, 0 };
	  u8 blurfiltr[7] = { 9, 9, 10,  9, 10, 9, 9 };
	//u8 blurfiltr[7]={ 63, 63, 63, 63, 63, 63, 63 };
	//u8 vfilter[7] = { 0, 31, 0, 0, 0, 31, 0 };
	GX_SetCopyFilter(GX_FALSE, NULL, GX_TRUE, vfilter);
	//GX_SetCopyFilter(rmode->aa,rmode->sample_pattern,GX_TRUE,rmode->vfilter);
	GX_SetFieldMode(rmode->field_rendering,((rmode->viHeight==2*rmode->xfbHeight)?GX_ENABLE:GX_DISABLE));

	GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);

	//GX_SetCullMode(GX_CULL_NONE);
	GX_SetCullMode(GX_CULL_FRONT);
	GX_CopyDisp(frameBuffer[fb],GX_TRUE);
	GX_SetDispCopyGamma((f32)GX_GM_1_0);


	GX_SetNumTexGens(1);

	// setup texture coordinate generation
	// args: texcoord slot 0-7, matrix type, source to generate texture coordinates from, matrix to use
	//GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEX0, GX_TEXMTX0);
	GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEX0, GX_IDENTITY);
	GX_SetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX3x4, GX_TG_TEX1, GX_IDENTITY);
	GX_SetTexCoordGen(GX_TEXCOORD2, GX_TG_MTX3x4, GX_TG_TEX2, GX_IDENTITY);
	GX_SetTexCoordGen(GX_TEXCOORD3, GX_TG_MTX3x4, GX_TG_TEX3, GX_IDENTITY);

	f32 aspect;
	//aspect = 21.0f/9.0f;
	if (CONF_GetAspectRatio() == CONF_ASPECT_16_9) {
		//aspect = 43.0f/27.0f; //closer to actual measured display output ratio
		aspect = 16.0f/9.0f;
		widescreen = 1;
	} else {
		aspect = 4.0f/3.0f;
	}
	guLightPerspective(mv,45, aspect, 1.05F, 1.0F, 0.0F, 0.0F);
	guMtxTrans(mr, 0.0F, 0.0F, -1.0F);
	guMtxConcat(mv, mr, mv);
	GX_LoadTexMtxImm(mv, GX_TEXMTX0, GX_MTX3x4);

	GX_InvalidateTexAll();

	TPL_OpenTPLFromMemory(&laketexTPL, (void *)laketex_tpl, laketex_tpl_size);
	TPL_GetTexture(&laketexTPL,laketex,&laketexTexObj);

	#ifdef TEST_GRID_TEX
	TPL_OpenTPLFromMemory(&cgridTPL, (void *)cgrid_tpl, cgrid_tpl_size);
	TPL_GetTexture(&cgridTPL,cgrid,&cgridTexObj);
	#endif

	// setup our camera at the origin
	// looking down the -z axis with y up
	guLookAt(view, &cam, &up, &look);
	guLookAt(overlayView, &overlayCam, &overlayUp, &overlaylook);

	guPerspective(overlayPersp, 45, aspect, 0.1F, 300.0F);
	guPerspective(perspective, 45, aspect, 0.1F, 300.0F); //not th one used @ runtime
	GX_LoadProjectionMtx(perspective, GX_PERSPECTIVE);

	guVector cubeAxis = {1,1,1};

	//if (!fatInitDefault()) {
	if (!fatInit(1, 1)) { //sector cache, default stdio
		printf("fatInitDefault failure: terminating\n");
		exit(-1);
	}

		struct liymParseResults parse;
		
	// Decompress lake mouth

		float *lakeMouthFullsizevertpos[8];
		float *lakeMouthFullsizevertnor[8];
		for(int i = 0; i < 8; i++) {
			lakeMouthFullsizevertpos[i] = (float *)malloc((lakeMouthBasistricount * 9) * sizeof(float));
			lakeMouthFullsizevertnor[i] = (float *)malloc((lakeMouthBasistricount * 9) * sizeof(float));
		}
		for(int j = 0; j < lakeMouthBasistricount * 3; j++) {
			lakeMouthFullsizevertpos[0][j*3] = lakeMouthBasisvertpos[lakeMouthBasisvertposidx[j] * 3];
			lakeMouthFullsizevertpos[0][(j*3)+1] = lakeMouthBasisvertpos[(lakeMouthBasisvertposidx[j] * 3)+1];
			lakeMouthFullsizevertpos[0][(j*3)+2] = lakeMouthBasisvertpos[(lakeMouthBasisvertposidx[j] * 3)+2];
			lakeMouthFullsizevertnor[0][j*3] = lakeMouthBasisvertnor[lakeMouthBasisvertnoridx[j] * 3];
			lakeMouthFullsizevertnor[0][(j*3)+1] = lakeMouthBasisvertnor[(lakeMouthBasisvertnoridx[j] * 3)+1];
			lakeMouthFullsizevertnor[0][(j*3)+2] = lakeMouthBasisvertnor[(lakeMouthBasisvertnoridx[j] * 3)+2];
		}
		for(int j = 0; j < lakeMouthBasistricount * 3; j++) {
			lakeMouthFullsizevertpos[1][j*3] = lakeMouthGenopenvertpos[lakeMouthGenopenvertposidx[j] * 3];
			lakeMouthFullsizevertpos[1][(j*3)+1] = lakeMouthGenopenvertpos[(lakeMouthGenopenvertposidx[j] * 3)+1];
			lakeMouthFullsizevertpos[1][(j*3)+2] = lakeMouthGenopenvertpos[(lakeMouthGenopenvertposidx[j] * 3)+2];
			lakeMouthFullsizevertnor[1][j*3] = lakeMouthGenopenvertnor[lakeMouthGenopenvertnoridx[j] * 3];
			lakeMouthFullsizevertnor[1][(j*3)+1] = lakeMouthGenopenvertnor[(lakeMouthGenopenvertnoridx[j] * 3)+1];
			lakeMouthFullsizevertnor[1][(j*3)+2] = lakeMouthGenopenvertnor[(lakeMouthGenopenvertnoridx[j] * 3)+2];
		}
		for(int j = 0; j < lakeMouthBasistricount * 3; j++) {
			lakeMouthFullsizevertpos[2][j*3] = lakeMouthGenwidevertpos[lakeMouthGenwidevertposidx[j] * 3];
			lakeMouthFullsizevertpos[2][(j*3)+1] = lakeMouthGenwidevertpos[(lakeMouthGenwidevertposidx[j] * 3)+1];
			lakeMouthFullsizevertpos[2][(j*3)+2] = lakeMouthGenwidevertpos[(lakeMouthGenwidevertposidx[j] * 3)+2];
			lakeMouthFullsizevertnor[2][j*3] = lakeMouthGenwidevertnor[lakeMouthGenwidevertnoridx[j] * 3];
			lakeMouthFullsizevertnor[2][(j*3)+1] = lakeMouthGenwidevertnor[(lakeMouthGenwidevertnoridx[j] * 3)+1];
			lakeMouthFullsizevertnor[2][(j*3)+2] = lakeMouthGenwidevertnor[(lakeMouthGenwidevertnoridx[j] * 3)+2];
		}
		for(int j = 0; j < lakeMouthBasistricount * 3; j++) {
			lakeMouthFullsizevertpos[3][j*3] = lakeMouthOvertpos[lakeMouthOvertposidx[j] * 3];
			lakeMouthFullsizevertpos[3][(j*3)+1] = lakeMouthOvertpos[(lakeMouthOvertposidx[j] * 3)+1];
			lakeMouthFullsizevertpos[3][(j*3)+2] = lakeMouthOvertpos[(lakeMouthOvertposidx[j] * 3)+2];
			lakeMouthFullsizevertnor[3][j*3] = lakeMouthOvertnor[lakeMouthOvertnoridx[j] * 3];
			lakeMouthFullsizevertnor[3][(j*3)+1] = lakeMouthOvertnor[(lakeMouthOvertnoridx[j] * 3)+1];
			lakeMouthFullsizevertnor[3][(j*3)+2] = lakeMouthOvertnor[(lakeMouthOvertnoridx[j] * 3)+2];
		}
		for(int j = 0; j < lakeMouthBasistricount * 3; j++) {
			lakeMouthFullsizevertpos[4][j*3] = lakeMouthLeftsneervertpos[lakeMouthLeftsneervertposidx[j] * 3];
			lakeMouthFullsizevertpos[4][(j*3)+1] = lakeMouthLeftsneervertpos[(lakeMouthLeftsneervertposidx[j] * 3)+1];
			lakeMouthFullsizevertpos[4][(j*3)+2] = lakeMouthLeftsneervertpos[(lakeMouthLeftsneervertposidx[j] * 3)+2];
			lakeMouthFullsizevertnor[4][j*3] = lakeMouthLeftsneervertnor[lakeMouthLeftsneervertnoridx[j] * 3];
			lakeMouthFullsizevertnor[4][(j*3)+1] = lakeMouthLeftsneervertnor[(lakeMouthLeftsneervertnoridx[j] * 3)+1];
			lakeMouthFullsizevertnor[4][(j*3)+2] = lakeMouthLeftsneervertnor[(lakeMouthLeftsneervertnoridx[j] * 3)+2];
		}
		for(int j = 0; j < lakeMouthBasistricount * 3; j++) {
			lakeMouthFullsizevertpos[5][j*3] = lakeMouthRightsneervertpos[lakeMouthRightsneervertposidx[j] * 3];
			lakeMouthFullsizevertpos[5][(j*3)+1] = lakeMouthRightsneervertpos[(lakeMouthRightsneervertposidx[j] * 3)+1];
			lakeMouthFullsizevertpos[5][(j*3)+2] = lakeMouthRightsneervertpos[(lakeMouthRightsneervertposidx[j] * 3)+2];
			lakeMouthFullsizevertnor[5][j*3] = lakeMouthRightsneervertnor[lakeMouthRightsneervertnoridx[j] * 3];
			lakeMouthFullsizevertnor[5][(j*3)+1] = lakeMouthRightsneervertnor[(lakeMouthRightsneervertnoridx[j] * 3)+1];
			lakeMouthFullsizevertnor[5][(j*3)+2] = lakeMouthRightsneervertnor[(lakeMouthRightsneervertnoridx[j] * 3)+2];
		}
		for(int j = 0; j < lakeMouthBasistricount * 3; j++) {
			lakeMouthFullsizevertpos[6][j*3] = lakeMouthTongueupvertpos[lakeMouthTongueupvertposidx[j] * 3];
			lakeMouthFullsizevertpos[6][(j*3)+1] = lakeMouthTongueupvertpos[(lakeMouthTongueupvertposidx[j] * 3)+1];
			lakeMouthFullsizevertpos[6][(j*3)+2] = lakeMouthTongueupvertpos[(lakeMouthTongueupvertposidx[j] * 3)+2];
			lakeMouthFullsizevertnor[6][j*3] = lakeMouthTongueupvertnor[lakeMouthTongueupvertnoridx[j] * 3];
			lakeMouthFullsizevertnor[6][(j*3)+1] = lakeMouthTongueupvertnor[(lakeMouthTongueupvertnoridx[j] * 3)+1];
			lakeMouthFullsizevertnor[6][(j*3)+2] = lakeMouthTongueupvertnor[(lakeMouthTongueupvertnoridx[j] * 3)+2];
		}
		for(int j = 0; j < lakeMouthBasistricount * 3; j++) {
			lakeMouthFullsizevertpos[7][j*3] = lakeMouthSmilevertpos[lakeMouthSmilevertposidx[j] * 3];
			lakeMouthFullsizevertpos[7][(j*3)+1] = lakeMouthSmilevertpos[(lakeMouthSmilevertposidx[j] * 3)+1];
			lakeMouthFullsizevertpos[7][(j*3)+2] = lakeMouthSmilevertpos[(lakeMouthSmilevertposidx[j] * 3)+2];
			lakeMouthFullsizevertnor[7][j*3] = lakeMouthSmilevertnor[lakeMouthSmilevertnoridx[j] * 3];
			lakeMouthFullsizevertnor[7][(j*3)+1] = lakeMouthSmilevertnor[(lakeMouthSmilevertnoridx[j] * 3)+1];
			lakeMouthFullsizevertnor[7][(j*3)+2] = lakeMouthSmilevertnor[(lakeMouthSmilevertnoridx[j] * 3)+2];
		}
		float *lakeTeethBasisFullsizevertpos = malloc(lakeTeethBasistricount * 9 * sizeof(float));
		float *lakeTeethBasisFullsizevertnor = malloc(lakeTeethBasistricount * 9 * sizeof(float));
		float *lakeTeethOpenFullsizevertpos = malloc(lakeTeethBasistricount * 9 * sizeof(float));
		float *lakeTeethOpenFullsizevertnor = malloc(lakeTeethBasistricount * 9 * sizeof(float));

		for(int j = 0; j < lakeTeethBasistricount * 3; j++) {
			lakeTeethBasisFullsizevertpos[ j*3   ] = lakeTeethBasisvertpos[ lakeTeethBasisvertposidx[j] * 3];
			lakeTeethBasisFullsizevertpos[(j*3)+1] = lakeTeethBasisvertpos[(lakeTeethBasisvertposidx[j] * 3)+1];
			lakeTeethBasisFullsizevertpos[(j*3)+2] = lakeTeethBasisvertpos[(lakeTeethBasisvertposidx[j] * 3)+2];
			lakeTeethBasisFullsizevertnor[ j*3   ] = lakeTeethBasisvertnor[ lakeTeethBasisvertnoridx[j] * 3];
			lakeTeethBasisFullsizevertnor[(j*3)+1] = lakeTeethBasisvertnor[(lakeTeethBasisvertnoridx[j] * 3)+1];
			lakeTeethBasisFullsizevertnor[(j*3)+2] = lakeTeethBasisvertnor[(lakeTeethBasisvertnoridx[j] * 3)+2];
		}
		for(int j = 0; j < lakeTeethBasistricount * 3; j++) {
			lakeTeethOpenFullsizevertpos[ j*3   ] = lakeTeethOpenvertpos[ lakeTeethOpenvertposidx[j] * 3];
			lakeTeethOpenFullsizevertpos[(j*3)+1] = lakeTeethOpenvertpos[(lakeTeethOpenvertposidx[j] * 3)+1];
			lakeTeethOpenFullsizevertpos[(j*3)+2] = lakeTeethOpenvertpos[(lakeTeethOpenvertposidx[j] * 3)+2];
			lakeTeethOpenFullsizevertnor[ j*3   ] = lakeTeethOpenvertnor[ lakeTeethOpenvertnoridx[j] * 3];
			lakeTeethOpenFullsizevertnor[(j*3)+1] = lakeTeethOpenvertnor[(lakeTeethOpenvertnoridx[j] * 3)+1];
			lakeTeethOpenFullsizevertnor[(j*3)+2] = lakeTeethOpenvertnor[(lakeTeethOpenvertnoridx[j] * 3)+2];
		}

		float *lakeMouthDeltavertpos[7];
		float *lakeMouthDeltavertnor[7];

		float *lakeTeethDeltavertpos = malloc(lakeTeethBasistricount * 9 * sizeof(float));
		float *lakeTeethDeltavertnor = malloc(lakeTeethBasistricount * 9 * sizeof(float));

		for(int j = 0; j < lakeTeethBasistricount * 9; j++) {
			lakeTeethDeltavertpos[j] = lakeTeethOpenFullsizevertpos[j] - lakeTeethBasisFullsizevertpos[j];
			lakeTeethDeltavertnor[j] = lakeTeethOpenFullsizevertnor[j] - lakeTeethBasisFullsizevertnor[j];
		}

		for(int i = 0; i < 7; i++) {
			lakeMouthDeltavertpos[i] = malloc(lakeMouthBasistricount * 9 * sizeof(float));
			lakeMouthDeltavertnor[i] = malloc(lakeMouthBasistricount * 9 * sizeof(float));
			for(int j = 0; j < lakeMouthBasistricount * 9; j++) {
				lakeMouthDeltavertpos[i][j] = lakeMouthFullsizevertpos[i+1][j] - lakeMouthFullsizevertpos[0][j];
				lakeMouthDeltavertnor[i][j] = lakeMouthFullsizevertnor[i+1][j] - lakeMouthFullsizevertnor[0][j];
			}
		}

		float *lakeTeethShapedVertpos = (float *)malloc((lakeTeethBasistricount * 9) * sizeof(float));
		float *lakeTeethShapedVertnor = (float *)malloc((lakeTeethBasistricount * 9) * sizeof(float));

		for(int i = 1; i < 8; i++) {
			free(lakeMouthFullsizevertpos[i]);
		}

		float *lakeMouthShapedVertpos = (float *)malloc((lakeMouthBasistricount * 9) * sizeof(float));
		float *lakeMouthShapedVertnor = (float *)malloc((lakeMouthBasistricount * 9) * sizeof(float));
		
	
	lakeSkeletonMtx = malloc(blurnowprimcount * sizeof(Mtx) / 9);
	lakeSkeletonMv = malloc(blurnowprimcount * sizeof(Mtx) / 9);

	float *lakeMouthBlendshapeWeights;
	lakeMouthBlendshapeWeights = malloc(7 * sizeof(float));
	float lakeToothweight;


	float *LotsOfRandoms; //its 256 k	
	
	LotsOfRandoms = malloc(65536 * sizeof(float));
	for(int i = 0; i < 65536; i++) {
		LotsOfRandoms[i] = ((float)(rand() % 100000) / 50000.0f) - 1.0f;
	}	

	#ifdef GUY_DEV	
		int kbinit = 0;
		//if (KEYBOARD_Init(keyboard_callback) == 0) {kbinit = 1;}

		printf("Render loop!\n"); 
	#endif
	float dialx = (widescreen) ? 7.0f : 5.0f;
	u64 frametime = 0;
	double frametimems = 16.0f;
	double framerate = 60.0f;

	int consoletog = 0;

	int posttog = 1;

	float exposure = 0.52f;

	float Lily_Float = 0.5f;

	int Other_Int = 0;

	int play = 1;
 
 	int advance = 0;

	VIDEO_SetBlack(false);

	VIDEO_SetNextFramebuffer(frameBuffer[fb]);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	
	int scene;

	while(1) {																 // # # # # Main loop # # # #
		#ifdef GUY_DEV
			if(frame % 120 == 0 && kbinit == 0) {
				//if (KEYBOARD_Init(keyboard_callback) == 0) {kbinit = 1;}
			} 
		#endif		

		basicStringsFrame = 0;

		uint8_t buttons = 0;

		GX_SetCopyClear(LC_BLACK, 0x00ffffff);

		u64 startframe = gettime();

		//setTestCam(view);
		
		GX_SetCurrentMtx(GX_PNMTX0);

		GX_LoadPosMtxImm(model, GX_PNMTX0);
		GX_LoadNrmMtxImm(model, GX_PNMTX0);
		GX_LoadPosMtxImm(model, GX_PNMTX1);
		GX_LoadNrmMtxImm(model, GX_PNMTX1);

		WPAD_ScanPads();
		if(WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) exit(0);
		if (WPAD_ButtonsHeld(0)&WPAD_BUTTON_UP) buttons = buttons | LIYB_UP;
		if (WPAD_ButtonsHeld(0)&WPAD_BUTTON_DOWN) buttons = buttons | LIYB_DOWN;
		if (WPAD_ButtonsHeld(0)&WPAD_BUTTON_LEFT) buttons = buttons | LIYB_LEFT;
		if (WPAD_ButtonsHeld(0)&WPAD_BUTTON_RIGHT) buttons = buttons | LIYB_RIGHT;
		if (WPAD_ButtonsHeld(0)&WPAD_BUTTON_1) { buttons = buttons | LIYB_1; }
		if (WPAD_ButtonsHeld(0)&WPAD_BUTTON_2) { buttons = buttons | LIYB_2; }
		if (WPAD_ButtonsHeld(0)&WPAD_BUTTON_A) { buttons = buttons | LIYB_A; }
		if (WPAD_ButtonsHeld(0)&WPAD_BUTTON_B) { buttons = buttons | LIYB_B; }

		int key; 
		//if(kbinit == 1) {key = getchar();}

		float movespeed = 0.03f;	

		//the dpad is setup for horizontal wii remote

		if(buttons & LIYB_RIGHT) {  //vert. up
			cam.y += movespeed * cos(-camrot.z);
			cam.x += movespeed * sin(-camrot.z);
		}
		if(buttons & LIYB_DOWN) {//vert. right
			cam.y += movespeed * sin(camrot.z);
			cam.x += movespeed * cos(camrot.z);
		}
		if(buttons & LIYB_LEFT) { //vert. down
			cam.y -= movespeed * cos(-camrot.z);
			cam.x -= movespeed * sin(-camrot.z);
		}
		if(buttons & LIYB_UP) { //vert. left
			cam.y -= movespeed * sin(camrot.z);
			cam.x -= movespeed * cos(camrot.z);
		}
		if(buttons & LIYB_1) {
			camrot.z += movespeed * camFov * 0.02f;
			//Lily_Float += 0.02f;
		}
		if(buttons & LIYB_2) {
			camrot.z -= movespeed * camFov * 0.02f;
			//Lily_Float -= 0.02f;
		}
		if(camrot.x < 3.14f) {
			if(buttons & LIYB_A) {
				//exposure += 0.02f;
				//camrot.x += movespeed * camFov * 0.02f;
			}
		}
		if(camrot.x > 0.0f) {
			if(buttons & LIYB_B) {
				//exposure -= 0.02f;
				//camrot.x -= movespeed * camFov * 0.02f;
			}
		}


		if(camFov > 0.1f) {
			if(WPAD_ButtonsHeld(0)&WPAD_BUTTON_PLUS) {
				#ifndef MINUSCONSOLE
				//camFov -= 5.0f / sqrt(camFov);
				#endif
			}
			if(WPAD_ButtonsHeld(0)&WPAD_BUTTON_MINUS) {
				#ifndef MINUSCONSOLE
				//camFov += 5.0f / sqrt(camFov);
				#endif
			}
		} else {
			camFov = 0.11f;
		}

		if(WPAD_ButtonsHeld(0)&WPAD_BUTTON_PLUS) {
			Lily_Float += 0.02f;
		}
		if(WPAD_ButtonsHeld(0)&WPAD_BUTTON_MINUS) {
			Lily_Float -= 0.02f;
		}

		#ifdef MINUSCONSOLE
		if(WPAD_ButtonsDown(0)&WPAD_BUTTON_MINUS) {
			consoletog ^= 1;
		}
		if(WPAD_ButtonsDown(0)&WPAD_BUTTON_PLUS) {
			posttog ^= 2;
		}
		#endif
		if(WPAD_ButtonsDown(0)&WPAD_BUTTON_A) {
			//Other_Int++;
			play ^= 1;
		}

		if(WPAD_ButtonsDown(0)&WPAD_BUTTON_B) {
			//Other_Int--;
			advance = 1;
		}

		Lily_Float = liym_max(Lily_Float, 0.03f);
		Lily_Float = liym_min(Lily_Float, 0.97f);

		//printf("Lily float is %f\n", Lily_Float);

		//liyt_genMtxPosRotZyx(view, cam.x, cam.y, cam.z, camrot.x, camrot.y, camrot.z);
		//guMtxInverse(view, view);

		view[0][0] = CameraAnim[(rframe * 12) + 0];
		view[0][1] = CameraAnim[(rframe * 12) + 1];
		view[0][2] = CameraAnim[(rframe * 12) + 2];
		view[0][3] = CameraAnim[(rframe * 12) + 3];	
		view[1][0] = CameraAnim[(rframe * 12) + 4];		
		view[1][1] = CameraAnim[(rframe * 12) + 5];		
		view[1][2] = CameraAnim[(rframe * 12) + 6];		
		view[1][3] = CameraAnim[(rframe * 12) + 7];		
		view[2][0] = CameraAnim[(rframe * 12) + 8];		
		view[2][1] = CameraAnim[(rframe * 12) + 9];		
		view[2][2] = CameraAnim[(rframe * 12) + 10];		
		view[2][3] = CameraAnim[(rframe * 12) + 11];

		guPerspective(perspective, 70.0f, aspect, 0.01F, 300.0F);
		GX_LoadProjectionMtx(perspective, GX_PERSPECTIVE);

		//Stream animation data for lake's skeleton and make matricies
			
			if(rframe > 344) {
				rframe = 1;
			}

			f32 lakeskelframe[blurnowprimcount];
			for(int i = 0; i < blurnowprimcount; i++) {
				lakeskelframe[i] = blurnowAnim[i + (blurnowprimcount * (rframe - 1))];
			}

			for(int i = 0; i < blurnowprimcount / 9; i++) {
				liyt_genMtxPosRotZyx(lakeSkeletonMtx[i], lakeskelframe[(i*9)], lakeskelframe[(i*9)+1], lakeskelframe[(i*9)+2],
									 lakeskelframe[(i*9)+3], lakeskelframe[(i*9)+4], lakeskelframe[(i*9)+5]);

				guMtxConcat(view,lakeSkeletonMtx[i],lakeSkeletonMv[i]);
				//LX_LoadMtxImm(lakeSkeletonMv[i], GX_PNMTX0);
				//drawTestaxis();
			}
			DCFlushRange(lakeSkeletonMtx, (blurnowprimcount / 9) * sizeof(Mtx));
	
		//Stream animation data for lake's mouth

			for(int i = 0; i < 7; i++) {
				lakeMouthBlendshapeWeights[i] = blurmouthAnim[i + (blurmouthprimcount * (rframe - 1))];;
			}

		// Calculate blendshape for lakes mouth

			int lakemouthcalccount = lakeMouthBasistricount * 9;
			for(int j = 0; j < lakemouthcalccount; j++) {
				lakeMouthShapedVertpos[j] = lakeMouthFullsizevertpos[0][j];
				lakeMouthShapedVertnor[j] = lakeMouthFullsizevertnor[0][j];
			}
			for(int i = 0; i < 7; i++) { //it's 7
				float wght = lakeMouthBlendshapeWeights[i];
				//float wght = 1.0f;
				for(int j = 0; j < lakemouthcalccount; j++) {
					lakeMouthShapedVertpos[j] += lakeMouthDeltavertpos[i][j] * wght;
					lakeMouthShapedVertnor[j] += lakeMouthDeltavertnor[i][j] * wght;
				}
			}

		//Stream teeeth

			lakeToothweight = toothnowAnim[rframe];

			for(int i = 0; i < lakeTeethBasistricount * 9; i++) {
				lakeTeethShapedVertpos[i] = lakeTeethBasisFullsizevertpos[i];
				lakeTeethShapedVertnor[i] = lakeTeethBasisFullsizevertnor[i];
			}
			for(int j = 0; j < lakeTeethBasistricount * 9; j++) {
				lakeTeethShapedVertpos[j] += lakeTeethDeltavertpos[j] * lakeToothweight;
				lakeTeethShapedVertnor[j] += lakeTeethDeltavertnor[j] * lakeToothweight;
			}

		if(rframe == 1) {
			PlayOgg(blurnow_ogg, blurnow_ogg_size, 0, OGG_ONE_TIME);
		}

		GX_SetNumTexGens(1);

		GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);	
		GX_SetColorUpdate(GX_TRUE);
		GX_SetAlphaUpdate(GX_FALSE);
		
		guMtxIdentity(model);
		guMtxConcat(model, view, modelview);
		LX_LoadMtxImm(modelview, GX_PNMTX0);
		drawTestaxis();
	
		//scene = Other_Int;	
		scene = 1;	

		setlight(view, background, scene);

		GX_SetNumTevStages(2);

		GX_SetTevOp(GX_TEVSTAGE0,GX_REPLACE);

		GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
		GX_SetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, GX_TRUE, GX_TEVPREV);

		GX_SetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_KONST, GX_CC_ZERO);
		GX_SetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, GX_TRUE, GX_TEVPREV);

		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
		GX_SetTevOrder(GX_TEVSTAGE1, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLORNULL);

		GXColor konstColor = {255,255,255, 255};
			
		GX_SetCullMode(GX_CULL_FRONT);

		// Rigged lake

			setlight(view, background, scene);

			GX_SetNumTevStages(1);

			GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_ONE, GX_CC_ZERO);
			GX_SetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, GX_TRUE, GX_TEVPREV);

			GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);

			LX_LoadMtxImm(lakeSkeletonMv[4], GX_PNMTX0);
			GX_SetCurrentMtx(GX_PNMTX0);
			liy_VtxDescConfig(LV_VP | LV_VN | LV_VC, 0);
			drawArrConfig(LV_VP | LV_VN | LV_VC, lakeMouthBasistricount, 0, 
					lakeMouthShapedVertpos, NULL, NULL,
					lakeMouthShapedVertnor, NULL, NULL,
					NULL, NULL, NULL,
					NULL,
					lakeMouthBasisvertcol);

			liy_VtxDescConfig(LV_VP | LV_VN | LV_VC, 0);
			drawArrConfig(LV_VP | LV_VN | LV_VC, lakeTeethBasistricount, 0, 
					lakeTeethShapedVertpos, NULL, NULL,
					lakeTeethShapedVertnor, NULL, NULL,
					NULL, NULL, NULL,
					NULL,
					lakeTeethBasisvertcol);
			
			LX_LoadMtxImm(lakeSkeletonMv[43], GX_PNMTX0);
			GX_SetCurrentMtx(GX_PNMTX0);


			liy_VtxDescConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC, 0);
			if(rframe < 205) {
				GX_SetArray(GX_VA_POS, &lakeSclaraevertpos[0], sizeof(f32)*3);
				GX_SetArray(GX_VA_NRM, &lakeSclaraevertnor[0], sizeof(f32)*3);
				drawArrConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC, lakeSclaraetricount, 0, 
						NULL, NULL, lakeSclaraevertposidx,
						NULL, NULL, lakeSclaraevertnoridx,
						NULL, NULL, NULL,
						NULL,
						lakeSclaraevertcol);
			} else {
				GX_SetArray(GX_VA_POS, &lakeSclaraeLoweredvertpos[0], sizeof(f32)*3);
				GX_SetArray(GX_VA_NRM, &lakeSclaraeLoweredvertnor[0], sizeof(f32)*3);
				drawArrConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC, lakeSclaraetricount, 0, 
						NULL, NULL, lakeSclaraeLoweredvertposidx,
						NULL, NULL, lakeSclaraeLoweredvertnoridx,
						NULL, NULL, NULL,
						NULL,
						lakeSclaraeLoweredvertcol);
			}


			drawLake();

		if(rframe == 188) {
			GX_SetCopyFilter(GX_FALSE, NULL, GX_TRUE, perfectfilter);
			GX_SetTexCopySrc(0, 0, ltw, lth);
			GX_SetTexCopyDst(ltw, lth, GX_TF_RGBA8, GX_FALSE);
			GX_CopyTex(Picture, GX_FALSE); 
			GX_PixModeSync();
			GX_InitTexObj(&PictureTexObj, Picture, ltw, lth, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
			GX_InvalidateTexAll();
		}


		if(rframe > 188) {
			GX_SetNumTevStages(1);
			GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_TEXC, GX_CC_ZERO);
			GX_SetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
			GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLORNULL);
			GX_SetNumTexGens(1);
			GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEX0, GX_IDENTITY);
			GX_LoadTexObj(&PictureTexObj, GX_TEXMAP0);		
			nolight();
			guMtxIdentity(model);

			liyt_genMtxPosRotZyx(model, photoAnim[(rframe * 6)], photoAnim[(rframe * 6) + 1], photoAnim[(rframe * 6) + 2], 
						photoAnim[(rframe * 6) + 3], photoAnim[(rframe * 6) + 4], photoAnim[(rframe * 6) + 5]);
			guMtxConcat(view, model, modelview);
			LX_LoadMtxImm(modelview, GX_PNMTX0);

			GX_SetCurrentMtx(GX_PNMTX0);

			GX_SetCullMode(GX_CULL_NONE);
			liy_VtxDescConfig(LV_VP | LV_TC, 0);
				GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
				GX_Position3f32(-0.09492f, 0.052798f, 0.0f);
				GX_TexCoord2f32(0.20f, 0.23f);//0.78 0.13	
				GX_Position3f32(0.100067f, 0.052798f, 0.0f);
				GX_TexCoord2f32(0.82f, 0.23f);
				GX_Position3f32(0.100067f, -0.070701f, 0.0f);
				GX_TexCoord2f32(0.82f, 0.88f);
				GX_Position3f32(-0.09492f, -0.070701f, 0.0f);
				GX_TexCoord2f32(0.20f, 0.88f);


			GX_LoadProjectionMtx(permIdentity44, GX_PERSPECTIVE);
			GX_LoadPosMtxImm(fullQuadMv, GX_PNMTX0);
			//liyrub_drawScreenquad(1.0f);

			//drawTestaxis();
		}


		//Postprocess
		
		GX_SetDispCopyGamma((f32)GX_GM_1_0);

		GX_SetCopyFilter(GX_FALSE, NULL, GX_TRUE, perfectfilter);
		
		GX_LoadProjectionMtx(permIdentity44, GX_PERSPECTIVE);
		GX_LoadPosMtxImm(fullQuadMv, GX_PNMTX0);
		GX_SetNumTevStages(1);
		GX_SetTevOp(GX_TEVSTAGE0,GX_REPLACE);
		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLORNULL);

		int dof = posttog & 1;
		//int dof = 0;

		//printf("Lily Float is %f\n", Lily_Float);

		if(dof) {
		float focuspoint = sqrt(focusAnim[rframe]);	
		float inverse = 1.0f / focuspoint;

		int inverseInt = (int)((float)inverse * 255.0f);
		int focusInt = (int)((float)focuspoint * 255.0f);
		int exposureInt = (int)((float)exposure * 255.0f);

		inverseInt += inverseIntEpsilonLut[focusInt];

		GXColor dofColA = {inverseInt, exposureInt, focusInt, 255};

		GX_SetTexCopySrc(0, 0, ltw, lth);
		GX_SetTexCopyDst(ltw, lth, GX_TF_Z16, GX_FALSE);
		GX_CopyTex(DofDefBufr, GX_FALSE); 
		GX_PixModeSync();
		GX_InitTexObj(&DofDefBufrTexObj, DofDefBufr, ltw, lth, GX_TF_Z16, GX_CLAMP, GX_CLAMP, GX_FALSE);

		GX_SetTexCopySrc(0, 0, ltw, lth);
		GX_SetTexCopyDst(ltw, lth, GX_TF_RGBA8, GX_FALSE);
		GX_CopyTex(DofDefClr, GX_FALSE); 
		GX_PixModeSync();
		GX_InitTexObj(&DofDefClrTexObj, DofDefClr, ltw, lth, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
		GX_InvalidateTexAll();
		GX_LoadTexObj(&DofDefClrTexObj, GX_TEXMAP0);

		//Create the lowres texture and copy it a couple times flipped to get the copy filter on many axes

		GX_SetNumTevStages(1);
		GX_SetTevOp(GX_TEVSTAGE0, GX_REPLACE);

		GX_SetNumTexGens(1);
		GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEX0, GX_IDENTITY);

		liyrub_drawRotScreenquad(4.0f);	
		GX_SetCopyFilter(GX_FALSE, NULL, GX_TRUE, blurfiltr);
		GX_SetTexCopySrc(0, 0, dsw, dsh);
		GX_SetTexCopyDst(dsw, dsh, GX_TF_RGBA8, GX_FALSE);
		GX_CopyTex(DofDefQtrsize, GX_FALSE); 
		GX_PixModeSync();
		GX_InitTexObj(&DofDefQtrsizeTexObj, DofDefQtrsize, dsw, dsh, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
		GX_InvalidateTexAll();
		GX_LoadTexObj(&DofDefQtrsizeTexObj, GX_TEXMAP0);

		for(int i = 0; i < 3; i++) {
			liyrub_drawRotScreenquad(4.0f);
			GX_SetTexCopySrc(0, 0, dsw, dsh);
			GX_SetTexCopyDst(dsw, dsh, GX_TF_RGBA8, GX_FALSE);
			GX_CopyTex(DofDefQtrsize, GX_FALSE); 
			GX_PixModeSync();
			GX_InitTexObj(&DofDefQtrsizeTexObj, DofDefQtrsize, dsw, dsh, GX_TF_RGBA8, GX_CLAMP, GX_CLAMP, GX_FALSE);
			GX_InvalidateTexAll();
			GX_LoadTexObj(&DofDefQtrsizeTexObj, GX_TEXMAP0);
		}

		//Done with wacky shit

		GX_SetCopyFilter(GX_FALSE, NULL, GX_TRUE, perfectfilter);

		GX_LoadTexObj(&DofDefBufrTexObj, GX_TEXMAP0);
		GX_LoadTexObj(&DofDefClrTexObj, GX_TEXMAP1);
		GX_LoadTexObj(&DofDefQtrsizeTexObj, GX_TEXMAP2);
	
		GX_SetNumTexGens(2);
		GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEX0, GX_IDENTITY);
		GX_SetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX3x4, GX_TG_TEX1, GX_IDENTITY);

		//TEV for a

		GX_SetNumTevStages(10);
		GX_SetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0_R); //A band
		GX_SetTevKColorSel(GX_TEVSTAGE3, GX_TEV_KCSEL_K0_B); //B band
		GX_SetTevKColorSel(GX_TEVSTAGE9, GX_TEV_KCSEL_K0_G); //exposure
		GX_SetTevKColor(GX_KCOLOR0, dofColA);

		GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_TEXC, GX_CC_KONST);
		GX_SetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0); //multiply first half of the curve by a konstant

		// output = d + ((a OP b) ? c : 0)
		GX_SetTevColorIn(GX_TEVSTAGE1, GX_CC_C0, GX_CC_ONE, GX_CC_ONE, GX_CC_ZERO);
		GX_SetTevColorOp(GX_TEVSTAGE1, GX_TEV_COMP_R8_EQ, GX_TB_SUBHALF, GX_CS_SCALE_2, GX_TRUE, GX_TEVPREV); //make white areas black
						//subhalf scale2 is to increase contrast in the matte
		GX_SetTevColorIn(GX_TEVSTAGE2, GX_CC_C0, GX_CC_ZERO, GX_CC_CPREV, GX_CC_ZERO);
		GX_SetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_SUBHALF, GX_CS_SCALE_2, GX_TRUE, GX_TEVREG0); //fisrt half of the curve is done
						//subhalf scale2 is to increase contrast in the matte
		//TEV for b

		GX_SetTevColorIn(GX_TEVSTAGE3, GX_CC_ONE, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST);
		GX_SetTevColorOp(GX_TEVSTAGE3, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG1); //invert and move up 

		// output = d + ((a OP b) ? c : 0)
		GX_SetTevColorIn(GX_TEVSTAGE4, GX_CC_C1, GX_CC_ONE, GX_CC_ONE, GX_CC_ZERO);
		GX_SetTevColorOp(GX_TEVSTAGE4, GX_TEV_COMP_R8_EQ, GX_TB_SUBHALF, GX_CS_SCALE_2, GX_TRUE, GX_TEVPREV); //make white areas black
						//subhalf scale2 is to increase contrast in the matte
		GX_SetTevColorIn(GX_TEVSTAGE5, GX_CC_C1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_ZERO);
		GX_SetTevColorOp(GX_TEVSTAGE5, GX_TEV_ADD, GX_TB_SUBHALF, GX_CS_SCALE_2, GX_TRUE, GX_TEVREG1); //make the second half of the curve
						//subhalf scale2 is to increase contrast in the matte
		GX_SetTevColorIn(GX_TEVSTAGE6, GX_CC_ZERO, GX_CC_ONE, GX_CC_C0, GX_CC_C1);      //TEVREG0
		GX_SetTevColorOp(GX_TEVSTAGE6, GX_TEV_ADD, GX_TB_SUBHALF, GX_CS_SCALE_2, GX_TRUE, GX_TEVREG0); //add two curves together
						//subhalf scale2 is to increase contrast in the matte
		GX_SetTevColorIn(GX_TEVSTAGE7, GX_CC_ZERO, GX_CC_TEXC, GX_CC_C0, GX_CC_ZERO);
		GX_SetTevColorOp(GX_TEVSTAGE7, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV); //multiply texture 1 by the curve
						//dont put one here it'll only apply to a half of the screen
		GX_SetTevColorIn(GX_TEVSTAGE8, GX_CC_TEXC, GX_CC_ZERO, GX_CC_C0, GX_CC_CPREV);
		GX_SetTevColorOp(GX_TEVSTAGE8, GX_TEV_ADD, GX_TB_ZERO, (posttog & 2) ? GX_CS_SCALE_2 : GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV); //multiply texture 2 by the curve's inverse and add last stage
						//hahahahahahaha these r just int defines by the way
		//none of the subhalf/scale2 are critical to operation of this effect, i just wanted a greater contrast in the matte

		GX_SetTevColorIn(GX_TEVSTAGE9, GX_CC_ZERO, GX_CC_CPREV, GX_CC_KONST, GX_CC_ZERO);
		GX_SetTevColorOp(GX_TEVSTAGE9, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, GX_TRUE, GX_TEVPREV); //exposure control
	
		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLORNULL);
		GX_SetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD0, GX_TEXMAP_NULL, GX_COLORNULL);
		GX_SetTevOrder(GX_TEVSTAGE2, GX_TEXCOORD0, GX_TEXMAP_NULL, GX_COLORNULL);
		GX_SetTevOrder(GX_TEVSTAGE3, GX_TEXCOORD0, GX_TEXMAP0, GX_COLORNULL);
		GX_SetTevOrder(GX_TEVSTAGE4, GX_TEXCOORD0, GX_TEXMAP_NULL, GX_COLORNULL);
		GX_SetTevOrder(GX_TEVSTAGE5, GX_TEXCOORD0, GX_TEXMAP_NULL, GX_COLORNULL);
		GX_SetTevOrder(GX_TEVSTAGE6, GX_TEXCOORD0, GX_TEXMAP_NULL, GX_COLORNULL);
		GX_SetTevOrder(GX_TEVSTAGE7, GX_TEXCOORD0, GX_TEXMAP1, GX_COLORNULL);
		GX_SetTevOrder(GX_TEVSTAGE8, GX_TEXCOORD1, GX_TEXMAP2, GX_COLORNULL);
		GX_SetTevOrder(GX_TEVSTAGE9, GX_TEXCOORD1, GX_TEXMAP2, GX_COLORNULL);


		liy_VtxDescConfig(LV_VP | LV_TC | LV_TC1, 0);
		GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
		GX_Position3f32(-1.0f, -1.0f, 0.0f);
		GX_TexCoord2f32(0.0f, 1.0f);	
		GX_TexCoord2f32(0.0f, 1.0f);
		GX_Position3f32(1.0f, -1.0f, 0.0f);
		GX_TexCoord2f32(1.0f, 1.0f);
		GX_TexCoord2f32(1.0f, 1.0f);
		GX_Position3f32(1.0f, 1.0f, 0.0f);
		GX_TexCoord2f32(1.0f, 0.0f);
		GX_TexCoord2f32(1.0f, 0.0f);
		GX_Position3f32(-1.0f, 1.0f, 0.0f);
		GX_TexCoord2f32(0.0f, 0.0f);
		GX_TexCoord2f32(0.0f, 0.0f);
		}

		GX_SetNumTevStages(1);
		GX_SetTevOp(GX_TEVSTAGE0, GX_REPLACE);
		GX_SetNumTexGens(1);
		GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX3x4, GX_TG_TEX0, GX_IDENTITY);
		GX_SetCopyFilter(GX_FALSE, NULL, GX_TRUE, blurfiltr);
	
		GX_SetCopyFilter(GX_FALSE, NULL, GX_TRUE, perfectfilter);

		// Overlay

		GX_SetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
		GX_SetCurrentMtx(GX_PNMTX0);

		GX_LoadProjectionMtx(overlayPersp, GX_PERSPECTIVE);
		
		liy_VtxDescConfig(LV_VP | LV_VC, 0);
		GX_SetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHTNULL, GX_DF_CLAMP, GX_AF_NONE);	

		GX_SetNumTevStages(1);
		GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
		GX_SetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	
		//dials
		/*
			guMtxIdentity(model);
			guMtxRotAxisRad(model, &zAxis, (float)pi+(framerate/9.554140f));
			guMtxTransApply(model, model, dialx, -3.7f, -10.0f);
			guMtxConcat(overlayView,model,modelview);
			GX_LoadPosMtxImm(modelview, GX_PNMTX0);
			GX_LoadNrmMtxImm(modelview, GX_PNMTX0);
			drawDial(0, 0, 0);

			guMtxIdentity(model);
			guMtxRotAxisRad(model, &zAxis, (float)pi+(frametimems/5.30780254f));
			//guMtxRotAxisRad(model, &zAxis, (float)pi+(grasstimems/5.30780254f));
			guMtxTransApply(model, model, dialx, -1.2f, -10.0f);
			guMtxConcat(overlayView,model,modelview);
			GX_LoadPosMtxImm(modelview, GX_PNMTX0);
			drawDial(0, 0, 0);*/

		// End overlay
		
		//End rendering
	
		GX_SetCopyFilter(GX_FALSE, NULL, GX_TRUE, vfilter);
		GX_SetDispCopyGamma((f32)GX_GM_1_0);

		if(rframe == 343) {
			GX_SetColorUpdate(GX_TRUE);
		} else {
			GX_SetColorUpdate(GX_FALSE);
		}

		GX_CopyDisp(frameBuffer[fb],GX_TRUE); //fb is cleared after copy when true
		GX_SetColorUpdate(GX_TRUE);
	

		GX_DrawDone();

		frame++;

		u64 endframe = gettime();
		frametime = endframe - startframe;
		frametimems = (double)ticks_to_microsecs(frametime) / 1000.0f;
	
		VIDEO_SetNextFramebuffer(frameBuffer[fb]);

		#ifdef MINUSCONSOLE
		if(consoletog) {
			VIDEO_SetNextFramebuffer(consoleTex);
		}
		#endif
		VIDEO_Flush();
		VIDEO_WaitVSync();

		fb ^= 1;


		//rquad -= 1.5f;

		framerate = liym_min((1000.0f / frametimems), 60.0f);

		if(frametimems < (100.0f / 6.0f)){
			//VIDEO_WaitVSync(); 
		}

		#ifdef GUY_DEV
			#ifdef MINUSCONSOLE
			if(consoletog == 1) {
			#endif
				if(frame % 20 == 0) {
					//printf("%f ms for the blendshapes\n", blendtimems);
					printf("%f ms for entire loop\n%f FPS\n", frametimems, framerate);
					//printf("Lily float is %f\n", Lily_Float);
				}
			#ifdef MINUSCONSOLE
			}
			#endif
		#endif		

		if(play | advance) {
			//printf("%d\r", rframe);
			rframe++;
		}
		advance = 0;
	}
}
