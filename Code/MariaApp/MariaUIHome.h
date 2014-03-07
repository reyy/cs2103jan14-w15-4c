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

class MariaUIHome : public MariaUIRolling {
private:
	QMainWindow *_parent;
	QLabel *_currentTime;

	std::vector<MariaUIDisplayPack*> _displayPackStack;
	std::queue<MariaUIDisplayPack*> _queuedisplayQueue;
	
	std::vector<QLabel *> _lineStack;
	std::vector<QLabel *> _lineTimerStack;
	
	void initImages();
	void updateStateMainAnimation();
	void clearQueueDisplay();

public:
	MariaUIHome(QMainWindow *parent);
	~MariaUIHome();

	void createUI();
	void addDisplay(MariaTask task);
	void clearActiveDisplay();
	void updateGUI();
};

