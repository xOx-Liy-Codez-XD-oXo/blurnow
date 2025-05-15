#ifndef PRIMITIVES_H
#define PRIMITIVES_H

float flip_endian_float(float f) {
    union {
        float f;
        uint32_t i;
        uint8_t b[4];
    } data;
    data.f = f;
    return (float)((data.b[0] << 24) | (data.b[1] << 16) | (data.b[2] << 8) | data.b[3]);
}

void drawTestaxis() {
	GX_SetNumTevStages(1);
	GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_RASC, GX_CC_ZERO);
	GX_SetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
	GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);
	nolight();
	GX_SetLineWidth(6, 6);
	liy_VtxDescConfig(LV_VP | LV_VC, GX_VTXFMT0);
	GX_Begin(GX_LINES, GX_VTXFMT0, 6);
		GX_Position3f32(0.0f, 0.0f, 0.0f);
		GX_Color3u8(255, 0, 0);
		GX_Position3f32(1.0f, 0.0f, 0.0f);
		GX_Color3u8(255, 0, 0);

		GX_Position3f32(0.0f, 0.0f, 0.0f);
		GX_Color3u8(0, 255, 0);
		GX_Position3f32(0.0f, 1.0f, 0.0f);
		GX_Color3u8(0, 255, 0);

		GX_Position3f32(0.0f, 0.0f, 0.0f);
		GX_Color3u8(0, 0, 255);
		GX_Position3f32(0.0f, 0.0f, 1.0f);
		GX_Color3u8(0, 0, 255);
}

