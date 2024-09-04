#include "ext.hpp"

void Ext::init()
{
	initialized=true;
	
	glActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
	glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");

	char *ext = (char*)glGetString( GL_EXTENSIONS );

	if( strstr( ext, "EXT_framebuffer_object" ) == NULL )
	{
		MessageBox(win.getHWND(),"EXT_framebuffer_object extension was not found. Upgrade your drivers or video card?","ERROR",
			MB_TOPMOST|MB_OK|MB_ICONEXCLAMATION);
		exit(-1);
	}
	else
	{
		glIsRenderbufferEXT = (PFNGLISRENDERBUFFEREXTPROC)wglGetProcAddress("glIsRenderbufferEXT");
		glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress("glBindRenderbufferEXT");
		glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
		glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress("glGenRenderbuffersEXT");
		glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress("glRenderbufferStorageEXT");
		glGetRenderbufferParameterivEXT = (PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC)wglGetProcAddress("glGetRenderbufferParameterivEXT");
		glIsFramebufferEXT = (PFNGLISFRAMEBUFFEREXTPROC)wglGetProcAddress("glIsFramebufferEXT");
		glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress("glBindFramebufferEXT");
		glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress("glDeleteFramebuffersEXT");
		glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");
		glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
		glFramebufferTexture1DEXT = (PFNGLFRAMEBUFFERTEXTURE1DEXTPROC)wglGetProcAddress("glFramebufferTexture1DEXT");
		glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
		glFramebufferTexture3DEXT = (PFNGLFRAMEBUFFERTEXTURE3DEXTPROC)wglGetProcAddress("glFramebufferTexture3DEXT");
		glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
		glGetFramebufferAttachmentParameterivEXT = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC)wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
		glGenerateMipmapEXT = (PFNGLGENERATEMIPMAPEXTPROC)wglGetProcAddress("glGenerateMipmapEXT");

		if( !glIsRenderbufferEXT || !glBindRenderbufferEXT || !glDeleteRenderbuffersEXT || 
			!glGenRenderbuffersEXT || !glRenderbufferStorageEXT || !glGetRenderbufferParameterivEXT || 
			!glIsFramebufferEXT || !glBindFramebufferEXT || !glDeleteFramebuffersEXT || 
			!glGenFramebuffersEXT || !glCheckFramebufferStatusEXT || !glFramebufferTexture1DEXT || 
			!glFramebufferTexture2DEXT || !glFramebufferTexture3DEXT || !glFramebufferRenderbufferEXT||  
			!glGetFramebufferAttachmentParameterivEXT || !glGenerateMipmapEXT )
		{
			MessageBox(win.getHWND(),"One or more EXT_framebuffer_object functions were not found.",
				"ERROR",MB_TOPMOST|MB_OK|MB_ICONEXCLAMATION);
			exit(-1);
		}
    }


	//------------------------------------------
	// Shaders	
	char *str = (char *)glGetString(GL_EXTENSIONS);

	//add every OpenGL extension the program will need
	const char* neededExtensions[64] = {
		"GL_ARB_fragment_shader",
		"GL_ARB_fragment_program",
		"GL_ARB_shading_language_100",
		"GL_ARB_vertex_shader",
		"GL_ARB_shader_objects",
	};

	// Uncomment this for extension listing
	// MessageBox(win.getHWND(), str, "Error!", MB_OK);

	for(int i = 0; neededExtensions[i] != NULL; i++)
	{
		if (strstr(str, (char *)neededExtensions[i])==NULL)
		{
			MessageBox(win.getHWND(), "No required OpenGL extensions available. \n [GL_ARB_*] - glsl-shaders.", "Error!",
				MB_TOPMOST | MB_ICONERROR | MB_OK);
			//return false;
			exit(0);
		}
	}

	glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC) wglGetProcAddress("glCreateProgramObjectARB");
	glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC) wglGetProcAddress("glDeleteObjectARB");
	glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC) wglGetProcAddress("glCreateShaderObjectARB");
	glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC) wglGetProcAddress("glShaderSourceARB");
	glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC) wglGetProcAddress("glCompileShaderARB");
	glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC) wglGetProcAddress("glGetObjectParameterivARB");
	glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC) wglGetProcAddress("glAttachObjectARB");
	glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC) wglGetProcAddress("glGetInfoLogARB");
	glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC) wglGetProcAddress("glLinkProgramARB");
	glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC) wglGetProcAddress("glUseProgramObjectARB");
	glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC) wglGetProcAddress("glGetUniformLocationARB");
	glUniform4fARB = (PFNGLUNIFORM4FARBPROC)wglGetProcAddress("glUniform4fARB");
	glUniform3fARB = (PFNGLUNIFORM3FARBPROC)wglGetProcAddress("glUniform3fARB");
	glUniform1fARB = (PFNGLUNIFORM1FARBPROC)wglGetProcAddress("glUniform1fARB");
	glUniform1iARB = (PFNGLUNIFORM1IARBPROC)wglGetProcAddress("glUniform1iARB");
	glDeleteProgram = (PFNGLDELETEPROGRAMPROC) wglGetProcAddress("glDeleteProgram");
	glDeleteShader = (PFNGLDELETESHADERPROC) wglGetProcAddress("glDeleteShader");
	glIsProgram = (PFNGLISPROGRAMPROC) wglGetProcAddress("glIsProgram");
	glIsShader = (PFNGLISSHADERPROC) wglGetProcAddress("glIsShader");
	glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC) wglGetProcAddress("glGetProgramInfoLog");
	glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC) wglGetProcAddress("glGetShaderInfoLog");

	glGetUniformfvARB =(PFNGLGETUNIFORMFVPROC) wglGetProcAddress("glGetUniformfvARB");
	glGetUniformivARB =(PFNGLGETUNIFORMIVPROC) wglGetProcAddress("glGetUniformivARB");
	glUniform4fARB =(PFNGLUNIFORM4FARBPROC) wglGetProcAddress("glUniform4fARB");
	glUniform3fARB =(PFNGLUNIFORM3FARBPROC) wglGetProcAddress("glUniform3fARB         ");
	glUniform2fARB =(PFNGLUNIFORM2FPROC) wglGetProcAddress("glUniform2fARB");
	glUniform1fARB =(PFNGLUNIFORM1FARBPROC) wglGetProcAddress("glUniform1fARB");
	glUniform1iARB =(PFNGLUNIFORM1IARBPROC) wglGetProcAddress("glUniform1iARB");
	glUniform1i =(PFNGLUNIFORM1IPROC) wglGetProcAddress("glUniform1i");
	glUniform2i =(PFNGLUNIFORM2IPROC) wglGetProcAddress("glUniform2i");
	glUniform3i =(PFNGLUNIFORM3IPROC) wglGetProcAddress("glUniform3i");
	glUniform4i =(PFNGLUNIFORM4IPROC) wglGetProcAddress("glUniform4i");
	glUniform1fv =(PFNGLUNIFORM1FVPROC) wglGetProcAddress("glUniform1fv");
	glUniform2fv =(PFNGLUNIFORM2FVPROC) wglGetProcAddress("glUniform2fv");
	glUniform3fv =(PFNGLUNIFORM3FVPROC) wglGetProcAddress("glUniform3fv");
	glUniform4fv =(PFNGLUNIFORM4FVPROC) wglGetProcAddress("glUniform4fv");
	glUniform1iv =(PFNGLUNIFORM1IVPROC) wglGetProcAddress("glUniform1iv");
	glUniform2iv =(PFNGLUNIFORM2IVPROC) wglGetProcAddress("glUniform2iv");
	glUniform3iv =(PFNGLUNIFORM3IVPROC) wglGetProcAddress("glUniform3iv");
	glUniform4iv =(PFNGLUNIFORM4IVPROC) wglGetProcAddress("glUniform4iv");
	glUniformMatrix2fv =(PFNGLUNIFORMMATRIX2FVPROC) wglGetProcAddress("glUniformMatrix2fv");
	glUniformMatrix3fv =(PFNGLUNIFORMMATRIX3FVPROC) wglGetProcAddress("glUniformMatrix3fv");
	glUniformMatrix4fv =(PFNGLUNIFORMMATRIX4FVPROC) wglGetProcAddress("glUniformMatrix4fv");
	glValidateProgram =(PFNGLVALIDATEPROGRAMPROC) wglGetProcAddress("glValidateProgram");
	glVertexAttrib1d =(PFNGLVERTEXATTRIB1DPROC) wglGetProcAddress("glVertexAttrib1d");
	glVertexAttrib1dv =(PFNGLVERTEXATTRIB1DVPROC) wglGetProcAddress("glVertexAttrib1dv");
	glVertexAttrib1f =(PFNGLVERTEXATTRIB1FPROC) wglGetProcAddress("glVertexAttrib1f");
	glVertexAttrib1fv =(PFNGLVERTEXATTRIB1FVPROC) wglGetProcAddress("glVertexAttrib1fv");
	glVertexAttrib1s =(PFNGLVERTEXATTRIB1SPROC) wglGetProcAddress("glVertexAttrib1s");
	glVertexAttrib1sv =(PFNGLVERTEXATTRIB1SVPROC) wglGetProcAddress("glVertexAttrib1sv");
	glVertexAttrib2d =(PFNGLVERTEXATTRIB2DPROC) wglGetProcAddress("glVertexAttrib2d");
	glVertexAttrib2dv =(PFNGLVERTEXATTRIB2DVPROC) wglGetProcAddress("glVertexAttrib2dv");
	glVertexAttrib2f =(PFNGLVERTEXATTRIB2FPROC) wglGetProcAddress("glVertexAttrib2f");
	glVertexAttrib2fv =(PFNGLVERTEXATTRIB2FVPROC) wglGetProcAddress("glVertexAttrib2fv");
	glVertexAttrib2s =(PFNGLVERTEXATTRIB2SPROC) wglGetProcAddress("glVertexAttrib2s");
	glVertexAttrib2sv =(PFNGLVERTEXATTRIB2SVPROC) wglGetProcAddress("glVertexAttrib2sv");
	glVertexAttrib3d =(PFNGLVERTEXATTRIB3DPROC) wglGetProcAddress("glVertexAttrib3d");
	glVertexAttrib3dv =(PFNGLVERTEXATTRIB3DVPROC) wglGetProcAddress("glVertexAttrib3dv");
	glVertexAttrib3f =(PFNGLVERTEXATTRIB3FPROC) wglGetProcAddress("glVertexAttrib3f");
	glVertexAttrib3fv =(PFNGLVERTEXATTRIB3FVPROC) wglGetProcAddress("glVertexAttrib3fv");
	glVertexAttrib3s =(PFNGLVERTEXATTRIB3SPROC) wglGetProcAddress("glVertexAttrib3s");
	glVertexAttrib3sv =(PFNGLVERTEXATTRIB3SVPROC) wglGetProcAddress("glVertexAttrib3sv");
	glVertexAttrib4Nbv =(PFNGLVERTEXATTRIB4NBVPROC) wglGetProcAddress("glVertexAttrib4Nbv");
	glVertexAttrib4Niv =(PFNGLVERTEXATTRIB4NIVPROC) wglGetProcAddress("glVertexAttrib4Niv");
	glVertexAttrib4Nsv =(PFNGLVERTEXATTRIB4NSVPROC) wglGetProcAddress("glVertexAttrib4Nsv");
	glVertexAttrib4Nub =(PFNGLVERTEXATTRIB4NUBPROC) wglGetProcAddress("glVertexAttrib4Nub");
	glVertexAttrib4Nubv =(PFNGLVERTEXATTRIB4NUBVPROC) wglGetProcAddress("glVertexAttrib4Nubv");
	glVertexAttrib4Nuiv =(PFNGLVERTEXATTRIB4NUIVPROC) wglGetProcAddress("glVertexAttrib4Nuiv");
	glVertexAttrib4Nusv =(PFNGLVERTEXATTRIB4NUSVPROC) wglGetProcAddress("glVertexAttrib4Nusv");
	glVertexAttrib4bv =(PFNGLVERTEXATTRIB4BVPROC) wglGetProcAddress("glVertexAttrib4bv");
	glVertexAttrib4d =(PFNGLVERTEXATTRIB4DPROC) wglGetProcAddress("glVertexAttrib4d");
	glVertexAttrib4dv =(PFNGLVERTEXATTRIB4DVPROC) wglGetProcAddress("glVertexAttrib4dv");
	glVertexAttrib4f =(PFNGLVERTEXATTRIB4FPROC) wglGetProcAddress("glVertexAttrib4f");
	glVertexAttrib4fv =(PFNGLVERTEXATTRIB4FVPROC) wglGetProcAddress("glVertexAttrib4fv");
	glVertexAttrib4iv =(PFNGLVERTEXATTRIB4IVPROC) wglGetProcAddress("glVertexAttrib4iv");
	glVertexAttrib4s =(PFNGLVERTEXATTRIB4SPROC) wglGetProcAddress("glVertexAttrib4s");
	glVertexAttrib4sv =(PFNGLVERTEXATTRIB4SVPROC) wglGetProcAddress("glVertexAttrib4sv");
	glVertexAttrib4ubv =(PFNGLVERTEXATTRIB4UBVPROC) wglGetProcAddress("glVertexAttrib4ubv");
	glVertexAttrib4uiv =(PFNGLVERTEXATTRIB4UIVPROC) wglGetProcAddress("glVertexAttrib4uiv");
	glVertexAttrib4us =(PFNGLVERTEXATTRIB4USVPROC) wglGetProcAddress("glVertexAttrib4us");
	glVertexAttribPointer =(PFNGLVERTEXATTRIBPOINTERPROC) wglGetProcAddress("glVertexAttribPointer");


	// Käytetään ruudun taustana aina 800x600 tekstuuria yksinkertaisuuden vuoksi
    screenWidth = 800;
    screenHeight = 600;

	smallW = 400;
	smallH = 300;
	
	glGenFramebuffersEXT(1, &this->frameBufferTexture1024);
	glGenFramebuffersEXT(1, &this->frameBufferTexture512);
	glGenFramebuffersEXT(1, &this->frameBufferTexture256);
	glGenFramebuffersEXT(1, &this->frameBufferTexture128);

	glGenRenderbuffersEXT(1, &this->depthRenderBufferTexture1024);
	glGenRenderbuffersEXT(1, &this->depthRenderBufferTexture512);
	glGenRenderbuffersEXT(1, &this->depthRenderBufferTexture256);
	glGenRenderbuffersEXT(1, &this->depthRenderBufferTexture128);

	// Initialize the render-buffer for usage as a depth buffer.
	// We don't really need this to render things into the frame-buffer object,
	// but without it the geometry will not be sorted properly.
	glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, depthRenderBufferTexture1024);
	glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 1024, 1024);
	
	glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, depthRenderBufferTexture512);
	glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 512, 512);
	
	glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, depthRenderBufferTexture256);
	glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 256, 256);

	glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, depthRenderBufferTexture128);
	glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 128, 128);


	GLenum status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );

	switch( status )
	{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			//MessageBox(win.getHWND(),"GL_FRAMEBUFFER_COMPLETE_EXT!","SUCCESS",MB_TOPMOST| MB_OK|MB_ICONEXCLAMATION);
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			MessageBox(win.getHWND(),"GL_FRAMEBUFFER_UNSUPPORTED_EXT!","ERROR",MB_TOPMOST| MB_OK|MB_ICONEXCLAMATION);
			exit(0);
			break;
		default:
			exit(0);
	}


	this->createTexture(this->glowTexture1024ID, 1024,1024);
	this->createTexture(this->glowTexture1024ID2, 1024,1024);
	this->createTexture(this->glowTexture512ID, 512,512);
	this->createTexture(this->glowTexture512ID2, 512,512);
	this->createTexture(this->glowTexture256ID, 256,256);
	this->createTexture(this->glowTexture128ID, 128,128);
	this->createTexture(this->glowTexture128ID2, 128,128);

	this->createTexture(this->motionBlur1024, 1024,1024);

