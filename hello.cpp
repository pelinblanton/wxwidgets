#include <wx/wxprec.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/timer.h>
#include <wx/datetime.h>

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class CalendarPanel : public wxPanel
{
public:
    CalendarPanel(wxWindow* parent);

private:
    void OnPaint(wxPaintEvent& event);
    wxDECLARE_EVENT_TABLE();
};

class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void UpdateTime(wxTimerEvent& event);

    wxStaticText* m_timeText;
    wxTimer m_timer;
    CalendarPanel* m_calendarPanel;
};

enum
{
    ID_Hello = 1,
    ID_Timer = 2
};

wxBEGIN_EVENT_TABLE(CalendarPanel, wxPanel)
    EVT_PAINT(CalendarPanel::OnPaint)
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame();
    frame->Show(true);
    return true;
}

CalendarPanel::CalendarPanel(wxWindow* parent)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(200, 200))
{
    SetBackgroundColour(*wxWHITE); //background color to white
}

void CalendarPanel::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxWHITE_BRUSH);
    dc.Clear(); //background to white


    dc.DrawText("August 2024", 50, 10);

    wxDateTime date(1, wxDateTime::Aug, 2024);
    int startDay = date.GetWeekDay();

    const int daySize = 30;
    int x = 10;
    int y = 40;

    
    wxString days[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
    for (int i = 0; i < 7; ++i)
    {
        dc.DrawText(days[i], x + i * daySize, y);
    }

    y += 20;

    //draw the days
    for (int i = 0; i < startDay; ++i)
    {
        x += daySize;
    }

    for (int day = 1; day <= 31; ++day)
    {
        if (x >= daySize * 7)
        {
            x = 10;
            y += daySize;
        }

        dc.DrawText(wxString::Format("%d", day), x, y);
        x += daySize;
    }
}

MyFrame::MyFrame()
    : wxFrame(NULL, wxID_ANY, "Daily Planner"), m_timer(this, ID_Timer)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");

    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    //background color to blue
    SetBackgroundColour(*wxBLUE);

    //wxStaticText for the current date and time
    m_timeText = new wxStaticText(this, wxID_ANY, wxDateTime::Now().FormatISOCombined(' '), 
                                  wxDefaultPosition, wxSize(400, 20), wxALIGN_CENTER_HORIZONTAL);
    m_timeText->SetForegroundColour(*wxWHITE);
    m_timeText->SetBackgroundColour(*wxBLUE);

    
    wxFont font(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL);
    m_timeText->SetFont(font);

    //Calendar panel
    m_calendarPanel = new CalendarPanel(this);

    // Arrange the controls using a sizer????
    wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(m_calendarPanel, 0, wxALL, 10);
    sizer->Add(m_timeText, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
    SetSizer(sizer);

    // Timer to update the time every second
    m_timer.Start(1000);
    Bind(wxEVT_TIMER, &MyFrame::UpdateTime, this, ID_Timer);

    Bind(wxEVT_MENU, &MyFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}

void MyFrame::UpdateTime(wxTimerEvent& event)
{
    m_timeText->SetLabel(wxDateTime::Now().FormatISOCombined(' '));
}



// clang++ -stdc++17 hello.cpp -o prog `./wx-config --cxxflags` 