#include "DummyShellStates.h"

const DummyShellStates *DummyShellStates::INSTANCE = new DummyShellStates();


DummyShellStates::DummyShellStates() :
	ShellStates(nullptr)
{

}


void DummyShellStates::start(const Vector3*, const Vector3*)
{
	//qDebug("DummyShellStates::start");
}


void DummyShellStates::updateState(const Vector3*, const Vector3*, int)
{

}


void DummyShellStates::savePrevPosition()
{

}


void DummyShellStates::destroy()
{

}