void drawCubePNT(int vtxfmt) {
	GX_Begin(GX_QUADS, vtxfmt, 24);			// Draw a Cube

			GX_Position3f32(-1.0f,1.0f,1.0f);	// Top Left of the quad (top)
			GX_Normal3f32(-1.0f,1.0f,1.0f);
			GX_TexCoord2f32(1.0f,0.0f);
			GX_Position3f32(1.0f,1.0f,1.0f);	// Top Right of the quad (top)
			GX_Normal3f32(1.0f,1.0f,1.0f);	// Top Right of the quad (top)
			GX_TexCoord2f32(1.0f,1.0f);
			GX_Position3f32(1.0f,1.0f,-1.0f);	// Bottom Right of the quad (top)
			GX_Normal3f32(1.0f,1.0f,-1.0f);	// Bottom Right of the quad (top)
			GX_TexCoord2f32(0.0f,1.0f);
			GX_Position3f32(-1.0f,1.0f,-1.0f);		// Bottom Left of the quad (top)
			GX_Normal3f32(-1.0f,1.0f,-1.0f);		// Bottom Left of the quad (top)
			GX_TexCoord2f32(0.0f,0.0f);

			GX_Position3f32(-1.0f,-1.0f,1.0f);	// Top Left of the quad (bottom)
			GX_Normal3f32(-1.0f,-1.0f,1.0f);	// Top Left of the quad (bottom)
			GX_TexCoord2f32(1.0f,0.0f);
			GX_Position3f32(1.0f,-1.0f,1.0f);	// Top Right of the quad (bottom)
			GX_Normal3f32(1.0f,-1.0f,1.0f);	// Top Right of the quad (bottom)
			GX_TexCoord2f32(1.0f,1.0f);
			GX_Position3f32(1.0f,-1.0f,-1.0f);	// Bottom Right of the quad (bottom)
			GX_Normal3f32(1.0f,-1.0f,-1.0f);	// Bottom Right of the quad (bottom)
			GX_TexCoord2f32(0.0f,1.0f);
			GX_Position3f32(-1.0f,-1.0f,-1.0f);		// Bottom Left of the quad (bottom)
			GX_Normal3f32(-1.0f,-1.0f,-1.0f);		// Bottom Left of the quad (bottom)
			GX_TexCoord2f32(0.0f,0.0f);

			GX_Position3f32(-1.0f,1.0f,1.0f);	// Top Left of the quad (front)
			GX_Normal3f32(-1.0f,1.0f,1.0f);	// Top Left of the quad (front)
			GX_TexCoord2f32(1.0f,0.0f);
			GX_Position3f32(-1.0f,-1.0f,1.0f);	// Top Right of the quad (front)
			GX_Normal3f32(-1.0f,-1.0f,1.0f);	// Top Right of the quad (front)
			GX_TexCoord2f32(1.0f,1.0f);
			GX_Position3f32(1.0f,-1.0f,1.0f);	// Bottom Right of the quad (front)
			GX_Normal3f32(1.0f,-1.0f,1.0f);	// Bottom Right of the quad (front)
			GX_TexCoord2f32(0.0f,1.0f);
			GX_Position3f32(1.0f,1.0f,1.0f);		// Bottom Left of the quad (front)
			GX_Normal3f32(1.0f,1.0f,1.0f);		// Bottom Left of the quad (front)
			GX_TexCoord2f32(0.0f,0.0f);

			GX_Position3f32(-1.0f,1.0f,-1.0f);	// Top Left of the quad (back)
			GX_Normal3f32(-1.0f,1.0f,-1.0f);	// Top Left of the quad (back)
			GX_TexCoord2f32(1.0f,0.0f);
			GX_Position3f32(-1.0f,-1.0f,-1.0f);	// Top Right of the quad (back)
			GX_Normal3f32(-1.0f,-1.0f,-1.0f);	// Top Right of the quad (back)
			GX_TexCoord2f32(1.0f,1.0f);
			GX_Position3f32(1.0f,-1.0f,-1.0f);	// Bottom Right of the quad (back)
			GX_Normal3f32(1.0f,-1.0f,-1.0f);	// Bottom Right of the quad (back)
			GX_TexCoord2f32(0.0f,1.0f);
			GX_Position3f32(1.0f,1.0f,-1.0f);		// Bottom Left of the quad (back)
			GX_Normal3f32(1.0f,1.0f,-1.0f);		// Bottom Left of the quad (back)
			GX_TexCoord2f32(0.0f,0.0f);

			GX_Position3f32(-1.0f,1.0f,1.0f);	// Top Left of the quad (left)
			GX_Normal3f32(-1.0f,1.0f,1.0f);	// Top Left of the quad (left)
			GX_TexCoord2f32(1.0f,0.0f);
			GX_Position3f32(-1.0f,1.0f,-1.0f);	// Top Right of the quad (back)
			GX_Normal3f32(-1.0f,1.0f,-1.0f);	// Top Right of the quad (back)
			GX_TexCoord2f32(1.0f,1.0f);
			GX_Position3f32(-1.0f,-1.0f,-1.0f);	// Bottom Right of the quad (back)
			GX_Normal3f32(-1.0f,-1.0f,-1.0f);	// Bottom Right of the quad (back)
			GX_TexCoord2f32(0.0f,1.0f);
			GX_Position3f32(-1.0f,-1.0f,1.0f);		// Bottom Left of the quad (back)
			GX_Normal3f32(-1.0f,-1.0f,1.0f);		// Bottom Left of the quad (back)
			GX_TexCoord2f32(0.0f,0.0f);

			GX_Position3f32(1.0f,1.0f,1.0f);	// Top Left of the quad (right)
			GX_Normal3f32(1.0f,1.0f,1.0f);	// Top Left of the quad (right)
			GX_TexCoord2f32(1.0f,0.0f);
			GX_Position3f32(1.0f,1.0f,-1.0f);	// Top Right of the quad (right)
			GX_Normal3f32(1.0f,1.0f,-1.0f);	// Top Right of the quad (right)
			GX_TexCoord2f32(1.0f,1.0f);
			GX_Position3f32(1.0f,-1.0f,-1.0f);	// Bottom Right of the quad (right)
			GX_Normal3f32(1.0f,-1.0f,-1.0f);	// Bottom Right of the quad (right)
			GX_TexCoord2f32(0.0f,1.0f);
			GX_Position3f32(1.0f,-1.0f,1.0f);		// Bottom Left of the quad (right)
			GX_Normal3f32(1.0f,-1.0f,1.0f);		// Bottom Left of the quad (right)
			GX_TexCoord2f32(0.0f,0.0f);
	GX_End(); 									// Done Drawing The Quad
}



