#pragma once
#include <wx/wx.h>
#include "Fonts.h"
#include "Client.h"

class SignUpPage
{
public:
	SignUpPage();
	void setAuthenticateButtonText(const string& authenticateButtonText);
	void setPanel(wxPanel* panel);

	wxPanel* getPanel();
	wxStaticText* getUsernamePrompt();
	wxStaticText* getPasswordPrompt();
	wxTextCtrl* getUsernameInput();
	wxTextCtrl* getPasswordInput();
	wxStaticBitmap* getLogo();
	wxButton* getAuthenticateButton();
	Client* getClient();
	wxBoxSizer* getMainSizer();
	wxBoxSizer* getUsernamePasswordSizer();
	Fonts& getFonts();

	void createControls();
	virtual void bindEventHandlers();
	void setUpSizers();
private:
	void onUserPressEnterInUsername(wxCommandEvent& evt);
	virtual void onUserEnterPassword(wxCommandEvent& evt);

	wxPanel* panel;
	wxStaticText* usernamePrompt;
	wxStaticText* passwordPrompt;
	wxTextCtrl* usernameInput;
	wxTextCtrl* passwordInput;
	wxStaticBitmap* logo;
	wxButton* authenticateButton;

	wxBoxSizer* mainSizer;
	wxBoxSizer* usernamePasswordSizer;

	Fonts fonts;
	Client client;
	string authenticateButtonText;
};

