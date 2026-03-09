#include "AuthPanel.h"

SignUpPage::SignUpPage()
{
	//client.connectToServer();
	authenticateButtonText = "Sign Up";
}

void SignUpPage::setAuthenticateButtonText(const string& authenticateButtonText)
{
	this->authenticateButtonText = authenticateButtonText;
}

void SignUpPage::setPanel(wxPanel* panel)
{
	this->panel = panel;
}

wxPanel* SignUpPage::getPanel()
{
	return panel;
}

wxStaticText* SignUpPage::getUsernamePrompt()
{
	return usernamePrompt;
}

wxStaticText* SignUpPage::getPasswordPrompt()
{
	return passwordPrompt;
}

wxTextCtrl* SignUpPage::getUsernameInput()
{
	return usernameInput;
}

wxTextCtrl* SignUpPage::getPasswordInput()
{
	return passwordInput;
}

wxStaticBitmap* SignUpPage::getLogo()
{
	return logo;
}

wxButton* SignUpPage::getAuthenticateButton()
{
	return authenticateButton;
}

Client* SignUpPage::getClient()
{
	return &client;
}

wxBoxSizer* SignUpPage::getMainSizer()
{
	return mainSizer;
}

wxBoxSizer* SignUpPage::getUsernamePasswordSizer()
{
	return usernamePasswordSizer;
}

Fonts& SignUpPage::getFonts()
{
	return fonts;
}

void SignUpPage::createControls()
{
	usernamePrompt = new wxStaticText(panel, wxID_ANY, "username:");
	passwordPrompt = new wxStaticText(panel, wxID_ANY, "password:");
	usernameInput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(300, -1), wxTE_PROCESS_ENTER);
	passwordInput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(300, -1), wxTE_PROCESS_ENTER | wxTE_PASSWORD);
	authenticateButton = new wxButton(panel, wxID_ANY, authenticateButtonText);

	wxImage img(wxT("logo.png"), wxBITMAP_TYPE_PNG);
	img.Rescale(150, 105);
	wxBitmap shrunkenBitmap(img);
	logo = new wxStaticBitmap(panel, wxID_ANY, shrunkenBitmap);

	usernamePrompt->SetFont(fonts.regularFont);
	passwordPrompt->SetFont(fonts.regularFont);
	usernameInput->SetFont(fonts.regularFont);
	passwordInput->SetFont(fonts.regularFont);
}

void SignUpPage::bindEventHandlers()
{
	getUsernameInput()->Bind(wxEVT_TEXT_ENTER, &SignUpPage::onUserPressEnterInUsername, this);
	getPasswordInput()->Bind(wxEVT_TEXT_ENTER, &SignUpPage::onUserEnterPassword, this);
	getAuthenticateButton()->Bind(wxEVT_BUTTON, &SignUpPage::onUserEnterPassword, this);

}

void SignUpPage::setUpSizers()
{

	mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(logo, wxSizerFlags().CenterHorizontal());
	mainSizer->AddSpacer(10);

	usernamePasswordSizer = new wxBoxSizer(wxVERTICAL);

	usernamePasswordSizer->Add(usernamePrompt, 0, wxALIGN_LEFT | wxLEFT, 10);
	usernamePasswordSizer->Add(usernameInput, 0, wxLEFT | wxRIGHT, 10);
	usernamePasswordSizer->AddSpacer(13);
	usernamePasswordSizer->Add(passwordPrompt, 0, wxALIGN_LEFT | wxLEFT, 10);
	usernamePasswordSizer->Add(passwordInput, 0, wxLEFT | wxRIGHT, 10);
	mainSizer->Add(usernamePasswordSizer, wxSizerFlags().CenterHorizontal());

	mainSizer->AddSpacer(5);
	mainSizer->Add(authenticateButton, wxSizerFlags().CenterHorizontal());

	wxGridSizer* outerSizer = new wxGridSizer(1);
	outerSizer->Add(mainSizer, wxSizerFlags().Border(wxALL, 25).Expand().CenterVertical());
}

void SignUpPage::onUserPressEnterInUsername(wxCommandEvent& evt)
{
	passwordInput->SetFocus();
}

void SignUpPage::onUserEnterPassword(wxCommandEvent& evt)
{
	string buffer = "";
	bool success = client.signup(
		usernameInput->GetValue().ToStdString(), 
		passwordInput->GetValue().ToStdString(), 
		buffer);
	if (!success)
	{
		wxMessageBox(buffer, "Error", wxOK);
	}
	else
	{
		wxMessageBox("Correct!", "Success", wxOK);
	}
}