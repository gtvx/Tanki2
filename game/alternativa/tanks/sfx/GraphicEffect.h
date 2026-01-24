#ifndef GRAPHICEFFECT_H
#define GRAPHICEFFECT_H

class Scene3DContainer;
class GameCamera;

class GraphicEffect
{
public:
	virtual void addedToScene(Scene3DContainer*) = 0;
	virtual bool play(int, GameCamera*) = 0;
	virtual void destroy() = 0;
	virtual void kill() = 0;
};

#endif // GRAPHICEFFECT_H
