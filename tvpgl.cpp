// Copied tvpgl functions, because we can't rely on the core to have them.

#include "tvpgl.h"
#include <simde/x86/mmx.h>
#include <simde/x86/sse.h>

#include "DivTable.h"

static tjs_uint64 c_0000ffffffffffff = 0x0000ffffffffffffull;
static tjs_uint64 c_0100000000000000 = 0x0100000000000000ull;

TVP_GL_FUNC_DECL(void, TVPDoBoxBlurAvg32_d_c, (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len))
{
	simde__m64  v6;  // mm7
	simde__m64  v7;  // mm7
	int         v8;  // esi
	tjs_uint32 *v9;  // edi
	simde__m64  v10; // mm1
	simde__m64  v11; // mm2
	simde__m64 *v12; // ebp
	simde__m64  v13; // mm3
	simde__m64  v14; // mm4
	int         v15; // eax
	int         v16; // ebx
	tjs_uint32 *v17; // [esp+0h] [ebp-20h]

	if (len > 0)
	{
		v6  = simde_mm_cvtsi32_si64((unsigned int)n >> 1);
		v7  = simde_m_punpckldq(v6, v6);
		v8  = 0x100000000ll / n;
		v9  = dest;
		v10 = *(simde__m64 *)sum;
		v11 = *((simde__m64 *)sum + 1);
		v12 = (simde__m64 *)sub;
		v17 = &dest[len];
		if (dest < v17)
		{
			do
			{
				v13 = simde_m_paddd(v10, v7);
				v14 = simde_m_paddd(v11, v7);
				v15 = simde_mm_cvtsi64_si32(simde_m_psrlqi(v14, 0x20u));
				v16 = (tjs_uint64)(v8 * (tjs_int64)v15) >> 32 << 8;
				*v9 = (TVPDivTable[v16 + ((tjs_uint64)(v8 * (tjs_int64)simde_mm_cvtsi64_si32(v14)) >> 32)] << 16) + (TVPDivTable[v16 + ((tjs_uint64)(v8 * (tjs_int64)simde_mm_cvtsi64_si32(simde_m_psrlqi(v13, 0x20u))) >> 32)] << 8) + TVPDivTable[v16 + ((tjs_uint64)(v8 * (tjs_int64)simde_mm_cvtsi64_si32(v13)) >> 32)] + ((tjs_uint64)(v8 * (tjs_int64)v15) >> 32 << 24);
				v10 = simde_m_paddd(simde_m_psubd(v10, *v12), *(simde__m64 *)add);
				v11 = simde_m_paddd(simde_m_psubd(v11, v12[1]), *((simde__m64 *)add + 1));
				add += 4;
				v12 += 2;
				++v9;
			} while (v9 < v17);
		}
		*(simde__m64 *)sum       = v10;
		*((simde__m64 *)sum + 1) = v11;
	}
	simde_m_empty();
}

