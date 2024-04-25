#include "cMain.h"

wxBEGIN_EVENT_TABLE(cMain, wxFrame)
EVT_MENU(MenuNewSmallID, cMain::OnMenuNew)
EVT_MENU(MenuNewMediumID, cMain::OnMenuNew)
EVT_MENU(MenuNewLargeID, cMain::OnMenuNew)
EVT_MENU(MenuNewEnormousID, cMain::OnMenuNew)

EVT_MENU(MenuFileOpenID, cMain::OnMenuOpen)
EVT_MENU(MenuSaveID, cMain::OnMenuSave)
EVT_MENU(MenuFileSaveID, cMain::OnMenuSaveToFile)
EVT_MENU(MenuExitID, cMain::OnMenuExit)

EVT_MENU(DesignLightID, cMain::OnMode)
EVT_MENU(DesignDarkID, cMain::OnMode)

EVT_MENU(DesignGridID, cMain::OnGrid)

EVT_MENU(ColorDefaultID, cMain::OnColorForeground)
EVT_MENU(ColorWhiteID, cMain::OnColorForeground)
EVT_MENU(ColorRedID, cMain::OnColorForeground)
EVT_MENU(ColorGreenID, cMain::OnColorForeground)
EVT_MENU(ColorBlueID, cMain::OnColorForeground)
EVT_MENU(ColorPurpleID, cMain::OnColorForeground)
EVT_MENU(ColorOrangeID, cMain::OnColorForeground)
EVT_MENU(ColorYellowID, cMain::OnColorForeground)

EVT_MENU(Speed025ID, cMain::OnMenuSpeed)
EVT_MENU(Speed050ID, cMain::OnMenuSpeed)
EVT_MENU(Speed100ID, cMain::OnMenuSpeed)
EVT_MENU(Speed200ID, cMain::OnMenuSpeed)
EVT_MENU(Speed500ID, cMain::OnMenuSpeed)
EVT_MENU(Speed1000ID, cMain::OnMenuSpeed)
EVT_MENU(Speed5000ID, cMain::OnMenuSpeed)

EVT_SLIDER(ZoomSliderID, cMain::OnZoomChange)

//Thread Stuff
EVT_CLOSE(cMain::OnClose)

wxEND_EVENT_TABLE();

wxDECLARE_EVENT(wxEVT_COMMAND_CTHREAD_UPDATE, wxThreadEvent);
wxDECLARE_EVENT(wxEVT_COMMAND_CTHREAD_COMPLETED, wxThreadEvent);



