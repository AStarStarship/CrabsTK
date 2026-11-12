#pragma once
#ifndef NANOVG_GL_H
#define NANOVG_GL_H

#ifdef __cplusplus
extern "C" {
#endif

// Create flags

enum CreateFlags {
  // Flag indicating if geometry based anti-aliasing is used (may not be needed when using MSAA).
  CFAntialias     = 1<<0,
  // Flag indicating if strokes should be drawn using stencil buffer. The rendering will be a little
  // slower, but path overlaps (i.e. self-intersecting or sharp turns) will be drawn just once.
  CFStencilStrokes  = 1<<1,
  // Flag indicating that additional debug checks are done.
  CFDebug       = 1<<2,
};

#if defined NANOVG_GL2_IMPLEMENTATION
#  define NANOVG_GL2 1
#  define NANOVG_GL_IMPLEMENTATION 1
#elif defined NANOVG_GL3_IMPLEMENTATION
#  define NANOVG_GL3 1
#  define NANOVG_GL_IMPLEMENTATION 1
#  define NANOVG_GL_USE_UNIFORMBUFFER 1
#elif defined NANOVG_GLES2_IMPLEMENTATION
#  define NANOVG_GLES2 1
#  define NANOVG_GL_IMPLEMENTATION 1
#elif defined NANOVG_GLES3_IMPLEMENTATION
#  define NANOVG_GLES3 1
#  define NANOVG_GL_IMPLEMENTATION 1
#endif

#define NANOVG_GL_USE_STATE_FILTER (1)

// Creates NanoVG contexts for different OpenGL (ES) versions.
// Flags should be combination of the create flags above.
// The caller-owned Wall must outlive the context. Reserve six backend slots
// beyond the VG/font budget: context, textures, calls, paths, verts, uniforms.
// Each growth consumes another slot and retains the old block until Wall reset.
// Initial arrays hold 4 textures, 128 calls/paths/uniforms and 4096 vertices;
// budget their bytes (uniforms use the GL-aligned fragSize) plus growth copies.
// Delete the context before resetting the Wall so GL resources are released.

#if defined NANOVG_GL2

RenderContext* ExoCreateGL2(ISC flags, _::TWall<>* wall);
void ExoDeleteGL2(RenderContext* context);

ISC ExolCreateImageFromHandleGL2(RenderContext* context, GLuint textureId, ISC w, ISC h, ISC flags);
GLuint ExolImageHandleGL2(RenderContext* context, ISC image);

#endif

#if defined NANOVG_GL3

RenderContext* ExoCreateGL3(ISC flags, _::TWall<>* wall);
void ExoDeleteGL3(RenderContext* context);

ISC ExolCreateImageFromHandleGL3(RenderContext* context, GLuint textureId, ISC w, ISC h, ISC flags);
GLuint ExolImageHandleGL3(RenderContext* context, ISC image);

#endif

#if defined NANOVG_GLES2

RenderContext* ExoCreateGLES2(ISC flags, _::TWall<>* wall);
void ExoDeleteGLES2(RenderContext* context);

ISC ExolCreateImageFromHandleGLES2(RenderContext* context, GLuint textureId, ISC w, ISC h, ISC flags);
GLuint ExolImageHandleGLES2(RenderContext* context, ISC image);

#endif

#if defined NANOVG_GLES3

RenderContext* ExoCreateGLES3(ISC flags, _::TWall<>* wall);
void ExoDeleteGLES3(RenderContext* context);

ISC ExolCreateImageFromHandleGLES3(RenderContext* context, GLuint textureId, ISC w, ISC h, ISC flags);
GLuint ExolImageHandleGLES3(RenderContext* context, ISC image);

#endif

// These are additional flags on top of ImageFlags.
enum GLImageFlags {
  ImageNoDelete      = 1<<16,  // Do not delete GL texture handle.
};

#ifdef __cplusplus
}
#endif

#endif /* NANOVG_GL_H */

#ifdef NANOVG_GL_IMPLEMENTATION

#include <stdio.h>
#include <math.h>
#include "VG.h"

enum GLExoUniformLoc {
  GLExoLocViewSize,
  GLExoLocTex,
  GLExoLocFrag,
  GLExoMaxLocs
};

enum GLExoShaderType {
  GLExoShaderFillGrad,
  GLExoShaderFillImg,
  GLExoShaderSimple,
  GLExoShaderImg
};

#if NANOVG_GL_USE_UNIFORMBUFFER
enum GLExoUniformBindings {
  GLExoFragBinding = 0,
};
#endif

struct GLExoShader {
  GLuint prog;
  GLuint frag;
  GLuint vert;
  GLint loc[GLExoMaxLocs];
};

struct GLExoTexture {
  ISC id;
  GLuint tex;
  ISC width, height;
  ISC type;
  ISC flags;
};

struct GLExoBlend
{
  GLenum srcRGB;
  GLenum dstRGB;
  GLenum srcAlpha;
  GLenum dstAlpha;
};

enum GLExoCallType {
  GLExoCallNone = 0,
  GLExoCallFill,
  GLExoCallConvexFill,
  GLExoCallStroke,
  GLExoCallTriangles,
};

struct GLExoCall {
  ISC type;
  ISC image;
  ISC pathOffset;
  ISC pathCount;
  ISC triangleOffset;
  ISC triangleCount;
  ISC uniformOffset;
  GLExoBlend blendFunc;
};

struct GLExoPath {
  ISC fillOffset;
  ISC fillCount;
  ISC strokeOffset;
  ISC strokeCount;
};

struct GLExoFragUniforms {
  #if NANOVG_GL_USE_UNIFORMBUFFER
    FPC scissorMat[12]; // matrices are actually 3 vec4s
    FPC paintMat[12];
    struct ColorFPC innerCol;
    struct ColorFPC outerCol;
    FPC scissorExt[2];
    FPC scissorScale[2];
    FPC extent[2];
    FPC radius;
    FPC feather;
    FPC strokeMult;
    FPC strokeThr;
    ISC texType;
    ISC type;
  #else
    // note: after modifying layout or size of uniform array,
    // don't forget to also update the fragment shader source!
    #define NANOVG_GL_UNIFORMARRAY_SIZE 11
    union {
      struct {
        FPC scissorMat[12]; // matrices are actually 3 vec4s
        FPC paintMat[12];
        struct ColorFPC innerCol;
        struct ColorFPC outerCol;
        FPC scissorExt[2];
        FPC scissorScale[2];
        FPC extent[2];
        FPC radius;
        FPC feather;
        FPC strokeMult;
        FPC strokeThr;
        FPC texType;
        FPC type;
      };
      FPC uniformArray[NANOVG_GL_UNIFORMARRAY_SIZE][4];
    };
  #endif
};

struct GLExoContext {
  _::TWall<>* wall;
  GLExoShader shader;
  GLExoTexture* textures;
  FPC view[2];
  ISC ntextures;
  ISC ctextures;
  ISC textureId;
  GLuint vertBuf;
#if defined NANOVG_GL3
  GLuint vertArr;
#endif
#if NANOVG_GL_USE_UNIFORMBUFFER
  GLuint fragBuf;
#endif
  ISC fragSize;
  ISC flags;

  // Per frame buffers
  GLExoCall* calls;
  ISC ccalls;
  ISC ncalls;
  GLExoPath* paths;
  ISC cpaths;
  ISC npaths;
  struct VertexFPC* verts;
  ISC cverts;
  ISC nverts;
  CHR* uniforms;
  ISC cuniforms;
  ISC nuniforms;

  // cached state
  #if NANOVG_GL_USE_STATE_FILTER
  GLuint boundTexture;
  GLuint stencilMask;
  GLenum stencilFunc;
  GLint stencilFuncRef;
  GLuint stencilFuncMask;
  GLExoBlend blendFunc;
  #endif

