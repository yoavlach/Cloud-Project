#pragma once
#include <wx/wx.h>
#include <wx/hyperlink.h>
#include "Client.h"
#include "LoginPage.h"

#define TITLE "YLCloud"

class MainFrame : public wxFrame
{
public:
	MainFrame();
	void switchToSignUpPage();
private:
	Client client;
	LoginPage* loginPage;
	SignUpPage* signupPage;

	void bindEventHandlers();
	void setUpSizers();

	void onUserPressEnterInUsername(wxCommandEvent& evt);
	void onUserEnterPassword(wxCommandEvent& evt);

};

