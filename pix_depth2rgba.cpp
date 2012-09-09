////////////////////////////////////////////////////////
//
// GEM - Graphics Environment for Multimedia
//
// zmoelnig@iem.kug.ac.at
//
// Implementation file
//
//    Copyright (c) 1997-1998 Mark Danks.
//    Copyright (c) Günther Geiger.
//    Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
//    For information on usage and redistribution, and for a DISCLAIMER OF ALL
//    WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.
//
/////////////////////////////////////////////////////////


#include <math.h>

#include "pix_depth2rgba.h"


//CPPEXTERN_NEW(pix_depth2rgba);
CPPEXTERN_NEW_WITH_ONE_ARG(pix_depth2rgba, t_floatarg, A_DEFFLOAT);

#define XtoZ 1.111466646194458
#define YtoZ 0.833599984645844

// user colors
float Colors[][3] =
{
	{1,1,1},
	{0,1,1},
	{0,0,1},
	{0,1,0},
	{1,1,0},
	{1,0,0},
	{1,.5,0},
	{.5,1,0},
	{0,.5,1},
	{.5,0,1},
	{1,1,.5}
};
int nColors = 10;

/////////////////////////////////////////////////////////
//
// pix_depth2rgba
//
/////////////////////////////////////////////////////////
// Constructor
//
/////////////////////////////////////////////////////////
pix_depth2rgba :: pix_depth2rgba(t_floatarg distance)
{
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("mode"));
    // inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("lo_thresh"));
    inlet_new(this->x_obj, &this->x_obj->ob_pd, gensym("float"), gensym("hi_thresh"));
		if (distance > 0)
		{
			m_hi_thresh = distance;
		} else {
			m_hi_thresh = 8000.0;
		}
    m_lo_thresh = 10.1;
    m_active = true;
		m_mode = true;
		t_mult = 1530.0 / m_hi_thresh;
		
		//depth map representation curve
		int i;
	  for (i=0; i<10000; i++) {
	  	float v = i/2048.0;
	  	v = powf(v, 3)* 6;
	  	t_gamma[i] = v*6*256;
	  }
}

/////////////////////////////////////////////////////////
// Destructor
//
/////////////////////////////////////////////////////////
pix_depth2rgba :: ~pix_depth2rgba()
{ }

/////////////////////////////////////////////////////////
// processImage
//
/////////////////////////////////////////////////////////
void pix_depth2rgba :: processRGBAImage(imageStruct &image)
{
	if (m_active)
	{
    int datasize = image.xsize * image.ysize;

    unsigned char *base = image.data;
		uint16_t value = 0;
    while(datasize--)
    {
				value = ((int)base[chRed] << 8) + (int)base[chGreen];
				int pval;
				if (m_mode)
				{
					//pval = t_gamma[value/4];
					// t_mult = 1530.0 / m_hi_thresh; // do it when changed
					//pval = value / 6; // linear mapping
					pval = (int)((float)value * t_mult); // linear mapping
				} else {
					pval = t_gamma[value];
				}						
				int lb = pval & 0xff;

				base[chAlpha] = 255; // default alpha value

				if (value <=  10) 
				{
					base[chRed] = 0;
					base[chGreen] = 0;
					base[chBlue] = 0;
					base[chAlpha] = 0; // alpha null for anything without depth value
				}
				
				int nColorID = base[chBlue]; // get user id from blue channel
				
				if (nColorID > 0) // if user id present display color
				{
					base[chRed] = Colors[nColorID][0];
					base[chGreen] = Colors[nColorID][1];
					base[chBlue] = Colors[nColorID][2];
				} else {
					switch (pval>>8) {
						case 0:																					
						base[chRed] = 255;
						base[chGreen] = (255-lb);
						base[chBlue] = (255-lb);
						break;
						case 1:
						base[chRed] = 255;
						base[chGreen] = lb;
						base[chBlue] = 0;
						break;
						case 2:
						base[chRed] = (255-lb);
						base[chGreen] = 255;
						base[chBlue] = 0;
						break;
						case 3:
						base[chRed] = 0;
						base[chGreen] = 255;
						base[chBlue] = lb;
						break;
						case 4:
						base[chRed] = 0;
						base[chGreen] = (255-lb);
						base[chBlue] = 255;
						break;
						case 5:
						base[chRed] = 0;
						base[chGreen] = 0;
						base[chBlue] = (255-lb);
						break;
						default:
						base[chRed] = 0;
						base[chGreen] = 0;
						base[chBlue] = 0;
						base[chAlpha] = 0;
						break;
					}
			}
		base += 4;
    }
	}
}

/////////////////////////////////////////////////////////
// YUV Depth Image --> Has to be corrected
//
/////////////////////////////////////////////////////////
/*
void pix_depth2rgba :: processYUVImage(imageStruct &image)
{

}
*/
/////////////////////////////////////////////////////////
// floatHiThreshMess
//
/////////////////////////////////////////////////////////
void pix_depth2rgba :: floatHiThreshMess(float arg)
{
    if (arg > 0.0)
    {
			m_hi_thresh = arg;
			verbose (1, "high threshold set to %f", m_hi_thresh);
			t_mult = 1530.0 / m_hi_thresh; //recalc mult.
		}
}

/////////////////////////////////////////////////////////
// floatLoThreshMess
//
/////////////////////////////////////////////////////////
void pix_depth2rgba :: floatLoThreshMess(float arg)
{
    if (arg >= 0)
    {
			m_lo_thresh = arg;
			verbose (1, "low threshold set to %f", m_lo_thresh);
		}
}

/////////////////////////////////////////////////////////
// floatActiveMess
//
/////////////////////////////////////////////////////////
void pix_depth2rgba :: floatActiveMess(float arg)
{
	if (arg < 0.5)
	{
		m_active = false;
	} else {
		m_active = true;
	}
}
/////////////////////////////////////////////////////////
// floatModeMess
//
/////////////////////////////////////////////////////////
void pix_depth2rgba :: floatModeMess(float arg)
{
	if (arg < 0.5)
	{
		m_mode = false;
	} else {
		m_mode = true;
	}
}

/////////////////////////////////////////////////////////
// static member function
//
/////////////////////////////////////////////////////////
void pix_depth2rgba :: obj_setupCallback(t_class *classPtr)
{
	  class_addfloat(classPtr, reinterpret_cast<t_method>(&pix_depth2rgba::floatActiveMessCallback));
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_depth2rgba::floatLoThreshMessCallback),
    	    gensym("lo_thresh"), A_FLOAT, A_NULL);
    class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_depth2rgba::floatHiThreshMessCallback),
    	    gensym("hi_thresh"), A_FLOAT, A_NULL);
		class_addmethod(classPtr, reinterpret_cast<t_method>(&pix_depth2rgba::floatModeMessCallback),
		 	    gensym("mode"), A_FLOAT, A_NULL);
}
void pix_depth2rgba :: floatLoThreshMessCallback(void *data, t_floatarg arg)
{
    GetMyClass(data)->floatLoThreshMess((float)arg);
}
void pix_depth2rgba :: floatHiThreshMessCallback(void *data, t_floatarg arg)
{
    GetMyClass(data)->floatHiThreshMess((float)arg);
}
void pix_depth2rgba :: floatActiveMessCallback(void *data, t_floatarg arg)
{
    GetMyClass(data)->floatActiveMess((float)arg);
}
void pix_depth2rgba :: floatModeMessCallback(void *data, t_floatarg arg)
{
    GetMyClass(data)->floatModeMess((float)arg);
}