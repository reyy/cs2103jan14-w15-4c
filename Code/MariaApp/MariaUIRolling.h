#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include <QtCore/QTimer>

#define DEFAULT_BEFORE_X 1
#define DEFAULT_BEFORE_Y 0
#define DEFAULT_DURING_X 0
#define DEFAULT_DURING_Y 0
#define DEFAULT_AFTER_X -1
#define DEFAULT_AFTER_Y 0

//The rolling class hold a coordinate _rollingXPos and _rollingYPos
//representing the middle center of the current window.
//
//Inherited class can then add in more images to this layer for
//transition.
//Inherited class should update its coordinate relative to rollingX
//rollingY to take advantage of the animation it provides.
//
/*
Default variables defined the position of the layer in
between transition.
Numbers in these values are a percentage of the overall
width and height.
E.G 
_parent->width()*DEFAULT_BEFORE_X;
 A value of 0 makes the layer centered in the window.
 A value of 1 makes the layer on the right side of the window.
*/

class MariaUIRolling : QWidget {
	Q_OBJECT
public:
	enum STATE_TYPE {
		BEFORE, DURING, AFTER, DONE, DEACTIVE
	};
private:
	QMainWindow *_parent;
	STATE_TYPE _currentState;
	QTimer *_preAnimationTimer;
	QTimer *_mainAnimationTimer;
	QTimer *_posAnimationTimer;

	float _rollingXPos;
	float _rollingYPos;

	bool _toEndAnimation;					//Flag to queue for animation end.

protected:
	float getRollingX();
	float getRollingY();
	void startMainAnimationTimer();
	void stopMainAnimationTimer();
	STATE_TYPE getCurrentState();

private slots:
	void updateStatePreAnimation();
	void updateStatePosAnimation();

protected slots:
	virtual void updateStateMainAnimation();

public:
	MariaUIRolling(QMainWindow *parent);
	~MariaUIRolling();

	virtual void updateGUI();

	bool startRolling();
	bool isStartRollingDone();
	bool endRolling();
	bool isEndRollingDone();
};

