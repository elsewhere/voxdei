#include "postproc.hpp"

extern class PostProc filter;


void PostProc::init(bool clear) {
	glExt.bindTextureFBO1024(glExt.glowTexture1024ID);
	
	if(clear) 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
}

void PostProc::glow(int blur_steps, float blur_step_x, float blur_step_y, float blur_brightness, float darken_alpha, float glow_amount) 
{

	// Rale 10.7.2006
   	glExt.unbindFBO();

	// Haetaan ruudun koko
	int screensizex = OPENGL_XRES;
    int screensizey = OPENGL_YRES;
	persp2D(screensizex, screensizey);
	glLoadIdentity();

	// Poistetaan syvyystestaus ja blendaus käytöstä
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(0);
	
	glEnable(GL_TEXTURE_2D);

	glColor4f(1,1,1,1);

	// Tarkastetaan että haluttiin darkensheideri käyttöön
	if(darken_alpha>=0) {
		
		glExt.glUseProgramObjectARB(darken->program);
		glExt.glActiveTextureARB(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, glExt.glowTexture1024ID);
		glExt.glUniform1i(glExt.glGetUniformLocationARB(darken->program, "tex"), 0);  
		glExt.glUniform1fARB(glExt.glGetUniformLocationARB(darken->program, "alpha"), darken_alpha);

	} else glExt.glUseProgramObjectARB(0);

	// Piirretään alkuperäinen kuva tummennettuna pikkubufferiin
	glExt.bindTextureFBO512(glExt.glowTexture512ID);

		glBindTexture(GL_TEXTURE_2D, glExt.glowTexture1024ID);
			glBegin(GL_QUADS);
				glTexCoord2i(0, 0);
				glVertex2i(0, 0);
				glTexCoord2i(1, 0);
				glVertex2i(screensizex, 0);
				glTexCoord2i(1, 1);
				glVertex2i(screensizex, screensizey);
				glTexCoord2i(0, 1);
				glVertex2i(0, screensizey);			
			glEnd();
	 
	glExt.unbindFBO();

	
	if(darken_alpha>=0) {
		glExt.glUseProgramObjectARB(0);
	}

	// Rendertargettia pienemmäksi

	glExt.bindTextureFBO256(glExt.glowTexture256ID);
			
		glBindTexture(GL_TEXTURE_2D, glExt.glowTexture512ID);
			glBegin(GL_QUADS);
				glTexCoord2i(0, 0);
				glVertex2i(0, 0);
				glTexCoord2i(1, 0);
				glVertex2i(screensizex, 0);
				glTexCoord2i(1, 1);
				glVertex2i(screensizex, screensizey);
				glTexCoord2i(0, 1);
				glVertex2i(0, screensizey);			
			glEnd();
	glExt.unbindFBO();

	glExt.bindTextureFBO128(glExt.glowTexture128ID);
		glBindTexture(GL_TEXTURE_2D, glExt.glowTexture256ID);
			glBegin(GL_QUADS);
				glTexCoord2i(0, 0);
				glVertex2i(0, 0);
				glTexCoord2i(1, 0);
				glVertex2i(screensizex, 0);
				glTexCoord2i(1, 1);
				glVertex2i(screensizex, screensizey);
				glTexCoord2i(0, 1);
				glVertex2i(0, screensizey);			
			glEnd();
	glExt.unbindFBO();

	// Seuraavaksi blurrataan pikkubufferikuvaa
	for(int i=0; i<blur_steps; i++) {

		glExt.glUseProgramObjectARB(blurx->program);
		glExt.glUniform1i(glExt.glGetUniformLocationARB(blurx->program, "tex"), 0);
		glExt.glUniform1fARB(glExt.glGetUniformLocationARB(blurx->program, "xoff"), blur_step_x);
		glExt.glUniform1fARB(glExt.glGetUniformLocationARB(blurx->program, "alpha"), blur_brightness);

			glExt.bindTextureFBO128(glExt.glowTexture128ID2);
				glBindTexture(GL_TEXTURE_2D, glExt.glowTexture128ID);
					 glBegin(GL_QUADS);
						glTexCoord2i(0, 0);
						glVertex2i(0, 0);
						glTexCoord2i(1, 0);
						glVertex2i(screensizex, 0);
						glTexCoord2i(1, 1);
						glVertex2i(screensizex, screensizey);
						glTexCoord2i(0, 1);
						glVertex2i(0, screensizey);
					 glEnd();   
			glExt.unbindFBO();

		glExt.glUseProgramObjectARB(0);
		
		glExt.glUseProgramObjectARB(blury->program);
		glExt.glUniform1i(glExt.glGetUniformLocationARB(blury->program, "tex"), 0);
		glExt.glUniform1fARB(glExt.glGetUniformLocationARB(blury->program, "yoff"), blur_step_x);
		glExt.glUniform1fARB(glExt.glGetUniformLocationARB(blury->program, "alpha"), blur_brightness);


			glExt.bindTextureFBO128(glExt.glowTexture128ID);
				glBindTexture(GL_TEXTURE_2D, glExt.glowTexture128ID2);
					 glBegin(GL_QUADS);
						glTexCoord2i(0, 0);
						glVertex2i(0, 0);
						glTexCoord2i(1, 0);
						glVertex2i(screensizex, 0);
						glTexCoord2i(1, 1);
						glVertex2i(screensizex, screensizey);
						glTexCoord2i(0, 1);
						glVertex2i(0, screensizey);
					 glEnd();   
			glExt.unbindFBO();

		glExt.glUseProgramObjectARB(0);
	}

	/**/

	// blendi päälle - että alphakanavat futaa
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE);
	glColor4f(1,1,1,1); 

	// Piirretään alkuperäinen ja blurrattu yhdessä 

	// glow shaderi päälle
	glExt.glUseProgramObjectARB(glowc->program);
		glExt.glUniform1fARB(glExt.glGetUniformLocationARB(glowc->program, "glow_amount"), glow_amount);

		glExt.glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,glExt.glowTexture1024ID);
		glExt.glUniform1i(glExt.glGetUniformLocationARB(glowc->program, "texunit0"), 0);

		glExt.glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,glExt.glowTexture128ID);
		glExt.glUniform1i(glExt.glGetUniformLocationARB(glowc->program, "texunit1"), 1);

				 glBegin(GL_QUADS);
					glExt.glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 0);
					glExt.glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 0);
					glVertex2i(0, 0);
					glExt.glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 0);
					glExt.glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1, 0);
					glVertex2i(screensizex, 0);
					glExt.glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 1);
					glExt.glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1, 1);
					glVertex2i(screensizex, screensizey);
					glExt.glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 1);
					glExt.glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 1);
					glVertex2i(0, screensizey);
				 glEnd();   

		glExt.glActiveTextureARB(GL_TEXTURE1);
		glDisable(GL_TEXTURE_2D);
		

	glExt.glActiveTextureARB(GL_TEXTURE0);
	glExt.glUseProgramObjectARB(0);


 
	/**/

	glEnable(GL_TEXTURE_2D);

	// Syvyystestaus taas päälle
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(1);


	// Takaisin 3d moodiin 
	persp3D();
	glLoadIdentity();

}

