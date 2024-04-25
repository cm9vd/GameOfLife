#pragma once
#include <wx/wfstream.h>
#include <wx/file.h>
#include <wx/string.h>

#include "wx/wx.h"

#include "Constants.h"
#include "cCanvas.h"
#include "cGame.h"
#include "cThread.h"
#include "wx/thread.h"

// Thread stuff
wxDECLARE_EVENT(wxEVT_COMMAND_CTHREAD_COMPLETED, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_COMMAND_CTHREAD_UPDATE, wxThreadEvent);

class cMain : public wxFrame
{
public:
	cMain();
	~cMain();

private:

	wxMenu* menuDesign = nullptr;

	wxToolBar* m_ToolBar = nullptr;
	wxMenuBar* m_MenuBar = nullptr;

	wxStatusBar* m_StatusBar = nullptr;
	wxSlider* m_ZoomSlider = nullptr;

	wxButton* bStart = nullptr;

	cCanvas* m_Canvas = nullptr;
	int* m_pHabitat = nullptr;
	int* save_pHabitat = nullptr;
	int height = 30;
	int width = 47;

	bool showStart = true;
	cGame* m_Game = nullptr;

	wxThread* m_pThread;

	bool threadAlive = false;

	// File Menu
	void OnMenuNew(wxCommandEvent& evt);
	void OnMenuOpen(wxCommandEvent& evt);
	void OnMenuSave(wxCommandEvent& evt);
	void OnMenuSaveToFile(wxCommandEvent& evt);
	void OnMenuExit(wxCommandEvent& evt);

	// Design Menu
	void OnMode(wxCommandEvent& evt);
	void OnGrid(wxCommandEvent& evt);

	// Color Menu
	void OnColorForeground(wxCommandEvent& evt);

	// Speed Menu
	void OnMenuSpeed(wxCommandEvent& evt);

	void OnZoomChange(wxCommandEvent& evt);

	// Toolbar buttons
	void OnClear(wxCommandEvent& evt);
	void OnReset(wxCommandEvent& evt);
	void OnStart(wxCommandEvent& evt);
	void OnBack(wxCommandEvent& evt);
	void OnNext(wxCommandEvent& evt);

	wxDECLARE_EVENT_TABLE();
	

	// Thread stuff
//public:
	void killThread();
	void OnClose(wxCloseEvent& evt);

protected:
	
	wxCriticalSection m_pThreadCS;  // protects the m_pThread pointer

	//friend class cThread;
};

