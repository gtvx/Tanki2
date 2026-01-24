#ifndef DUMMYSHELLSTATES_H
#define DUMMYSHELLSTATES_H

#include "ShellStates.h"

class DummyShellStates : public ShellStates
{
public:

	static const DummyShellStates *INSTANCE;

	DummyShellStates();

	void start(const Vector3 *, const Vector3 *) override;
	void updateState(const Vector3 *, const Vector3 *, int = 666666) override;
	void savePrevPosition() override;
	void destroy() override;
};

#endif // DUMMYSHELLSTATES_H
