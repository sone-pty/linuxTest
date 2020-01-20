#include <iostream>
#include "TutorialConfig.h"

#ifdef USE_MYMATH
#include "Math.h"
#endif

using namespace std;

int main(void)
{
	cout << "Major_VERSION:" << Tutorial_VERSION_MAJOR << endl;
	cout << "Minor_VERSION:" << Tutorial_VERSION_MINOR << endl;
#ifdef USE_MYMATH
	cout << "add(1+2) = " << add(1, 2) << endl; 
#endif
	return 0;
}
