/*-----------------------------------------------------------------
LOG
GEM - Graphics Environment for Multimedia

Clamp pixel values to a threshold

Copyright (c) 1997-1998 Mark Danks. mark@danks.org
Copyright (c) Günther Geiger. geiger@epy.co.at
Copyright (c) 2001-2011 IOhannes m zmölnig. forum::für::umläute. IEM. zmoelnig@iem.at
Copyright (c) 2002 James Tittle & Chris Clepper
For information on usage and redistribution, and for a DISCLAIMER OF ALL
WARRANTIES, see the file, "GEM.LICENSE.TERMS" in this distribution.

-----------------------------------------------------------------*/

#ifndef _INCLUDE__GEM_PIXES_pix_depth2rgba_H_
#define _INCLUDE__GEM_PIXES_pix_depth2rgba_H_

#include "Base/GemPixObj.h"

#ifdef _WIN32
#include "stdint.h"
#endif

/*-----------------------------------------------------------------
-------------------------------------------------------------------
CLASS
    pix_depth2rgba
    
    Clamp pixel values to a threshold

KEYWORDS
    pix
    
DESCRIPTION

    Inlet for a list - "vec_thresh"
    Inlet for a float - "ft1"
    
    "vec_thresh" - The threshold vector
    "ft1" - Set all thresholds to one value
   
-----------------------------------------------------------------*/
#ifdef _WIN32
class GEM_EXPORT pix_depth2rgba : public GemPixObj
#else
class GEM_EXTERN pix_depth2rgba : public GemPixObj
#endif
{
    CPPEXTERN_HEADER(pix_depth2rgba, GemPixObj);

    public:

        //////////
        // Constructor
			pix_depth2rgba(t_floatarg hi_thresh, t_floatarg lo_thresh);
    	
    protected:
    	
    	//////////
    	// Destructor
    	virtual ~pix_depth2rgba();

    	//////////
    	// Do the processing
    	virtual void 	processRGBAImage(imageStruct &image);
    	// virtual void 	processYUVImage(imageStruct &image); // not done yet!
    	

    	//////////

    	//////////
    	// Set the new threshold value
    	void	    	floatHiThreshMess(float hi_thresh);
    	void	    	floatLoThreshMess(float lo_thresh);
			void	    	floatModeMess(float arg);
    	void	    	floatActiveMess(float arg);

			// distance thresholds - min < passing < max
			float m_hi_thresh;
			float m_lo_thresh;
			
			float t_mult;
			
			// if true convert into "human colorspace"
			bool m_active;

			// if true depth input is in mm, false if kinect raw 11bit data
			bool m_mode;
			
			// convertion curves
			uint16_t t_gamma[10000];
			
			
    private:
    
    	//////////
    	// Static member functions
    	static void 	floatHiThreshMessCallback(void *data, t_floatarg arg);
    	static void 	floatLoThreshMessCallback(void *data, t_floatarg arg);
    	static void 	floatModeMessCallback(void *data, t_floatarg arg);
    	static void 	floatActiveMessCallback(void *data, t_floatarg arg);
};

#endif	// for header file
