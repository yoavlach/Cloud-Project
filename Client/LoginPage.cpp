#include "LoginPage.h"

void LoginPage::createControls()
{
	setAuthenticateButtonText("Log In");
	SignUpPage::createControls();
	welcomeMessage = new wxStaticText(getPanel(), wxID_ANY, "Welcome to YLCloud!\nPlease Log In To Your Account", wxDefaultPosition, wxDefaultSize, wxTE_CENTRE);
	welcomeMessage->SetFont(getFonts().boldAndLarge);
}
void LoginPage::bindEventHandlers()
{
	SignUpPage::bindEventHandlers();
	getPasswordInput()->Bind(wxEVT_TEXT_ENTER, &LoginPage::onUserEnterPassword, this);
	getAuthenticateButton()->Bind(wxEVT_BUTTON, &LoginPage::onUserEnterPassword, this);
}
void LoginPage::setUpSizers()
{
	SignUpPage::setUpSizers();
	getMainSizer()->Insert(0, welcomeMessage, wxSizerFlags().CenterHorizontal());
	getMainSizer()->InsertSpacer(1, 25);
}

void LoginPage::onUserEnterPassword(wxCommandEvent& evt)
{
	string buffer = "";
	bool success = getClient()->login(getUsernameInput()->GetValue().ToStdString(), getPasswordInput()->GetValue().ToStdString(), buffer);
	if (!success)
	{
		wxMessageBox(buffer, "Error", wxOK);
	}
	else
	{
		wxMessageBox("Correct!", "Success", wxOK);
	}
}