void PostProc::blur(int blur_steps, float blur_step_x, float blur_step_y) {

   	
	glExt.unbindFBO();

	// Haetaan ruudun koko
	int screensizex = OPENGL_XRES;
    int screensizey = OPENGL_YRES;
	persp2D(screensizex, screensizey);
	glLoadIdentity();

	// Poistetaan syvyystestaus ja blendaus käytöstä
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(0);
	
	glEnable(GL_TEXTURE_2D);

	glColor4f(1,1,1,1);

	glExt.bindTextureFBO512(glExt.glowTexture512ID);

		glBindTexture(GL_TEXTURE_2D, glExt.glowTexture1024ID);
			glBegin(GL_QUADS);
				glTexCoord2i(0, 0);
				glVertex2i(0, 0);
				glTexCoord2i(1, 0);
				glVertex2i(screensizex, 0);
				glTexCoord2i(1, 1);
				glVertex2i(screensizex, screensizey);
				glTexCoord2i(0, 1);
				glVertex2i(0, screensizey);			
			glEnd();
	 
	glExt.unbindFBO();

	// Seuraavaksi blurrataan pikkubufferikuvaa
	for(int i=0; i<blur_steps; i++) {

		glExt.glUseProgramObjectARB(blur2x->program);
		glExt.glUniform1i(glExt.glGetUniformLocationARB(blur2x->program, "tex"), 0);  
		glExt.glUniform1fARB(glExt.glGetUniformLocationARB(blur2x->program, "xoff"), blur_step_x);
		glExt.glUniform1fARB(glExt.glGetUniformLocationARB(blur2x->program, "alpha"), 1);

			glExt.bindTextureFBO512(glExt.glowTexture512ID2);
				glBindTexture(GL_TEXTURE_2D, glExt.glowTexture512ID);
					 glBegin(GL_QUADS);
						glTexCoord2i(0, 0);
						glVertex2i(0, 0);
						glTexCoord2i(1, 0);
						glVertex2i(screensizex, 0);
						glTexCoord2i(1, 1);
						glVertex2i(screensizex, screensizey);
						glTexCoord2i(0, 1);
						glVertex2i(0, screensizey);
					 glEnd();   
			glExt.unbindFBO();

		glExt.glUseProgramObjectARB(0);

		glExt.glUseProgramObjectARB(blur2y->program);
		glExt.glUniform1i(glExt.glGetUniformLocationARB(blur2y->program, "tex"), 0);  
		glExt.glUniform1fARB(glExt.glGetUniformLocationARB(blur2y->program, "xoff"), blur_step_y);
		glExt.glUniform1fARB(glExt.glGetUniformLocationARB(blur2y->program, "alpha"), 1);


			glExt.bindTextureFBO512(glExt.glowTexture512ID);
				glBindTexture(GL_TEXTURE_2D, glExt.glowTexture512ID2);
					 glBegin(GL_QUADS);
						glTexCoord2i(0, 0);
						glVertex2i(0, 0);
						glTexCoord2i(1, 0);
						glVertex2i(screensizex, 0);
						glTexCoord2i(1, 1);
						glVertex2i(screensizex, screensizey);
						glTexCoord2i(0, 1);
						glVertex2i(0, screensizey);
					 glEnd();   
			glExt.unbindFBO();

		glExt.glUseProgramObjectARB(0);
	}

		glBindTexture(GL_TEXTURE_2D, glExt.glowTexture512ID);
			glBegin(GL_QUADS);
				glTexCoord2i(0, 0);
				glVertex2i(0, 0);
				glTexCoord2i(1, 0);
				glVertex2i(screensizex, 0);
				glTexCoord2i(1, 1);
				glVertex2i(screensizex, screensizey);
				glTexCoord2i(0, 1);
				glVertex2i(0, screensizey);			
			glEnd();
	/**/

	// Syvyystestaus taas päälle
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(1);

	// Takaisin 3d moodiin 
	persp3D();
	glLoadIdentity();

}