cMain::cMain() : wxFrame(nullptr, wxID_ANY, "Game of Life", wxPoint(550,220), wxSize(800, 600))
{

	// Add a menu bar
	m_MenuBar = new wxMenuBar();
	this->SetMenuBar(m_MenuBar);

	// Add Menu for file operations
	wxMenu* menuFile = new wxMenu();

	wxMenu* subMenuNew = new wxMenu();
	subMenuNew->SetTitle("Size");
	subMenuNew->Append(MenuNewSmallID, "Small");
	subMenuNew->Append(MenuNewMediumID, "Medium");
	subMenuNew->Append(MenuNewLargeID, "Large");
	subMenuNew->Append(MenuNewEnormousID, "Enormous");

	menuFile->AppendSubMenu(subMenuNew, "New");
	menuFile->Append(MenuFileOpenID, "Open");
	menuFile->Append(MenuSaveID, "Save");
	menuFile->Append(MenuFileSaveID, "Save as");
	menuFile->Append(MenuExitID, "Exit");

	// Add Menu for Design
	menuDesign = new wxMenu();

	wxMenu* subDesignMode = new wxMenu();
	subDesignMode->AppendRadioItem(DesignLightID, "Light");
	subDesignMode->AppendRadioItem(DesignDarkID, "Dark");
	menuDesign->AppendSubMenu(subDesignMode, "Mode");

	menuDesign->AppendCheckItem(DesignGridID, "Grid");
	menuDesign->Check(DesignGridID, true);

	// Add Menu for Color changes
	wxMenu* menuColor = new wxMenu();
	menuColor->AppendRadioItem(ColorDefaultID, "default");
	menuColor->AppendRadioItem(ColorWhiteID, "white");
	menuColor->AppendRadioItem(ColorRedID, "red");
	menuColor->AppendRadioItem(ColorGreenID, "green");
	menuColor->AppendRadioItem(ColorBlueID, "blue");
	menuColor->AppendRadioItem(ColorPurpleID, "purple");
	menuColor->AppendRadioItem(ColorOrangeID, "orange");
	menuColor->AppendRadioItem(ColorYellowID, "yellow");

	// Add Menu for Speed changes
	wxMenu* menuSpeed = new wxMenu();
	menuSpeed->AppendRadioItem(Speed100ID, "x 1");
	menuSpeed->AppendRadioItem(Speed200ID, "x 2");
	menuSpeed->AppendRadioItem(Speed500ID, "x 5");
	menuSpeed->AppendRadioItem(Speed1000ID, "x 10");
	menuSpeed->AppendRadioItem(Speed5000ID, "x 50");
	menuSpeed->PrependRadioItem(Speed050ID, "x 0.5");
	menuSpeed->PrependRadioItem(Speed025ID, "x 0.25");
	
	
	// Add Menus to Menu Bar
	m_MenuBar->Append(menuFile, "File");
	m_MenuBar->Append(menuDesign, "Design");
	m_MenuBar->Append(menuColor, "Color");
	m_MenuBar->Append(menuSpeed, "Speed");

	// Add statusbar with zoom slider
	m_StatusBar = this->CreateStatusBar(3, wxSTB_DEFAULT_STYLE, wxID_ANY);
	m_ZoomSlider = new wxSlider(m_StatusBar, ZoomSliderID, 15, 1, 32);

	m_StatusBar->SetStatusText(wxString("Zoom: ") << m_ZoomSlider->GetValue(), 1);
	m_StatusBar->SetStatusText(wxString("Speed: ") << 1.00, 2);


	// Add toolbar for control
	m_ToolBar = this->CreateToolBar(wxTB_HORIZONTAL, wxID_ANY);

	wxButton* bClear = new wxButton(m_ToolBar, buttonClearID, "Clear", wxDefaultPosition, wxSize(40, 24), 0);
	bClear->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(cMain::OnClear), nullptr, this);
	m_ToolBar->AddControl(bClear);

	wxButton* bReset = new wxButton(m_ToolBar, buttonResetID, "Reset", wxDefaultPosition, wxSize(40, 24), 0);
	bReset->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(cMain::OnReset), nullptr, this);
	m_ToolBar->AddControl(bReset);

	bStart = new wxButton(m_ToolBar, buttonStartID, "Start", wxDefaultPosition, wxSize(40, 24), 0);
	bStart->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(cMain::OnStart), nullptr, this);
	m_ToolBar->AddControl(bStart);

	wxButton* bBack = new wxButton(m_ToolBar, buttonBackID, "Back", wxDefaultPosition, wxSize(40, 24), 0);
	bBack->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(cMain::OnBack), nullptr, this);
	m_ToolBar->AddControl(bBack);

	wxButton* bNext = new wxButton(m_ToolBar, buttonNextID, "Next", wxDefaultPosition, wxSize(40, 24), 0);
	bNext->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(cMain::OnNext), nullptr, this);
	m_ToolBar->AddControl(bNext);

	m_ToolBar->Realize();

	// Initialize Canvas with habitat
	m_Canvas = new cCanvas(this);
	m_pHabitat = new int[height * width]{ 0 };
	save_pHabitat = new int[height * width]{ 0 };
	m_Canvas->SetHabitatData(height, width, m_pHabitat);

	m_Game = new cGame(m_Canvas);
}

cMain::~cMain()
{
	delete[] m_pHabitat;
	delete[] save_pHabitat;
	delete m_Game;
}

void cMain::OnMenuNew(wxCommandEvent& evt)
{
	// pause old habitat if running
	if (bStart->GetLabel() == "Stop") {
		bStart->SetLabel("Start");

		// stop simulation
		killThread();
	}

	delete[] m_pHabitat;
	delete[] save_pHabitat;
	switch (evt.GetId()) {
	case MenuNewLargeID:
		height = 75;
		width = 150;
		break;
	case MenuNewMediumID:
		height = 30;
		width = 47;
		break;
	case MenuNewSmallID:
		height = 15;
		width = 20;
		break;
	case MenuNewEnormousID:
		height = 150;
		width = 300;
	}
	
	m_Game->resetBack(true, height, width);
	m_pHabitat = new int[height * width]{ 0 };
	save_pHabitat = new int[height * width]{ 0 };
	m_Canvas->SetHabitatData(height, width, m_pHabitat);
	evt.Skip();
}

