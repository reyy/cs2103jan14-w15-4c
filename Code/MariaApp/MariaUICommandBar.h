#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/qlabel.h>
#include <QtCore/QTimer>
#include "MariaUITextbox.h"
#include "MariaUIStatus.h"

class MariaUICommandBar : public QWidget {
	Q_OBJECT

public:
	static const int GUI_UPDATE_FREQUENCY = 1;
	static const float DEFAULT_X_POSITION;
	static const float DEFAULT_Y_POSITION;
	static const float FLOW_FACTOR;
	static const float VALUE_THRESHOLD;

	MariaUICommandBar(QMainWindow *qmainWindow);
	~MariaUICommandBar();

	float getPosition();
	void setDestination(float yPosition);

	MariaUITextbox* getTextbox();
	MariaUIStatus* getStatus();

protected slots:
	void updateGUI();

private:
	QMainWindow *_qmainWindow;
	MariaUITextbox *_textbox;
	MariaUIStatus *_status;

	QTimer *_updateGUITimer;

	float _yPosition;
	float _yDestination;
};

