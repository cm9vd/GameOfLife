#pragma once
#include "wx/wx.h"
#include "wx/thread.h"
#include <wx/event.h>
#include "cMain.h"
#include "cGame.h"

class cThread : public wxThread
{
public:
	cThread(wxEvtHandler* pParent, cGame* game, bool* threadAlive, wxButton* bStart);
	~cThread();

private:
	cGame* m_game = nullptr;
	bool* m_threadAlive;

	wxButton* m_bStart = nullptr;

private:
	void* Entry();

protected:
	wxEvtHandler* m_pParent;
};

