
#include "tjsCommHead.h"
#include "tvpgl.h"
#include "LayerBitmapIntf.h"
#include "LayerBitmapDoBoxBlur.h"
#include "DivTable.h"
#include "argb.h"


#define RET_VOID
#define BOUND_CHECK(x) \
{ \
	tjs_int i; \
	if(rect.left < 0) rect.left = 0; \
	if(rect.top < 0) rect.top = 0; \
	if(rect.right > (i=destbmp->GetWidth())) rect.right = i; \
	if(rect.bottom > (i=destbmp->GetHeight())) rect.bottom = i; \
	if(rect.right - rect.left <= 0 || rect.bottom - rect.top <= 0) \
		return x; \
}

//---------------------------------------------------------------------------
// some blur operation template functions to select algorithm by base integer type
template <typename tARGB, typename base_int_type>
void TVPAddSubVertSum(base_int_type *dest, const tjs_uint32 *addline,
	const tjs_uint32 *subline, tjs_int len)
{
}
template <>
void TVPAddSubVertSum<tTVPARGB<tjs_uint16>, tjs_uint16 >(tjs_uint16 *dest,
	const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
{
	TVPAddSubVertSum16(dest, addline, subline, len);
}
template <>
void TVPAddSubVertSum<tTVPARGB_AA<tjs_uint16>, tjs_uint16 >(tjs_uint16 *dest,
	const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
{
	TVPAddSubVertSum16_d(dest, addline, subline, len);
}
template <>
void TVPAddSubVertSum<tTVPARGB<tjs_uint32>, tjs_uint32 >(tjs_uint32 *dest,
	const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
{
	TVPAddSubVertSum32(dest, addline, subline, len);
}
template <>
void TVPAddSubVertSum<tTVPARGB_AA<tjs_uint32>, tjs_uint32 >(tjs_uint32 *dest,
	const tjs_uint32 *addline, const tjs_uint32 *subline, tjs_int len)
{
	TVPAddSubVertSum32_d(dest, addline, subline, len);
}

template <typename tARGB, typename base_int_type>
void TVPDoBoxBlurAvg(tjs_uint32 *dest, base_int_type *sum,
	const base_int_type * add, const base_int_type * sub,
		tjs_int n, tjs_int len)
{
}
template <>
void TVPDoBoxBlurAvg<tTVPARGB<tjs_uint16>, tjs_uint16 >(tjs_uint32 *dest, tjs_uint16 *sum,
	const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len)
{
	TVPDoBoxBlurAvg16(dest, sum, add, sub, n, len);
}
template <>
void TVPDoBoxBlurAvg<tTVPARGB_AA<tjs_uint16>, tjs_uint16  >(tjs_uint32 *dest, tjs_uint16 *sum,
	const tjs_uint16 * add, const tjs_uint16 * sub, tjs_int n, tjs_int len)
{
	TVPDoBoxBlurAvg16_d(dest, sum, add, sub, n, len);
}
template <>
void TVPDoBoxBlurAvg<tTVPARGB<tjs_uint32>, tjs_uint32  >(tjs_uint32 *dest, tjs_uint32 *sum,
	const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len)
{
	TVPDoBoxBlurAvg32(dest, sum, add, sub, n, len);
}
template <>
void TVPDoBoxBlurAvg<tTVPARGB_AA<tjs_uint32>, tjs_uint32  >(tjs_uint32 *dest, tjs_uint32 *sum,
	const tjs_uint32 * add, const tjs_uint32 * sub, tjs_int n, tjs_int len)
{
	TVPDoBoxBlurAvg32_d(dest, sum, add, sub, n, len);
}


//---------------------------------------------------------------------------


template <typename tARGB>
void DoBoxBlurLoop(tTVPBaseBitmap *destbmp, const tTVPRect &rect, const tTVPRect & area)
{
	// Box-Blur template function used by tTVPBaseBitmap::DoBoxBlur family.
	// Based on contributed blur code by yun, say thanks to him.

	typedef typename tARGB::base_int_type base_type;

	tjs_int width = destbmp->GetWidth();
	tjs_int height = destbmp->GetHeight();

	tjs_int dest_buf_size = area.top <= 0 ? (1-area.top) : 0;

	tjs_int vert_sum_left_limit = rect.left + area.left;
	if(vert_sum_left_limit < 0) vert_sum_left_limit = 0;
	tjs_int vert_sum_right_limit = (rect.right-1) + area.right;
	if(vert_sum_right_limit >= width) vert_sum_right_limit = width - 1;


	tARGB * vert_sum = NULL; // vertical sum of the pixel
	tjs_uint32 * * dest_buf = NULL; // destination pixel temporary buffer

	tjs_int vert_sum_count;

	try
	{
		// allocate buffers
		vert_sum = (tARGB*)TJSAlignedAlloc(sizeof(tARGB) *
			(vert_sum_right_limit - vert_sum_left_limit + 1 + 1), 4); // use 128bit aligned allocation

		if(dest_buf_size)
		{
			dest_buf = new tjs_uint32 * [dest_buf_size];
			for(tjs_int i = 0; i < dest_buf_size; i++)
				dest_buf[i] = new tjs_uint32[rect.right - rect.left];
		}

		// initialize vert_sum
		{
			for(tjs_int i = vert_sum_right_limit - vert_sum_left_limit + 1 -1; i>=0; i--)
				vert_sum[i].Zero();

			tjs_int v_init_start = rect.top + area.top;
			if(v_init_start < 0) v_init_start = 0;
			tjs_int v_init_end = rect.top + area.bottom;
			if(v_init_end >= height) v_init_end = height - 1;
			vert_sum_count = v_init_end - v_init_start + 1;
			for(tjs_int y = v_init_start; y <= v_init_end; y++)
			{
				const tjs_uint32 * add_line;
				add_line = (const tjs_uint32*)destbmp->GetScanLine(y);
				tARGB * vs = vert_sum;
				for(int x = vert_sum_left_limit; x <= vert_sum_right_limit; x++)
					*(vs++) += add_line[x];
			}
		}

		// prepare variables to be used in following loop
		tjs_int h_init_start = rect.left + area.left; // this always be the same value as vert_sum_left_limit
		if(h_init_start < 0) h_init_start = 0;
		tjs_int h_init_end = rect.left + area.right;
		if(h_init_end >= width) h_init_end = width - 1;

		tjs_int left_frac_len =
			rect.left + area.left < 0 ? -(rect.left + area.left) : 0;
		tjs_int right_frac_len =
			rect.right + area.right >= width ? rect.right + area.right - width + 1: 0;
		tjs_int center_len = rect.right - rect.left - left_frac_len - right_frac_len;

		if(center_len < 0)
		{
			left_frac_len = rect.right - rect.left;
			right_frac_len = 0;
			center_len = 0;
		}
		tjs_int left_frac_lim = rect.left + left_frac_len;
		tjs_int center_lim = rect.left + left_frac_len + center_len;

		// for each line
		tjs_int dest_buf_free = dest_buf_size;
		tjs_int dest_buf_wp = 0;

		for(tjs_int y = rect.top; y < rect.bottom; y++)
		{
			// rotate dest_buf
			if(dest_buf_free == 0)
			{
				// dest_buf is full;
				// write last dest_buf back to the bitmap
				memcpy(
					rect.left + ((tjs_uint32*)destbmp->GetScanLineForWrite(y - dest_buf_size)),
					dest_buf[dest_buf_wp],
					(rect.right - rect.left) * sizeof(tjs_uint32));
			}
			else
			{
				dest_buf_free --;
			}

			// build initial sum
			tARGB sum;
			sum.Zero();
			tjs_int horz_sum_count = h_init_end - h_init_start + 1;

			for(tjs_int x = h_init_start; x <= h_init_end; x++)
				sum += vert_sum[x - vert_sum_left_limit];

			// process a line
			tjs_uint32 *dp = dest_buf[dest_buf_wp];
			tjs_int x = rect.left;

			//- do left fraction part
			for(; x < left_frac_lim; x++)
			{
				tARGB tmp = sum;
				tmp.average(horz_sum_count * vert_sum_count);

				*(dp++) = tmp;

				// update sum
				if(x + area.left >= 0)
				{
					sum -= vert_sum[x + area.left - vert_sum_left_limit];
					horz_sum_count --;
				}
				if(x + area.right + 1 < width)
				{
					sum += vert_sum[x + area.right + 1 - vert_sum_left_limit];
					horz_sum_count ++;
				}
			}

			//- do center part
			if(center_len > 0)
			{
				// uses function in tvpgl
				TVPDoBoxBlurAvg<tARGB>(dp, (base_type*)&sum,
					(const base_type *)(vert_sum + x + area.right + 1 - vert_sum_left_limit),
					(const base_type *)(vert_sum + x + area.left - vert_sum_left_limit),
					horz_sum_count * vert_sum_count,
					center_len);
				dp += center_len;
			}
			x = center_lim;

			//- do right fraction part
			for(; x < rect.right; x++)
			{
				tARGB tmp = sum;
				tmp.average(horz_sum_count * vert_sum_count);

				*(dp++) = tmp;

				// update sum
				if(x + area.left >= 0)
				{
					sum -= vert_sum[x + area.left - vert_sum_left_limit];
					horz_sum_count --;
				}
				if(x + area.right + 1 < width)
				{
					sum += vert_sum[x + area.right + 1 - vert_sum_left_limit];
					horz_sum_count ++;
				}
			}

			// update vert_sum
			if(y != rect.bottom - 1)
			{
				const tjs_uint32 * sub_line;
				const tjs_uint32 * add_line;
				sub_line =
					y + area.top < 0 ?
						(const tjs_uint32 *)NULL :
						(const tjs_uint32 *)destbmp->GetScanLine(y + area.top);
				add_line =
					y + area.bottom + 1 >= height ?
						(const tjs_uint32 *)NULL :
						(const tjs_uint32 *)destbmp->GetScanLine(y + area.bottom + 1);

				if(sub_line && add_line)
				{
					// both sub_line and add_line are available
					// uses function in tvpgl
					TVPAddSubVertSum<tARGB>((base_type*)vert_sum,
						add_line + vert_sum_left_limit,
						sub_line + vert_sum_left_limit,
						vert_sum_right_limit - vert_sum_left_limit + 1);

				}
				else if(sub_line)
				{
					// only sub_line is available
					tARGB * vs = vert_sum;
					for(int x = vert_sum_left_limit; x <= vert_sum_right_limit; x++)
						*vs -= sub_line[x], vs ++;
					vert_sum_count --;
				}
				else if(add_line)
				{
					// only add_line is available
					tARGB * vs = vert_sum;
					for(int x = vert_sum_left_limit; x <= vert_sum_right_limit; x++)
						*vs += add_line[x], vs ++;
					vert_sum_count ++;
				}
			}

			// step dest_buf_wp
			dest_buf_wp++;
			if(dest_buf_wp >= dest_buf_size) dest_buf_wp = 0;
		}

		// write remaining dest_buf back to the bitmap
		while(dest_buf_free < dest_buf_size)
		{
			memcpy(
				rect.left +
				(tjs_uint32*)(destbmp->GetScanLineForWrite(rect.bottom - (dest_buf_size - dest_buf_free))),
				dest_buf[dest_buf_wp], (rect.right - rect.left) * sizeof(tjs_uint32));

			dest_buf_wp++;
			if(dest_buf_wp >= dest_buf_size) dest_buf_wp = 0;
			dest_buf_free++;
		}
	}
	catch(...)
	{
		// exception caught
		if(vert_sum) TJSAlignedDealloc(vert_sum);
		if(dest_buf_size)
		{
			if(dest_buf)
			{
				for(tjs_int i = 0 ; i < dest_buf_size; i++)
					if(dest_buf[i]) delete [] dest_buf[i];
				delete [] dest_buf;
			}
		}
		throw;
	}

	// free buffeers
	TJSAlignedDealloc(vert_sum);
	if(dest_buf_size)
	{
		for(tjs_int i = 0 ; i < dest_buf_size; i++) delete [] dest_buf[i];
		delete [] dest_buf;
	}
}
//---------------------------------------------------------------------------
bool InternalDoBoxBlur(tTVPBaseBitmap *destbmp, tTVPRect rect, tTVPRect area, bool hasalpha)
{
	BOUND_CHECK(false);

	if(area.right < area.left)
		std::swap(area.right, area.left);
	if(area.bottom < area.top)
		std::swap(area.bottom, area.top);

	if(area.left == 0 && area.right == 0 &&
		area.top == 0 && area.bottom == 0) return false; // no conversion occurs

	if(area.left > 0 || area.right < 0 || area.top > 0 || area.bottom < 0)
		TVPThrowExceptionMessage(TJS_W("Box blur area must contain center pixel"));


	tjs_uint64 area_size = (tjs_uint64)
		(area.right - area.left + 1) * (area.bottom - area.top + 1);
	if(area_size < 256)
	{
		if(!hasalpha)
			DoBoxBlurLoop<tTVPARGB<tjs_uint16> >(destbmp, rect, area);
		else
			DoBoxBlurLoop<tTVPARGB_AA<tjs_uint16> >(destbmp, rect, area);
	}
	else if(area_size < (1L<<24))
	{
		if(!hasalpha)
			DoBoxBlurLoop<tTVPARGB<tjs_uint32> >(destbmp, rect, area);
		else
			DoBoxBlurLoop<tTVPARGB_AA<tjs_uint32> >(destbmp, rect, area);
	}
	else
		TVPThrowExceptionMessage(TJS_W("Box blur area must be smaller than 16 million"));

	return true;
}
//---------------------------------------------------------------------------
bool DoBoxBlur(tTVPBaseBitmap *destbmp, const tTVPRect & rect, const tTVPRect & area)
{
	// Blur the bitmap with box-blur algorithm.
	// 'rect' is a rectangle to blur.
	// 'area' is an area which destination pixel refers.
	// right and bottom of 'area' *does contain* pixels in the boundary.
	// eg. area:(-1,-1,1,1)  : Blur is to be performed using average of 3x3
	//                          pixels around the destination pixel.
	//     area:(-10,0,10,0) : Blur is to be performed using average of 21x1
	//                          pixels around the destination pixel. This results
	//                          horizontal blur.

	return InternalDoBoxBlur(destbmp, rect, area, false);
}
//---------------------------------------------------------------------------
bool DoBoxBlurForAlpha(tTVPBaseBitmap *destbmp, const tTVPRect & rect, const tTVPRect &area)
{
	return InternalDoBoxBlur(destbmp, rect, area, true);
}
