#ifndef UVFRAME_H
#define UVFRAME_H

class UVFrame
{
public:

	double topLeftU;
	double topLeftV;
	double bottomRightU;
	double bottomRightV;

	UVFrame(double topLeftU,
			double topLeftV,
			double bottomRightU,
			double bottomRightV)
	{
		this->topLeftU = topLeftU;
		this->topLeftV = topLeftV;
		this->bottomRightU = bottomRightU;
		this->bottomRightV = bottomRightV;
	}
};

#endif // UVFRAME_H
