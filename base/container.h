#pragma once

// array<> is much like std::vector<>
//
// @@ move this towards a strict subset of std::vector ?  Compatibility is good.
template<class T> class array : public std::vector<T>
{
public:
  array(){}
	array(int size_hint):std::vector<T>(size_hint){}
	int	size() const { return (int) std::vector<T>::size(); }

	void	append(const array<T>& other)
	// Append the given data to our array.
	{
		std::vector<T>::insert(std::vector<T>::end(), other.begin(), other.end());
	}

	void	append(const T other[], int count)
	{
		// This will probably work.  Depends on std::vector<T>::iterator being typedef'd as T*
		std::vector<T>::insert(std::vector<T>::end(), &other[0], &other[count]);
	}

	void	remove(int index)
	{
		std::vector<T>::erase(std::vector<T>::begin() + index);
	}

	void	insert(int index, const T& val = T())
	{
		std::vector<T>::insert(std::vector<T>::begin() + index, val);
	}

	void	release()
	{
		// Drop all storage.
		std::vector<T>	temp;
		this->swap(temp);
	}
};
