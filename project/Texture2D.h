#pragma once

#include "_common.h"
#include "Color.h"
#include "project/_platform.h"
#include <sstream>

using namespace pkpy;

struct Texture2D {
    PY_CLASS(Texture2D, pype, Texture2D)

    px_texture* ptr;
    Texture2D(px_texture* ptr) : ptr(ptr) {}

    static void _register(VM* vm, PyObject* mod, PyObject* type){
        vm->bind_static_method<1>(type, "__new__", [](VM* vm, ArgsView args){
            px_texture* ptr = CAST(px_texture*, args[0]);
            return VAR_T(Texture2D, ptr);
        });

        vm->bind_method<0>(type, "__repr__", [](VM* vm, ArgsView args){
            Texture2D& self = CAST(Texture2D&, args[0]);
            std::stringstream ss;
            ss << "Texture2D(" << self.ptr->width << ", " << self.ptr->height << ")";
            return VAR(ss.str());
        });

#define PROPERTY(name)                                              \
        type->attr().set(#name, vm->property(                       \
            [](VM* vm, ArgsView args){                              \
                Texture2D& self = CAST(Texture2D&, args[0]);        \
                return VAR(self.ptr->name);                         \
            }));

        PROPERTY(width);
        PROPERTY(height);
        PROPERTY(limit_left);
        PROPERTY(limit_top);
        PROPERTY(limit_right);
        PROPERTY(limit_bottom);

#undef PROPERTY
        
    }
};


inline px_void _PX_TextureRenderEx(px_surface *psurface,px_texture *resTexture,px_int x,px_int y,PX_ALIGN refPoint,PX_TEXTURERENDER_BLEND *blend,px_float scale,px_float Angle, bool flip)
{
	px_int newWidth,newHeight;
	
	px_int i,j,resHeight,resWidth;

	px_float invCosAgl,invSinAgl;

	px_double ref_x,ref_y;
	px_int mapX,mapY;

	px_color clr;
	px_int bA,bR,bG,bB;

	//////////////////////////////////////////////////////////////////////////
	px_int left,right,top,bottom;

	
	//////////////////////////////////////////////////////////////////////////

	resHeight=resTexture->height;
	resWidth=resTexture->width;

	newHeight=PX_TRUNC(resHeight*scale);
	newWidth=PX_TRUNC(resWidth*scale);

	//////////////////////////////////////////////////////////////////////////
	switch (refPoint)
	{
	case PX_ALIGN_LEFTTOP:
		break;
	case PX_ALIGN_MIDTOP:
		x-=newWidth/2;
		break;
	case PX_ALIGN_RIGHTTOP:
		x-=newWidth;
		break;
	case PX_ALIGN_LEFTMID:
		y-=newHeight/2;
		break;
	case PX_ALIGN_CENTER:
		y-=newHeight/2;
		x-=newWidth/2;
		break;
	case PX_ALIGN_RIGHTMID:
		y-=newHeight/2;
		x-=newWidth;
		break;
	case PX_ALIGN_LEFTBOTTOM:
		y-=newHeight;
		break;
	case PX_ALIGN_MIDBOTTOM:
		y-=newHeight;
		x-=newWidth/2;
		break;
	case PX_ALIGN_RIGHTBOTTOM:
		y-=newHeight;
		x-=newWidth;
		break;
	}


	if (x<-newWidth)
	{
		return;
	}
	if (x>psurface->width-1)
	{
		return;
	}
	if (y<-newHeight)
	{
		return;
	}
	if (y>psurface->height-1)
	{
		return;
	}

	if (x<0)
	{
		left=-x;
	}
	else
	{
		left=0;
	}

	if (x+newWidth>psurface->width)
	{
		right=psurface->width-x-1;
	}
	else
	{
		right=newWidth-1;
	}

	if (y<0)
	{
		top=-y;
	}
	else
	{
		top=0;
	}

	if (y+newHeight>psurface->height)
	{
		bottom=psurface->height-y-1;
	}
	else
	{
		bottom=newHeight-1;
	}
	//////////////////////////////////////////////////////////////////////////


	invCosAgl=PX_cos_angle(-Angle);
	invSinAgl=PX_sin_angle(-Angle);

    px_int Ab=(px_int)(blend->alpha*1000);
    px_int Rb=(px_int)(blend->hdr_R*1000);
    px_int Gb=(px_int)(blend->hdr_G*1000);
    px_int Bb=(px_int)(blend->hdr_B*1000);

    for (j=top;j<=bottom;j++)
    {
        for (i=left;i<=right;i++)
        {
            ref_x = (i - newWidth / 2.0);
            ref_y = (j - newHeight / 2.0);

            mapX = (px_int)((ref_x * invCosAgl - ref_y * invSinAgl) / scale + resWidth / 2);
            mapY = (px_int)((ref_x * invSinAgl + ref_y * invCosAgl) / scale + resHeight / 2);

            if (mapX<0||mapX>=resWidth)
            {
                continue;
            }

            if (mapY<0||mapY>=resHeight)
            {
                continue;
            }

            if(flip){
                mapX = resWidth - mapX - 1;
            }

            clr=PX_SURFACECOLOR(resTexture,mapX,mapY);
            bA=(px_int)(clr._argb.a*Ab/1000);
            bR=(px_int)(clr._argb.r*Rb/1000);
            bG=(px_int)(clr._argb.g*Gb/1000);
            bB=(px_int)(clr._argb.b*Bb/1000);

            clr._argb.a=bA>255?255:(px_uchar)bA;
            clr._argb.r=bR>255?255:(px_uchar)bR;
            clr._argb.g=bG>255?255:(px_uchar)bG;
            clr._argb.b=bB>255?255:(px_uchar)bB;
            PX_SurfaceDrawPixel(psurface,x+i,y+j,clr);
        }

    }
}

