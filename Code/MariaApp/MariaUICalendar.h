#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include <QtCore/QTimer>
#include <vector>
#include <queue>
#include "MariaUIRolling.h"
#include "MariaTask.h"
#include "MariaTime.h"
#include "MariaUIDisplayPack.h"

class MariaUICalendar : public MariaUIRolling {
public:
	enum VIEW_TYPE {
		DEFAULT, DAY, WEEK, MONTH, YEAR
	};
private:
	QMainWindow *_parent;
	QLabel *_currentTimeLine;

	QPixmap *_currentTimeLineImage;
	QPixmap *_timeLineImage;

	std::vector<MariaUIDisplayPack*> _displayPackStack;
	std::queue<MariaUIDisplayPack*> _queuedisplayQueue;
	
	std::vector<QLabel *> _lineStack;
	std::vector<QLabel *> _lineTimerStack;

	//The amount of pixel representing 1 unit.
	float _calendarUnit;
	
	void initImages();
	void addLine(int amount);
	void updateStateMainAnimation();
	void clearQueueDisplay();

public:
	MariaUICalendar(QMainWindow *parent);
	~MariaUICalendar();

	void createUI(VIEW_TYPE type);
	void addDisplay(MariaTask task);
	void clearActiveDisplay();
	void updateGUI();
};