// Functions for sending arrays of data to FIFO in direct mode. Organized in 0 particular order



// FIFO overflows if we send it too many triangles
// in direct mode in a single call, so the Largeformat
// functions split them into multiple calls of
// triangles, the amount of triangles in each batch
// here . 
#define LARGEFORMAT_BATCHSIZE 4000

void drawArrDirectP(float *verts, int tricount, int vtxfmt) {
	GX_Begin(GX_TRIANGLES, vtxfmt, tricount * 3);
		for(int i = 0; i < tricount * 3; i++) {
			GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
		}
	GX_End(); 
}

void drawArrDirectPT(float *verts, float *texCoords, int tricount, int vtxfmt) {
	GX_Begin(GX_TRIANGLES, vtxfmt, tricount * 3);
		for(int i = 0; i < tricount * 3; i++) {
			GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
			GX_TexCoord2f32(texCoords[(i*2)],texCoords[(i*2)+1]);
		}
	GX_End(); 
}

void drawArrDirectPNT(float *verts, float *vertNors, float *texCoords, int tricount, int vtxfmt) {
	GX_Begin(GX_TRIANGLES, vtxfmt, tricount * 3);
		for(int i = 0; i < tricount * 3; i++) {
			GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
			GX_Normal3f32(vertNors[(i*3)],vertNors[(i*3)+1],vertNors[(i*3)+2]);
			GX_TexCoord2f32(texCoords[(i*2)],texCoords[(i*2)+1]);
		}
	GX_End(); 
}

void drawArrDirectPNT_Largeformat(float* verts, float* vertNors, float *texCoords, int tricount, int vtxfmt) {
	int trisleft = tricount;
	int trissubtracted = 0;
	while(trisleft > 0) {
		int tristhisround = liym_min(trisleft, LARGEFORMAT_BATCHSIZE);
		GX_Begin(GX_TRIANGLES, vtxfmt, tristhisround * 3);
			for(int i = (3 * trissubtracted); i < 3 * (trissubtracted + tristhisround); i++) {
				GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
				GX_Normal3f32(vertNors[(i*3)],vertNors[(i*3)+1],vertNors[(i*3)+2]);
				GX_TexCoord2f32(texCoords[(i*2)],texCoords[(i*2)+1]);
			}
		GX_End();
		trisleft = trisleft - tristhisround;
		trissubtracted = trissubtracted + tristhisround;
	}
}

void drawArrDirectPN_Largeformat(float* verts, float* vertNors, int tricount, int vtxfmt) {
	int trisleft = tricount;
	int trissubtracted = 0;
	while(trisleft > 0) {
		int tristhisround = liym_min(trisleft, LARGEFORMAT_BATCHSIZE);
		GX_Begin(GX_TRIANGLES, vtxfmt, tristhisround * 3);
			for(int i = (3 * trissubtracted); i < 3 * (trissubtracted + tristhisround); i++) {
				GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
				GX_Normal3f32(vertNors[(i*3)],vertNors[(i*3)+1],vertNors[(i*3)+2]);
			}
		GX_End();
		trisleft = trisleft - tristhisround;
		trissubtracted = trissubtracted + tristhisround;
	}
}