//	createTextureRECT(screenTextureID, screenWidth, screenHeight);
//	createTextureRECT(smallScreenTextureID,  smallW, smallH);
//	createTextureRECT(smallScreenTextureID2, smallW, smallH);

	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );

}

void Ext::deinit() {

	if(initialized==true) {

		// Free textures
		glDeleteTextures (1, &glowTexture1024ID);
		glDeleteTextures (1, &glowTexture1024ID2);
		glDeleteTextures (1, &glowTexture512ID);
		glDeleteTextures (1, &glowTexture512ID2);
		glDeleteTextures (1, &glowTexture256ID);
		glDeleteTextures (1, &glowTexture128ID);
		glDeleteTextures (1, &glowTexture128ID2);
		
		glDeleteTextures (1, &motionBlur1024);


		// Free framebuffers
		glDeleteFramebuffersEXT(1, &frameBufferTexture1024);
		glDeleteFramebuffersEXT(1, &frameBufferTexture512);
		glDeleteFramebuffersEXT(1, &frameBufferTexture256);
		glDeleteFramebuffersEXT(1, &frameBufferTexture128);
		// Free renderbuffers
		glDeleteRenderbuffersEXT(1, &depthRenderBufferTexture1024);
		glDeleteRenderbuffersEXT(1, &depthRenderBufferTexture512);
		glDeleteRenderbuffersEXT(1, &depthRenderBufferTexture256);
		glDeleteRenderbuffersEXT(1, &depthRenderBufferTexture128);
		
	}

}

