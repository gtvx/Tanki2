#ifndef TWINSMODELBASE_H
#define TWINSMODELBASE_H

#include "entrance/platform/client/fp10/core/model/impl/Model.h"

class TwinsCC;

class TwinsModelBase : public Model
{
	Long *modelId;
	Long *_fireId;
	Long *_fireDummyId;


public:
	TwinsModelBase();

protected:
	TwinsCC* getInitParam();

private:
	void initCodecs();
};

#endif // TWINSMODELBASE_H