void drawArrDirectPNTPmtx_Largeformat(float* verts, float* vertNors, float *texCoords, int *mtxidx, int tricount, int vtxfmt) {
	int trisleft = tricount;
	int trissubtracted = 0;
	while(trisleft > 0) {
		int tristhisround = liym_min(trisleft, LARGEFORMAT_BATCHSIZE);
		GX_Begin(GX_TRIANGLES, vtxfmt, tristhisround * 3);
			for(int i = (3 * trissubtracted); i < 3 * (trissubtracted + tristhisround); i++) {
				GX_MatrixIndex1x8(mtxidx[i]*3);
				GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
				GX_Normal3f32(vertNors[(i*3)],vertNors[(i*3)+1],vertNors[(i*3)+2]);
				GX_TexCoord2f32(texCoords[(i*2)],texCoords[(i*2)+1]);
			}
		GX_End();
		trisleft = trisleft - tristhisround;
		trissubtracted = trissubtracted + tristhisround;
	}
}

void drawArrDirectPNTPmtx(float* verts, float* vertNors, float *texCoords, int *mtxidx, int tricount, int vtxfmt) {
	GX_Begin(GX_TRIANGLES, vtxfmt, tricount * 3);
		for(int i = 0; i < tricount * 3; i++) {
			GX_MatrixIndex1x8(mtxidx[i]*3);
			GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
			GX_Normal3f32(vertNors[(i*3)],vertNors[(i*3)+1],vertNors[(i*3)+2]);
			GX_TexCoord2f32(texCoords[(i*2)],texCoords[(i*2)+1]);
		}
	GX_End();
}

void drawArrDirectPTPmtx(float* verts, float *texCoords, int *mtxidx, int tricount, int vtxfmt) {
	GX_Begin(GX_TRIANGLES, vtxfmt, tricount * 3);
		for(int i = 0; i < tricount * 3; i++) {
			GX_MatrixIndex1x8(mtxidx[i]*3);
			GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
			GX_TexCoord2f32(texCoords[(i*2)],texCoords[(i*2)+1]);
		}
	GX_End();
}

void drawArrDirectPNTPmtxCidx_Largeformat(float* verts, float* vertNors, float *texCoords, int *mtxidx, int *colidx, int tricount, int vtxfmt) {
	int trisleft = tricount;
	int trissubtracted = 0;
	while(trisleft > 0) {
		int tristhisround = liym_min(trisleft, LARGEFORMAT_BATCHSIZE);
		GX_Begin(GX_TRIANGLES, vtxfmt, tristhisround * 3);
			for(int i = (3 * trissubtracted); i < 3 * (trissubtracted + tristhisround); i++) {
				GX_MatrixIndex1x8(mtxidx[i]*3);
				GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
				GX_Normal3f32(vertNors[(i*3)],vertNors[(i*3)+1],vertNors[(i*3)+2]);
				GX_Color1x8(colidx[i]);
				GX_TexCoord2f32(texCoords[(i*2)],texCoords[(i*2)+1]);
			}
		GX_End();
		trisleft = trisleft - tristhisround;
		trissubtracted = trissubtracted + tristhisround;
	}
}

void drawArrDirectPNTPmtxCidx(float* verts, float* vertNors, float *texCoords, int *mtxidx, int *colidx, int tricount, int vtxfmt) {
	GX_Begin(GX_TRIANGLES, vtxfmt, tricount * 3);
		for(int i = 0; i < tricount * 3; i++) {
			GX_MatrixIndex1x8(mtxidx[i]*3);
			GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
			GX_Normal3f32(vertNors[(i*3)],vertNors[(i*3)+1],vertNors[(i*3)+2]);
			GX_Color1x8(colidx[i]);
			GX_TexCoord2f32(texCoords[(i*2)],texCoords[(i*2)+1]);
		}
	GX_End();
}

void drawArrDirectPNCidx(float* verts, float* vertNors, int *colidx, int tricount, int vtxfmt) {
	GX_Begin(GX_TRIANGLES, vtxfmt, tricount * 3);
		for(int i = 0; i < tricount * 3; i++) {
			GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
			GX_Normal3f32(vertNors[(i*3)],vertNors[(i*3)+1],vertNors[(i*3)+2]);
			GX_Color1x8(colidx[i]);
		}
	GX_End(); 
}