TVP_GL_FUNC_DECL(void, TVPDoBoxBlurAvg32_c, (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len))
{
	simde__m64  v6;  // mm7
	simde__m64  v7;  // mm7
	int         v8;  // esi
	tjs_uint32 *v9;  // edi
	simde__m64  v10; // mm1
	simde__m64  v11; // mm2
	simde__m64      *v12; // ebx
	simde__m64      *v13; // ebp
	simde__m64  v14; // mm3
	simde__m64  v15; // mm4
	tjs_uint32 *v16; // [esp+0h] [ebp-20h]

	if (len > 0)
	{
		v6  = simde_mm_cvtsi32_si64((unsigned int)n >> 1);
		v7  = simde_m_punpckldq(v6, v6);
		v9  = dest;
		v10 = *(simde__m64 *)sum;
		v11 = *((simde__m64 *)sum + 1);
		v12 = (simde__m64 *)add;
		v13 = (simde__m64 *)sub;
		v16 = &dest[len];
		if (dest < v16)
		{
			do
			{
				v14 = simde_m_paddd(v10, v7);
				v15 = simde_m_paddd(v11, v7);
				v8  = 0x100000000ll / n;
				*v9 = ((tjs_uint64)(v8 * (tjs_int64)simde_mm_cvtsi64_si32(simde_m_psrlqi(v15, 0x20u))) >> 32 << 24) + ((tjs_uint64)(v8 * (tjs_int64)simde_mm_cvtsi64_si32(v15)) >> 32 << 16) + ((tjs_uint64)(v8 * (tjs_int64)simde_mm_cvtsi64_si32(simde_m_psrlqi(v14, 0x20u))) >> 32 << 8) + ((tjs_uint64)(v8 * (tjs_int64)simde_mm_cvtsi64_si32(v14)) >> 32);
				v10 = simde_m_psubd(simde_m_paddd(v10, *v12), *v13);
				v11 = simde_m_psubd(simde_m_paddd(v11, v12[1]), v13[1]);
				v12 += 2;
				v13 += 2;
				++v9;
			} while (v9 < v16);
		}
		*(simde__m64 *)sum       = v10;
		*((simde__m64 *)sum + 1) = v11;
	}
	simde_m_empty();
}

TVP_GL_FUNC_DECL(void, TVPDoBoxBlurAvg16_d_c, (tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len))
{
	simde__m64  v7;  // mm7
	simde__m64  v8;  // mm7
	simde__m64  v10; // mm6
	simde__m64  v11; // mm6
	tjs_uint32 *v12; // edi
	simde__m64  v13; // mm5
	simde__m64 *v14; // ebx
	simde__m64 *v15; // ebp
	tjs_uint32 *v16; // esi
	simde__m64  v17; // mm1
	tjs_uint8 * v18; // ecx

	if (len > 0)
	{
		v7  = simde_mm_set1_pi16((unsigned int)n >> 1);
		v8  = v7;
		v10 = simde_mm_set1_pi16(0x10000 / n);
		v11 = v10;
		v12 = dest;
		v13 = *(simde__m64 *)sum;
		v14 = (simde__m64 *)add;
		v15 = (simde__m64 *)sub;
		v16 = &dest[len];
		if (dest < v16)
		{
			do
			{
				v17  = simde_m_packuswb(simde_m_pmulhuw(simde_m_paddw(v13, v8), v11), simde_mm_setzero_si64());
				v18  = &TVPDivTable[((unsigned int)simde_mm_cvtsi64_si32(v17) >> 16) & 0xFF00];
				*v12 = (v18[((unsigned int)simde_mm_cvtsi64_si32(v17) >> 16) & 0xFF] << 16) + (v18[(tjs_uint16)simde_mm_cvtsi64_si32(v17) >> 8] << 8) + v18[(tjs_uint8)simde_mm_cvtsi64_si32(v17)] + (simde_mm_cvtsi64_si32(v17) & 0xFF000000);
				v13  = simde_m_psubw(simde_m_paddw(v13, *v14), *v15);
				++v14;
				++v15;
				++v12;
			} while (v12 < v16);
		}
		*(simde__m64 *)sum = v13;
	}
	simde_m_empty();
}

TVP_GL_FUNC_DECL(void, TVPDoBoxBlurAvg16_c, (tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len))
{
	simde__m64  v7;  // mm7
	simde__m64  v8;  // mm7
	simde__m64  v10; // mm6
	simde__m64  v11; // mm6
	tjs_uint32 *v12; // edi
	simde__m64  v13; // mm5
	simde__m64 *v14; // ebx
	simde__m64 *v15; // ebp
	tjs_uint32 *v16; // esi
	simde__m64  v17; // mm1

	if (len > 0)
	{
		v7  = simde_mm_set1_pi16((unsigned int)n >> 1);
		v8  = v7;
		v10 = simde_mm_set1_pi16(0x10000 / n);
		v11 = v10;
		v12 = dest;
		v13 = *(simde__m64 *)sum;
		v14 = (simde__m64 *)add;
		v15 = (simde__m64 *)sub;
		v16 = &dest[len];
		if (dest < v16)
		{
			do
			{
				v17  = simde_m_pmulhuw(simde_m_paddw(v13, v8), v11);
				*v12 = simde_mm_cvtsi64_si32(simde_m_packuswb(v17, v17));
				v13  = simde_m_psubw(simde_m_paddw(v13, *v14), *v15);
				++v14;
				++v15;
				++v12;
			} while (v12 < v16);
		}
		*(simde__m64 *)sum = v13;
	}
	simde_m_empty();
}

