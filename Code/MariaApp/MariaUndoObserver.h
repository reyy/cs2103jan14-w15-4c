#pragma once

class MariaTaskInterface {};

class MariaUndoObserver{
public:
	virtual void notifyAction(MariaTaskInterface* task, bool isAddTask = false) = 0;
};
