#include "SpeedCharacteristicsModelBase.h"


SpeedCharacteristicsModelBase::SpeedCharacteristicsModelBase()
{

}


SpeedCharacteristicsCC* SpeedCharacteristicsModelBase::getInitParam()
{
	return initParams<SpeedCharacteristicsCC>(Model::object()).get();
}
