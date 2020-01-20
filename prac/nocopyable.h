#ifndef NOCOPYABLE_H_
#define NOCOPYABLE_H_

namespace sone
{

class nocopyable{
protected:
	nocopyable() = default;
	~nocopyable() = default;
	nocopyable(const nocopyable&) = delete;
	nocopyable& operator=(const nocopyable&) = delete;
};

}

#endif
