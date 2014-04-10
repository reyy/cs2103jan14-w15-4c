#pragma once

#include <algorithm> 
#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include "MariaStateObject.h"
#include "MariaUITask.h"
#include "MariaTask.h"

class MariaUIStateDisplay : public MariaStateObject {
public:
	static const int TEXTBOX_X_OFFSET = 30;
	static const float TITLE_Y_OFFSET;
	static const float TITLE_WIDTH;
	static const float TITLE_HEIGHT;
	static const float FONT_SIZE;

protected:
	QMainWindow *_qmainWindow;
	int _maxTaskDisplay;

private:
	std::vector<MariaUITask*> _taskStack;
	std::vector<MariaUITask*> _taskDisposeStack;

	float _taskStartHeight;
	int _page;
	QLabel *_pageText;

public:
	MariaUIStateDisplay(QMainWindow* qmainWindow, float taskStartHeight, int maxTaskDisplay);
	~MariaUIStateDisplay();

	MariaUITask* addUITask(MariaTask *task, MariaUITask::DISPLAY_TYPE type);
	void updateUITaskPosition();
	void updateUITaskNumber();
	MariaUITask* eraseUITask(int index);
	MariaUITask* eraseUITask(MariaTask* task);
	void eraseAllUITask();
	void clearUITask();
	int getTotalUITask();
	bool isAllTaskAtLocation();
	
	void setPage(int page);
	void setPageEnd();
	int getPage();
	bool isPageValid(int page);
	void updatePage();
	void updateTitleText();
	void updatePageTitleGUI();

	//Inheritable function to update GUI if there is any UITask operations.
	virtual void updateGUI();
};