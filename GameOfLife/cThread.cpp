#include "cThread.h"
DEFINE_EVENT_TYPE(wxEVT_MYTHREAD)

cThread::cThread(wxEvtHandler* pParent, cGame* game, bool* threadAlive, wxButton* bStart) : wxThread(wxTHREAD_DETACHED), m_pParent(pParent)
{
	m_game = game;
	m_threadAlive = threadAlive;
	m_bStart = bStart;
}

cThread::~cThread()
{
	//wxCriticalSectionLocker enter(m_pHandler->m_pThreadCS);
	// the thread is being destroyed; make sure not to leave dangling pointers around
	//m_pHandler->m_pThread = NULL;
	//delete[] m_game;
}

void* cThread::Entry()
{
	wxCommandEvent evt(wxEVT_MYTHREAD, GetId());

	m_game->startGame();
	
	//delete[] m_game;

	//delete m_pParent;

	if (*m_threadAlive) {
		m_bStart->SetLabel("Start");
	}

	// communicate termination
	*m_threadAlive = false;

	return 0;
}

