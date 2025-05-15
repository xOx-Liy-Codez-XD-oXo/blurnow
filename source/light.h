#ifndef LIGHT_H
#define LIGHT_H

void setlight(Mtx view, GXColor ambcol, int scene)
{
	guVector lpos;
	GXLightObj lobj;	

	GX_SetNumChans(1);

	switch(scene) {
	case 0:
	static GXColor grascol = {2, 55, 52, 255};

	//Sun
	lpos.x = 500.0f;
	lpos.y = -100.0f;
	lpos.z = 500.0f;

	guVecMultiply(view,&lpos,&lpos);

	GX_InitLightPos(&lobj,lpos.x,lpos.y,lpos.z);
	GX_InitLightColor(&lobj, LC_WHITE);
   	GX_InitLightDistAttn(&lobj, 10.0f, 10.0f, GX_DA_OFF); //sun lamp is a dist attn light with the dist attn off coz i dont wanna fw matricies 2day
	GX_LoadLightObj(&lobj,GX_LIGHT0);

	//Grass lightbounce
	lpos.x = 0.0f;
	lpos.y = 0.0f;
	lpos.z = -500.0f;

	guVecMultiply(view,&lpos,&lpos);

	GX_InitLightPos(&lobj,lpos.x,lpos.y,lpos.z);
	GX_InitLightColor(&lobj,grascol);
   	GX_InitLightDistAttn(&lobj, 1.0f, 1.0f, GX_DA_OFF);
	GX_LoadLightObj(&lobj,GX_LIGHT1);
	
	GX_SetChanAmbColor(GX_COLOR0A0,ambcol); //ambcol
	GX_SetChanMatColor(GX_COLOR0A0,LC_WHITE);
	GX_SetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT0 | GX_LIGHT1, GX_DF_CLAMP, GX_AF_NONE);
	break;

	case 1:
	lpos.x = 0.0f;
	lpos.y = -500.0f;
	lpos.z = 0.0f;

	guVecMultiply(view, &lpos, &lpos);
	
	GX_InitLightPos(&lobj, lpos.x, lpos.y, lpos.z);
	GX_InitLightColor(&lobj, LC_DARKGREY);
	GX_InitLightDistAttn(&lobj, 10.0f, 10.0f, GX_DA_OFF);
	GX_LoadLightObj(&lobj, GX_LIGHT0);

	lpos.x = 0.0f;
	lpos.y = 0.0f;
	lpos.z = 500.0f;

	guVecMultiply(view, &lpos, &lpos);
	
	GX_InitLightPos(&lobj, lpos.x, lpos.y, lpos.z);
	GX_InitLightColor(&lobj, LC_GREY);
	GX_InitLightDistAttn(&lobj, 10.0f, 10.0f, GX_DA_OFF);
	GX_LoadLightObj(&lobj, GX_LIGHT1);

	GX_SetChanAmbColor(GX_COLOR0A0, LC_DARKGREY); //ambcol
	GX_SetChanMatColor(GX_COLOR0A0, LC_WHITE);
	GX_SetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT0 | GX_LIGHT1, GX_DF_CLAMP, GX_AF_NONE);
	
	break;
	}
}

void nolight() {
	GX_SetChanAmbColor(GX_COLOR0A0,LC_WHITE); //ambcol
	GX_SetChanMatColor(GX_COLOR0A0,LC_WHITE);
	GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHTNULL, GX_DF_NONE, GX_AF_NONE);
}

#endif