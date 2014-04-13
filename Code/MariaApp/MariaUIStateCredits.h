#pragma once

#include <string>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include <QtCore/QTimer>
#include "MariaStateObject.h"

using namespace std;

class MariaUIStateCredits : public MariaStateObject {
public:
	MariaUIStateCredits(QMainWindow* qmainWindow);
	~MariaUIStateCredits();

	void setDoneAnimating();

private:
	static const int LOGO_SPEED_RETARDER = 80;
	static const int FONT_SIZE = 18;
	static const int FONT_SIZE_BODY = 14;
	static const string CREDITS_STRING;
	static const string CREDITS_STRING_BODY;
	static const float FLOW_FACTOR;
	static const float VALUE_THRESHOLD;
	static const float DISPLAY_TEXT_WIDTH;
	static const float DISPLAY_TEXT_HEIGHT;
	static const float DISPLAYBODY_TEXT_WIDTH;
	static const float DISPLAYBODY_TEXT_HEIGHT;
	static const float LOGO_START_Y;
	static const float LOGO_STAGE_Y_SCALE;
	static const float TEXT_STAGE_Y_SCALE;
	static const float TEXTBODY_STAGE_Y_SCALE;
	
	QMainWindow *_qmainWindow;
	
	QLabel *_displayText;
	QLabel *_displayTextBody;
	QLabel *_logo;
	int _logoImageIndex;
	bool _doneAnimating;

	void initBeginState();
	void initActiveState();
	void initEndState();
	bool timerBeginState();
	bool timerActiveState();
	bool timerEndState();
	
	void animateLogo();
	void updateGUIPosition();
};