  ISC dummyTex;
};

ISC glExo__maxi(ISC a, ISC b) { return a > b ? a : b; }

#ifdef NANOVG_GLES2
IUC glExo__nearestPow2(IUC num)
{
  unsigned n = num > 0 ? num - 1 : 0;
  n |= n >> 1;
  n |= n >> 2;
  n |= n >> 4;
  n |= n >> 8;
  n |= n >> 16;
  n++;
  return n;
}
#endif

void glExo__bindTexture(GLExoContext* gl, GLuint tex)
{
#if NANOVG_GL_USE_STATE_FILTER
  if (gl->boundTexture != tex) {
    gl->boundTexture = tex;
    glBindTexture(GL_TEXTURE_2D, tex);
  }
#else
  glBindTexture(GL_TEXTURE_2D, tex);
#endif
}

void glExo__stencilMask(GLExoContext* gl, GLuint mask)
{
#if NANOVG_GL_USE_STATE_FILTER
  if (gl->stencilMask != mask) {
    gl->stencilMask = mask;
    glStencilMask(mask);
  }
#else
  glStencilMask(mask);
#endif
}

void glExo__stencilFunc(GLExoContext* gl, GLenum func, GLint ref, GLuint mask)
{
#if NANOVG_GL_USE_STATE_FILTER
  if ((gl->stencilFunc != func) ||
    (gl->stencilFuncRef != ref) ||
    (gl->stencilFuncMask != mask)) {

    gl->stencilFunc = func;
    gl->stencilFuncRef = ref;
    gl->stencilFuncMask = mask;
    glStencilFunc(func, ref, mask);
  }
#else
  glStencilFunc(func, ref, mask);
#endif
}
void glExo__blendFuncSeparate(GLExoContext* gl, const GLExoBlend* blend)
{
#if NANOVG_GL_USE_STATE_FILTER
  if ((gl->blendFunc.srcRGB != blend->srcRGB) ||
    (gl->blendFunc.dstRGB != blend->dstRGB) ||
    (gl->blendFunc.srcAlpha != blend->srcAlpha) ||
    (gl->blendFunc.dstAlpha != blend->dstAlpha)) {

    gl->blendFunc = *blend;
    glBlendFuncSeparate(blend->srcRGB, blend->dstRGB, blend->srcAlpha,blend->dstAlpha);
  }
#else
  glBlendFuncSeparate(blend->srcRGB, blend->dstRGB, blend->srcAlpha,blend->dstAlpha);
#endif
}

GLExoTexture* glExo__allocTexture(GLExoContext* gl)
{
  GLExoTexture* tex = nullptr;
  ISC i;

  for (i = 0; i < gl->ntextures; i++) {
    if (gl->textures[i].id == 0) {
      tex = &gl->textures[i];
      break;
    }
  }
  if (tex == nullptr) {
    if (gl->ntextures+1 > gl->ctextures) {
      GLExoTexture* textures;
      ISC ctextures = glExo__maxi(gl->ntextures+1, 4) +  gl->ctextures/2; // 1.5x Overallocate
      textures = (GLExoTexture*)Exo__realloc(gl->wall, gl->textures,
          sizeof(GLExoTexture)*gl->ctextures, sizeof(GLExoTexture)*ctextures);
      if (textures == nullptr) return nullptr;
      gl->textures = textures;
      gl->ctextures = ctextures;
    }
    tex = &gl->textures[gl->ntextures++];
  }

  _::ArrayFill(tex, sizeof(*tex));
  tex->id = ++gl->textureId;

  return tex;
}

GLExoTexture* glExo__findTexture(GLExoContext* gl, ISC id)
{
  ISC i;
  for (i = 0; i < gl->ntextures; i++)
    if (gl->textures[i].id == id)
      return &gl->textures[i];
  return nullptr;
}

ISC glExo__deleteTexture(GLExoContext* gl, ISC id)
{
  ISC i;
  for (i = 0; i < gl->ntextures; i++) {
    if (gl->textures[i].id == id) {
      if (gl->textures[i].tex != 0 && (gl->textures[i].flags & ImageNoDelete) == 0)
        glDeleteTextures(1, &gl->textures[i].tex);
      _::ArrayFill(&gl->textures[i], sizeof(gl->textures[i]));
      return 1;
    }
  }
  return 0;
}

void glExo__dumpShaderError(GLuint shader, const CHR* name, const CHR* type)
{
  GLchar str[512+1];
  GLsizei len = 0;
  glGetShaderInfoLog(shader, 512, &len, str);
  if (len > 512) len = 512;
  str[len] = '\0';
  printf("Shader %s/%s error:\n%s\n", name, type, str);
}

void glExo__dumpProgramError(GLuint prog, const CHR* name)
{
  GLchar str[512+1];
  GLsizei len = 0;
  glGetProgramInfoLog(prog, 512, &len, str);
  if (len > 512) len = 512;
  str[len] = '\0';
  printf("Program %s error:\n%s\n", name, str);
}

void glExo__checkError(GLExoContext* gl, const CHR* str)
{
  GLenum err;
  if ((gl->flags & CFDebug) == 0) return;
  err = glGetError();
  if (err != GL_NO_ERROR) {
    printf("Error %08x after %s\n", err, str);
    return;
  }
}

ISC glExo__createShader(GLExoShader* shader, const CHR* name, const CHR* header, const CHR* opts, const CHR* vshader, const CHR* fshader)
{
  GLint status;
  GLuint prog, vert, frag;
  const CHR* str[3];
  str[0] = header;
  str[1] = opts != nullptr ? opts : "";

  _::ArrayFill(shader, sizeof(*shader));

  prog = glCreateProgram();
  vert = glCreateShader(GL_VERTEX_SHADER);
  frag = glCreateShader(GL_FRAGMENT_SHADER);
  str[2] = vshader;
  glShaderSource(vert, 3, str, 0);
  str[2] = fshader;
  glShaderSource(frag, 3, str, 0);

  glCompileShader(vert);
  glGetShaderiv(vert, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    glExo__dumpShaderError(vert, name, "vert");
    return 0;
  }

  glCompileShader(frag);
  glGetShaderiv(frag, GL_COMPILE_STATUS, &status);
  if (status != GL_TRUE) {
    glExo__dumpShaderError(frag, name, "frag");
    return 0;
  }

  glAttachShader(prog, vert);
  glAttachShader(prog, frag);

  glBindAttribLocation(prog, 0, "vertex");
  glBindAttribLocation(prog, 1, "tcoord");

  glLinkProgram(prog);
  glGetProgramiv(prog, GL_LINK_STATUS, &status);
  if (status != GL_TRUE) {
    glExo__dumpProgramError(prog, name);
    return 0;
  }

  shader->prog = prog;
  shader->vert = vert;
  shader->frag = frag;

  return 1;
}

void glExo__deleteShader(GLExoShader* shader)
{
  if (shader->prog != 0)
    glDeleteProgram(shader->prog);
  if (shader->vert != 0)
    glDeleteShader(shader->vert);
  if (shader->frag != 0)
    glDeleteShader(shader->frag);
}

void glExo__getUniforms(GLExoShader* shader)
{
  shader->loc[GLExoLocViewSize] = glGetUniformLocation(shader->prog, "viewSize");
  shader->loc[GLExoLocTex] = glGetUniformLocation(shader->prog, "tex");

#if NANOVG_GL_USE_UNIFORMBUFFER
  shader->loc[GLExoLocFrag] = glGetUniformBlockIndex(shader->prog, "frag");
#else
  shader->loc[GLExoLocFrag] = glGetUniformLocation(shader->prog, "frag");
#endif
}

