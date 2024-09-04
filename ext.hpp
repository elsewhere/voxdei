#ifndef _EXT_HPP_
#define _EXT_HPP_

#include "stuff.hpp"

#include "windows.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glext.h"

class Ext;

extern Ext glExt;

class Ext
{
public:

	Ext() {
		initialized = false;
	}

	bool initialized;
	void init();
	void deinit();

	bool logFBOErrors();
	bool logFBOErrors(char *kohta);

	void createTexture(GLuint &i, int w, int h);
	void createTextureRECT(GLuint &i, int w, int h);

	PFNGLMULTITEXCOORD2FARBPROC 					glMultiTexCoord2fARB;
	PFNGLACTIVETEXTUREARBPROC 						glActiveTextureARB;
	PFNGLPOINTPARAMETERFARBPROC 					glPointParameterfARB;
	PFNGLPOINTPARAMETERFVARBPROC 					glPointParameterfvARB;
    PFNGLISRENDERBUFFEREXTPROC 						glIsRenderbufferEXT;
    PFNGLBINDRENDERBUFFEREXTPROC 					glBindRenderbufferEXT;
    PFNGLDELETERENDERBUFFERSEXTPROC 				glDeleteRenderbuffersEXT;
    PFNGLGENRENDERBUFFERSEXTPROC 					glGenRenderbuffersEXT;
    PFNGLRENDERBUFFERSTORAGEEXTPROC 				glRenderbufferStorageEXT;
    PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC 			glGetRenderbufferParameterivEXT;
    PFNGLISFRAMEBUFFEREXTPROC 						glIsFramebufferEXT;
    PFNGLBINDFRAMEBUFFEREXTPROC 					glBindFramebufferEXT;
    PFNGLDELETEFRAMEBUFFERSEXTPROC 					glDeleteFramebuffersEXT;
    PFNGLGENFRAMEBUFFERSEXTPROC 					glGenFramebuffersEXT;
    PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC				glCheckFramebufferStatusEXT;
    PFNGLFRAMEBUFFERTEXTURE1DEXTPROC				glFramebufferTexture1DEXT;
    PFNGLFRAMEBUFFERTEXTURE2DEXTPROC				glFramebufferTexture2DEXT;
    PFNGLFRAMEBUFFERTEXTURE3DEXTPROC				glFramebufferTexture3DEXT;
    PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC				glFramebufferRenderbufferEXT;
    PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT;
    PFNGLGENERATEMIPMAPEXTPROC						glGenerateMipmapEXT;

	PFNGLGETUNIFORMFVPROC				glGetUniformfvARB;
	PFNGLGETUNIFORMIVPROC				glGetUniformivARB;

	PFNGLCREATEPROGRAMOBJECTARBPROC		glCreateProgramObjectARB;
	PFNGLDELETEOBJECTARBPROC			glDeleteObjectARB;
	PFNGLCREATESHADEROBJECTARBPROC		glCreateShaderObjectARB;
	PFNGLSHADERSOURCEARBPROC			glShaderSourceARB;
	PFNGLCOMPILESHADERARBPROC			glCompileShaderARB;
	PFNGLGETOBJECTPARAMETERIVARBPROC	glGetObjectParameterivARB;
	PFNGLATTACHOBJECTARBPROC			glAttachObjectARB;
	PFNGLGETINFOLOGARBPROC				glGetInfoLogARB;
	PFNGLLINKPROGRAMARBPROC				glLinkProgramARB;
	PFNGLUSEPROGRAMOBJECTARBPROC		glUseProgramObjectARB;
	PFNGLGETUNIFORMLOCATIONARBPROC		glGetUniformLocationARB;
	PFNGLDELETEPROGRAMPROC				glDeleteProgram;
	PFNGLDELETESHADERPROC				glDeleteShader;
	PFNGLISPROGRAMPROC					glIsProgram;
	PFNGLISSHADERPROC					glIsShader;
	PFNGLGETPROGRAMINFOLOGPROC			glGetProgramInfoLog;
	PFNGLGETSHADERINFOLOGPROC			glGetShaderInfoLog;

	PFNGLUNIFORM4FARBPROC			glUniform4fARB;
	PFNGLUNIFORM3FARBPROC			glUniform3fARB;         
	PFNGLUNIFORM2FPROC				glUniform2fARB;
	PFNGLUNIFORM1FARBPROC			glUniform1fARB;
	PFNGLUNIFORM1IARBPROC			glUniform1iARB;

	PFNGLUNIFORM1IPROC				glUniform1i;
	PFNGLUNIFORM2IPROC				glUniform2i;
	PFNGLUNIFORM3IPROC				glUniform3i;
	PFNGLUNIFORM4IPROC				glUniform4i;


	PFNGLUNIFORM1FVPROC				glUniform1fv;
	PFNGLUNIFORM2FVPROC				glUniform2fv;
	PFNGLUNIFORM3FVPROC				glUniform3fv;
	PFNGLUNIFORM4FVPROC				glUniform4fv;
	PFNGLUNIFORM1IVPROC				glUniform1iv;
	PFNGLUNIFORM2IVPROC				glUniform2iv;
	PFNGLUNIFORM3IVPROC				glUniform3iv;
	PFNGLUNIFORM4IVPROC				glUniform4iv;
	PFNGLUNIFORMMATRIX2FVPROC		glUniformMatrix2fv;
	PFNGLUNIFORMMATRIX3FVPROC		glUniformMatrix3fv;
	PFNGLUNIFORMMATRIX4FVPROC		glUniformMatrix4fv;

