/*
 *  // Made By: Santiago Arribas Maroto
 *  // 2018/2019
 *  // Contact: Santisabirra@gmail.com
 */

#ifndef COLOR_BUFFER_RGBA8888_HEADER
#define COLOR_BUFFER_RGBA8888_HEADER

#include <SFML/OpenGL.hpp>
#include <stdint.h>
#include <vector>

#include "ColorBuffer.hpp"

namespace Anim3D_Project2
{

	class ColorBuffer_RGBA8888 : public ColorBuffer
	{

	public:

		//Definición de que es un color en un buffer de colores
		struct Color
		{
			//Union con la información total de cada píxel de tipo RGBA8888 - 32Bits
			union
			{
				//Componentes RGBA del color individual
				struct
				{
					uint8_t r;
					uint8_t g;
					uint8_t b;
					uint8_t a;
				}   component;

				//Valor conjunto del píxel a 32bits por pixel
				uint32_t value;

			} data;

			//Setter de los valores de un píxel individual con alpha a 1
			void set(int r, int g, int b)
			{
				data.component.r = r;
				data.component.g = g;
				data.component.b = b;
				data.component.a = 255;
			}

			//Operador de igualdad para setear los valores del píxel
			Color& operator = (const int & value)
			{
				data.value = uint32_t(value);
				return (*this);
			}
		};

	private:

		std::vector< Color > buffer;
		Color  color;

	public:

		ColorBuffer_RGBA8888(size_t width, size_t height) : ColorBuffer(width, height), buffer(width * height) {}

		//Setter del píxel en sus componentes RGBA (a 8 BPP)
		void set_color(int r, int g, int b)
		{
			color.data.component.r = uint8_t(r < 0 ? 0 : r > 255 ? 255 : r);
			color.data.component.g = uint8_t(g < 0 ? 0 : g > 255 ? 255 : g);
			color.data.component.b = uint8_t(b < 0 ? 0 : b > 255 ? 255 : b);
			color.data.component.a = 0xFF;
		}

		//Setter de colores
		void set_color(const Color & new_color) { color = new_color; }

		//Setter de píxeles individuales a partir del offset o la posición del mismo
		void set_pixel(size_t offset) { buffer[offset] = color; }
		void set_pixel(int x, int y) { buffer[y * buffer_w + x] = color; }
		 
		Color*  colors() { return (&buffer.front());             }   //Array de colores del buffer de píxeles
		const Color* colors() const { return (&buffer.front());  }   //const Array de colores del buffer de píxeles
		int bits_per_color()  const { return (sizeof(Color) * 8);}   //Profundidad de color
		size_t size() const { return (buffer.size());            }   //Tamaño del buffer de píxeles

		//Rasterizado del buffer para OpenGL
		void gl_draw_pixels(int ras_x, int ras_y) const
		{
			glRasterPos2i(ras_x, ras_y);
			glDrawPixels(buffer_w, buffer_h, GL_RGBA, GL_UNSIGNED_BYTE, colors());
		}
	};
}

#endif