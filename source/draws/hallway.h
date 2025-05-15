#ifndef HALLWAY_DRAW_H
#define HALLWAY_DRAW_H
void drawHallway(float l1, float l2) {
	float hallscale = 0.2f; //1 / 5
			Mtx hallOrigin, hallOriginMv;
			guMtxIdentity(hallOrigin);
			guMtxScaleApply(hallOrigin, hallOrigin, hallscale, hallscale, hallscale);
			guMtxConcat(view, hallOrigin, hallOriginMv);

			nolight();

			GX_LoadTexObj(&hallway0TexObj, GX_TEXMAP0);
			GX_LoadTexObj(&hallway1TexObj, GX_TEXMAP1);
			GX_LoadTexObj(&hallwaytilesTexObj, GX_TEXMAP2);
			
			GX_SetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHTNULL, GX_DF_CLAMP, GX_AF_NONE);	

			GX_SetNumTevStages(1);

			GX_SetTevOp(GX_TEVSTAGE0,GX_PASSCLR);
	
			GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_RASC, GX_CC_ZERO);
			GX_SetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

			GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);


			LX_LoadMtxImm(hallOriginMv, GX_PNMTX0);

			GX_SetArray(GX_VA_POS, &hallwayvcolexvertpos[0], sizeof(f32)*3);

			liy_VtxDescConfig(LV_VPIDX16 | LV_VC, 0); 
			drawArrConfig(LV_VPIDX16 | LV_VC, hallwayvcolextricount, 0, 
					NULL, NULL, hallwayvcolexvertposidx,
					NULL, NULL, NULL,
					NULL, NULL, NULL,
					NULL,
					hallwayvcolexvertcol);

			guMtxIdentity(model);
			guMtxRotAxisRad(model, &zAxis, 0.5f);
			guMtxTransApply(model, model, 34.54f, 5.73f, 7.63f);
			guMtxScaleApply(model, model, hallscale, hallscale, hallscale);
			guMtxConcat(view, model, modelview);
			LX_LoadMtxImm(modelview, GX_PNMTX0);

			GX_SetArray(GX_VA_POS, &hallwayoplockervertpos[0], sizeof(f32)*3);
			
			drawArrConfig(LV_VPIDX16 | LV_VC, hallwayoplockertricount, 0, 
					NULL, NULL, hallwayoplockervertposidx,
					NULL, NULL, NULL,
					NULL, NULL, NULL,
					NULL,
					hallwayoplockervertcol);

			guMtxIdentity(model);
			guMtxRotAxisRad(model, &zAxis, 0.5f);
			guMtxTransApply(model, model, 42.61f, 5.73f, 7.63f);
			guMtxScaleApply(model, model, hallscale, hallscale, hallscale);
			guMtxConcat(view, model, modelview);
			LX_LoadMtxImm(modelview, GX_PNMTX0);
			
			drawArrConfig(LV_VPIDX16 | LV_VC, hallwayoplockertricount, 0, 
					NULL, NULL, hallwayoplockervertposidx,
					NULL, NULL, NULL,
					NULL, NULL, NULL,
					NULL,
					hallwayoplockervertcol);

			GX_SetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHTNULL, GX_DF_CLAMP, GX_AF_NONE);

			GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_TEXC, GX_CC_ZERO);
			GX_SetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

			GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);

			LX_LoadMtxImm(hallOriginMv, GX_PNMTX0);

			GX_SetArray(GX_VA_POS, &hallwayfakelockersvertpos[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_TEX0, &hallwayfakelockerstexcord[0], sizeof(f32)*2);

			liy_VtxDescConfig(LV_VPIDX16 | LV_TCIDX16, 0); 
			drawArrConfig(LV_VPIDX16 | LV_TCIDX16, hallwayfakelockerstricount, 0, 
					NULL, NULL, hallwayfakelockersvertposidx,
					NULL, NULL, NULL,
					NULL, NULL, hallwayfakelockerstexcordidx,
					NULL,
					NULL);

			GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP1, GX_COLOR0A0);
	
			GX_SetArray(GX_VA_POS, &hallwayendcapvertpos[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_TEX0, &hallwayendcaptexcord[0], sizeof(f32)*2);

			drawArrConfig(LV_VPIDX16 | LV_TCIDX16, hallwayendcaptricount, 0, 
					NULL, NULL, hallwayendcapvertposidx,
					NULL, NULL, NULL,
					NULL, NULL, hallwayendcaptexcordidx,
					NULL,
					NULL);

			GX_SetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHTNULL, GX_DF_CLAMP, GX_AF_NONE);

			GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
			GX_SetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);

			GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP2, GX_COLOR0A0);

			GX_SetArray(GX_VA_POS, &hallwayfloorceilvertpos[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_TEX0, &hallwayfloorceiltexcord[0], sizeof(f32)*2);

			liy_VtxDescConfig(LV_VPIDX16 | LV_TCIDX16 | LV_VC, 0); 
			drawArrConfig(LV_VPIDX16 | LV_TCIDX16 | LV_VC, hallwayfloorceiltricount, 0, 
					NULL, NULL, hallwayfloorceilvertposidx,
					NULL, NULL, NULL,
					NULL, NULL, hallwayfloorceiltexcordidx,
					NULL,
					hallwayfloorceilvertcol);
			
			GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);

			GX_SetArray(GX_VA_POS, &hallwaycorkboardvertpos[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_TEX0, &hallwaycorkboardtexcord[0], sizeof(f32)*2);
	
			drawArrConfig(LV_VPIDX16 | LV_TCIDX16 | LV_VC, hallwaycorkboardtricount, 0, 
					NULL, NULL, hallwaycorkboardvertposidx,
					NULL, NULL, NULL,
					NULL, NULL, hallwaycorkboardtexcordidx,
					NULL,
					hallwaycorkboardvertcol);
}
#endif