	PFNGLVALIDATEPROGRAMPROC		glValidateProgram;
	PFNGLVERTEXATTRIB1DPROC			glVertexAttrib1d;
	PFNGLVERTEXATTRIB1DVPROC		glVertexAttrib1dv;
	PFNGLVERTEXATTRIB1FPROC			glVertexAttrib1f;
	PFNGLVERTEXATTRIB1FVPROC		glVertexAttrib1fv;
	PFNGLVERTEXATTRIB1SPROC			glVertexAttrib1s;
	PFNGLVERTEXATTRIB1SVPROC		glVertexAttrib1sv;
	PFNGLVERTEXATTRIB2DPROC			glVertexAttrib2d;
	PFNGLVERTEXATTRIB2DVPROC		glVertexAttrib2dv;
	PFNGLVERTEXATTRIB2FPROC			glVertexAttrib2f;
	PFNGLVERTEXATTRIB2FVPROC		glVertexAttrib2fv;
	PFNGLVERTEXATTRIB2SPROC			glVertexAttrib2s;
	PFNGLVERTEXATTRIB2SVPROC		glVertexAttrib2sv;
	PFNGLVERTEXATTRIB3DPROC			glVertexAttrib3d;
	PFNGLVERTEXATTRIB3DVPROC		glVertexAttrib3dv;
	PFNGLVERTEXATTRIB3FPROC			glVertexAttrib3f;
	PFNGLVERTEXATTRIB3FVPROC		glVertexAttrib3fv;
	PFNGLVERTEXATTRIB3SPROC			glVertexAttrib3s;
	PFNGLVERTEXATTRIB3SVPROC		glVertexAttrib3sv;
	PFNGLVERTEXATTRIB4NBVPROC		glVertexAttrib4Nbv;
	PFNGLVERTEXATTRIB4NIVPROC		glVertexAttrib4Niv;
	PFNGLVERTEXATTRIB4NSVPROC		glVertexAttrib4Nsv;
	PFNGLVERTEXATTRIB4NUBPROC		glVertexAttrib4Nub;
	PFNGLVERTEXATTRIB4NUBVPROC		glVertexAttrib4Nubv;
	PFNGLVERTEXATTRIB4NUIVPROC		glVertexAttrib4Nuiv;
	PFNGLVERTEXATTRIB4NUSVPROC		glVertexAttrib4Nusv;
	PFNGLVERTEXATTRIB4BVPROC		glVertexAttrib4bv;
	PFNGLVERTEXATTRIB4DPROC			glVertexAttrib4d;
	PFNGLVERTEXATTRIB4DVPROC		glVertexAttrib4dv;
	PFNGLVERTEXATTRIB4FPROC			glVertexAttrib4f;
	PFNGLVERTEXATTRIB4FVPROC		glVertexAttrib4fv;
	PFNGLVERTEXATTRIB4IVPROC		glVertexAttrib4iv;
	PFNGLVERTEXATTRIB4SPROC			glVertexAttrib4s;
	PFNGLVERTEXATTRIB4SVPROC		glVertexAttrib4sv;
	PFNGLVERTEXATTRIB4UBVPROC		glVertexAttrib4ubv;
	PFNGLVERTEXATTRIB4UIVPROC		glVertexAttrib4uiv;
	PFNGLVERTEXATTRIB4USVPROC		glVertexAttrib4us;
	PFNGLVERTEXATTRIBPOINTERPROC	glVertexAttribPointer;

    //BAD BAD BAD BAD :)

    //purukumi - tuggummi
    //tehdään nää sit myöhemmin kunnolla

    void bindTextureFBO1024(GLuint textureID);
	void bindTextureFBO512(GLuint textureID);
	void bindTextureFBO256(GLuint textureID);
	void bindTextureFBO128(GLuint textureID);
//	void bindScreenFBO(GLuint textureID);
//	void bindSmallScreenFBO(GLuint textureID);
    void unbindFBO();

	// Draws screenbuffercontents back into screen
	void drawTexture(GLuint textureID);

    GLuint frameBufferTexture1024;
	GLuint frameBufferTexture512;
	GLuint frameBufferTexture256;
	GLuint frameBufferTexture128;
//    GLuint frameBufferScreen;
//    GLuint frameBufferSmallScreen;

    GLuint depthRenderBufferTexture1024;
	GLuint depthRenderBufferTexture512;
	GLuint depthRenderBufferTexture256;
	GLuint depthRenderBufferTexture128;
//    GLuint depthRenderBufferScreen;
//    GLuint depthRenderBufferSmallScreen;

    int screenWidth;
    int screenHeight;

    int smallW;
    int smallH;

	//Particle motion blur tekstuuri
	GLuint motionBlur1024;

	// Ruudun kokoinen tekstuuri
	GLuint glowTexture1024ID;
	GLuint glowTexture1024ID2;
	GLuint glowTexture512ID;
	GLuint glowTexture512ID2;
	GLuint glowTexture256ID;
	GLuint glowTexture128ID;
	GLuint glowTexture128ID2;

//  GLuint screenTextureID;
//	GLuint smallScreenTextureID;
//	GLuint smallScreenTextureID2;

	// glow
	int glowTextureW;
	int glowTextureH;
};


#endif