ISC glExo__renderCreateTexture(void* uptr, ISC type, ISC w, ISC h, ISC imageFlags, const IUA* data);

ISC glExo__renderCreate(void* uptr)
{
  GLExoContext* gl = (GLExoContext*)uptr;
  ISC align = 4;

  // TODO: mediump FPC may not be enough for GLES2 in iOS.
  // see the following discussion: https://github.com/memononen/nanovg/issues/46
  const CHR* shaderHeader =
#if defined NANOVG_GL2
    "#define NANOVG_GL2 1\n"
#elif defined NANOVG_GL3
    "#version 150 core\n"
    "#define NANOVG_GL3 1\n"
#elif defined NANOVG_GLES2
    "#version 100\n"
    "#define NANOVG_GL2 1\n"
#elif defined NANOVG_GLES3
    "#version 300 es\n"
    "#define NANOVG_GL3 1\n"
#endif

#if NANOVG_GL_USE_UNIFORMBUFFER
  "#define USE_UNIFORMBUFFER 1\n"
#else
  "#define UNIFORMARRAY_SIZE 11\n"
#endif
  "\n";

  const CHR* fillVertShader =
    "#ifdef NANOVG_GL3\n"
    "  uniform vec2 viewSize;\n"
    "  in vec2 vertex;\n"
    "  in vec2 tcoord;\n"
    "  out vec2 ftcoord;\n"
    "  out vec2 fpos;\n"
    "#else\n"
    "  uniform vec2 viewSize;\n"
    "  attribute vec2 vertex;\n"
    "  attribute vec2 tcoord;\n"
    "  varying vec2 ftcoord;\n"
    "  varying vec2 fpos;\n"
    "#endif\n"
    "void main(void) {\n"
    "  ftcoord = tcoord;\n"
    "  fpos = vertex;\n"
    "  gl_Position = vec4(2.0*vertex.x/viewSize.x - 1.0, 1.0 - 2.0*vertex.y/viewSize.y, 0, 1);\n"
    "}\n";

  const CHR* fillFragShader =
    "#ifdef GL_ES\n"
    "#if defined(GL_FRAGMENT_PRECISION_HIGH) || defined(NANOVG_GL3)\n"
    " precision highp float;\n"
    "#else\n"
    " precision mediump float;\n"
    "#endif\n"
    "#endif\n"
    "#ifdef NANOVG_GL3\n"
    "#ifdef USE_UNIFORMBUFFER\n"
    "  layout(std140) uniform frag {\n"
    "    mat3 scissorMat;\n"
    "    mat3 paintMat;\n"
    "    vec4 innerCol;\n"
    "    vec4 outerCol;\n"
    "    vec2 scissorExt;\n"
    "    vec2 scissorScale;\n"
    "    vec2 extent;\n"
    "    float radius;\n"
    "    float feather;\n"
    "    float strokeMult;\n"
    "    float strokeThr;\n"
    "    int texType;\n"
    "    int type;\n"
    "  };\n"
    "#else\n" // NANOVG_GL3 && !USE_UNIFORMBUFFER
    "  uniform vec4 frag[UNIFORMARRAY_SIZE];\n"
    "#endif\n"
    "  uniform sampler2D tex;\n"
    "  in vec2 ftcoord;\n"
    "  in vec2 fpos;\n"
    "  out vec4 outColor;\n"
    "#else\n" // !NANOVG_GL3
    "  uniform vec4 frag[UNIFORMARRAY_SIZE];\n"
    "  uniform sampler2D tex;\n"
    "  varying vec2 ftcoord;\n"
    "  varying vec2 fpos;\n"
    "#endif\n"
    "#ifndef USE_UNIFORMBUFFER\n"
    "  #define scissorMat mat3(frag[0].xyz, frag[1].xyz, frag[2].xyz)\n"
    "  #define paintMat mat3(frag[3].xyz, frag[4].xyz, frag[5].xyz)\n"
    "  #define innerCol frag[6]\n"
    "  #define outerCol frag[7]\n"
    "  #define scissorExt frag[8].xy\n"
    "  #define scissorScale frag[8].zw\n"
    "  #define extent frag[9].xy\n"
    "  #define radius frag[9].z\n"
    "  #define feather frag[9].w\n"
    "  #define strokeMult frag[10].x\n"
    "  #define strokeThr frag[10].y\n"
    "  #define texType int(frag[10].z)\n"
    "  #define type int(frag[10].w)\n"
    "#endif\n"
    "\n"
    "float sdroundrect(vec2 pt, vec2 ext, float rad) {\n"
    "  vec2 ext2 = ext - vec2(rad,rad);\n"
    "  vec2 d = abs(pt) - ext2;\n"
    "  return min(max(d.x,d.y),0.0) + length(max(d,0.0)) - rad;\n"
    "}\n"
    "\n"
    "// Scissoring\n"
    "float scissorMask(vec2 p) {\n"
    "  vec2 sc = (abs((scissorMat * vec3(p,1.0)).xy) - scissorExt);\n"
    "  sc = vec2(0.5,0.5) - sc * scissorScale;\n"
    "  return clamp(sc.x,0.0,1.0) * clamp(sc.y,0.0,1.0);\n"
    "}\n"
    "#ifdef EDGE_AA\n"
    "// Stroke - from [0..1] to clipped pyramid, where the slope is 1px.\n"
    "float strokeMask() {\n"
    "  return min(1.0, (1.0-abs(ftcoord.x*2.0-1.0))*strokeMult) * min(1.0, ftcoord.y);\n"
    "}\n"
    "#endif\n"
    "\n"
    "void main(void) {\n"
    "   vec4 result;\n"
    "  float scissor = scissorMask(fpos);\n"
    "#ifdef EDGE_AA\n"
    "  float strokeAlpha = strokeMask();\n"
    "  if (strokeAlpha < strokeThr) discard;\n"
    "#else\n"
    "  float strokeAlpha = 1.0;\n"
    "#endif\n"
    "  if (type == 0) {      // Gradient\n"
    "    // Calculate gradient color using box gradient\n"
    "    vec2 pt = (paintMat * vec3(fpos,1.0)).xy;\n"
    "    float d = clamp((sdroundrect(pt, extent, radius) + feather*0.5) / feather, 0.0, 1.0);\n"
    "    vec4 color = mix(innerCol,outerCol,d);\n"
    "    // Combine alpha\n"
    "    color *= strokeAlpha * scissor;\n"
    "    result = color;\n"
    "  } else if (type == 1) {    // Image\n"
    "    // Calculate color fron texture\n"
    "    vec2 pt = (paintMat * vec3(fpos,1.0)).xy / extent;\n"
    "#ifdef NANOVG_GL3\n"
    "    vec4 color = texture(tex, pt);\n"
    "#else\n"
    "    vec4 color = texture2D(tex, pt);\n"
    "#endif\n"
    "    if (texType == 1) color = vec4(color.xyz*color.w,color.w);"
    "    if (texType == 2) color = vec4(color.x);"
    "    // Apply color tint and alpha.\n"
    "    color *= innerCol;\n"
    "    // Combine alpha\n"
    "    color *= strokeAlpha * scissor;\n"
    "    result = color;\n"
    "  } else if (type == 2) {    // Stencil fill\n"
    "    result = vec4(1,1,1,1);\n"
    "  } else if (type == 3) {    // Textured tris\n"
    "#ifdef NANOVG_GL3\n"
    "    vec4 color = texture(tex, ftcoord);\n"
    "#else\n"
    "    vec4 color = texture2D(tex, ftcoord);\n"
    "#endif\n"
    "    if (texType == 1) color = vec4(color.xyz*color.w,color.w);"
    "    if (texType == 2) color = vec4(color.x);"
    "    color *= scissor;\n"
    "    result = color * innerCol;\n"
    "  }\n"
    "#ifdef NANOVG_GL3\n"
    "  outColor = result;\n"
    "#else\n"
    "  gl_FragColor = result;\n"
    "#endif\n"
    "}\n";

  glExo__checkError(gl, "init");

  if (gl->flags & CFAntialias) {
    if (glExo__createShader(&gl->shader, "shader", shaderHeader, "#define EDGE_AA 1\n", fillVertShader, fillFragShader) == 0)
      return 0;
  } else {
    if (glExo__createShader(&gl->shader, "shader", shaderHeader, nullptr, fillVertShader, fillFragShader) == 0)
      return 0;
  }

  glExo__checkError(gl, "uniform locations");
  glExo__getUniforms(&gl->shader);

  // Create dynamic vertex array
#if defined NANOVG_GL3
  glGenVertexArrays(1, &gl->vertArr);
#endif
  glGenBuffers(1, &gl->vertBuf);

#if NANOVG_GL_USE_UNIFORMBUFFER
  // Create UBOs
  glUniformBlockBinding(gl->shader.prog, gl->shader.loc[GLExoLocFrag], GLExoFragBinding);
  glGenBuffers(1, &gl->fragBuf);
  glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &align);