void Ext::createTexture(GLuint &i, int w, int h) {

	GLuint texID;
	glGenTextures (1, &texID);
	glBindTexture(GL_TEXTURE_2D,texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGB, GL_FLOAT, 0);
	i = texID;
}

void Ext::createTextureRECT(GLuint &i, int w, int h) {

	GLuint texID;
	glGenTextures (1, &texID);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB,texID);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_RECTANGLE_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, 4, w, h, 0, GL_RGB, GL_FLOAT, 0);
	i = texID;
}

void Ext::bindTextureFBO1024(GLuint textureID)
{
    //"ruutu" osoittamaan framebufferobjektiin
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, glExt.frameBufferTexture1024);

    //glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, g_depthRenderBuffer ); //oli esimerkkisorsassa kommentoitu(?)
    
    //siihen kiinni tekstuuri (rendaa jotain -> menee tekstuuriin) 
	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, 
                                textureID, 0 );
    //renderbufferi kiinni (tarvitaan, se "ruutu" johon extensio rendaa ja joka sitten on kiinni 
    //framebufferissa)
	glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, 
                                GL_RENDERBUFFER_EXT, glExt.depthRenderBufferTexture1024);

	logFBOErrors("bindTextureFBO");

	glViewport( 0, 0, 1024, 1024);
}

