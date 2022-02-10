// Copied tvpgl functions, because we can't rely on the core to have them.

#include "tvpgl.h"
#include "DivTable.h"

TVP_GL_FUNC_DECL(void, TVPDoBoxBlurAvg32_d_c, (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len))
{
	/* This function is very slow since using divisiion in loop. Function written in assembly should be used. */
	tjs_int half_n = n >> 1u;
	{
		for(int ___index = 0; ___index < len; ___index++)
		{
			{
				tjs_int a = ((sum[3u] + half_n) / n);
				tjs_uint8 * t = TVPDivTable + (a << 8u);
				dest[___index] =
					(t[(sum[0u] + half_n) / n]       )+
					(t[(sum[1u] + half_n) / n] << 8u  )+
					(t[(sum[2u] + half_n) / n] << 16u )+
					(a << 24u );

				sum[0u] += add[___index*4u+0u] - sub[___index*4u+0u];
				sum[1u] += add[___index*4u+1u] - sub[___index*4u+1u];
				sum[2u] += add[___index*4u+2u] - sub[___index*4u+2u];
				sum[3u] += add[___index*4u+3u] - sub[___index*4u+3u];
			}
		}
	}
}

TVP_GL_FUNC_DECL(void, TVPDoBoxBlurAvg32_c, (tjs_uint32 *dest, tjs_uint32 *sum, const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len))
{
	/* This function is very slow since using divisiion in loop. Function written in assembly should be used. */
	tjs_int half_n = n >> 1u;
	{
		for(int ___index = 0; ___index < len; ___index++)
		{
			{
				dest[___index] =
					(((sum[0u] + half_n) / n)       )+
					(((sum[1u] + half_n) / n) << 8u  )+
					(((sum[2u] + half_n) / n) << 16u )+
					(((sum[3u] + half_n) / n) << 24u );

				sum[0u] += add[___index*4u+0u] - sub[___index*4u+0u];
				sum[1u] += add[___index*4u+1u] - sub[___index*4u+1u];
				sum[2u] += add[___index*4u+2u] - sub[___index*4u+2u];
				sum[3u] += add[___index*4u+3u] - sub[___index*4u+3u];
			}
		}
	}
}

TVP_GL_FUNC_DECL(void, TVPDoBoxBlurAvg16_d_c, (tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len))
{
	tjs_int rcp = (1u<<16u) / n;
	tjs_int half_n = n >> 1u;
	{
		for(int ___index = 0; ___index < len; ___index++)
		{
			{
				tjs_int a = ((sum[3u] + half_n) * rcp >> 16u);
				tjs_uint8 * t = TVPDivTable + (a << 8u);
				dest[___index] =
					(t[(sum[0u] + half_n) * rcp >> 16u]       )+
					(t[(sum[1u] + half_n) * rcp >> 16u] << 8u  )+
					(t[(sum[2u] + half_n) * rcp >> 16u] << 16u )+
					(a << 24u );

				sum[0u] += add[___index*4u+0u] - sub[___index*4u+0u];
				sum[1u] += add[___index*4u+1u] - sub[___index*4u+1u];
				sum[2u] += add[___index*4u+2u] - sub[___index*4u+2u];
				sum[3u] += add[___index*4u+3u] - sub[___index*4u+3u];
			}
		}
	}
}

TVP_GL_FUNC_DECL(void, TVPDoBoxBlurAvg16_c, (tjs_uint32 *dest, tjs_uint16 *sum, const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len))
{
	tjs_int rcp = (1u<<16u) / n;
	tjs_int half_n = n >> 1u;
	{
		for(int ___index = 0; ___index < len; ___index++)
		{
			{
				dest[___index] =
					(((sum[0u] + half_n) * rcp >> 16u)       )+
					(((sum[1u] + half_n) * rcp >> 16u) << 8u  )+
					(((sum[2u] + half_n) * rcp >> 16u) << 16u )+
					(((sum[3u] + half_n) * rcp >> 16u) << 24u );

				sum[0u] += add[___index*4u+0u] - sub[___index*4u+0u];
				sum[1u] += add[___index*4u+1u] - sub[___index*4u+1u];
				sum[2u] += add[___index*4u+2u] - sub[___index*4u+2u];
				sum[3u] += add[___index*4u+3u] - sub[___index*4u+3u];
			}
		}
	}
}

