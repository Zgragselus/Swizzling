#include "Float4.h"
#include <iostream>
#include <string>

void printf4(const std::string& s, const float4& v)
{
	std::cout << s << " = (" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")" << std::endl;
}

int main()
{
	std::cout << "Swizzling test" << std::endl;

	float4 u = float4(1.0f, 2.0f, 3.0f, 4.0f);
	float4 v = float4(2.0f, 6.0f, 1.0f, 1.0f);
	float4 w = float4();
	w.xy = v.xy;
	float4 x = w;
	float4 y;
	y.xwz = u.zwy;

	printf4("u", u);
	printf4("v", v);
	printf4("w", w);
	printf4("x", x);
	printf4("y", y);

	std::cin.get();

	return 0;
}