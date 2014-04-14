//@author A0111784H
#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include "MariaStateObject.h"
#include "MariaUITask.h"
#include "MariaTask.h"

class MariaUIStateDisplay : public MariaStateObject {
public:
	MariaUIStateDisplay(QMainWindow* qmainWindow, float taskStartHeight, int maxTaskDisplay);
	~MariaUIStateDisplay();

	MariaUITask* addUITask(MariaTask *task, MariaUITask::DisplayType type);
	void updateUITaskPosition();
	void updateUITaskNumber();
	MariaUITask* eraseUITask(int index);
	MariaUITask* eraseUITask(MariaTask* task);
	void eraseAllUITask();
	void clearUITask();
	int getTotalUITask();
	bool isAllTaskAtLocation();
	
	int getMaxTaskDisplay();

	bool setPage(int page);
	void setPageEnd();
	int getPage();
	bool isPageValid(int page);
	void updatePage();
	void updatePageTitleGUI();

	//Inheritable function to update GUI if there is any UITask operations.
	virtual void updateGUI();

protected:
	QMainWindow *_qmainWindow;
	int _maxTaskDisplay;

private:
	static const int TEXTBOX_X_OFFSET = 30;
	static const string MESSAGE_ITEM_ABOVE;
	static const string MESSAGE_ITEMS_ABOVE;
	static const string MESSAGE_ITEM_BELOW;
	static const string MESSAGE_ITEMS_BELOW;
	static const float TITLE_Y_OFFSET;
	static const float TITLE_WIDTH;
	static const float TITLE_HEIGHT;
	static const float FONT_SIZE;

	std::vector<MariaUITask*> _taskStack;
	std::vector<MariaUITask*> _taskDisposeStack;

	float _taskStartHeight;
	int _page;
	QLabel *_pageText;
	void updateTitleText();
};