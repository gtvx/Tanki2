#ifndef GAMEACTIONLISTENER_H
#define GAMEACTIONLISTENER_H

enum class GameActionEnum;

class GameActionListener
{
public:
	 virtual void onGameAction(GameActionEnum, bool) = 0;
};

#endif // GAMEACTIONLISTENER_H
