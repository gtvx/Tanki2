#ifndef ISPACE_H
#define ISPACE_H

class Long;
class IGameObject;
class IGameClass;
class QString;

class ISpace
{
public:
	virtual Long getId() = 0;
	//ICommandSender* commandSender();
	virtual IGameObject* getObject(const Long &id) = 0;
	virtual IGameObject* getRootObject() = 0;
	virtual IGameObject* createObject(const Long &id, IGameClass*, const QString &name) = 0;
	virtual void destroyObject(const Long &id) = 0;
	//function get objects():Vector.<IGameObject>;
	virtual void close() = 0;
	//function addEventListener(_arg_1:ISpaceListener):void;
	//function removeEventListener(_arg_1:ISpaceListener):void;
};

#endif // ISPACE_H