void cMain::OnMenuOpen(wxCommandEvent& evt)
{
	// pause old habitat if running
	if (bStart->GetLabel() == "Stop") {
		bStart->SetLabel("Start");

		// stop simulation
		killThread();
	}

	wxFileDialog dlg(this, "Open GameOfLife - Habitat File", "", "", ".glh Files (*glh)|*.glh", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK) {

		wxFileInputStream input_stream(dlg.GetPath());
		if (!input_stream.IsOk()) {
			wxLogError("Cannot open file '%s'.", dlg.GetPath());
			return;
		}
		wxFile* file = input_stream.GetFile();
		wxString* content = new wxString;
		file->ReadAll(content);

		// check file format
			// read/check and set height + width
			// first is a char (s, m, l, e)
		switch ((char)content->GetChar(0))
		{
		case 'L':
			height = 75;
			width = 150;
			break;
		case 'M':
			height = 30;
			width = 47;
			break;
		case 'S':
			height = 15;
			width = 20;
			break;
		case 'E':
			height = 150;
			width = 300;
			break;
		default:
			wxLogError("Cannot read file '%s'.", dlg.GetPath());
			return;
		}

		m_Game->resetBack(true, height, width);
		delete[] m_pHabitat;
		m_pHabitat = new int[height * width]{ 0 };

		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				m_pHabitat[j * width + i] = (int) content->GetChar(j * width + i+1) - 48;
			}

		m_Canvas->SetHabitatData(height, width, m_pHabitat);

		delete content;
	}
	evt.Skip();
}

void cMain::OnMenuSave(wxCommandEvent& evt)
{
	memcpy(save_pHabitat, m_pHabitat, height * width * sizeof(int));
	evt.Skip();
}

