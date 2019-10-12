#ifndef __SHADERS_H__
#define __SHADERS_H__

/* 
 * shaders sources
 */
 
/* default libfreetype-gl vertex shader */
static const GLchar s_vertex_shader_code[] =
   "precision mediump float; \
    uniform mat4 model; \
    uniform mat4 view; \
    uniform mat4 projection; \
    \
    attribute vec3 vertex; \
    attribute vec2 tex_coord; \
    attribute vec4 color; \
    \
    varying vec2 vTexCoord; \
    varying vec4 fragColor; \
    \
    void main() \
    { \
        vTexCoord.xy = tex_coord.xy; \
        fragColor    = color; \
        gl_Position  = projection*(view*(model*vec4(vertex,1.0))); \
    }";

/* default libfreetype-gl fragment shader */
static const GLchar s_fragment_shader_code[] =
   "precision mediump float; \
    uniform sampler2D texture; \
    \
    varying vec2 vTexCoord; \
    varying vec4 fragColor; \
    \
    void main() \
    { \
        float a = texture2D(texture, vTexCoord).a; \
        gl_FragColor = vec4(fragColor.rgb, fragColor.a*a); \
    }";

#endif /* SHADERS_H */