TVP_GL_FUNC_DECL(void, TVPAddSubVertSum32_d_c, (tjs_uint32 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len))
{
	{
		for(int ___index = 0; ___index < len; ___index++)
		{
			{
				tjs_uint32 add, sub;
				tjs_int add_a, sub_a;
				add = addline[___index];
				sub = subline[___index];
				dest[___index*4u+3u] += (add_a = (add>>24u)       ) - (sub_a = (sub>>24u)       );
				add_a += add_a >> 7u;
				sub_a += sub_a >> 7u;
				dest[___index*4u+0u] += (((add    ) & 0xffu) * add_a >> 8u) - (((sub    ) & 0xffu) * sub_a >> 8u);
				dest[___index*4u+1u] += (((add>>8u ) & 0xffu) * add_a >> 8u) - (((sub>>8u ) & 0xffu) * sub_a >> 8u);
				dest[___index*4u+2u] += (((add>>16u) & 0xffu) * add_a >> 8u) - (((sub>>16u) & 0xffu) * sub_a >> 8u);
			}
		}
	}
}

TVP_GL_FUNC_DECL(void, TVPAddSubVertSum32_c, (tjs_uint32 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len))
{
	{
		for(int ___index = 0; ___index < len; ___index++)
		{
			{
				tjs_uint32 add, sub;
				add = addline[___index];
				sub = subline[___index];
				dest[___index*4u+0u] += ((add    ) & 0xffu) - ((sub    ) & 0xffu);
				dest[___index*4u+1u] += ((add>>8u ) & 0xffu) - ((sub>>8u ) & 0xffu);
				dest[___index*4u+2u] += ((add>>16u) & 0xffu) - ((sub>>16u) & 0xffu);
				dest[___index*4u+3u] += ((add>>24u)       ) - ((sub>>24u)       );
			}
		}
	}
}

TVP_GL_FUNC_DECL(void, TVPAddSubVertSum16_d_c, (tjs_uint16 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len))
{
	{
		for(int ___index = 0; ___index < len; ___index++)
		{
			{
				tjs_uint32 add, sub;
				tjs_int add_a, sub_a;
				add = addline[___index];
				sub = subline[___index];
				dest[___index*4u+3u] += (add_a = (add>>24u)       ) - (sub_a = (sub>>24u)       );
				add_a += add_a >> 7u;
				sub_a += sub_a >> 7u;
				dest[___index*4u+0u] += (((add    ) & 0xffu) * add_a >> 8u) - (((sub    ) & 0xffu) * sub_a >> 8u);
				dest[___index*4u+1u] += (((add>>8u ) & 0xffu) * add_a >> 8u) - (((sub>>8u ) & 0xffu) * sub_a >> 8u);
				dest[___index*4u+2u] += (((add>>16u) & 0xffu) * add_a >> 8u) - (((sub>>16u) & 0xffu) * sub_a >> 8u);
			}
		}
	}
}

TVP_GL_FUNC_DECL(void, TVPAddSubVertSum16_c, (tjs_uint16 *dest, const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len))
{
	{
		for(int ___index = 0; ___index < len; ___index++)
		{
			{
				tjs_uint32 add, sub;
				add = addline[___index];
				sub = subline[___index];
				dest[___index*4u+0u] += ((add    ) & 0xffu) - ((sub    ) & 0xffu);
				dest[___index*4u+1u] += ((add>>8u ) & 0xffu) - ((sub>>8u ) & 0xffu);
				dest[___index*4u+2u] += ((add>>16u) & 0xffu) - ((sub>>16u) & 0xffu);
				dest[___index*4u+3u] += ((add>>24u)       ) - ((sub>>24u)       );
			}
		}
	}
}