void drawArrDirectPNCPmtx_UniformCol(float* verts, float* vertNors, int *mtxidx, int tricount, GXColor *col, int vtxfmt) {
	GX_Begin(GX_TRIANGLES, vtxfmt, tricount * 3);
		for(int i = 0; i < tricount * 3; i++) {
			GX_MatrixIndex1x8(mtxidx[i]*3);
			GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
			GX_Normal3f32(vertNors[(i*3)],vertNors[(i*3)+1],vertNors[(i*3)+2]);
			GX_Color3u8(col->r, col->g, col->b);
		}
	GX_End();
}

void drawArrDirectPC_UniformCol(float *verts, int tricount, GXColor *col, int vtxfmt) {
	GX_Begin(GX_TRIANGLES, vtxfmt, tricount * 3);
		for(int i = 0; i < tricount * 3; i++) {
			GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
			GX_Color3u8(col->r, col->g, col->b);
		}
	GX_End(); 
}

void drawArrDirectPNC_UniformCol(float* verts, float* vertNors, int tricount, GXColor *col, int vtxfmt) {
	GX_Begin(GX_TRIANGLES, vtxfmt, tricount * 3);
		for(int i = 0; i < tricount * 3; i++) {
			GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
			GX_Normal3f32(vertNors[(i*3)],vertNors[(i*3)+1],vertNors[(i*3)+2]);
			GX_Color3u8(col->r, col->g, col->b);
		}
	GX_End(); 
}

void drawArrDirectPNC_UniformCol_Largeformat(float* verts, float* vertNors, int tricount, GXColor *col, int vtxfmt) {
	int trisleft = tricount;
	int trissubtracted = 0;
	while(trisleft > 0) {
		int tristhisround = liym_min(trisleft, LARGEFORMAT_BATCHSIZE);
		GX_Begin(GX_TRIANGLES, vtxfmt, tristhisround * 3);
			for(int i = (3 * trissubtracted); i < 3 * (trissubtracted + tristhisround); i++) {
				GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
				GX_Normal3f32(vertNors[(i*3)],vertNors[(i*3)+1],vertNors[(i*3)+2]);
				GX_Color3u8(col->r, col->g, col->b);
			}
		GX_End();
		trisleft = trisleft - tristhisround;
		trissubtracted = trissubtracted + tristhisround;
	}
}

void drawArrDirectPCPmtx_UniformCol(float* verts, int *mtxidx, int tricount, GXColor *col, int vtxfmt) {
	GX_Begin(GX_TRIANGLES, vtxfmt, tricount * 3);
		for(int i = 0; i < tricount * 3; i++) {
			GX_MatrixIndex1x8(mtxidx[i]*3);
			GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
			GX_Color3u8(col->r, col->g, col->b);
		}
	GX_End(); 
}

void drawArrDirectPNCPmtx_Largeformat(float* verts, float* vertNors, int *vertCols, int *mtxidx, int tricount, int vtxfmt) {
	int trisleft = tricount;
	int trissubtracted = 0;
	while(trisleft > 0) {
		int tristhisround = liym_min(trisleft, LARGEFORMAT_BATCHSIZE);
		GX_Begin(GX_TRIANGLES, vtxfmt, tristhisround * 3);
			for(int i = (3 * trissubtracted); i < 3 * (trissubtracted + tristhisround); i++) {
				GX_MatrixIndex1x8(mtxidx[i]*3);
				GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
				GX_Normal3f32(vertNors[(i*3)],vertNors[(i*3)+1],vertNors[(i*3)+2]);
				GX_Color3u8( (u8)vertCols[(i*3)] , (u8)vertCols[(i*3)+1], (u8)vertCols[(i*3)+2] );
			}
		GX_End();
		trisleft = trisleft - tristhisround;
		trissubtracted = trissubtracted + tristhisround;
	}
}