void Ext::bindTextureFBO512(GLuint textureID)
{
    //"ruutu" osoittamaan framebufferobjektiin
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, glExt.frameBufferTexture512);

    //glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, g_depthRenderBuffer ); //oli esimerkkisorsassa kommentoitu(?)
    
    //siihen kiinni tekstuuri (rendaa jotain -> menee tekstuuriin) 
	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, 
                                textureID, 0 );
    //renderbufferi kiinni (tarvitaan, se "ruutu" johon extensio rendaa ja joka sitten on kiinni 
    //framebufferissa)
	glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, 
                                GL_RENDERBUFFER_EXT, glExt.depthRenderBufferTexture512);

	logFBOErrors("bindTextureFBO");

	glViewport( 0, 0, 512, 512);
}

void Ext::bindTextureFBO256(GLuint textureID)
{
    //"ruutu" osoittamaan framebufferobjektiin
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, glExt.frameBufferTexture256);

    //glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, g_depthRenderBuffer ); //oli esimerkkisorsassa kommentoitu(?)
    
    //siihen kiinni tekstuuri (rendaa jotain -> menee tekstuuriin) 
	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, 
                                textureID, 0 );
    //renderbufferi kiinni (tarvitaan, se "ruutu" johon extensio rendaa ja joka sitten on kiinni 
    //framebufferissa)
	glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, 
                                GL_RENDERBUFFER_EXT, glExt.depthRenderBufferTexture256);

	logFBOErrors("bindTextureFBO");

	glViewport( 0, 0, 256, 256);
}

