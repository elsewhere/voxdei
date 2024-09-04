#include "shader.hpp"

//-------------
// shader initialization
//-------------

#include <list>

void shaders_init() {

		static const char *emptyVertex = {
				"varying vec2 Tex_coord;"\
				"void main(void) "\
				"{"\
				"   gl_Position = ftransform();"\
				"   gl_TexCoord[0] = gl_MultiTexCoord0;"\
				"}"	
		};

		// *** BlurX

		static const char *blurxVertex = {
				"varying vec2 pos0;"\
				"varying vec2 pos1;"\
				"varying vec2 pos2;"\
				"varying vec2 pos3;"\
				"varying vec2 pos4;"\
				"uniform float xoff;"\
				"void main(void) "\
				"{"\
				"   vec2 DX = vec2(xoff,0.0);"\
				"   vec2 DX2 = vec2(2.0*xoff,0.0);"\
		        "   pos0 = gl_MultiTexCoord0.st-DX2;"\
				"   pos1 = gl_MultiTexCoord0.st-DX;"\
				"   pos2 = gl_MultiTexCoord0.st;"\
				"   pos3 = gl_MultiTexCoord0.st+DX;"\
				"   pos4 = gl_MultiTexCoord0.st+DX2;"\
				"   gl_Position = ftransform();"\
				"}"	
		};

		// *** BlurY

		static const char *bluryVertex = {
				"varying vec2 pos0;"\
				"varying vec2 pos1;"\
				"varying vec2 pos2;"\
				"varying vec2 pos3;"\
				"varying vec2 pos4;"\
				"uniform float yoff;"\
				"void main(void) "\
				"{"\
				"   vec2 DX = vec2(0.0, yoff);"\
				"   vec2 DX2 = vec2(0.0, 2.0*yoff);"\
		        "   pos0 = gl_MultiTexCoord0.st-DX2;"\
				"   pos1 = gl_MultiTexCoord0.st-DX;"\
				"   pos2 = gl_MultiTexCoord0.st;"\
				"   pos3 = gl_MultiTexCoord0.st+DX;"\
				"   pos4 = gl_MultiTexCoord0.st+DX2;"\
				"   gl_Position = ftransform();"\
				"}"	
		};

		// *** BlurFragment

		static const char *blurFragment = {
          "uniform sampler2D tex;"\
		  "varying vec2 pos0;"\
		  "varying vec2 pos1;"\
		  "varying vec2 pos2;"\
		  "varying vec2 pos3;"\
		  "varying vec2 pos4;"\
          "uniform float alpha;"\
          "void main()"\
          "{"\

          "   vec4 col = vec4(0,0,0,0);"\
          "   col+=texture2D(tex, pos0)*0.1;"\
          "   col+=texture2D(tex, pos1)*0.25;"\
          "   col+=texture2D(tex, pos2)*0.5;"\
          "   col+=texture2D(tex, pos3)*0.25;"\
          "   col+=texture2D(tex, pos4)*0.1;"\
          "   gl_FragColor = col*alpha;"\
          "}"
		};

		static const char *blur2Fragment = {
          "uniform sampler2D tex;"\
		  "varying vec2 pos0;"\
		  "varying vec2 pos1;"\
		  "varying vec2 pos2;"\
		  "varying vec2 pos3;"\
		  "varying vec2 pos4;"\
          "uniform float alpha;"\
          "void main()"\
          "{"\
          "   vec4 col = vec4(0,0,0,0);"\
          "   col+=texture2D(tex, pos0)*0.1;"\
          "   col+=texture2D(tex, pos1)*0.2;"\
          "   col+=texture2D(tex, pos2)*0.4;"\
          "   col+=texture2D(tex, pos3)*0.2;"\
          "   col+=texture2D(tex, pos4)*0.1;"\
          "   gl_FragColor = col*alpha;"\
          "}"
		};

		// *** glow combine

		static const char *glowcVertex = {
			"varying vec2 coord;"\
			"void main(void) "\
			"{"\
			"   coord = gl_MultiTexCoord0.xy;"\
			"   gl_Position = ftransform();"\
			"}"
		};


		static const char *glowcFragment = {
			"uniform sampler2D texunit0;"\
			"uniform sampler2D texunit1;"\
			"uniform float glow_amount;"\
			"varying vec2 coord;"\
			"void main()"\
			"{"\
			"	vec4 norm = texture2D(texunit0, coord);"\
			"	vec4 glow = texture2D(texunit1, coord);"\
			"	gl_FragColor = mix(norm,glow,glow_amount);"\
			"}"
		};

        // *** displacement

		static const char *displaceVertex = {
			"varying vec2 coord;"\
			"void main(void) "\
			"{"\
			"   coord = gl_MultiTexCoord0.xy;"\
			"   gl_Position = ftransform();"\
			"}"
		};

        static const char *displaceFragment = {
            "uniform sampler2D texunit0;"\
            "uniform sampler2D texunit1;"\
            "uniform float displace_amount;"\
            "uniform float alpha;"\
            "varying vec2 coord;"\
            "void main()"\
            "{"\
            "   vec2 offs = texture2D(texunit1, coord).xy;"\
            "   vec4 col = texture2D(texunit0, coord+offs*displace_amount);"\
            "   col *= alpha; "\
            "   gl_FragColor = col;"\
            "}"
        };

        // *** invert

		static const char *invertVertex = {
			"varying vec2 coord;"\
			"void main(void) "\
			"{"\
			"   coord = gl_MultiTexCoord0.xy;"\
			"   gl_Position = ftransform();"\
			"}"
		};

        static const char *invertFragment = {
            "uniform sampler2D texunit0;"\
            "uniform float invert_amount ;"\
            "uniform float alpha;"\
            "varying vec2 coord;"\
            "void main()"\
            "{"\
            "   vec4 col = texture2D(texunit0, coord);"\
            "   col.x = col.x * (1.0-invert_amount) + (1.0-col.x) * invert_amount; "\
            "   col.y = col.y * (1.0-invert_amount) + (1.0-col.y) * invert_amount; "\
            "   col.z = col.z * (1.0-invert_amount) + (1.0-col.z) * invert_amount; "\
            "   col *= alpha; "\
            "   gl_FragColor = col;"\
            "}"
        };


		// *** darken 

		static const char *darkenVertex = {
			"varying vec2 coord;"\
			"void main(void) "\
			"{"\
			"   gl_Position = ftransform();"\
			"   coord = gl_MultiTexCoord0.xy;"\
			"}"
		};

		static const char *darkenFragment = {
			"uniform sampler2D tex;"\
			"uniform float alpha;"\
			"varying vec2 coord;"\
			"void main()		"\
			"{"\
			"   vec4 c = texture2D(tex, coord); "\
			"	gl_FragColor = c*c*alpha; "\
			"}"
		};


	blurx = new shader;
	init_shader(blurx, blurFragment, blurxVertex);

	blury = new shader;
	init_shader(blury, blurFragment, bluryVertex);

	blur2x = new shader;
	init_shader(blur2x, blur2Fragment, blurxVertex);

	blur2y = new shader;
	init_shader(blur2y, blur2Fragment, bluryVertex);

	darken = new shader;
	init_shader(darken,darkenFragment, darkenVertex);

	glowc = new shader;
	init_shader(glowc, glowcFragment, glowcVertex);

	displace = new shader;
	init_shader(displace, displaceFragment, displaceVertex);

}
void shaders_free() {
	free_shader(blurx);
	free_shader(blury);
	free_shader(blur2x);
	free_shader(blur2y);
	free_shader(darken);
	free_shader(glowc);
	free_shader(displace);
}


