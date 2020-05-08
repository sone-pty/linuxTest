#include "test.h"
#include "../LRUCache.h"

int main(void)
{
	LRUCache cache(5);
	cout << (cache.getValue("/home/sone/web_root/index.html")  == nullptr) << endl;
	cache.setValue("/home/sone/web_root/index.html");
	const char* p = cache.getValue("/home/sone/web_root/index.html");
	cout << p << endl;
	return 0;
}
