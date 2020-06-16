#pragma once
#include <Windows.h>
#include <process.h>
#include <iostream>
#include<winsock.h>
#pragma comment(lib,"ws2_32.lib")
#include "comm.h"


typedef void (*MESSAGE_HANDLER) (void* context, DataToPC_typ);

class Socket_CPP
{
private:
public:
	Socket_CPP();
	~Socket_CPP();
	char* m_cip;
	int m_iport;
	SOCKADDR_IN server_addr;
	DataToPC_typ *m_Dmsg_ToPC;
	DataFromPC_typ	*m_Dmsg_FromPC;
	HANDLE h_THeartBeat;
	bool initialization();
	bool connectServer(const char*, int);
	bool disconnect();
	bool StartWork();
	bool StopWork();
	bool InitWork();
	bool ResetError();
	bool SetResult(int counter, unsigned int alarm[4]);
	void set_message_handler(MESSAGE_HANDLER, void*);
	bool Communicate_PLC(DataFromPC_typ*, DataToPC_typ*);
};