void Ext::bindTextureFBO128(GLuint textureID)
{
    //"ruutu" osoittamaan framebufferobjektiin
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, glExt.frameBufferTexture128);

    //glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, g_depthRenderBuffer ); //oli esimerkkisorsassa kommentoitu(?)
    
    //siihen kiinni tekstuuri (rendaa jotain -> menee tekstuuriin) 
	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, 
                                textureID, 0 );
    //renderbufferi kiinni (tarvitaan, se "ruutu" johon extensio rendaa ja joka sitten on kiinni 
    //framebufferissa)
	glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, 
                                GL_RENDERBUFFER_EXT, glExt.depthRenderBufferTexture128);

	logFBOErrors("bindTextureFBO");

	glViewport( 0, 0, 128, 128);
}
/*
void Ext::bindScreenFBO(GLuint textureID)
{
    //"ruutu" osoittamaan framebufferobjektiin
//	glExt.glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, glExt.frameBufferScreen);

    //glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, g_depthRenderBuffer ); //oli esimerkkisorsassa kommentoitu(?)
    
    //siihen kiinni tekstuuri (rendaa jotain -> menee tekstuuriin) 
//	glExt.glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,  GL_TEXTURE_RECTANGLE_ARB, 
  //                              textureID, 0 );
    //renderbufferi kiinni (tarvitaan, se "ruutu" johon extensio rendaa ja joka sitten on kiinni 
    //framebufferissa)
	//glExt.glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, 
//                                GL_RENDERBUFFER_EXT, glExt.depthRenderBufferScreen );

//	logFBOErrors("bindScreenFBO");

//	glViewport( 0, 0, screenWidth, screenHeight);
}


/*void Ext::bindSmallScreenFBO(GLuint textureID)
{
    //"ruutu" osoittamaan framebufferobjektiin
	glExt.glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, glExt.frameBufferSmallScreen);

    //glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, g_depthRenderBuffer ); //oli esimerkkisorsassa kommentoitu(?)
    
    //siihen kiinni tekstuuri (rendaa jotain -> menee tekstuuriin) 
	glExt.glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,  GL_TEXTURE_RECTANGLE_ARB, 
                                textureID, 0 );
    //renderbufferi kiinni (tarvitaan, se "ruutu" johon extensio rendaa ja joka sitten on kiinni 
    //framebufferissa)
	glExt.glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, 
                                GL_RENDERBUFFER_EXT, glExt.depthRenderBufferSmallScreen );

	logFBOErrors("bindSmallScreenFBO");

	glViewport( 0, 0, smallW, smallH);

}
*/
void Ext::unbindFBO()
{
    //nolla lienee varsinainen OpenGL:n rendercontexti

	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
	//glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, 0 );

    //normaaliin ruutuun
	//glViewport(0, 0, screenWidth, screenHeight);
	//otetaan koko alkuperäisestä 
     glViewport(0, 0, OPENGL_XRES, OPENGL_YRES);

}

