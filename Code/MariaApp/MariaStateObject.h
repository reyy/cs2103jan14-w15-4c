#pragma once
#include <QtWidgets/QMainWindow>

class MariaStateObject {
public:
	static const float FLOW_FACTOR;
	static const float VALUE_THRESHOLD;

private:
	QMainWindow* _qmainWindow;

	QPointF _position;
	QPointF _destination;

	void setStartDestination();
	void setEndDestination();
	bool updatePosition();
	friend class MariaStateManager;
protected:
	bool _transitionAuto;

	QPointF getPosition();

	virtual void initBeginState();
	virtual void initActiveState();
	virtual void initEndState();
	
	//Timer functions that continuously run to update UI.
	//Inherited child shall return a false for MariaStateManager to stop updating.
	virtual bool timerBeginState();
	virtual bool timerActiveState();
	virtual bool timerEndState();

public:
	MariaStateObject(QMainWindow* qmainWindow);
	~MariaStateObject();
};

