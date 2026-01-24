#include "TwinsModelBase.h"
#include "entrance/alternativa/types/Long.h"


TwinsModelBase::TwinsModelBase()
{
	//this->client = ITwinsModelBase(this);
	this->modelId = Long::getLong(1952266263, -1912192267);
	this->_fireId = Long::getLong(823399156, 0x14F11500);
	this->_fireDummyId = Long::getLong(1289133603, -862527368);
	this->initCodecs();
}


void TwinsModelBase::initCodecs()
{

}


TwinsCC* TwinsModelBase::getInitParam()
{
	return initParams<TwinsCC>(Model::object()).get();
}
