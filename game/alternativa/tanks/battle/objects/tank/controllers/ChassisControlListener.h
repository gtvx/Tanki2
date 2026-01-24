#ifndef CHASSISCONTROLLISTENER_H
#define CHASSISCONTROLLISTENER_H

class ChassisControlListener
{
public:
	 virtual void onChassisControlChanged(int, bool) = 0;
};

#endif // CHASSISCONTROLLISTENER_H
