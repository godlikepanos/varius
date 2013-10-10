#ifndef VEC_H
#define VEC_H

struct Vec2
{
	float arr[2];

	Vec2(float x, float y)
	{
		arr[0] = x;
		arr[1] = y;
	}

	float& x()
	{
		return arr[0];
	}
	float x() const
	{
		return arr[0];
	}
	float& y()
	{
		return arr[1];
	}
	float y() const
	{
		return arr[1];
	}
	float& operator[](const size_t i)
	{
		return arr[i];
	}
	float operator[](const size_t i) const
	{
		return arr[i];
	}

	void print()
	{
		std::cout << "Vec2(" << arr[0] << ", " << arr[1] << ")" << std::endl;
	}
};

#endif

