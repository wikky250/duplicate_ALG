#pragma once
#include "AllRelayHead.h"
#include <QObject>

class InitFunction : public QObject
{
	Q_OBJECT

public:
	InitFunction(QObject *parent);
	~InitFunction();

	int TypeOFCamera;
	bool StartModel;
private:
	//QVector<CAMERASTRUCT*> m_vectorCamera;
public:
	bool GetAllCamera();
	int ReadConfig();
};