#endif
  gl->fragSize = sizeof(GLExoFragUniforms) + align - sizeof(GLExoFragUniforms) % align;

  // Some platforms does not allow to have samples to unset textures.
  // Create empty one which is bound when there's no texture specified.
  gl->dummyTex = glExo__renderCreateTexture(gl, TextureAlpha, 1, 1, 0, nullptr);

  glExo__checkError(gl, "create done");

  glFinish();

  return 1;
}

ISC glExo__renderCreateTexture(void* uptr, ISC type, ISC w, ISC h, ISC imageFlags, const IUA* data)
{
  GLExoContext* gl = (GLExoContext*)uptr;
  GLExoTexture* tex = glExo__allocTexture(gl);

  if (tex == nullptr) return 0;

#ifdef NANOVG_GLES2
  // Check for non-power of 2.
  if (glExo__nearestPow2(w) != (IUC)w || glExo__nearestPow2(h) != (IUC)h) {
    // No repeat
    if ((imageFlags & ImageRepeatX) != 0 || (imageFlags & ImageRepeatY) != 0) {
      printf("Repeat X/Y is not supported for non power-of-two textures (%d x %d)\n", w, h);
      imageFlags &= ~(ImageRepeatX | ImageRepeatY);
    }
    // No mips.
    if (imageFlags & ImageGenerateMipMaps) {
      printf("Mip-maps is not support for non power-of-two textures (%d x %d)\n", w, h);
      imageFlags &= ~ImageGenerateMipMaps;
    }
  }
#endif

  glGenTextures(1, &tex->tex);
  tex->width = w;
  tex->height = h;
  tex->type = type;
  tex->flags = imageFlags;
  glExo__bindTexture(gl, tex->tex);

  glPixelStorei(GL_UNPACK_ALIGNMENT,1);
#ifndef NANOVG_GLES2
  glPixelStorei(GL_UNPACK_ROW_LENGTH, tex->width);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
#endif

#if defined (NANOVG_GL2)
  // GL 1.4 and later has support for generating mipmaps using a tex parameter.
  if (imageFlags & ImageGenerateMipMaps) {
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  }
#endif

  if (type == TextureRGBA)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  else
#if defined(NANOVG_GLES2) || defined (NANOVG_GL2)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, w, h, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
#elif defined(NANOVG_GLES3)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, data);
#else
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, data);
#endif

  if (imageFlags & ImageGenerateMipMaps) {
    if (imageFlags & ImageNearest) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    } else {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
  } else {
    if (imageFlags & ImageNearest) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    } else {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
  }

  if (imageFlags & ImageNearest) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  } else {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }

  if (imageFlags & ImageRepeatX)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  else
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

  if (imageFlags & ImageRepeatY)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  else
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
#ifndef NANOVG_GLES2
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
#endif

  // The new way to build mipmaps on GLES and GL3
#if !defined(NANOVG_GL2)
  if (imageFlags & ImageGenerateMipMaps) {
    glGenerateMipmap(GL_TEXTURE_2D);
  }
#endif

  glExo__checkError(gl, "create tex");
  glExo__bindTexture(gl, 0);

  return tex->id;
}


ISC glExo__renderDeleteTexture(void* uptr, ISC image)
{
  GLExoContext* gl = (GLExoContext*)uptr;
  return glExo__deleteTexture(gl, image);
}

ISC glExo__renderUpdateTexture(void* uptr, ISC image, ISC x, ISC y, ISC w, ISC h, const IUA* data)
{
  GLExoContext* gl = (GLExoContext*)uptr;
  GLExoTexture* tex = glExo__findTexture(gl, image);

  if (tex == nullptr) return 0;
  glExo__bindTexture(gl, tex->tex);

  glPixelStorei(GL_UNPACK_ALIGNMENT,1);

#ifndef NANOVG_GLES2
  glPixelStorei(GL_UNPACK_ROW_LENGTH, tex->width);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, x);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, y);
#else
  // No support for all of skip, need to update a whole row at a time.
  if (tex->type == TextureRGBA)
    data += y*tex->width*4;
  else
    data += y*tex->width;
  x = 0;
  w = tex->width;
#endif

  if (tex->type == TextureRGBA)
    glTexSubImage2D(GL_TEXTURE_2D, 0, x,y, w,h, GL_RGBA, GL_UNSIGNED_BYTE, data);
  else
#if defined(NANOVG_GLES2) || defined(NANOVG_GL2)
    glTexSubImage2D(GL_TEXTURE_2D, 0, x,y, w,h, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
#else
    glTexSubImage2D(GL_TEXTURE_2D, 0, x,y, w,h, GL_RED, GL_UNSIGNED_BYTE, data);
#endif

  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
#ifndef NANOVG_GLES2
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
  glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
#endif

  glExo__bindTexture(gl, 0);

  return 1;
}

ISC glExo__renderGetTextureSize(void* uptr, ISC image, ISC* w, ISC* h)
{
  GLExoContext* gl = (GLExoContext*)uptr;
  GLExoTexture* tex = glExo__findTexture(gl, image);
  if (tex == nullptr) return 0;
  *w = tex->width;
  *h = tex->height;
  return 1;
}

void glExo__xformToMat3x4(FPC* m3, FPC* t)
{
  m3[0] = t[0];
  m3[1] = t[1];
  m3[2] = 0.0f;
  m3[3] = 0.0f;
  m3[4] = t[2];
  m3[5] = t[3];
  m3[6] = 0.0f;
  m3[7] = 0.0f;
  m3[8] = t[4];
  m3[9] = t[5];
  m3[10] = 1.0f;
  m3[11] = 0.0f;
}

ColorFPC glExo__premulColor(ColorFPC c)
{
  c.r *= c.a;
  c.g *= c.a;
  c.b *= c.a;
  return c;
}