void drawArrDirectPNCPmtx(float* verts, float* vertNors, int *vertCols, int *mtxidx, int tricount, int vtxfmt) {
	GX_Begin(GX_TRIANGLES, vtxfmt, tricount * 3);
		for(int i = 0; i < tricount * 3; i++) {
			GX_MatrixIndex1x8(mtxidx[i]*3);
			GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
			GX_Normal3f32(vertNors[(i*3)],vertNors[(i*3)+1],vertNors[(i*3)+2]);
			GX_Color3u8( (u8)vertCols[(i*3)] , (u8)vertCols[(i*3)+1], (u8)vertCols[(i*3)+2] );
		}
	GX_End(); 
}

void drawArrDirectPNCPmtxT(float* verts, float* vertNors, float* texCoords, int *vertCols, int *mtxidx, int tricount, int vtxfmt) {
	GX_Begin(GX_TRIANGLES, vtxfmt, tricount * 3);
		for(int i = 0; i < tricount * 3; i++) {
			GX_MatrixIndex1x8(mtxidx[i]*3);
			GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
			GX_Normal3f32(vertNors[(i*3)],vertNors[(i*3)+1],vertNors[(i*3)+2]);
			GX_Color3u8( (u8)vertCols[(i*3)] , (u8)vertCols[(i*3)+1], (u8)vertCols[(i*3)+2] );
			GX_TexCoord2f32(texCoords[(i*2)],texCoords[(i*2)+1]);
		}
	GX_End(); 
}

void drawArrDirectPNCPmtxT_Largeformat(float* verts, float* vertNors, float* texCoords, int *vertCols, int *mtxidx, int tricount, int vtxfmt) {
	int trisleft = tricount;
	int trissubtracted = 0;
	while(trisleft > 0) {
		int tristhisround = liym_min(trisleft, LARGEFORMAT_BATCHSIZE);
		GX_Begin(GX_TRIANGLES, vtxfmt, tristhisround * 3);
			for(int i = (3 * trissubtracted); i < 3 * (trissubtracted + tristhisround); i++) {
				GX_MatrixIndex1x8(mtxidx[i]*3);
				GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
				GX_Normal3f32(vertNors[(i*3)],vertNors[(i*3)+1],vertNors[(i*3)+2]);
				GX_Color3u8( (u8)vertCols[(i*3)] , (u8)vertCols[(i*3)+1], (u8)vertCols[(i*3)+2] );
				GX_TexCoord2f32(texCoords[(i*2)],texCoords[(i*2)+1]);
			}
		GX_End();
		trisleft = trisleft - tristhisround;
		trissubtracted = trissubtracted + tristhisround;
	} 
}

void drawArrDirectPidxNidxCPmtxTidx_Largeformat(uint16_t* verts, uint16_t* vertNors, uint16_t* texCoords, uint8_t *vertCols, uint8_t *mtxidx, int tricount, int vtxfmt) {
	int trisleft = tricount;
	int trissubtracted = 0;
	while(trisleft > 0) {
		int tristhisround = liym_min(trisleft, LARGEFORMAT_BATCHSIZE);
		GX_Begin(GX_TRIANGLES, vtxfmt, tristhisround * 3);
			for(int i = (3 * trissubtracted); i < 3 * (trissubtracted + tristhisround); i++) {
				GX_MatrixIndex1x8(mtxidx[i]*3);
				GX_Position1x16(verts[i]);
				GX_Normal1x16(vertNors[i]);
				GX_Color3u8( (u8)vertCols[(i*3)] , (u8)vertCols[(i*3)+1], (u8)vertCols[(i*3)+2] );
				GX_TexCoord1x16(texCoords[i]);
			}
		GX_End();
		trisleft = trisleft - tristhisround;
		trissubtracted = trissubtracted + tristhisround;
	} 
}

