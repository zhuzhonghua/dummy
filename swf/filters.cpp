#include "stdafx.h"

#include "filters.h"

namespace SWF{
void readFilterList( Stream* in )
{
  // reads FILTERLIST
  int filters = in->readU8();
  for (int i = 0; i < filters; i++)
  {
    int filter_id = in->readU8();
    switch (filter_id)
    {
			case 0 : // DropShadowFilter
				{
					RGBA drop_shadow_color;
					drop_shadow_color.readRGBA(in);	// RGBA Color of the shadow
					float blur_x = in->readFixed();	// Horizontal blur amount
					float blur_y = in->readFixed();	// Vertical blur amount
					float angle = in->readFixed();	// Radian angle of the drop shadow
					float distance = in->readFixed();	// Distance of the drop shadow
					float strength = in->readSI8();	// hack, must be FIXED8 Strength of the drop shadow
					bool inner_shadow = in->readBool();	// Inner shadow mode
					bool knockout = in->readBool();	// Knockout mode
					bool composite_source = in->readBool();	// Composite source Always 1
					int passes = in->readUint(5); // passes
					in->readU8();// If not present, when parsing several filters, filter type is not correct
					//IF_VERBOSE_PARSE(log_msg("  filter = DropShadowFilter\n" ));

					UNUSED(blur_x);
					UNUSED(blur_y);
					UNUSED(angle);
					UNUSED(distance);
					UNUSED(strength);
					UNUSED(inner_shadow);
					UNUSED(knockout);
					UNUSED(composite_source);
					UNUSED(passes);

					break;
				}

			case 1 : // BlurFilter
				{
					float blur_x = in->readFixed(); // Horizontal blur amount
					float blur_y = in->readFixed(); // Vertical blur amount
					int passes = in->readUint(5);	// Number of blur passes
					in->read_uint(3);	// Reserved UB[3] Must be 0
					IF_VERBOSE_PARSE(log_msg("  filter = BlurFilter\n" ));

					UNUSED(blur_x);
					UNUSED(blur_y);
					UNUSED(passes);

					break;
				}

			case 2 : // GlowFilter
				{
					RGBA glow_color;
					glow_color.readRGBA(in);	//RGBA Color of the shadow
					float blur_x = in->readFixed();	// Horizontal blur amount
					float blur_y = in->readFixed();	// Vertical blur amount
					float strength = in->readSI8();	// hack, must be FIXED8 Strength of the drop shadow
					bool inner_glow = in->readBool();	// Inner glow mode
					bool knockout = in->readBool();	// Knockout mode
					bool composite_source = in->readBool();	// Composite source Always 1
					in->readUint(5); // passes
					in->readU8();// If not present, when parsing several filters, filter type is not correct
					//IF_VERBOSE_PARSE(log_msg("  filter = GlowFilter\n" ));

					UNUSED(blur_x);
					UNUSED(blur_y);
					UNUSED(strength);
					UNUSED(inner_glow);
					UNUSED(knockout);
					UNUSED(composite_source);

					break;
				}

			case 3 : // BevelFilter
				{
					RGBA shadow_color;
					shadow_color.readRGBA(in);	//RGBA Color of the shadow
					RGBA highlight_color;
					highlight_color.readRGBA(in);	//RGBA Color of the highlight
					float blur_x = in->readFixed();	// Horizontal blur amount
					float blur_y = in->readFixed();	// Vertical blur amount
					float angle = in->readFixed();	// Radian angle of the drop shadow
					float distance = in->readFixed();	// Distance of the drop shadow
					float strength = in->readSI8();	// hack, must be FIXED8 Strength of the drop shadow
					bool inner_shadow = in->readBool();	// Inner shadow mode
					bool knockout = in->readBool();	// Knockout mode
					bool composite_source = in->readBool();	// Composite source Always 1
					bool on_top = in->readBool();	// Composite source Always 1
					int passes = in->readUint(4); // passes
					in->readU8();// If not present, when parsing several filters, filter type is not correct
					//IF_VERBOSE_PARSE(log_msg("  filter = BevelFilter\n" ));

					UNUSED(blur_x);
					UNUSED(blur_y);
					UNUSED(angle);
					UNUSED(distance);
					UNUSED(strength);
					UNUSED(inner_shadow);
					UNUSED(knockout);
					UNUSED(composite_source);
					UNUSED(on_top);
					UNUSED(passes);

					break;
				}

			case 4 : // GradientGlowFilter
				{
					int num_colors = in->readU8();		// Number of colors in the gradient
					for (int i = 0; i < num_colors; i++)
					{
						RGBA gradient_colors;
						gradient_colors.readRGBA(in);	// RGBA[NumColors] Gradient colors
					}
					for (int i = 0; i < num_colors; i++)
					{
						int gradient_ratio = in->readU8();	// UI8[NumColors] Gradient ratios
						UNUSED(gradient_ratio);
					}
					float blur_x = in->readFixed();	// Horizontal blur amount
					float blur_y = in->readFixed();	// Vertical blur amount
					float angle = in->readFixed();	// Radian angle of the drop shadow
					float distance = in->readFixed();	// Distance of the drop shadow
					float strength = in->readSI8();	// hack, must be FIXED8 Strength of the drop shadow
					bool inner_shadow = in->readBool();	// Inner shadow mode
					bool knockout = in->readBool();	// Knockout mode
					bool composite_source = in->readBool();	// Composite source Always 1
					bool on_top = in->readBool();	// Composite source Always 1
					int passes = in->readUint(4); // passes

					UNUSED(blur_x);
					UNUSED(blur_y);
					UNUSED(angle);
					UNUSED(distance);
					UNUSED(strength);
					UNUSED(inner_shadow);
					UNUSED(knockout);
					UNUSED(composite_source);
					UNUSED(on_top);
					UNUSED(passes);

					//IF_VERBOSE_PARSE(log_msg("  filter = GradientGlowFilter\n" ));
					break;
				}

			case 5 : // ConvolutionFilter
				{
					int matrix_x = in->readU8();	// Horizontal matrix size
					int matrix_y = in->readU8();	// Vertical matrix size
					float divisor = in->read_float();	// Divisor applied to the matrix values
					float bias = in->readFloat();	// Bias applied to the matrix values
					for (int k = 0; k < matrix_x * matrix_y; k++)
					{
						in->readFloat();	// Matrix values
					}
					RGBA default_color;
					default_color.readRGBA(in);	// RGBA Default color for pixels outside the image
					in->readUint(6);		// Reserved UB[6] Must be 0
					bool clamp = in->readBool();	// UB[1] Clamp mode
					bool preserve_alpha = in->readBool();	// UB[1]
					//IF_VERBOSE_PARSE(log_msg("  filter = ConvolutionFilter\n" ));

					UNUSED(divisor);
					UNUSED(bias);
					UNUSED(clamp);
					UNUSED(preserve_alpha);

					break;
				}

			case 6 : // ColorMatrixFilter
				// matrix is float[20]
				for (int k = 0; k < 20; k++)
				{
					in->readFloat();
				}
				IF_VERBOSE_PARSE(log_msg("  filter = ColorMatrixFilter\n" ));
				break;

			case 7 : // GradientBevelFilter
				{
					int num_colors = in->readU8();		// Number of colors in the gradient
					for (int i = 0; i < num_colors; i++)
					{
						RGBA gradient_colors;
						gradient_colors.readRGBA(in);	// RGBA[NumColors] Gradient colors
					}
					for (int i = 0; i < num_colors; i++)
					{
						int gradient_ratio = in->readU8();	// UI8[NumColors] Gradient ratios
						UNUSED(gradient_ratio);
					}
					float blur_x = in->readFixed();	// Horizontal blur amount
					float blur_y = in->readFixed();	// Vertical blur amount
					float angle = in->readFixed();	// Radian angle of the drop shadow
					float distance = in->readFixed();	// Distance of the drop shadow
					float strength = in->readSI8();	// hack, must be FIXED8 Strength of the drop shadow
					bool inner_shadow = in->readBool();	// Inner shadow mode
					bool knockout = in->readBool();	// Knockout mode
					bool composite_source = in->readBool();	// Composite source Always 1
					bool on_top = in->readBool();	// Composite source Always 1
					int passes = in->readUint(4); // passes
					in->readU8();// If not present, when parsing several filters, filter type is not correct
					//IF_VERBOSE_PARSE(log_msg("  filter = GradientBevelFilter\n" ));

					UNUSED(blur_x);
					UNUSED(blur_y);
					UNUSED(angle);
					UNUSED(distance);
					UNUSED(strength);
					UNUSED(inner_shadow);
					UNUSED(knockout);
					UNUSED(composite_source);
					UNUSED(on_top);
					UNUSED(passes);

					break;
				}

			default:
				ASSERT(0);	// invalid input
    }
  }
}
};
