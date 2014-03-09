#pragma once
#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include "MariaTask.h"

class MariaUITask{
private:
	float _destinationX;
	float _destinationY;
	float _x;
	float _y;

	QMainWindow *_parent;
	QLabel *_displayTitle;
	float _calendarUnit;

public:
	MariaUITask(QMainWindow *parent, MariaTask task, float calendarUnit);
	~MariaUITask();

	void setDestinationX(float x);
	void setDestinationY(float y);
	void setRealX(float x);
	void setRealY(float y);

	bool isCoordinateMatch();

	//Updates the position of x and y to the destination x and y.
	//@returns false if there is nothing to update.
	bool updatePosition();
	void updateGUI(float layerX,float layerY);

	void show();
	void hide();

};
