#pragma once
#include "AuthPanel.h"
#include "wx/hyperlink.h"

class LoginPage : public SignUpPage
{
public:
	void createControls();
	void bindEventHandlers();
	void setUpSizers();

private:
	wxStaticText* welcomeMessage;

	void onUserEnterPassword(wxCommandEvent& evt) override;
};