bool Ext::logFBOErrors() {
	return logFBOErrors("place undefined:");
}
bool Ext::logFBOErrors(char *kohta) {

#ifdef _NOT_FINAL_BUILD_

	int status = glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT );
	switch( status )
	{
		case GL_FRAMEBUFFER_COMPLETE_EXT:
			//MessageBox(win.getHWND(),"GL_FRAMEBUFFER_COMPLETE_EXT!","SUCCESS",MB_TOPMOST|MB_OK|MB_ICONEXCLAMATION);
			break;
        case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
            MessageBox(win.getHWND(),"%s: GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT - Framebuffer incomplete,incomplete attachment\n",MB_TOPMOST|MB_OK|MB_ICONEXCLAMATION);
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
            MessageBox(win.getHWND(),"%s: GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT - Framebuffer incomplete,missing attachment\n",MB_TOPMOST|MB_OK|MB_ICONEXCLAMATION);
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
            MessageBox(win.getHWND(),"%s: GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT - Framebuffer incomplete,attached images must have same dimensions\n",MB_TOPMOST|MB_OK|MB_ICONEXCLAMATION);
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
            MessageBox(win.getHWND(),"%s: GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT - Framebuffer incomplete,attached images must have same format\n",MB_TOPMOST|MB_OK|MB_ICONEXCLAMATION);
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
            MessageBox(win.getHWND(),"%s: GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT - Framebuffer incomplete,missing draw buffer\n",MB_TOPMOST|MB_OK|MB_ICONEXCLAMATION);
            return false;
        case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
            MessageBox(win.getHWND(),"%s: GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT - Framebuffer incomplete,missing read buffer\n",MB_TOPMOST|MB_OK|MB_ICONEXCLAMATION);
            return false;
		case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
			MessageBox(win.getHWND(),"GL_FRAMEBUFFER_UNSUPPORTED_EXT!","ERROR",MB_TOPMOST|MB_OK|MB_ICONEXCLAMATION);
			exit(0);
			break;

		default:
			exit(0);
	}

#endif

	return true;
}