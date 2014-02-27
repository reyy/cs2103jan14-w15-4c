#pragma once

#include <QtWidgets/qlabel.h>
#include <QtCore/QTimer>
#include <QtWidgets/QMainWindow>

#define AMOUNT_OF_DOTS 5
#define LOGO_YOFFSET 100
#define DOTS_XOFFSET 10
#define DOTS_XSEPARATE 50
#define DOTS_XSPEED 1

class MariaUILoading : QWidget {
	Q_OBJECT
public:
	enum STATE_TYPE {
		BEFORE, DURING, AFTER, DONE
	};
private:
	QMainWindow *_parent;
	
	QPixmap *_imageHandleLogo;
	QPixmap *_imageHandleDots;
	
	QTimer *_loadingAnimationTimer;
	QLabel *_logo;
	float _logoYPos;						//Y position of logo.
	
	QLabel *_loadingDots[AMOUNT_OF_DOTS];
	float _dotsXPos[AMOUNT_OF_DOTS];		//X position of dots.
	
	STATE_TYPE _currentState;
	
	bool _toEndAnimation;					//Flag to queue for animation end.

private slots:
	void updateStateAnimation();

public:
	MariaUILoading(QMainWindow *parent);
	~MariaUILoading();

	//Force UI Class to update values prematurelly.
	void updateGUI();
	bool isAnimationDone();

	//Call the function before loading commence.
	void startLoadingAnimation();

	//Call the function after loading has been completed.
	void endLoadingAnimation();
};

