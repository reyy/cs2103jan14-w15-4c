#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include <QtCore/QTimer>
#include <vector>
#include <queue>
#include "mariauirolling.h"
#include "mariaTask.h"

class displayPack{
private:
	float _destinationX;
	float _destinationY;
	float _x;
	float _y;

public:
	QMainWindow *_parent;

	QLabel *displayTitle;
	QString backgroundColor;

	displayPack(QMainWindow *parent);
	displayPack(QMainWindow *parent, QString title,int type, time_t start,time_t end);
	~displayPack();

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

class MariaUICalendar : public MariaUIRolling {
public:
	enum VIEW_TYPE {
		DAY, WEEK, MONTH, YEAR
	};
private:
	QMainWindow *_parent;
	std::vector<displayPack*> _displayPackStack;
	std::queue<displayPack*> _queuedisplayQueue;

	void updateStateMainAnimation();
	void clearQueueDisplay();

public:
	MariaUICalendar(QMainWindow *parent);
	~MariaUICalendar();

	void addTask(const MariaTask task);
	void clearActiveDisplay();
	void updateGUI();
};

