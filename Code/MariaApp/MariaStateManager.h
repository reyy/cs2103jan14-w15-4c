//@author A0111784H
#pragma once

#include <queue>
#include <QtCore/QTimer>
#include <QtWidgets/QMainWindow>
#include "MariaStateObject.h"
#include "MariaUIStateType.h"

class MariaStateManager : public QWidget {
	 Q_OBJECT

public:
	typedef enum {
		BEGIN, 
		ACTIVE, 
		END
	} StateTransition;

	MariaStateManager();
	~MariaStateManager();
	
	void queueState(StateType type, MariaStateObject* stateObject);
	void transitState();
	StateType getCurrentState();
	StateTransition getTransition();
	void clearQueuedState();
	MariaStateObject* getCurrentStateObject();
	bool runActiveUpdate();

private:
	static const int STATE_UPDATE_FREQUENCY = 1;
	StateType _currentState;
	StateTransition _currentTransition;
	MariaStateObject* _currentStateObject;
	std::queue<StateType> _stateQueue;
	std::queue<MariaStateObject*> _stateQueueObject;

	bool _transitCalled;

	QTimer* _stateBeginTimer;
	QTimer* _stateActiveTimer;
	QTimer* _stateEndTimer;

	void initBeginState();
	void initActiveState();
	void initEndState();

private slots:
	void timerBeginState();
	void timerActiveState();
	void timerEndState();
};

