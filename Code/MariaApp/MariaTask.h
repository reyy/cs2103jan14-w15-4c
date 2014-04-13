//@author A0111823U
#pragma once

#include <functional>
#include <string>
#include "MariaTime.h"
#include "MariaUndoObserver.h"

using namespace std;

class MariaTask : public MariaTaskInterface {
public:
	typedef enum {
		FLOATING, 
		DEADLINE, 
		TIMED,
		INVALID
	} TaskType;
	
	MariaTask(string title = "", MariaTime *start = NULL, MariaTime* end = NULL);
	MariaTask(string title, string description, MariaTime* start = NULL, MariaTime* end = NULL, bool isDone = false);
	~MariaTask();
	
	
	TaskType	getType();
	string		getTitle();
	string		getDescription();
	string		getTimeFromNow();
	MariaTime*	getStart();
	MariaTime*	getEnd();
	MariaTime*	getCreated();
	double		getDuration();
	MariaTask*	getClone();
	bool		getIsDone();
	
	void setTitle(string);
	void setDescription(string);
	void setStart(MariaTime*);
	void setEnd(MariaTime*);
	void setCreated(MariaTime*);
	void setIsDone(bool);
	void setAll(MariaTask*);

	void refreshTaskType();

	bool operator<(MariaTask rhs);

	static void initObserver(MariaUndoObserver*);
	

private:
	TaskType	type;
	string		title;
	string		description;
	MariaTime*	start;
	MariaTime*	end;
	MariaTime*	created;
	bool		isDone;

	static MariaUndoObserver *observer;
};
