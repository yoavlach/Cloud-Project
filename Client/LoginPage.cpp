#include "LoginPage.h"
#include "MainFrame.h"

void LoginPage::createControls()
{
	setAuthenticateButtonText("Log In");
	SignUpPage::createControls();
	signUpHyperLink = new wxHyperlinkCtrl(getPanel(), wxID_ANY, "Or Sign Up Instead", "");
	welcomeMessage = new wxStaticText(getPanel(), wxID_ANY, "Welcome to YLCloud!\nPlease Log In To Your Account", wxDefaultPosition, wxDefaultSize, wxTE_CENTRE);
	welcomeMessage->SetFont(getFonts().boldAndLarge);
}
void LoginPage::bindEventHandlers()
{
	SignUpPage::bindEventHandlers();
	getPasswordInput()->Bind(wxEVT_TEXT_ENTER, &LoginPage::onUserEnterPassword, this);
	getAuthenticateButton()->Bind(wxEVT_BUTTON, &LoginPage::onUserEnterPassword, this);
	signUpHyperLink->Bind(wxEVT_HYPERLINK, &LoginPage::onPressSignUpInstead, this);
}
void LoginPage::setUpSizers()
{
	SignUpPage::setUpSizers();
	getMainSizer()->Insert(1, welcomeMessage, wxSizerFlags().CenterHorizontal());
	getMainSizer()->InsertSpacer(2, 25);
	getMainSizer()->InsertSpacer(7, 13);
	getMainSizer()->Insert(8, signUpHyperLink, wxSizerFlags().CenterHorizontal());
	getMainSizer()->InsertSpacer(9, 13);
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

void LoginPage::onPressSignUpInstead(wxHyperlinkEvent& evt)
{
	static_cast<MainFrame*>(getPanel()->GetParent())->switchToSignUpPage();
}