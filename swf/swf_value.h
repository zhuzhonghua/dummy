#pragma once

class FNCall;
class ASCFunction;
class ASFunction;
class ASObject;
class ASEnvironment;

bool stringToNumber(int* result, const char* str, int base = 10);
bool stringToNumber(double* result, const char* str);

typedef void (*ASCFunctionPtr)(const FNCall& fn);

// helper, used in as_value
class ASProperty
{
public:
  ASFunction*	getter;
  ASFunction*	setter;

  ASProperty(const ASValue& getter,	const ASValue& setter);
  ~ASProperty();
	
  void	set(ASObject* target, const ASValue& val);
  void	get(ASObject* target, ASValue* val) const;
  void	get(const ASValue& primitive, ASValue* val) const;
};

class ASValue
{
public:
  // flags defining the level of protection of a value
  enum ValueFlag
  {
    DONT_ENUM = 0x01,
    DONT_DELETE = 0x02,
    READ_ONLY = 0x04
  };

private:
  enum Type
  {
    UNDEFINED,
    BOOLEAN,
    NUMBER,
    STRING,
    OBJECT,
    PROPERTY
  };
  Type	_type;

  std::string	_string;
  union
  {
    double _number;
    bool _bool;
  };

  ASObject* _object;
  ASObject* _propertyTarget;
  ASProperty* _property;
		
  // Numeric flags
  int _flags;

public:

  // constructors
  ASValue();
  ASValue(const ASValue& v);
  ASValue(const char* str);
  ASValue(const wchar_t* wstr);
  ASValue(bool val);
  ASValue(int val);
  ASValue(float val);
  ASValue(double val);
  ASValue(ASObject* obj);
  ASValue(ASCFunctionPtr func);
  ASValue(ASSFunction* func);
  ASValue(const ASValue& getter, const ASValue& setter);

  ~ASValue() { }

  // Useful when changing types/values.
  //exported_module void	drop_refs();

  // for debuging
  const char*	to_xstring() const;

  const char*	toString() const;
  //const std::string&	toString() const;
  //const tu_stringi&	to_tu_stringi() const;
  double	toNumber() const;
  int	toInt() const { return (int) toNumber(); };
  float	toFloat() const { return (float) toNumber(); };
  bool	toBool() const;
  ASFunction*	toFunction() const;
  ASObject*	toObject() const;
  ASProperty*	toProperty() const;

  // These set_*()'s are more type-safe; should be used
  // in preference to generic overloaded set().  You are
  // more likely to get a warning/error if misused.
  //void	set_tu_string(const tu_string& str);
  void	setString(const char* str);
  void	setDouble(double val);
  void	setBool(bool val);
  void	setInt(int val) { setDouble(val); }
  void	setNan() { setDouble(getNan()); }
  void	setASObject(ASObject* obj);
  void	setASCFunction(ASCFunctionPtr func);
  void	setUndefined() { _type = UNDEFINED; }
  void	setNull() { setASObject(NULL); }

  void	setProperty(const ASValue& val);
  void	getProperty(ASValue* val) const;
  void	getProperty(const ASValue& primitive, ASValue* val) const;
  const ASObject*	getPropertyTarget() const;  // for debugging
  void	setPropertyTarget(ASObject* new_target);

  void	operator=(const ASValue& v);
  bool	operator==(const ASValue& v) const;
  bool	operator!=(const ASValue& v) const;
  bool	operator<(double v) const { return toNumber() < v; }
  void	operator+=(double v) { setDouble(toNumber() + v); }
  void	operator-=(double v) { setDouble(toNumber() - v); }
  void	operator*=(double v) { setDouble(toNumber() * v); }
  void	operator/=(double v) { setDouble(toNumber() / v); }  // @@ check for div/0
  void	operator&=(int v) { setInt(int(toNumber()) & v); }
  void	operator|=(int v) { setInt(int(toNumber()) | v); }
  void	operator^=(int v) { setInt(int(toNumber()) ^ v); }
  void	shl(int v) { setInt(int(toNumber()) << v); }
  void	asr(int v) { setInt(int(toNumber()) >> v); }
  void	lsr(int v) { setInt((Uint32(toNumber()) >> v)); }

  bool isFunction() const;
  inline bool isBool() const { return _type == BOOLEAN; }
  inline bool isString() const { return _type == STRING; }
  inline bool isNumber() const { return _type == NUMBER && isnan(m_number) == false; }
  inline bool isObject() const { return _type == OBJECT; }
  inline bool isProperty() const { return _type == PROPERTY; }
  inline bool isNull() const { return _type == OBJECT && _object == NULL; }
  inline bool isUndefined() const { return _type == UNDEFINED; }

  const char* typeOf() const;
  bool isInstanceOf(const ASFunction* constructor) const;
  bool findProperty(const std::string& name, ASValue* val);
  bool findPropertyOwner(const std::string& name, ASValue* val);

  // flags
  inline bool isEnum() const { return _flags & DONT_ENUM ? false : true; }
  inline bool isReadonly() const { return _flags & READ_ONLY ? true : false; }
  inline bool isProtected() const { return _flags & DONT_DELETE ? true : false; }
  inline int getFlags() const { return _flags; }
  inline void setFlags(int flags) const  { _flags = flags; }

  static bool abstractEqualityComparison( const ASValue & first, const ASValue & second );
  static as_value abstractRelationalComparison( const ASValue & first, const ASValue & second );
};