inline px_bool _PX_TextureCreateFromMemory(px_memorypool *mp,px_void *data,px_int size,px_texture *tex)
{
	px_int width;
	px_int height;
	if (PX_BitmapVerify(data,size))
	{
		width=PX_BitmapGetWidth(data);
		height=PX_BitmapGetHeight(data);
		if(PX_TextureCreate(mp,tex,width,height))
		{
			PX_BitmapRender(tex,data,size,0,0);
			return PX_TRUE;
		}
		else
		{
			PXError("PX_TextureCreate() 失败");
			return PX_FALSE;
		}

	}

	if (PX_TRawVerify(data,size))
	{
		width=PX_TRawGetWidth(data);
		height=PX_TRawGetHeight(data);
		if(PX_TextureCreate(mp,tex,width,height))
		{
			PX_TRawRender(tex,data,0,0);
			return PX_TRUE;
		}
		else
		{
			PXError("PX_TextureCreate() 失败");
			return PX_FALSE;
		}
	}

	if (PX_PngVerify((px_byte *)data,size,&width,&height,0))
	{
		if (PX_TextureCreate(mp, tex, width, height))
		{
			if (!PX_PngToRenderBuffer(mp, (px_byte*)data, size, tex)){
				PXError("PX_PngToRenderBuffer() 失败");
				return PX_FALSE;
			}
			return PX_TRUE;
		}
		else
		{
			PXError("PX_TextureCreate() 失败");
			return PX_FALSE;
		}
	}

	if (PX_JpgVerify((px_byte*)data, size))
	{
		PX_JpgDecoder decoder;
		if (PX_JpgDecoderInitialize(mp, &decoder, (px_byte*)data, size))
		{
			if (PX_TextureCreate(mp, tex, decoder.width, decoder.height))
			{
				PX_JpgDecoderRenderToSurface(&decoder, tex);
				PX_JpgDecoderFree(&decoder);
				return PX_TRUE;
			}
			PX_JpgDecoderFree(&decoder);
			PXError("PX_TextureCreate() 失败");
		}else{
			PXError("PX_JpgDecoderInitialize() 失败");
		}
	}
	PXError("不支持的图片格式");
	return PX_FALSE;
}