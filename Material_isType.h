#ifndef MATERIAL_ISTYPE_H
#define MATERIAL_ISTYPE_H

template <typename T>
bool Material_isTypeFind(Material* obj) {
	return dynamic_cast<T*>(obj) != nullptr;
}

template <typename T>
bool Material_isType(Material* obj) {
	return typeid(*obj) == typeid(T);
}

#endif // MATERIAL_ISTYPE_H
