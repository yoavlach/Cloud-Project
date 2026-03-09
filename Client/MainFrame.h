#pragma once
#include <wx/wx.h>
#include <wx/hyperlink.h>
#include "Client.h"

#define TITLE "YLCloud"

class MainFrame : public wxFrame
{
public:
	MainFrame();
private:

	Client client;

	wxPanel* panel;
	wxStaticText* welcomeMessage;
	wxStaticText* usernamePrompt;
	wxStaticText* passwordPrompt;
	wxTextCtrl* usernameInput;
	wxTextCtrl* passwordInput;
	wxHyperlinkCtrl* signUpHyperLink;
	wxStaticBitmap* logo;
	wxButton* logInButton;

	wxFont boldAndLarge;
	wxFont regualFont;

	wxBoxSizer* mainSizer;
	wxBoxSizer* usernamePasswordSizer;

	void createControls();
	void bindEventHandlers();
	void setUpSizers();

	void onUserPressEnterInUsername(wxCommandEvent& evt);
	void onUserPressEnterInPassword(wxCommandEvent& evt);
};