//-------------
// single shader stuff 
//-------------

GLint GetUniLoc(shader *s, const GLcharARB *name)
{
	GLint loc;
	loc = glExt.glGetUniformLocationARB(s->program, name);
	return loc;
}


void debug_shader(GLuint obj)
{
	int infologLength = 0;
	char infoLog[1024];
 
	if (glExt.glIsShader(obj))  {
		glExt.glGetShaderInfoLog(obj, 1024, &infologLength, infoLog);
		if(infologLength>0) 
			MessageBox(NULL, infoLog, "Error", MB_OK|MB_TOPMOST);		
	}  else  {
		glExt.glGetProgramInfoLog(obj, 1024, &infologLength, infoLog);
	}	
}

void init_shader(shader *s, const char *frag, const char *vert) {

		s->vertex = glExt.glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		glExt.glShaderSourceARB(s->vertex, 1, &vert,NULL);
		glExt.glCompileShaderARB(s->vertex);
		debug_shader(s->vertex);

		s->fragment = glExt.glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
		glExt.glShaderSourceARB(s->fragment, 1, &frag,NULL);
		glExt.glCompileShaderARB(s->fragment);
		debug_shader(s->fragment);

		s->program = glExt.glCreateProgramObjectARB();
		glExt.glAttachObjectARB(s->program,s->vertex);
		glExt.glAttachObjectARB(s->program,s->fragment);
		glExt.glLinkProgramARB(s->program);
		debug_shader(s->program);

		//glUseProgramObjectARB(s->program);
		
}

void free_shader(shader *s) {
		glExt.glDeleteShader(s->vertex);
		glExt.glDeleteShader(s->fragment);
		glExt.glDeleteProgram(s->program);
}
