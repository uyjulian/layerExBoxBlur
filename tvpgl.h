
#pragma once

#include "tjsCommHead.h"

#define TVPDoBoxBlurAvg32_d TVPDoBoxBlurAvg32_d_c
#define TVPDoBoxBlurAvg32 TVPDoBoxBlurAvg32_c
#define TVPDoBoxBlurAvg16_d TVPDoBoxBlurAvg16_d_c
#define TVPDoBoxBlurAvg16 TVPDoBoxBlurAvg16_c
#define TVPAddSubVertSum32_d TVPAddSubVertSum32_d_c
#define TVPAddSubVertSum32 TVPAddSubVertSum32_c
#define TVPAddSubVertSum16_d TVPAddSubVertSum16_d_c
#define TVPAddSubVertSum16 TVPAddSubVertSum16_c

#define TVP_GL_FUNCNAME(funcname) funcname
#define TVP_GL_FUNC_DECL(rettype, funcname, arg)  rettype TVP_GL_FUNCNAME(funcname) arg
#define TVP_GL_FUNC_STATIC_DECL(rettype, funcname, arg)  static rettype TVP_GL_FUNCNAME(funcname) arg
#define TVP_GL_FUNC_EXTERN_DECL(rettype, funcname, arg)  extern rettype TVP_GL_FUNCNAME(funcname) arg
#define TVP_GL_FUNC_PTR_EXTERN_DECL TVP_GL_FUNC_EXTERN_DECL

TVP_GL_FUNC_PTR_EXTERN_DECL(void, TVPDoBoxBlurAvg32_d,  (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len));
TVP_GL_FUNC_PTR_EXTERN_DECL(void, TVPDoBoxBlurAvg32,  (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len));
TVP_GL_FUNC_PTR_EXTERN_DECL(void, TVPDoBoxBlurAvg16_d,  (tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len));
TVP_GL_FUNC_PTR_EXTERN_DECL(void, TVPDoBoxBlurAvg16,  (tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len));
TVP_GL_FUNC_PTR_EXTERN_DECL(void, TVPAddSubVertSum32_d,  (tjs_uint32 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len));
TVP_GL_FUNC_PTR_EXTERN_DECL(void, TVPAddSubVertSum32,  (tjs_uint32 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len));
TVP_GL_FUNC_PTR_EXTERN_DECL(void, TVPAddSubVertSum16_d,  (tjs_uint16 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len));
TVP_GL_FUNC_PTR_EXTERN_DECL(void, TVPAddSubVertSum16,  (tjs_uint16 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len));
