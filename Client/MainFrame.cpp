#include "MainFrame.h"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, TITLE)
{
	try
	{
		//client.connectToServer();
		loginPage.setPanel(new wxPanel(this));
		loginPage.createControls();
		loginPage.bindEventHandlers();

		loginPage.setUpSizers();
		signUpHyperLink = new wxHyperlinkCtrl(loginPage.getPanel(), wxID_ANY, "Or Sign Up Instead", "");
		loginPage.getMainSizer()->InsertSpacer(6, 25);
		loginPage.getMainSizer()->Insert(7, signUpHyperLink, wxSizerFlags().CenterHorizontal());
		loginPage.getMainSizer()->InsertSpacer(8, 13);

		loginPage.getPanel()->SetSizer(loginPage.getMainSizer());
		loginPage.getMainSizer()->SetSizeHints(this);
	}
	catch (const exception& e)
	{
		wxMessageBox(e.what(), "Connection Error", wxOK);
	}
}