TVP_GL_FUNC_DECL(void, TVPAddSubVertSum32_d_c, (tjs_uint32 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len))
{
	simde__m64   *v4;  // edi
	tjs_uint32   *v5;  // eax
	tjs_uint32   *v6;  // ebx
	simde__m64    v7;  // mm3
	simde__m64    v8;  // mm4
	simde__m64    v9;  // mm5
	simde__m64    v10; // mm6
	simde__m64    v11; // mm5
	simde__m64    v12; // mm6
	simde__m64    v13; // mm3
	simde__m64    v14; // mm4
	simde__m64    v15; // mm2

	if (len > 0)
	{
		v4 = (simde__m64 *)dest;
		v5 = (tjs_uint32   *)addline;
		v6 = (tjs_uint32   *)subline;
		if (dest < &dest[4 * len])
		{
			do
			{
				v7  = simde_m_punpcklbw(simde_mm_cvtsi32_si64(*v5), simde_mm_setzero_si64());
				v8  = simde_m_punpcklbw(simde_mm_cvtsi32_si64(*v6), simde_mm_setzero_si64());
				v9  = simde_m_psrlqi(simde_m_paddw(simde_m_psrlwi(v7, 7u), v7), 0x30u);
				v10 = simde_m_psrlqi(simde_m_paddw(simde_m_psrlwi(v8, 7u), v8), 0x30u);
				v11 = simde_mm_set1_pi16((tjs_uint16)simde_mm_cvtsi64_si32(v9));
				v12 = simde_mm_set1_pi16((tjs_uint16)simde_mm_cvtsi64_si32(v10));
				v13 = simde_m_psrlwi(simde_m_pmullw(v7, simde_m_por(simde_m_pand(v11, (simde__m64)c_0000ffffffffffff), (simde__m64)c_0100000000000000)), 8u);
				v14 = simde_m_psrlwi(simde_m_pmullw(v8, simde_m_por(simde_m_pand(v12, (simde__m64)c_0000ffffffffffff), (simde__m64)c_0100000000000000)), 8u);
				v15 = simde_m_psubd(simde_m_paddd(v4[1], simde_m_punpckhwd(v13, simde_mm_setzero_si64())), simde_m_punpckhwd(v14, simde_mm_setzero_si64()));
				*v4 = simde_m_psubd(
					simde_m_paddd(*v4, simde_m_punpcklwd(v13, simde_mm_setzero_si64())),
					simde_m_punpcklwd(v14, simde_mm_setzero_si64()));
				v4[1] = v15;
				v4 += 2;
				++v5;
				++v6;
			} while (v4 < (simde__m64 *)&dest[4 * len]);
		}
	}
	simde_m_empty();
}

