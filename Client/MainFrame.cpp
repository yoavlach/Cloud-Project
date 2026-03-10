#include "MainFrame.h"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, TITLE)
{
	try
	{
		//client.connectToServer();
		loginPage = new LoginPage();
		loginPage->setPanel(new wxPanel(this));

		loginPage->createControls();
		loginPage->bindEventHandlers();
		loginPage->setUpSizers();

		loginPage->getPanel()->SetSizer(loginPage->getMainSizer());

		wxBoxSizer* frameSizer = new wxBoxSizer(wxVERTICAL);
		frameSizer->Add(loginPage->getPanel(), 1, wxEXPAND);

		this->SetSizer(frameSizer);
		this->Layout();
	}
	catch (const exception& e)
	{
		wxMessageBox(e.what(), "Connection Error", wxOK);
	}
}

void MainFrame::switchToSignUpPage()
{
	loginPage->getPanel()->Hide();

	signupPage = new SignUpPage();
	signupPage->setPanel(new wxPanel(this));
	signupPage->createControls();
	signupPage->bindEventHandlers();
	signupPage->setUpSizers();

	signupPage->getPanel()->SetSizer(signupPage->getMainSizer());
	this->GetSizer()->Add(signupPage->getPanel(), 1, wxEXPAND);
	this->Layout();
}