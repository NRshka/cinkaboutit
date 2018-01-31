#ifndef _PAIR_H
#define _PAIR_H

namespace Pair {
	template<class K, class T>
	class pair {
	public:
		K first;
		T second;
		pair() {

		}
		pair(K one, T two) :first(one), second(two) {

		}
		pair(const pair& obj) {
			first = obj.first;
			second = obj.second;
		}
		pair& operator=(pair& x) {
			first = x.first;
			second = x.second;
			return *this;
		}
	};
}
#endif // !_PAIR_H