void persp2D(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();	
	glOrtho(0,w,0,h,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); 
	glLoadIdentity();
}

void persp3D(void)
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void PostProc::draw_grid(Texture *t, float alpha, float mx, float my, float r, float g, float b, float scale,
						 int type) {
    
	GLuint src, dst;
	if(type==0) {
		// 'lisää tekstuurin'		
		src = GL_SRC_ALPHA;
		dst = GL_ONE;
	} else {
		// 'poistaa värejä'
		src = GL_ZERO;
		dst = GL_ONE_MINUS_SRC_ALPHA;
	}

	persp2D(1,1);
    glLoadIdentity();

	//nyt piirretään blurrattu 
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glBlendFunc(src, dst);

    glColor4f(r,g,b, alpha);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, t->ID);	

	//	glScalef(scale,scale,scale);
	// Tekstuurin skaalausfaktori

	// MoveX , MoveY - tekstuuriliikkuu
	// mx = 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBegin(GL_QUADS);
    glTexCoord2f(((0+mx)/128.0f)*scale, ((0+my)/128.0f)*scale);
    glVertex2f(0, 0);
    glTexCoord2f((1+(0+mx)/128.0f)*scale, ((0+my)/128.0f)*scale);
    glVertex2f(1, 0);
    glTexCoord2f((1+(0+mx)/128.0f)*scale, (1+(0+my)/128.0f)*scale);
    glVertex2f(1, 1);
    glTexCoord2f(((0+mx)/128.0f)*scale, (1+(0+my)/128.0f)*scale);
    glVertex2f(0, 1);
    glEnd();

    persp3D();
    glLoadIdentity();
    glDepthMask(1);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D); 
}