ISC glExo__convertPaint(GLExoContext* gl, GLExoFragUniforms* frag, VectorPaint* paint,
                 Scissor* scissor, FPC width, FPC fringe, FPC strokeThr)
{
  GLExoTexture* tex = nullptr;
  FPC invxform[6];

  _::ArrayFill(frag, sizeof(*frag));

  frag->innerCol = glExo__premulColor(paint->inner_color);
  frag->outerCol = glExo__premulColor(paint->outer_color);

  if (scissor->extent[0] < -0.5f || scissor->extent[1] < -0.5f) {
    _::ArrayFill(frag->scissorMat, sizeof(frag->scissorMat));
    frag->scissorExt[0] = 1.0f;
    frag->scissorExt[1] = 1.0f;
    frag->scissorScale[0] = 1.0f;
    frag->scissorScale[1] = 1.0f;
  } else {
    ExoTransformInverse(invxform, scissor->xform);
    glExo__xformToMat3x4(frag->scissorMat, invxform);
    frag->scissorExt[0] = scissor->extent[0];
    frag->scissorExt[1] = scissor->extent[1];
    frag->scissorScale[0] = sqrtf(scissor->xform[0]*scissor->xform[0] + scissor->xform[2]*scissor->xform[2]) / fringe;
    frag->scissorScale[1] = sqrtf(scissor->xform[1]*scissor->xform[1] + scissor->xform[3]*scissor->xform[3]) / fringe;
  }

  _::ArrayCopy(frag->extent, sizeof(frag->extent), paint->extent, sizeof(frag->extent));
  frag->strokeMult = (width*0.5f + fringe*0.5f) / fringe;
  frag->strokeThr = strokeThr;

  if (paint->image != 0) {
    tex = glExo__findTexture(gl, paint->image);
    if (tex == nullptr) return 0;
    if ((tex->flags & ImageFlipY) != 0) {
      FPC m1[6], m2[6];
      ExoTransformTranslate(m1, 0.0f, frag->extent[1] * 0.5f);
      ExoTransformMultiply(m1, paint->xform);
      ExoTransformScale(m2, 1.0f, -1.0f);
      ExoTransformMultiply(m2, m1);
      ExoTransformTranslate(m1, 0.0f, -frag->extent[1] * 0.5f);
      ExoTransformMultiply(m1, m2);
      ExoTransformInverse(invxform, m1);
    } else {
      ExoTransformInverse(invxform, paint->xform);
    }
    frag->type = GLExoShaderFillImg;

    #if NANOVG_GL_USE_UNIFORMBUFFER
    if (tex->type == TextureRGBA)
      frag->texType = (tex->flags & ImagePremultiplied) ? 0 : 1;
    else
      frag->texType = 2;
    #else
    if (tex->type == TextureRGBA)
      frag->texType = (tex->flags & ImagePremultiplied) ? 0.0f : 1.0f;
    else
      frag->texType = 2.0f;
    #endif
//    printf("frag->texType = %d\n", frag->texType);
  } else {
    frag->type = GLExoShaderFillGrad;
    frag->radius = paint->radius;
    frag->feather = paint->feather;
    ExoTransformInverse(invxform, paint->xform);
  }

  glExo__xformToMat3x4(frag->paintMat, invxform);

  return 1;
}

GLExoFragUniforms* Exo__fragUniformPtr(GLExoContext* gl, ISC i);

void glExo__setUniforms(GLExoContext* gl, ISC uniformOffset, ISC image)
{
  GLExoTexture* tex = nullptr;
#if NANOVG_GL_USE_UNIFORMBUFFER
  glBindBufferRange(GL_UNIFORM_BUFFER, GLExoFragBinding, gl->fragBuf, uniformOffset, sizeof(GLExoFragUniforms));
#else
  GLExoFragUniforms* frag = Exo__fragUniformPtr(gl, uniformOffset);
  glUniform4fv(gl->shader.loc[GLExoLocFrag], NANOVG_GL_UNIFORMARRAY_SIZE, &(frag->uniformArray[0][0]));
#endif

  if (image != 0) {
    tex = glExo__findTexture(gl, image);
  }
  // If no image is set, use empty texture
  if (tex == nullptr) {
    tex = glExo__findTexture(gl, gl->dummyTex);
  }
  glExo__bindTexture(gl, tex != nullptr ? tex->tex : 0);
  glExo__checkError(gl, "tex paint tex");
}

void glExo__renderViewport(void* uptr, FPC width, FPC height, FPC devicePixelRatio)
{
  _NotUsed(devicePixelRatio);
  GLExoContext* gl = (GLExoContext*)uptr;
  gl->view[0] = width;
  gl->view[1] = height;
}

void glExo__fill(GLExoContext* gl, GLExoCall* call)
{
  GLExoPath* paths = &gl->paths[call->pathOffset];
  ISC i, npaths = call->pathCount;

  // Draw shapes
  glEnable(GL_STENCIL_TEST);
  glExo__stencilMask(gl, 0xff);
  glExo__stencilFunc(gl, GL_ALWAYS, 0, 0xff);
  glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

  // set bindpoint for solid loc
  glExo__setUniforms(gl, call->uniformOffset, 0);
  glExo__checkError(gl, "fill simple");

  glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, GL_INCR_WRAP);
  glStencilOpSeparate(GL_BACK, GL_KEEP, GL_KEEP, GL_DECR_WRAP);
  glDisable(GL_CULL_FACE);
  for (i = 0; i < npaths; i++)
    glDrawArrays(GL_TRIANGLE_FAN, paths[i].fillOffset, paths[i].fillCount);
  glEnable(GL_CULL_FACE);

  // Draw anti-aliased pixels
  glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

  glExo__setUniforms(gl, call->uniformOffset + gl->fragSize, call->image);
  glExo__checkError(gl, "fill fill");

  if (gl->flags & CFAntialias) {
    glExo__stencilFunc(gl, GL_EQUAL, 0x00, 0xff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    // Draw fringes
    for (i = 0; i < npaths; i++)
      glDrawArrays(GL_TRIANGLE_STRIP, paths[i].strokeOffset, paths[i].strokeCount);
  }

  // Draw fill
  glExo__stencilFunc(gl, GL_NOTEQUAL, 0x0, 0xff);
  glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
  glDrawArrays(GL_TRIANGLE_STRIP, call->triangleOffset, call->triangleCount);

  glDisable(GL_STENCIL_TEST);
}

void glExo__convexFill(GLExoContext* gl, GLExoCall* call)
{
  GLExoPath* paths = &gl->paths[call->pathOffset];
  ISC i, npaths = call->pathCount;

  glExo__setUniforms(gl, call->uniformOffset, call->image);
  glExo__checkError(gl, "convex fill");

  for (i = 0; i < npaths; i++) {
    glDrawArrays(GL_TRIANGLE_FAN, paths[i].fillOffset, paths[i].fillCount);
    // Draw fringes
    if (paths[i].strokeCount > 0) {
      glDrawArrays(GL_TRIANGLE_STRIP, paths[i].strokeOffset, paths[i].strokeCount);
    }
  }
}