void cMain::OnMenuSaveToFile(wxCommandEvent& evt)
{
	wxFileDialog dlg(this, "Save as GameOfLife - Habitat File", "", "", ".glh Files (*glh)|*.glh", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (dlg.ShowModal() == wxID_OK) {

		wxFileOutputStream output_stream(dlg.GetPath());
		if (!output_stream.IsOk())
		{
			wxLogError("Cannot save current contents in file '%s'.", dlg.GetPath());
			return;
		}
		wxFile* file = output_stream.GetFile();
		char* temp = new char[height * width +2];

		switch (height)
		{
		case 150:
			temp[0] = 'E';
			break;
		case 75:
			temp[0] = 'L';
			break;
		case 30:
			temp[0] = 'M';
			break;
		case 15:
			temp[0] = 'S';
			break;
		}

		for (int i = 0; i < width; i++)
			for (int j = 0; j < height; j++)
			{
				temp[j * width + i +1] = '0'+ m_pHabitat[j * width + i];
			}

		// terminate string
		temp[height * width + 1] = '\0';
		const char* help = temp;
		wxString content = wxString::FromUTF8(help);

		//wxString content = wxString(temp, height * width + 2);
		file->Write(content);

		delete[] temp;
	}
	evt.Skip();
}

void cMain::OnMenuExit(wxCommandEvent& evt)
{
	killThread();
	Close();
	evt.Skip();
}

void cMain::OnMode(wxCommandEvent& evt)
{
	switch (evt.GetId())
	{
	case DesignLightID:
		m_Canvas->SetMode(true);
		break;
	case DesignDarkID:
		m_Canvas->SetMode(false);
		break;
	}
}

void cMain::OnGrid(wxCommandEvent& evt)
{
	m_Canvas->SetGrid();
}

void cMain::OnColorForeground(wxCommandEvent& evt)
{
	//10011 - 10018
	m_Canvas->SetColour(evt.GetId() - 10010); // decode id to color
	evt.Skip();
}

void cMain::OnMenuSpeed(wxCommandEvent& evt)
{
	switch (evt.GetId())
	{
	case Speed025ID:
		m_Game->setTime(2000);
		m_StatusBar->SetStatusText(wxString("Speed: ") << 0.25, 2);
		break;
	case Speed050ID:
		m_Game->setTime(1000);
		m_StatusBar->SetStatusText(wxString("Speed: ") << 0.50, 2);
		break;
	case Speed100ID:
		m_Game->setTime(500);
		m_StatusBar->SetStatusText(wxString("Speed: ") << 1.00, 2);
		break;
	case Speed200ID:
		m_Game->setTime(250);
		m_StatusBar->SetStatusText(wxString("Speed: ") << 2.00, 2);
		break;
	case Speed500ID:
		m_Game->setTime(100);
		m_StatusBar->SetStatusText(wxString("Speed: ") << 5.00, 2);
		break;
	case Speed1000ID:
		m_Game->setTime(50);
		m_StatusBar->SetStatusText(wxString("Speed: ") << 10.00, 2);
		break;
	case Speed5000ID:
		m_Game->setTime(10);
		m_StatusBar->SetStatusText(wxString("Speed: ") << 50.00, 2);
		break;
	}
}

void cMain::OnZoomChange(wxCommandEvent& evt)
{
	m_StatusBar->SetStatusText(wxString("Zoom: ") << m_ZoomSlider->GetValue(), 1);
	m_Canvas->SetPixelSize(m_ZoomSlider->GetValue());
	evt.Skip();
}

void cMain::OnClear(wxCommandEvent& evt)
{
	// stop simulation
	killThread();
	if (bStart->GetLabel() == "Stop") {
		bStart->SetLabel("Start");
	}

	m_Game->resetBack(false);
	delete[] m_pHabitat;
	m_pHabitat = new int[height * width]{ 0 };
	m_Canvas->SetHabitatData(height, width, m_pHabitat);
	
	
}

void cMain::OnReset(wxCommandEvent& evt)
{
	// stop simulation
	killThread();
	if (bStart->GetLabel() == "Stop") {
		bStart->SetLabel("Start");
	}

	m_Game->resetBack(false);
	memcpy(m_pHabitat, save_pHabitat, height * width * sizeof(int));
	m_Canvas->SetHabitatData(height, width, m_pHabitat);
}

void cMain::OnStart(wxCommandEvent& evt)
{
	
	if (bStart->GetLabel() == "Start") {
		bStart->SetLabel("Stop");

		// start simulation
		m_Game->setRunGame(true);
		m_Game->setHabitat(height, width, m_pHabitat);
		
		// check if new Thread necessary
		if (threadAlive) {
			if (m_pThread->Resume() != wxTHREAD_NO_ERROR)
				wxLogError("Can't resume the thread!");
		}
		else {
			m_pThread = new cThread(this, m_Game, &threadAlive, bStart);
			m_pThread->Create();

			if (m_pThread->Run() != wxTHREAD_NO_ERROR) {
				wxLogError("Can't create the thread!");
				delete m_pThread;
				m_pThread = NULL;
			}

			threadAlive = true;
		}
		
	}
	else {
		bStart->SetLabel("Start");

		// stop simulation
		//m_Game->setRunGame(false);

		
		/*// Pause Thread
		if (threadAlive) {
			if (m_pThread->Pause() != wxTHREAD_NO_ERROR)
				wxLogError("Can't pause the thread!");*/
		
		killThread();
		
	}
}

void cMain::OnBack(wxCommandEvent& evt)
{
	m_Game->back();
}

void cMain::OnNext(wxCommandEvent& evt)
{
	if (bStart->GetLabel() == "Start") {
		m_Game->setHabitat(height, width, m_pHabitat);
		m_Game->oneStep();
	}
}


void cMain::killThread()
{
	m_Game->setRunGame(false);
	{
		wxCriticalSectionLocker enter(m_pThreadCS);
		if (threadAlive)         // does the thread still exist?
		{
			// set to false, so that the thread skips button change
			threadAlive = false;
			wxMessageOutputDebug().Printf("cMain: deleting thread");
			if (m_pThread->Delete() != wxTHREAD_NO_ERROR)
				wxLogError("Can't delete the thread!");
		}
	}       // exit from the critical section to give the thread
			// the possibility to enter its destructor
			// (which is guarded with m_pThreadCS critical section!)
	while (1)
	{
		{ // was the ~MyThread() function executed?
			wxCriticalSectionLocker enter(m_pThreadCS);
			if (!threadAlive) break;
		}
		// wait for thread completion
		wxThread::This()->Sleep(1);
	}
}

void cMain::OnClose(wxCloseEvent&)
{
	killThread();
	Destroy();
}