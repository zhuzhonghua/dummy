#include "stdafx.h"

#include "styles.h"
#include "movie_definition_sub.h"
#include "character.h"

GradientRecord::GradientRecord():_ratio(0)
{
}

void	GradientRecord::read(Stream* in, int tag_type)
{
  _ratio = in->readU8();
  _color.read(in, tag_type);
}

FillStyle::FillStyle() :
		_type(0),
		_gradientBitmapInfo(0),
		_bitmapCharacter(0)
{
  ASSERT(_gradients.size() == 0);
}

FillStyle::~FillStyle()
{
}

void	FillStyle::read(Stream* in, int tag_type, MovieDefinitionSub* md)
{
  _type = in->readU8();

  INFO("  fill_style read type = 0x%X", _type);

  if (_type == 0x00)
  {
    // 0x00: solid fill
    if (tag_type <= Tag::DEFINESHAPE2) {//22
      _color.readRGB(in);
    } else {
      _color.readRGBA(in);
    }
  }
  else if (_type == 0x10 || _type == 0x12)
  {
    // 0x10: linear gradient fill
    // 0x12: radial gradient fill

    Matrix	input_matrix;
    input_matrix.read(in);

    if (_type == 0x10)
    {
      _gradientMatrix.setIdentity();
      _gradientMatrix.concatenateTranslation(128.f, 0.f);
      _gradientMatrix.concatenateScale(1.0f / 128.0f);
    }
    else 
    {
      _gradientMatrix.setIdentity();
      _gradientMatrix.concatenateTranslation(32.f, 32.f);
      _gradientMatrix.concatenateScale(1.0f / 512.0f);
    }

    Matrix	m;
    m.setInverse(input_matrix);
    _gradientMatrix.concatenate(m);
				
    // GRADIENT

    // 'spread_mode' and 'interpolation_mode' are not used for now
    // and so they are commented out
    //			in->align();
    //			int spread_mode = in->read_uint(2);
    //			int interpolation_mode = in->read_uint(2);
    //			int num_gradients = in->read_uint(4);

    // SWF 8 and later supports up to 15 gradient control points
    int num_gradients = in->readU8() & 0x0F;

    _gradients.resize(num_gradients);
    for (int i = 0; i < num_gradients; i++)
    {
      _gradients[i].read(in, tag_type);
    }

    INFO("  gradients: num_gradients = %d", num_gradients);

    // @@ hack.
    if (num_gradients > 0)
    {
      _color = _gradients[0]._color;
    }

    // Make sure our movie_def_impl knows about this bitmap.
    ASSERT(_gradientBitmapInfo);
    md->addBitmapInfo(_gradientBitmapInfo);
  }
  else if (_type == 0x13)
  {			
    ASSERT( tag_type == 83 );//Tag::DefineShape4
    // focal gradient fill, Flash 8
    // parsed but not implemented yet

    Matrix	input_matrix;
    input_matrix.read(in);

    // 'spread_mode' and 'interpolation_mode' are not used for now
    // and so they are commented out
    //			in->align();
    //			int spread_mode = in->read_uint(2);
    //			int interpolation_mode = in->read_uint(2);
    //			int gradient_count = in->read_uint(4);

    int num_gradients = in->readU8() & 0x0F;

    for (int i = 0; i < num_gradients; i++)
    {
      int ratio = in->readU8();
      UNUSED(ratio);
      RGBA color;
      color.readRGBA(in);
    }

    in->readU16(); //Fixed 8
  }
  else if (_type >= 0x40 && _type <= 0x43)
  {
    // 0x40: tiled bitmap fill
    // 0x41: clipped bitmap fill
    // 0x42: non-smoothed repeating bitmap, Flash 8, TODO
    // 0x43: non-smoothed clipped bitmap, Flash 8, TODO

    int	bitmap_char_id = in->readU16();
    INFO("  bitmap_char = %d", bitmap_char_id);

    // Look up the bitmap character.
		// TODO: maybe bitmapcharacter is null, remove from fillstyle list
    _bitmapCharacter = md->getBitmapCharacter(bitmap_char_id);

    Matrix	m;
    m.read(in);

    // For some reason, it looks like they store the inverse of the
    // TWIPS-to-texcoords matrix.
    _bitmapMatrix.setInverse(m);
    //IF_VERBOSE_PARSE(m_bitmap_matrix.print());
  }
  else
  {
    ASSERT(0);
  }
}

void FillStyle::display(Character* ch)
{
	if (_type >= 0x40 && _type <= 0x43)
	{
		// 0x40: tiled bitmap fill
    // 0x41: clipped bitmap fill
    // 0x42: non-smoothed repeating bitmap, Flash 8, TODO
    // 0x43: non-smoothed clipped bitmap, Flash 8, TODO
		if (_bitmapCharacter)
		{
			//BitmapInfo* bi = _bitmapCharacter->getBitmapInfo();
			_bitmapCharacter->display(ch);
		}
	}
}

LineStyle::LineStyle()
		:
		_width(0),
		_startCapstyle(0),
		_joinstyle(0),
		_hasFillFlag(false),
		_noHscaleFlag(false),
		_noVscaleFlag(false),
		_pixelhintingFlag(false),
		_noclose(false),
		_endCapstyle(0),
		_miterLimitFactor(0)
{
}

void	LineStyle::read(Stream* in, int tag_type, MovieDefinitionSub* m)
{
  _width = in->readU16();
  if (tag_type == 83)	// SHAPE 4
  {
    // 0 = Round cap
    // 1 = No cap
    // 2 = Square cap
    _startCapstyle = in->readUint(2);

    // 0 = Round join
    // 1 = Bevel join
    // 2 = Miter join
    _joinstyle = in->readUint(2);

    // If 0, uses Color field.
    _hasFillFlag  = in->readUint(1) == 1 ? true : false;

    // If 1, stroke thickness will not scale if the object is scaled horizontally.
    _noHscaleFlag = in->readUint(1) == 1 ? true : false;

    // If 1, stroke thickness will not scale if the object is scaled vertically.
    _noVscaleFlag = in->readUint(1) == 1 ? true : false;

    // If 1, all anchors will be aligned to full pixels.
    _pixelhintingFlag = in->readUint(1) == 1 ? true : false;

    in->readUint(5); // Reserved UB[5] Must be 0.

    // If 1, stroke will not be closed if the stroke’s last point
    // matches its first point. Flash Player will apply caps instead of a join.
    _noclose = in->readUint(1) == 1 ? true : false;

    // 0 = Round cap
    // 1 = No cap
    // 2 = Square cap
    _endCapstyle = in->readUint(2);

    if (_joinstyle == 2)
    {
      // Miter limit factor is an 8.8 fixed-point value
      _miterLimitFactor = in->readU16();
    }

    if (_hasFillFlag)
    {
      _fillStyle.read(in, tag_type, m);
    }
    else
    {
      _color.read(in, tag_type);
    }
  }
  else
  {
    _color.read(in, tag_type);
  }
}
