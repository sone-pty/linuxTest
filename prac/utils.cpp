#include "utils.h"

namespace util
{
	int byteorder()
	{
		union
		{
			short s;
			char bytes[sizeof(short) / BYTE_NUMS];
		}t;

		t.s = 0x0102;

		if(t.bytes[0] == 1 && t.bytes[1] == 2)
			return BYTEORDER_BIG;
		else if(t.bytes[0] == 2 && t.bytes[1] == 1)
			return BYTEORDER_LITTLE;
		else
			return -1;
	}
}
