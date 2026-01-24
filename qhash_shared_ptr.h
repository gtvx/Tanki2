#ifndef QHASH_SHARED_PTR_H
#define QHASH_SHARED_PTR_H

#include <QHash>
#include <memory>

template<typename T>
inline uint qHash(const std::shared_ptr<T> &ptr, uint seed = 0) noexcept {
	return qHash(reinterpret_cast<quintptr>(ptr.get()), seed);
}

template<typename T>
inline bool operator==(const std::shared_ptr<T> &a, const std::shared_ptr<T> &b) noexcept {
	return a.get() == b.get();
}

#endif // QHASH_SHARED_PTR_H
