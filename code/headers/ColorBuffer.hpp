/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#ifndef COLOR_BUFFER_HEADER
#define COLOR_BUFFER_HEADER

namespace Anim3D_Project2
{
	class ColorBuffer
	{
	public:

		size_t buffer_w; //Ancho del buffer de píxeles
		size_t buffer_h; //Alto del buffer de píxeles

		//Getters de ancho y alto del buffer
		size_t get_width ()  const { return (buffer_w);}
		size_t get_height() const { return (buffer_h);}

		//Cálculo del offset del buffer
		int pixel_offset(int x, int y) const{ return (y * buffer_w + x); }

		//Ajuste de profundidad de color 
		virtual int  bits_per_color () const = 0;

		//Seters para cada píxel
		virtual void set_color (int r, int  g, int b) = 0;
		virtual void set_pixel (int x, int  y)        = 0;
		virtual void set_pixel (size_t offset)        = 0;

		//Rasterizado del buffer en OpenGL
		virtual void gl_draw_pixels (int raster_x, int raster_y) const = 0;		
		
		ColorBuffer(size_t buffer_width, size_t buffer_height): buffer_w(buffer_width), buffer_h(buffer_height) {}
	};
}

#endif
