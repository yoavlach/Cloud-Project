#pragma once
#include "SignUpPage.h"
#include "wx/hyperlink.h"

class LoginPage : public SignUpPage
{
public:
	void createControls();
	void bindEventHandlers();
	void setUpSizers();
private:
	wxHyperlinkCtrl* signUpHyperLink;
	wxStaticText* welcomeMessage;

	void onUserEnterPassword(wxCommandEvent& evt) override;
	void onPressSignUpInstead(wxHyperlinkEvent& evt);
};