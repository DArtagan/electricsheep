#ifndef _MGL__H_
#define _MGL__H_

#ifdef	MAC

#ifdef _DisplayGL_H_
#error "DisplayGL.h included before mgl.h!"
#endif

#include "glee.h"
#include "../DisplayOutput.h"
#undef Random
#include <ApplicationServices/ApplicationServices.h>
#include <OpenGL/OpenGL.h>


namespace DisplayOutput
{
	
extern "C" {
	void SetupPool();
}
	
/*
	CMacGL.
	Macintosh OpenGL output.
*/
class CMacGL : public CDisplayOutput
{
	CGLContextObj	m_glContext;


	static uint32 s_DefaultWidth;
	static uint32 s_DefaultHeight;

	public:
			CMacGL();
			virtual ~CMacGL();

			//static LRESULT CALLBACK	wndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

			static const char *Description()	{	return "Macintosh OpenGL display";	};
				
			virtual bool	Initialize( CGLContextObj _glContext, bool _bPreview );
			
			virtual void SetContext( CGLContextObj glContext ) { m_glContext = glContext; }
			virtual CGLContextObj GetContext( void ) { return m_glContext; }

			virtual void ForceWidthAndHeight( uint32 _width, uint32 _height );
			
			static void SetDefaultWidthAndHeight( uint32 defWidth, uint32 defHeight );
	
			virtual bool HasShaders() { return true; };

			//
			virtual void	Title( const std::string &_title );
			virtual void	Update();

			void    SwapBuffers();
};

typedef	CMacGL	CDisplayGL;

};

#endif

#endif
