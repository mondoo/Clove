#include "clvpch.hpp"
#include "XLibHelpers.hpp"

#include <GL/glx.h>

namespace clv::XLibHelpers{
	XVisualInfo* getVisualInfo(Display* display, gfx::API api){
		//Note: api is used because we only support opengl on X

		static XVisualInfo* visual = nullptr;

		if(visual){
			return visual;
		}

		GLint glxAttribs[] = {
            GLX_RGBA,
            GLX_DEPTH_SIZE, 24,
            GLX_DOUBLEBUFFER,
            0
        };

        visual = glXChooseVisual(display, 0, glxAttribs);
        if(!visual){
            //TODO: Exception
            CLV_LOG_CRITICAL("Could not create visual");
        }

		return visual;
	}
}