TVP_GL_FUNC_DECL(void, TVPAddSubVertSum32_c, (tjs_uint32 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len))
{
	simde__m64   *v4; // edi
	tjs_uint32   *v5; // eax
	tjs_uint32   *v6; // ebx
	simde__m64    v7; // mm3
	simde__m64    v8; // mm4
	simde__m64    v9; // mm2

	if (len > 0)
	{
		v4 = (simde__m64 *)dest;
		v5 = (tjs_uint32   *)addline;
		v6 = (tjs_uint32   *)subline;
		if (dest < &dest[4 * len])
		{
			do
			{
				v7  = simde_m_punpcklbw(simde_mm_cvtsi32_si64(*v5), simde_mm_setzero_si64());
				v8  = simde_m_punpcklbw(simde_mm_cvtsi32_si64(*v6), simde_mm_setzero_si64());
				v9  = simde_m_psubd(simde_m_paddd(v4[1], simde_m_punpckhwd(v7, simde_mm_setzero_si64())), simde_m_punpckhwd(v8, simde_mm_setzero_si64()));
				*v4 = simde_m_psubd(
					simde_m_paddd(*v4, simde_m_punpcklwd(v7, simde_mm_setzero_si64())),
					simde_m_punpcklwd(v8, simde_mm_setzero_si64()));
				v4[1] = v9;
				v4 += 2;
				++v5;
				++v6;
			} while (v4 < (simde__m64 *)&dest[4 * len]);
		}
	}
	simde_m_empty();
}

TVP_GL_FUNC_DECL(void, TVPAddSubVertSum16_d_c, (tjs_uint16 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len))
{
	simde__m64   *v4;  // edi
	tjs_uint32   *v5;  // eax
	tjs_uint32   *v6;  // ebx
	simde__m64    v7;  // mm3
	simde__m64    v8;  // mm4
	simde__m64    v9;  // mm5
	simde__m64    v10; // mm6
	simde__m64    v11; // mm5
	simde__m64    v12; // mm6

	if (len > 0)
	{
		v4 = (simde__m64 *)dest;
		v5 = (tjs_uint32   *)addline;
		v6 = (tjs_uint32   *)subline;
		if (dest < &dest[4 * len])
		{
			do
			{
				v7  = simde_m_punpcklbw(simde_mm_cvtsi32_si64(*v5), simde_mm_setzero_si64());
				v8  = simde_m_punpcklbw(simde_mm_cvtsi32_si64(*v6), simde_mm_setzero_si64());
				v9  = simde_m_psrlqi(simde_m_paddw(simde_m_psrlwi(v7, 7u), v7), 0x30u);
				v10 = simde_m_psrlqi(simde_m_paddw(simde_m_psrlwi(v8, 7u), v8), 0x30u);
				v11 = simde_mm_set1_pi16((tjs_uint16)simde_mm_cvtsi64_si32(v9));
				v12 = simde_mm_set1_pi16((tjs_uint16)simde_mm_cvtsi64_si32(v10));
				*v4 = simde_m_psubd(
					simde_m_paddd(
						*v4,
						simde_m_psrlwi(
							simde_m_pmullw(
								v7,
								simde_m_por(
									simde_m_pand(v11, (simde__m64)c_0000ffffffffffff),
									(simde__m64)c_0100000000000000)),
							8u)),
					simde_m_psrlwi(
						simde_m_pmullw(
							v8,
							simde_m_por(simde_m_pand(v12, (simde__m64)c_0000ffffffffffff), (simde__m64)c_0100000000000000)),
						8u));
				++v4;
				++v5;
				++v6;
			} while (v4 < (simde__m64 *)&dest[4 * len]);
		}
	}
	simde_m_empty();
}

TVP_GL_FUNC_DECL(void, TVPAddSubVertSum16_c, (tjs_uint16 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len))
{
	simde__m64   *v4; // edi
	tjs_uint32   *v5; // eax
	tjs_uint32   *v6; // ebx

	if (len > 0)
	{
		v4 = (simde__m64 *)dest;
		v5 = (tjs_uint32   *)addline;
		v6 = (tjs_uint32   *)subline;
		if (dest < &dest[4 * len])
		{
			do
			{
				*v4 = simde_m_psubd(
					simde_m_paddd(*v4, simde_m_punpcklbw(simde_mm_cvtsi32_si64(*v5), simde_mm_setzero_si64())),
					simde_m_punpcklbw(simde_mm_cvtsi32_si64(*v6), simde_mm_setzero_si64()));
				++v4;
				++v5;
				++v6;
			} while (v4 < (simde__m64 *)&dest[4 * len]);
		}
	}
	simde_m_empty();
}
