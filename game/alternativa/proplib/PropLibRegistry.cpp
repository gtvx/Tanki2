#include "PropLibRegistry.h"
#include "PropLibrary.h"


PropLibRegistry::PropLibRegistry()
{

}

void PropLibRegistry::addLibrary(PropLibrary *lib)
{
	libs[lib->getName()] = lib;
}

PropLibrary* PropLibRegistry::getLibrary(const QString &libName)
{
	return libs[libName];
}
