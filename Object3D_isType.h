#ifndef OBJECT3D_ISTYPE_H
#define OBJECT3D_ISTYPE_H

template <typename T>
bool Object3D_isTypeFind(Object3D* obj) {
	return dynamic_cast<T*>(obj) != nullptr;
}

template <typename T>
bool Object3D_isType(Object3D* obj) {
	return typeid(*obj) == typeid(T);
}

#endif // OBJECT3D_ISTYPE_H