void drawArrDirectPidxNidxPmtxTidx_Largeformat(uint16_t* verts, uint16_t* vertNors, uint16_t* texCoords, uint8_t *mtxidx, int tricount, int vtxfmt) {
	int trisleft = tricount;
	int trissubtracted = 0;
	while(trisleft > 0) {
		int tristhisround = liym_min(trisleft, LARGEFORMAT_BATCHSIZE);
		GX_Begin(GX_TRIANGLES, vtxfmt, tristhisround * 3);
			for(int i = (3 * trissubtracted); i < 3 * (trissubtracted + tristhisround); i++) {
				GX_MatrixIndex1x8(mtxidx[i]*3);
				GX_Position1x16(verts[i]);
				GX_Normal1x16(vertNors[i]);
				GX_TexCoord1x16(texCoords[i]);
			}
		GX_End();
		trisleft = trisleft - tristhisround;
		trissubtracted = trissubtracted + tristhisround;
	} 
}

void drawArrDirectPNC(float* verts, float* vertNors, int *vertCols, int tricount, int vtxfmt) {
	GX_Begin(GX_TRIANGLES, vtxfmt, tricount * 3);
		for(int i = 0; i < tricount * 3; i++) {
			GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
			GX_Normal3f32(vertNors[(i*3)],vertNors[(i*3)+1],vertNors[(i*3)+2]);
			GX_Color3u8( (u8)vertCols[(i*3)] , (u8)vertCols[(i*3)+1], (u8)vertCols[(i*3)+2] );
		}
	GX_End(); 
}

void drawArrDirectPNC_Largeformat(float* verts, float* vertNors, int *vertCols, int tricount, int vtxfmt) {
	int trisleft = tricount;
	int trissubtracted = 0;
	while(trisleft > 0) {
		int tristhisround = liym_min(trisleft, LARGEFORMAT_BATCHSIZE);
		GX_Begin(GX_TRIANGLES, vtxfmt, tristhisround * 3);
			for(int i = (3 * trissubtracted); i < 3 * (trissubtracted + tristhisround); i++) {
				GX_Position3f32(verts[(i*3)],verts[(i*3)+1],verts[(i*3)+2]);
				GX_Normal3f32(vertNors[(i*3)],vertNors[(i*3)+1],vertNors[(i*3)+2]);
				GX_Color3u8( (u8)vertCols[(i*3)] , (u8)vertCols[(i*3)+1], (u8)vertCols[(i*3)+2] );
			}
		GX_End();
		trisleft = trisleft - tristhisround;
		trissubtracted = trissubtracted + tristhisround;
	}
}

void drawArrConfig(uint16_t config, long tricount, uint8_t vtxfmt, 
                   float *vertpos, uint8_t *vpindex8, uint16_t *vpindex16, 
                   float *vertnor, uint8_t *vnindex8, uint16_t *vnindex16, 
                   float *texcord, uint8_t *tcindex8, uint16_t *tcindex16, 
                   uint8_t *mtxidx, 
                   uint8_t *vertcol) {
	GX_Begin(GX_TRIANGLES, vtxfmt, tricount * 3);
	for(int i = 0; i < tricount * 3; i++) {
		int i3 = i * 3;
		if(config & LV_MX)     { GX_MatrixIndex1x8( mtxidx[i] * 3); }
		if(config & LV_VP)     { GX_Position3f32(vertpos[i3], vertpos[i3+1], vertpos[i3+2]); }
		if(config & LV_VPIDX8) { GX_Position1x8( vpindex8[i] ); }
		if(config & LV_VPIDX16){ GX_Position1x16( vpindex16[i] ); }
		if(config & LV_VN)     { GX_Normal3f32(vertnor[i3], vertnor[i3+1], vertnor[i3+2]); }
		if(config & LV_VNIDX8) { GX_Normal1x8( vnindex8[i] ); }
		if(config & LV_VNIDX16){ GX_Normal1x16( vnindex16[i] ); }
		if(config & LV_VC)     { GX_Color3u8(vertcol[i3], vertcol[i3+1], vertcol[i3+2]); }
		if(config & LV_TC)     { int i2 = i*2; GX_TexCoord2f32(texcord[i2], texcord[i2+1]); }
		if(config & LV_TCIDX8) { GX_TexCoord1x8( tcindex8[i] ); }
		if(config & LV_TCIDX16){ GX_TexCoord1x16( tcindex16[i] ); }
	}
}

#endif