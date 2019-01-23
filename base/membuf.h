#pragma once

class membuf
{
public:
	exported_module membuf();
	exported_module membuf(const void* data, int size);
	exported_module membuf(const membuf& buf);
	exported_module membuf(const std::string& str);
	exported_module ~membuf();

	// Construct a read-only membuf that points at the given data,
	// instead of copying it.
	enum read_only_enum { READ_ONLY };
	exported_module membuf(read_only_enum e, const void* data, int size);

	exported_module int size() const { return m_size; }
	exported_module const void* data() const { return m_data; }
	exported_module void* data() { assert(!m_read_only); return m_data; }

	// Don't call these mutators on read-only membufs.
	
	exported_module void resize(int new_size);
	exported_module void append(const void* data, int size);
	exported_module void append(const membuf& buf);
	exported_module void append(Uint8 byte);

	// We do not append the terminating '\0'.
	exported_module void append(const std::string& str);

	exported_module Uint8&	operator[](int index);
	exported_module const Uint8&	operator[](int index) const;
	exported_module void	operator=(const membuf& buf);
	exported_module bool	operator==(const membuf& buf) const;
	exported_module bool	operator!=(const membuf& buf) const;

private:

	int m_size;
	int m_capacity;
	void* m_data;
	bool m_read_only;
};