void glExo__stroke(GLExoContext* gl, GLExoCall* call)
{
  GLExoPath* paths = &gl->paths[call->pathOffset];
  ISC npaths = call->pathCount, i;

  if (gl->flags & CFStencilStrokes) {

    glEnable(GL_STENCIL_TEST);
    glExo__stencilMask(gl, 0xff);

    // Fill the stroke base without overlap
    glExo__stencilFunc(gl, GL_EQUAL, 0x0, 0xff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
    glExo__setUniforms(gl, call->uniformOffset + gl->fragSize, call->image);
    glExo__checkError(gl, "stroke fill 0");
    for (i = 0; i < npaths; i++)
      glDrawArrays(GL_TRIANGLE_STRIP, paths[i].strokeOffset, paths[i].strokeCount);

    // Draw anti-aliased pixels.
    glExo__setUniforms(gl, call->uniformOffset, call->image);
    glExo__stencilFunc(gl, GL_EQUAL, 0x00, 0xff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    for (i = 0; i < npaths; i++)
      glDrawArrays(GL_TRIANGLE_STRIP, paths[i].strokeOffset, paths[i].strokeCount);

    // Clear stencil buffer.
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glExo__stencilFunc(gl, GL_ALWAYS, 0x0, 0xff);
    glStencilOp(GL_ZERO, GL_ZERO, GL_ZERO);
    glExo__checkError(gl, "stroke fill 1");
    for (i = 0; i < npaths; i++)
      glDrawArrays(GL_TRIANGLE_STRIP, paths[i].strokeOffset, paths[i].strokeCount);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glDisable(GL_STENCIL_TEST);

//    glExo__convertPaint(gl, Exo__fragUniformPtr(gl, call->uniformOffset + gl->fragSize), paint, scissor, strokeWidth, fringe, 1.0f - 0.5f/255.0f);

  } else {
    glExo__setUniforms(gl, call->uniformOffset, call->image);
    glExo__checkError(gl, "stroke fill");
    // Draw Strokes
    for (i = 0; i < npaths; i++)
      glDrawArrays(GL_TRIANGLE_STRIP, paths[i].strokeOffset, paths[i].strokeCount);
  }
}

void glExo__triangles(GLExoContext* gl, GLExoCall* call)
{
  glExo__setUniforms(gl, call->uniformOffset, call->image);
  glExo__checkError(gl, "triangles fill");

  glDrawArrays(GL_TRIANGLES, call->triangleOffset, call->triangleCount);
}

void glExo__renderCancel(void* uptr) {
  GLExoContext* gl = (GLExoContext*)uptr;
  gl->nverts = 0;
  gl->npaths = 0;
  gl->ncalls = 0;
  gl->nuniforms = 0;
}

GLenum glExo_convertBlendFuncFactor(ISC factor)
{
  if (factor == BFZero)
    return GL_ZERO;
  if (factor == BFOne)
    return GL_ONE;
  if (factor == BFSourceColor)
    return GL_SRC_COLOR;
  if (factor == BFOneMinusSourceColor)
    return GL_ONE_MINUS_SRC_COLOR;
  if (factor == BFDestinationColor)
    return GL_DST_COLOR;
  if (factor == BFOneMinusDestinationColor)
    return GL_ONE_MINUS_DST_COLOR;
  if (factor == BFSourceAlpha)
    return GL_SRC_ALPHA;
  if (factor == BFOneMinusSourceAlpha)
    return GL_ONE_MINUS_SRC_ALPHA;
  if (factor == BFDestinationAlpha)
    return GL_DST_ALPHA;
  if (factor == BFOneMinusDestinationAlpha)
    return GL_ONE_MINUS_DST_ALPHA;
  if (factor == BFSourceAlphaSaturate)
    return GL_SRC_ALPHA_SATURATE;
  return GL_INVALID_ENUM;
}

GLExoBlend glExo__blendCompositeOperation(CompositeOperationState op)
{
  GLExoBlend blend;
  blend.srcRGB = glExo_convertBlendFuncFactor(op.source_rgb);
  blend.dstRGB = glExo_convertBlendFuncFactor(op.destination_rgb);
  blend.srcAlpha = glExo_convertBlendFuncFactor(op.source_alpha);
  blend.dstAlpha = glExo_convertBlendFuncFactor(op.destination_alpha);
  if (blend.srcRGB == GL_INVALID_ENUM || blend.dstRGB == GL_INVALID_ENUM || blend.srcAlpha == GL_INVALID_ENUM || blend.dstAlpha == GL_INVALID_ENUM)
  {
    blend.srcRGB = GL_ONE;
    blend.dstRGB = GL_ONE_MINUS_SRC_ALPHA;
    blend.srcAlpha = GL_ONE;
    blend.dstAlpha = GL_ONE_MINUS_SRC_ALPHA;
  }
  return blend;
}

void glExo__renderFlush(void* uptr)
{
  GLExoContext* gl = (GLExoContext*)uptr;
  ISC i;

  if (gl->ncalls > 0) {

    // Setup require GL state.
    glUseProgram(gl->shader.prog);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_SCISSOR_TEST);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilMask(0xffffffff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilFunc(GL_ALWAYS, 0, 0xffffffff);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    #if NANOVG_GL_USE_STATE_FILTER
    gl->boundTexture = 0;
    gl->stencilMask = 0xffffffff;
    gl->stencilFunc = GL_ALWAYS;
    gl->stencilFuncRef = 0;
    gl->stencilFuncMask = 0xffffffff;
    gl->blendFunc.srcRGB = GL_INVALID_ENUM;
    gl->blendFunc.srcAlpha = GL_INVALID_ENUM;
    gl->blendFunc.dstRGB = GL_INVALID_ENUM;
    gl->blendFunc.dstAlpha = GL_INVALID_ENUM;
    #endif

#if NANOVG_GL_USE_UNIFORMBUFFER
    // Upload ubo for frag shaders
    glBindBuffer(GL_UNIFORM_BUFFER, gl->fragBuf);
    glBufferData(GL_UNIFORM_BUFFER, gl->nuniforms * gl->fragSize, gl->uniforms, GL_STREAM_DRAW);
#endif

    // Upload vertex data
#if defined NANOVG_GL3
    glBindVertexArray(gl->vertArr);
#endif
    glBindBuffer(GL_ARRAY_BUFFER, gl->vertBuf);
    glBufferData(GL_ARRAY_BUFFER, gl->nverts * sizeof(VertexFPC), gl->verts, GL_STREAM_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFPC), (const GLvoid*)(size_t)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFPC), (const GLvoid*)(0 + 2*sizeof(FPC)));

    // Set view and texture just once per frame.
    glUniform1i(gl->shader.loc[GLExoLocTex], 0);
    glUniform2fv(gl->shader.loc[GLExoLocViewSize], 1, gl->view);

#if NANOVG_GL_USE_UNIFORMBUFFER
    glBindBuffer(GL_UNIFORM_BUFFER, gl->fragBuf);
#endif

    for (i = 0; i < gl->ncalls; i++) {
      GLExoCall* call = &gl->calls[i];
      glExo__blendFuncSeparate(gl,&call->blendFunc);
      if (call->type == GLExoCallFill)
        glExo__fill(gl, call);
      else if (call->type == GLExoCallConvexFill)
        glExo__convexFill(gl, call);
      else if (call->type == GLExoCallStroke)
        glExo__stroke(gl, call);
      else if (call->type == GLExoCallTriangles)
        glExo__triangles(gl, call);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
#if defined NANOVG_GL3
    glBindVertexArray(0);
#endif
    glDisable(GL_CULL_FACE);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
    glUseProgram(0);
    glExo__bindTexture(gl, 0);
  }

  // Reset calls
  gl->nverts = 0;
  gl->npaths = 0;
  gl->ncalls = 0;
  gl->nuniforms = 0;
}

ISC glExo__maxVertCount(const RenderPath* paths, ISC npaths)
{
  ISC i, count = 0;
  for (i = 0; i < npaths; i++) {
    count += paths[i].nfill;
    count += paths[i].nstroke;
  }
  return count;
}

GLExoCall* glExo__allocCall(GLExoContext* gl)
{
  GLExoCall* ret = nullptr;
  if (gl->ncalls+1 > gl->ccalls) {
    GLExoCall* calls;
    ISC ccalls = glExo__maxi(gl->ncalls+1, 128) + gl->ccalls/2; // 1.5x Overallocate
    calls = (GLExoCall*)Exo__realloc(gl->wall, gl->calls,
        sizeof(GLExoCall) * gl->ccalls, sizeof(GLExoCall) * ccalls);
    if (calls == nullptr) return nullptr;
    gl->calls = calls;
    gl->ccalls = ccalls;
  }
  ret = &gl->calls[gl->ncalls++];
  _::ArrayFill(ret, sizeof(GLExoCall));
  return ret;
}

ISC glExo__allocPaths(GLExoContext* gl, ISC n)
{
  ISC ret = 0;
  if (gl->npaths+n > gl->cpaths) {
    GLExoPath* paths;
    ISC cpaths = glExo__maxi(gl->npaths + n, 128) + gl->cpaths/2; // 1.5x Overallocate
    paths = (GLExoPath*)Exo__realloc(gl->wall, gl->paths,
        sizeof(GLExoPath) * gl->cpaths, sizeof(GLExoPath) * cpaths);
    if (paths == nullptr) return -1;
    gl->paths = paths;
    gl->cpaths = cpaths;
  }
  ret = gl->npaths;
  gl->npaths += n;
  return ret;
}

ISC glExo__allocVerts(GLExoContext* gl, ISC n)
{
  ISC ret = 0;
  if (gl->nverts+n > gl->cverts) {
    VertexFPC* verts;
    ISC cverts = glExo__maxi(gl->nverts + n, 4096) + gl->cverts/2; // 1.5x Overallocate
    verts = (VertexFPC*)Exo__realloc(gl->wall, gl->verts,
        sizeof(VertexFPC) * gl->cverts, sizeof(VertexFPC) * cverts);
    if (verts == nullptr) return -1;
    gl->verts = verts;
    gl->cverts = cverts;
  }
  ret = gl->nverts;
  gl->nverts += n;
  return ret;
}

ISC glExo__allocFragUniforms(GLExoContext* gl, ISC n)
{
  ISC ret = 0, structSize = gl->fragSize;
  if (gl->nuniforms+n > gl->cuniforms) {
    CHR* uniforms;
    ISC cuniforms = glExo__maxi(gl->nuniforms+n, 128) + gl->cuniforms/2; // 1.5x Overallocate
    uniforms = (CHR*)Exo__realloc(gl->wall, gl->uniforms,
        structSize * gl->cuniforms, structSize * cuniforms);
    if (uniforms == nullptr) return -1;
    gl->uniforms = uniforms;
    gl->cuniforms = cuniforms;
  }
  ret = gl->nuniforms * structSize;
  gl->nuniforms += n;
  return ret;
}

GLExoFragUniforms* Exo__fragUniformPtr(GLExoContext* gl, ISC i)
{
  return (GLExoFragUniforms*)&gl->uniforms[i];
}

void glExo__vset(VertexFPC* vtx, FPC x, FPC y, FPC u, FPC v)
{
  vtx->x = x;
  vtx->y = y;
  vtx->u = u;
  vtx->v = v;
}

void glExo__renderFill(void* uptr, VectorPaint* paint, CompositeOperationState compositeOperation, Scissor* scissor, FPC fringe,
                const FPC* bounds, const RenderPath* paths, ISC npaths)
{
  GLExoContext* gl = (GLExoContext*)uptr;
  GLExoCall* call = glExo__allocCall(gl);
  VertexFPC* quad;
  GLExoFragUniforms* frag;
  ISC i, maxverts, offset;

  if (call == nullptr) return;

  call->type = GLExoCallFill;
  call->triangleCount = 4;
  call->pathOffset = glExo__allocPaths(gl, npaths);
  if (call->pathOffset == -1) goto error;
  call->pathCount = npaths;
  call->image = paint->image;
  call->blendFunc = glExo__blendCompositeOperation(compositeOperation);

  if (npaths == 1 && paths[0].convex)
  {
    call->type = GLExoCallConvexFill;
    call->triangleCount = 0;  // Bounding box fill quad not needed for convex fill
  }

  // Allocate vertices for all the paths.
  maxverts = glExo__maxVertCount(paths, npaths) + call->triangleCount;
  offset = glExo__allocVerts(gl, maxverts);
  if (offset == -1) goto error;

  for (i = 0; i < npaths; i++) {
    GLExoPath* copy = &gl->paths[call->pathOffset + i];
    const RenderPath* path = &paths[i];
    _::ArrayFill(copy, sizeof(GLExoPath));
    if (path->nfill > 0) {
      copy->fillOffset = offset;
      copy->fillCount = path->nfill;
      _::ArrayCopy(&gl->verts[offset], sizeof(VertexFPC) * path->nfill, path->fill, sizeof(VertexFPC) * path->nfill);
      offset += path->nfill;
    }
    if (path->nstroke > 0) {
      copy->strokeOffset = offset;
      copy->strokeCount = path->nstroke;
      _::ArrayCopy(&gl->verts[offset], sizeof(VertexFPC) * path->nstroke, path->stroke, sizeof(VertexFPC) * path->nstroke);
      offset += path->nstroke;
    }
  }

  // Setup uniforms for draw calls
  if (call->type == GLExoCallFill) {
    // Quad
    call->triangleOffset = offset;
    quad = &gl->verts[call->triangleOffset];
    glExo__vset(&quad[0], bounds[2], bounds[3], 0.5f, 1.0f);
    glExo__vset(&quad[1], bounds[2], bounds[1], 0.5f, 1.0f);
    glExo__vset(&quad[2], bounds[0], bounds[3], 0.5f, 1.0f);
    glExo__vset(&quad[3], bounds[0], bounds[1], 0.5f, 1.0f);

    call->uniformOffset = glExo__allocFragUniforms(gl, 2);
    if (call->uniformOffset == -1) goto error;
    // Simple shader for stencil
    frag = Exo__fragUniformPtr(gl, call->uniformOffset);
    _::ArrayFill(frag, sizeof(*frag));
    frag->strokeThr = -1.0f;
    frag->type = GLExoShaderSimple;
    // Fill shader
    glExo__convertPaint(gl, Exo__fragUniformPtr(gl, call->uniformOffset + gl->fragSize), paint, scissor, fringe, fringe, -1.0f);
  } else {
    call->uniformOffset = glExo__allocFragUniforms(gl, 1);
    if (call->uniformOffset == -1) goto error;
    // Fill shader
    glExo__convertPaint(gl, Exo__fragUniformPtr(gl, call->uniformOffset), paint, scissor, fringe, fringe, -1.0f);
  }

  return;

error:
  // We get here if call alloc was ok, but something else is not.
  // Roll back the last call to prevent drawing it.
  if (gl->ncalls > 0) gl->ncalls--;
}

void glExo__renderStroke(void* uptr, VectorPaint* paint, CompositeOperationState compositeOperation, Scissor* scissor, FPC fringe,
                FPC strokeWidth, const RenderPath* paths, ISC npaths)
{
  GLExoContext* gl = (GLExoContext*)uptr;
  GLExoCall* call = glExo__allocCall(gl);
  ISC i, maxverts, offset;

  if (call == nullptr) return;

  call->type = GLExoCallStroke;
  call->pathOffset = glExo__allocPaths(gl, npaths);
  if (call->pathOffset == -1) goto error;
  call->pathCount = npaths;
  call->image = paint->image;
  call->blendFunc = glExo__blendCompositeOperation(compositeOperation);

  // Allocate vertices for all the paths.
  maxverts = glExo__maxVertCount(paths, npaths);
  offset = glExo__allocVerts(gl, maxverts);
  if (offset == -1) goto error;

  for (i = 0; i < npaths; i++) {
    GLExoPath* copy = &gl->paths[call->pathOffset + i];
    const RenderPath* path = &paths[i];
    _::ArrayFill(copy, sizeof(GLExoPath));
    if (path->nstroke) {
      copy->strokeOffset = offset;
      copy->strokeCount = path->nstroke;
      _::ArrayCopy(&gl->verts[offset], sizeof(VertexFPC) * path->nstroke, path->stroke, sizeof(VertexFPC) * path->nstroke);
      offset += path->nstroke;
    }
  }

  if (gl->flags & CFStencilStrokes) {
    // Fill shader
    call->uniformOffset = glExo__allocFragUniforms(gl, 2);
    if (call->uniformOffset == -1) goto error;

    glExo__convertPaint(gl, Exo__fragUniformPtr(gl, call->uniformOffset), paint, scissor, strokeWidth, fringe, -1.0f);
    glExo__convertPaint(gl, Exo__fragUniformPtr(gl, call->uniformOffset + gl->fragSize), paint, scissor, strokeWidth, fringe, 1.0f - 0.5f/255.0f);

  } else {
    // Fill shader
    call->uniformOffset = glExo__allocFragUniforms(gl, 1);
    if (call->uniformOffset == -1) goto error;
    glExo__convertPaint(gl, Exo__fragUniformPtr(gl, call->uniformOffset), paint, scissor, strokeWidth, fringe, -1.0f);
  }

  return;

error:
  // We get here if call alloc was ok, but something else is not.
  // Roll back the last call to prevent drawing it.
  if (gl->ncalls > 0) gl->ncalls--;
}

void glExo__renderTriangles(void* uptr, VectorPaint* paint, CompositeOperationState compositeOperation, Scissor* scissor,
                   const VertexFPC* verts, ISC nverts, FPC fringe)
{
  GLExoContext* gl = (GLExoContext*)uptr;
  GLExoCall* call = glExo__allocCall(gl);
  GLExoFragUniforms* frag;

  if (call == nullptr) return;

  call->type = GLExoCallTriangles;
  call->image = paint->image;
  call->blendFunc = glExo__blendCompositeOperation(compositeOperation);

  // Allocate vertices for all the paths.
  call->triangleOffset = glExo__allocVerts(gl, nverts);
  if (call->triangleOffset == -1) goto error;
  call->triangleCount = nverts;

  _::ArrayCopy(&gl->verts[call->triangleOffset], sizeof(VertexFPC) * nverts, verts, sizeof(VertexFPC) * nverts);

  // Fill shader
  call->uniformOffset = glExo__allocFragUniforms(gl, 1);
  if (call->uniformOffset == -1) goto error;
  frag = Exo__fragUniformPtr(gl, call->uniformOffset);
  glExo__convertPaint(gl, frag, paint, scissor, 1.0f, fringe, -1.0f);
  frag->type = GLExoShaderImg;

  return;

error:
  // We get here if call alloc was ok, but something else is not.
  // Roll back the last call to prevent drawing it.
  if (gl->ncalls > 0) gl->ncalls--;
}

void glExo__renderDelete(void* uptr)
{
  GLExoContext* gl = (GLExoContext*)uptr;
  ISC i;
  if (gl == nullptr) return;

  glExo__deleteShader(&gl->shader);

#if NANOVG_GL3
#if NANOVG_GL_USE_UNIFORMBUFFER
  if (gl->fragBuf != 0)
    glDeleteBuffers(1, &gl->fragBuf);
#endif
  if (gl->vertArr != 0)
    glDeleteVertexArrays(1, &gl->vertArr);
#endif
  if (gl->vertBuf != 0)
    glDeleteBuffers(1, &gl->vertBuf);

  for (i = 0; i < gl->ntextures; i++) {
    if (gl->textures[i].tex != 0 && (gl->textures[i].flags & ImageNoDelete) == 0)
      glDeleteTextures(1, &gl->textures[i].tex);
  }
  // CPU blocks belong to the caller's Wall, including superseded arrays.
}


#if defined NANOVG_GL2
RenderContext* ExoCreateGL2(ISC flags, _::TWall<>* wall)
#elif defined NANOVG_GL3
RenderContext* ExoCreateGL3(ISC flags, _::TWall<>* wall)
#elif defined NANOVG_GLES2
RenderContext* ExoCreateGLES2(ISC flags, _::TWall<>* wall)
#elif defined NANOVG_GLES3
RenderContext* ExoCreateGLES3(ISC flags, _::TWall<>* wall)
#endif
{
  RenderParams params;
  RenderContext* context = nullptr;
  GLExoContext* gl = (GLExoContext*)Exo__alloc(wall, sizeof(GLExoContext));
  if (gl == nullptr) goto error;
  _::ArrayFill(gl, sizeof(GLExoContext));
  gl->wall = wall;

  _::ArrayFill(&params, sizeof(params));
  params.renderCreate = glExo__renderCreate;
  params.renderCreateTexture = glExo__renderCreateTexture;
  params.renderDeleteTexture = glExo__renderDeleteTexture;
  params.renderUpdateTexture = glExo__renderUpdateTexture;
  params.renderGetTextureSize = glExo__renderGetTextureSize;
  params.renderViewport = glExo__renderViewport;
  params.renderCancel = glExo__renderCancel;
  params.renderFlush = glExo__renderFlush;
  params.renderFill = glExo__renderFill;
  params.renderStroke = glExo__renderStroke;
  params.renderTriangles = glExo__renderTriangles;
  params.renderDelete = glExo__renderDelete;
  params.userPtr = gl;
  params.edgeAntiAlias = flags & CFAntialias ? 1 : 0;

  gl->flags = flags;

  context = ExoCreateInternal(&params, wall);
  if (context == nullptr) goto error;

  return context;

error:
  // ExoCreateInternal cleans up GL resources on failure; Wall owns CPU blocks.
  if (context != nullptr) ExoDeleteInternal(context);
  return nullptr;
}

#if defined NANOVG_GL2
void ExoDeleteGL2(RenderContext* context)
#elif defined NANOVG_GL3
void ExoDeleteGL3(RenderContext* context)
#elif defined NANOVG_GLES2
void ExoDeleteGLES2(RenderContext* context)
#elif defined NANOVG_GLES3
void ExoDeleteGLES3(RenderContext* context)
#endif
{
  ExoDeleteInternal(context);
}

#if defined NANOVG_GL2
ISC ExolCreateImageFromHandleGL2(RenderContext* context, GLuint textureId, ISC w, ISC h, ISC imageFlags)
#elif defined NANOVG_GL3
ISC ExolCreateImageFromHandleGL3(RenderContext* context, GLuint textureId, ISC w, ISC h, ISC imageFlags)
#elif defined NANOVG_GLES2
ISC ExolCreateImageFromHandleGLES2(RenderContext* context, GLuint textureId, ISC w, ISC h, ISC imageFlags)
#elif defined NANOVG_GLES3
ISC ExolCreateImageFromHandleGLES3(RenderContext* context, GLuint textureId, ISC w, ISC h, ISC imageFlags)
#endif
{
  GLExoContext* gl = (GLExoContext*)ExoInternalParams(context)->userPtr;
  GLExoTexture* tex = glExo__allocTexture(gl);

  if (tex == nullptr) return 0;

  tex->type = TextureRGBA;
  tex->tex = textureId;
  tex->flags = imageFlags;
  tex->width = w;
  tex->height = h;

  return tex->id;
}

#if defined NANOVG_GL2
GLuint ExolImageHandleGL2(RenderContext* context, ISC image)
#elif defined NANOVG_GL3
GLuint ExolImageHandleGL3(RenderContext* context, ISC image)
#elif defined NANOVG_GLES2
GLuint ExolImageHandleGLES2(RenderContext* context, ISC image)
#elif defined NANOVG_GLES3
GLuint ExolImageHandleGLES3(RenderContext* context, ISC image)
#endif
{
  GLExoContext* gl = (GLExoContext*)ExoInternalParams(context)->userPtr;
  GLExoTexture* tex = glExo__findTexture(gl, image);
  return tex->tex;
}

#endif /* NANOVG_GL_IMPLEMENTATION */
