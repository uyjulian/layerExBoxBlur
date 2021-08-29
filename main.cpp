/////////////////////////////////////////////
//                                         //
//    Copyright (C) 2021-2021 Julian Uy    //
//  https://sites.google.com/site/awertyb  //
//                                         //
//   See details of license at "LICENSE"   //
//                                         //
/////////////////////////////////////////////

#include "ncbind/ncbind.hpp"
#include "LayerBitmapIntf.h"
#include "LayerBitmapUtility.h"
#include "LayerBitmapDoBoxBlur.h"
#include "DivTable.h"
#include <string.h>
#include <stdio.h>

static void PreRegistCallback()
{
	InitDivTable();
	iTJSDispatch2 *global = TVPGetScriptDispatch();
	if (global)
	{
		tTJSVariant layer_val;
		static ttstr Layer_name(TJS_W("Layer"));
		global->PropGet(0, Layer_name.c_str(), Layer_name.GetHint(), &layer_val, global);
		tTJSVariantClosure layer_valclosure = layer_val.AsObjectClosureNoAddRef();
		if (layer_valclosure.Object)
		{
			layer_valclosure.DeleteMember(TJS_IGNOREPROP, TJS_W("doBoxBlur"), 0, NULL);
		}
	}
}

NCB_PRE_REGIST_CALLBACK(PreRegistCallback);

class LayerLayerExBoxBlur
{
public:
	static tjs_error TJS_INTF_METHOD doBoxBlur(
		tTJSVariant	*result,
		tjs_int numparams,
		tTJSVariant **param,
		iTJSDispatch2 *objthis)
	{

		if(numparams == 0) return TJS_S_OK;

		tjs_int xblur = 1;
		tjs_int yblur = 1;

		if(numparams >= 1 && param[0]->Type() != tvtVoid)
			xblur = (tjs_int)*param[0];
		
		if(numparams >= 2 && param[1]->Type() != tvtVoid)
			yblur = (tjs_int)*param[1];
		
		if(objthis == NULL) TVPThrowExceptionMessage(TJS_W("Specify Layer or Bitmap class object"));
		tTJSVariant bmpobject = tTJSVariant(objthis, objthis);
		tTJSVariantClosure bmpobject_clo = bmpobject.AsObjectClosureNoAddRef();

		// Now get information (this will independ the bitmap)
		tjs_int bmpwidth = 0;
		tjs_int bmpheight = 0;
		tjs_int bmppitch = 0;
		tjs_uint8* bmpdata = NULL;
		GetBitmapInformationFromObject(bmpobject_clo, true, &bmpwidth, &bmpheight, &bmppitch, &bmpdata);
		if(!bmpdata) TVPThrowExceptionMessage(TJS_W("Specify Layer or Bitmap class object"));
		tTVPBaseBitmap bmpinfo(bmpwidth, bmpheight, bmppitch, bmpdata);

		tTVPDrawFace Face = dfAuto; // (outward) current drawing layer face
		tTVPLayerType DisplayType = ltOpaque; // actual Type
		tTVPRect ClipRect;
		tjs_int ImageLeft = 0;
		tjs_int ImageTop = 0;

		GetLayerInformationFromLayerObject(bmpobject_clo, &Face, &DisplayType, &ClipRect, NULL, &ImageLeft, &ImageTop, NULL);

		tTVPDrawFace DrawFace = GetDrawFace(Face, DisplayType); // (actual) current drawing layer face

		bool updated = false;

		if(DrawFace != dfAlpha)
			updated = DoBoxBlur(&bmpinfo, ClipRect, tTVPRect(-xblur, -yblur, xblur, yblur));
		else
			updated = DoBoxBlurForAlpha(&bmpinfo, ClipRect, tTVPRect(-xblur, -yblur, xblur, yblur));

		if (updated)
		{
			UpdateLayerWithLayerObject(bmpobject_clo, &ClipRect, &ImageLeft, &ImageTop);
		}
		return TJS_S_OK;
	}
};

NCB_ATTACH_CLASS(LayerLayerExBoxBlur, Layer)
{
	RawCallback("doBoxBlur", &Class::doBoxBlur, 0);
};
