#pragma once
#include <assert.h>
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

private:
	StateType _currentState;
	StateTransition _currentTransition;
	MariaStateObject* _currentStateObject;
	std::queue<StateType> _stateQueue;
	std::queue<MariaStateObject*> _stateQueueObject;

	//A flag to activate endUpdate right after active starts.
	bool _transitCalled;

	QTimer *_stateBeginTimer;
	QTimer *_stateActiveTimer;
	QTimer *_stateEndTimer;

	void initBeginState();
	void initActiveState();
	void initEndState();

private slots:
	void timerBeginState();
	void timerActiveState();
	void timerEndState();

public:
	MariaStateManager();
	~MariaStateManager();
	
	void queueState(StateType type, MariaStateObject* stateObject);
	void transitState();
	StateType getCurrentState();
	StateTransition getTransition();
	void clearQueuedState();
	MariaStateObject* getCurrentStateObject();

	/*
	Recall the activeUpdate timer for the current MariaStateObject again.
	This function is used when the activeUpdate function needs to be called
	again.
	
	Note: Custom functions (e.g. MariaUIStateDisplay) does not update through this function.
	They have to be called manually by their respective update functions unless they are also
	part of activeUpdate.
	*/
	bool runActiveUpdate();
};

