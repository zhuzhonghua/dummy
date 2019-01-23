#pragma once

class GradientRecord
{
public:
  GradientRecord();
  void	read(Stream* in, int tag_type);

	//data:
  Uint8	_ratio;
  RGBA	_color;
};

class BaseFillStyle
{
public:
  virtual ~BaseFillStyle() {}
  //virtual void apply(int fill_side, float ratio, render_handler::bitmap_blend_mode bm) const = 0;
};

class MovieDefinitionSub;
class BitmapCharacterDef;

class FillStyle : public BaseFillStyle
// For the interior of outline shapes.
{
public:
  FillStyle();
  virtual ~FillStyle();

  void	read(Stream* in, int tag_type, MovieDefinitionSub* m);
  RGBA	sampleGradient(int ratio) const;
  BitmapInfo*	createGradientBitmap() const;
  //virtual void	apply(int fill_side, float ratio, render_handler::bitmap_blend_mode bm) const;

  RGBA	getColor() const { return _color; }
  void	setColor(RGBA new_color) { _color = new_color; }
  int	    getType() const { return _type; }
  void    setBlendMode(Uint8 bm) {_blendMode = bm;}

  // For shape morphing
  void	setLerp(const FillStyle& a, const FillStyle& b, float t);

private:
  //friend struct morph2_character_def;
  //friend struct canvas;

  int	_type;
  Uint8 _blendMode;
  RGBA	_color;
  Matrix	_gradientMatrix;
  std::vector<GradientRecord>	_gradients;
  BitmapInfo*	_gradientBitmapInfo;
  BitmapCharacterDef*	_bitmapCharacter;
  Matrix	_bitmapMatrix;
};

class BaseLineStyle
{
public:
  virtual ~BaseLineStyle() {}
  //virtual void apply(float ratio) const = 0;
};

class LineStyle : public BaseLineStyle
// For the outside of outline shapes, or just bare lines.
{
public:
  LineStyle();
  virtual ~LineStyle() {}
  void	read(Stream* in, int tag_type, MovieDefinitionSub* m);
  //virtual void	apply(float ratio) const;

  Uint16	getWidth() const { return _width; }
  const RGBA&	getColor() const { return _color; }

private:
  //friend struct morph2_character_def;
  //friend struct canvas;

  Uint16	_width;	// in TWIPS
  RGBA	_color;
  FillStyle _fillStyle;
  Uint8 _startCapstyle;
  Uint8 _joinstyle;
  bool _hasFillFlag;
  bool _noHscaleFlag;
  bool _noVscaleFlag;
  bool _pixelhintingFlag;
  bool _noclose;
  Uint8 _endCapstyle;
  Uint16 _miterLimitFactor;
};
