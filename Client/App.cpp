#include "App.h"

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    wxInitAllImageHandlers();
    MainFrame* mainFrame = new MainFrame();
    mainFrame->SetClientSize(wxSize(1000, 800));
    mainFrame->Center();
    mainFrame->Show();
    return true;
}