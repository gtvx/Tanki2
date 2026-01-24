#ifndef QHASH_STD_STRING_H
#define QHASH_STD_STRING_H

inline uint qHash(const std::string &s, uint seed = 0) noexcept {
	std::size_t h = std::hash<std::string>{}(s);
	return static_cast<uint>(h) ^ seed;
}

#endif // QHASH_STD_STRING_H
