#ifndef GILL_DRAW_H
#define GILL_DRAW_H
void drawGill() {
			GX_SetNumTevStages(1);

			GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_ONE, GX_CC_ZERO);
			GX_SetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_2, GX_TRUE, GX_TEVPREV);

			GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);

			GX_LoadTexObj(&gilltexTexObj, GX_TEXMAP0);

			GX_SetArray(GX_VA_POS, &gillTorsovertpos[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_NRM, &gillTorsovertnor[0], sizeof(f32)*3);

			liy_batchLoadMtx(gillSkeletonMv, gillTorsoMtxidxTable, LM_ALL);
			liy_VtxDescConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC | LV_MX, 0); 
			drawArrConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC | LV_MX, gillTorsotricount, 0, 
					NULL, NULL, gillTorsovertposidx,
					NULL, NULL, gillTorsovertnoridx,
					NULL, NULL, NULL,
					gillTorsomtxidx,
					gillTorsovertcol);

			GX_SetArray(GX_VA_POS, &gillHandLvertpos[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_NRM, &gillHandLvertnor[0], sizeof(f32)*3);

			liy_batchLoadMtx(gillSkeletonMv, gillHandLMtxidxTable, LM_0 | LM_1 | LM_2 | LM_3);
			liy_VtxDescConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC | LV_MX, 0); 
			drawArrConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC | LV_MX, gillHandLtricount, 0, 
					NULL, NULL, gillHandLvertposidx,
					NULL, NULL, gillHandLvertnoridx,
					NULL, NULL, NULL,
					gillHandLmtxidx,
					gillHandLvertcol);

			GX_SetArray(GX_VA_POS, &gillHandLfingersvertpos[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_NRM, &gillHandLfingersvertnor[0], sizeof(f32)*3);

			liy_batchLoadMtx(gillSkeletonMv, gillHandLfingersMtxidxTable, LM_ALL);	
			liy_VtxDescConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC | LV_MX, 0); 
			drawArrConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC | LV_MX, gillHandLfingerstricount, 0, 
					NULL, NULL, gillHandLfingersvertposidx,
					NULL, NULL, gillHandLfingersvertnoridx,
					NULL, NULL, NULL,
					gillHandLfingersmtxidx,
					gillHandLfingersvertcol);

			GX_SetArray(GX_VA_POS, &gillHandRvertpos[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_NRM, &gillHandRvertnor[0], sizeof(f32)*3);

			liy_batchLoadMtx(gillSkeletonMv, gillHandRMtxidxTable, LM_0 | LM_1 | LM_2 | LM_3);
			liy_VtxDescConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC | LV_MX, 0); 
			drawArrConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC | LV_MX, gillHandRtricount, 0, 
					NULL, NULL, gillHandRvertposidx,
					NULL, NULL, gillHandRvertnoridx,
					NULL, NULL, NULL,
					gillHandRmtxidx,
					gillHandRvertcol);

			GX_SetArray(GX_VA_POS, &gillHandRfingersvertpos[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_NRM, &gillHandRfingersvertnor[0], sizeof(f32)*3);

			liy_batchLoadMtx(gillSkeletonMv, gillHandRfingersMtxidxTable, LM_ALL);	
			liy_VtxDescConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC | LV_MX, 0); 
			drawArrConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC | LV_MX, gillHandRfingerstricount, 0, 
					NULL, NULL, gillHandRfingersvertposidx,
					NULL, NULL, gillHandRfingersvertnoridx,
					NULL, NULL, NULL,
					gillHandRfingersmtxidx,
					gillHandRfingersvertcol);

			liy_batchLoadMtx(gillSkeletonMv, gillPresocksMtxidxTable, LM_0 | LM_1);	
			GX_SetArray(GX_VA_POS, &gillPresocksvertpos[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_NRM, &gillPresocksvertnor[0], sizeof(f32)*3);

			liy_VtxDescConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC | LV_MX, 0); 
			drawArrConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC | LV_MX, gillPresockstricount, 0, 
					NULL, NULL, gillPresocksvertposidx,
					NULL, NULL, gillPresocksvertnoridx,
					NULL, NULL, NULL,
					gillPresocksmtxidx,
					gillPresocksvertcol);

			liy_batchLoadMtx(gillSkeletonMv, gillSocksMtxidxTable, LM_0 | LM_1 | LM_2 | LM_3 | LM_4 | LM_5);
			GX_SetArray(GX_VA_POS, &gillSocksvertpos[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_NRM, &gillSocksvertnor[0], sizeof(f32)*3);

			liy_VtxDescConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC | LV_MX, 0); 
			drawArrConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC | LV_MX, gillSockstricount, 0, 
					NULL, NULL, gillSocksvertposidx,
					NULL, NULL, gillSocksvertnoridx,
					NULL, NULL, NULL,
					gillSocksmtxidx,
					gillSocksvertcol);

			LX_LoadMtxImm(gillSkeletonMv[4], GX_PNMTX0);

			GX_SetArray(GX_VA_POS, &gillHeadvertpos[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_NRM, &gillHeadvertnor[0], sizeof(f32)*3);

			liy_VtxDescConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC, 0); 
			drawArrConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC, gillHeadtricount, 0, 
					NULL, NULL, gillHeadvertposidx,
					NULL, NULL, gillHeadvertnoridx,
					NULL, NULL, NULL,
					NULL,
					gillHeadvertcol);

			LX_LoadMtxImm(gillSkeletonMv[54], GX_PNMTX0);
			LX_LoadMtxImm(gillSkeletonMv[53], GX_PNMTX1);

			GX_SetArray(GX_VA_POS, &gillEarvertpos[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_NRM, &gillEarvertnor[0], sizeof(f32)*3);

			liy_VtxDescConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC | LV_MX, 0); 
			drawArrConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC | LV_MX, gillEartricount, 0, 
					NULL, NULL, gillEarvertposidx,
					NULL, NULL, gillEarvertnoridx,
					NULL, NULL, NULL,
					gillEarmtxidx,
					gillEarvertcol);
			
			LX_LoadMtxImm(gillSkeletonMv[50], GX_PNMTX0);
			GX_SetArray(GX_VA_POS, &gillEyesvertpos[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_NRM, &gillEyesvertnor[0], sizeof(f32)*3);
			liy_VtxDescConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC, 0);
			drawArrConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC, gillEyestricount, 0, 
					NULL, NULL, gillEyesvertposidx,
					NULL, NULL, gillEyesvertnoridx,
					NULL, NULL, NULL,
					NULL,
					gillEyesvertcol);

			liy_batchLoadMtx(gillSkeletonMv, gillBrowMtxidxTable, LM_0 | LM_1 | LM_2 | LM_3 | LM_4 | LM_5);
			GX_SetArray(GX_VA_POS, &gillBrowvertpos[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_NRM, &gillBrowvertnor[0], sizeof(f32)*3);
			liy_VtxDescConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC | LV_MX, 0);
			drawArrConfig(LV_VPIDX16 | LV_VNIDX16 | LV_VC | LV_MX, gillBrowtricount, 0, 
					NULL, NULL, gillBrowvertposidx,
					NULL, NULL, gillBrowvertnoridx,
					NULL, NULL, NULL,
					gillBrowmtxidx,
					gillBrowvertcol);

			//textured objects
			GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_TEXC, GX_CC_RASC, GX_CC_ZERO);
			GX_SetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0 | GX_LIGHT1, GX_DF_CLAMP, GX_AF_NONE);	
			// on CONSOLE ONLY, if second source is VTX, the draw call may take on the vertex color 
			// of the last vertex of a previous draw call, if the draw call contains no vertex color data

			GX_SetArray(GX_VA_POS, &gillHairvertpos[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_NRM, &gillHairvertnor[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_TEX0, &gillHairtexcord[0], sizeof(f32)*2);
			
			liy_VtxDescConfig(LV_VPIDX16 | LV_VNIDX16 | LV_TCIDX16, 0); 
			drawArrConfig(LV_VPIDX16 | LV_VNIDX16 | LV_TCIDX16, gillHairtricount, 0, 
					NULL, NULL, gillHairvertposidx,
					NULL, NULL, gillHairvertnoridx,
					NULL, NULL, gillHairtexcordidx,
					NULL,
					NULL);

			GX_SetArray(GX_VA_POS, &gillPantvertpos[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_NRM, &gillPantvertnor[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_TEX0, &gillPanttexcord[0], sizeof(f32)*2);

			liy_batchLoadMtx(gillSkeletonMv, gillPantsMtxidxTable, LM_0 | LM_1 | LM_2 | LM_3 | LM_4);
			liy_VtxDescConfig(LV_VPIDX16 | LV_VNIDX16 | LV_TCIDX16 | LV_MX, 0);
			drawArrConfig(LV_VPIDX16 | LV_VNIDX16 | LV_TCIDX16 | LV_MX, gillPanttricount, 0, 
					NULL, NULL, gillPantvertposidx,
					NULL, NULL, gillPantvertnoridx,
					NULL, NULL, gillPanttexcordidx,
					gillPantmtxidx,
					NULL);
	
			LX_LoadMtxImm(gillSkeletonMv[52], GX_PNMTX0);
			LX_LoadMtxImm(gillSkeletonMv[51], GX_PNMTX1);
			LX_LoadMtxImm(gillSkeletonMv[56], GX_PNMTX2);
			LX_LoadMtxImm(gillSkeletonMv[57], GX_PNMTX3);

			GX_SetArray(GX_VA_POS, &gillPupilsvertpos[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_NRM, &gillPupilsvertnor[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_TEX0, &gillPupilstexcord[0], sizeof(f32)*2);

			liy_VtxDescConfig(LV_VPIDX16 | LV_VNIDX16 | LV_TCIDX16 | LV_MX, 0);
			drawArrConfig(LV_VPIDX16 | LV_VNIDX16 | LV_TCIDX16 | LV_MX, gillPupilstricount, 0, 
					NULL, NULL, gillPupilsvertposidx,
					NULL, NULL, gillPupilsvertnoridx,
					NULL, NULL, gillPupilstexcordidx,
					gillPupilsmtxidx,
					NULL);

			//the shirt is special. it has per vert colors and a texture on the front. 

			GX_SetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT0 | GX_LIGHT1, GX_DF_CLAMP, GX_AF_NONE);	
			liy_VtxDescConfig(LV_VPIDX16 | LV_VNIDX16 | LV_TCIDX16 | LV_VC | LV_MX, 0);
			liy_batchLoadMtx(gillSkeletonMv, gillShirtMtxidxTable, LM_ALL);
			GX_SetArray(GX_VA_POS, &gillShirtvertpos[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_NRM, &gillShirtvertnor[0], sizeof(f32)*3);
			GX_SetArray(GX_VA_TEX0, &gillShirttexcord[0], sizeof(f32)*2);
			drawArrConfig(LV_VPIDX16 | LV_VNIDX16 | LV_TCIDX16 | LV_VC | LV_MX, gillShirttricount, 0, 
					NULL, NULL, gillShirtvertposidx,
					NULL, NULL, gillShirtvertnoridx,
					NULL, NULL, gillShirttexcordidx,
					gillShirtmtxidx,
					gillShirtvertcol);
}
#endif
