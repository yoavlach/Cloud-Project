#include "MainFrame.h"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, TITLE)
{
	try
	{
		client.connectToServer();
		panel = new wxPanel(this);
		createControls();
		bindEventHandlers();
		setUpSizers();
	}
	catch (const exception& e)
	{
		cout << e.what();
	}
}

void MainFrame::createControls()
{
	welcomeMessage = new wxStaticText(panel, wxID_ANY, "Welcome to YLCloud!");
	usernamePrompt = new wxStaticText(panel, wxID_ANY, "username:");
	passwordPrompt = new wxStaticText(panel, wxID_ANY, "password:");
	boldAndLarge = { 18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD };
	regualFont = { 12 , wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL };
	usernameInput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(300, -1), wxTE_PROCESS_ENTER);
	passwordInput = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(300, -1), wxTE_PROCESS_ENTER | wxTE_PASSWORD);
	signUpHyperLink = new wxHyperlinkCtrl(panel, wxID_ANY, "Or Sign Up Instead", "");
	logInButton = new wxButton(panel, wxID_ANY, "Log In");

	wxImage img(wxT("logo.png"), wxBITMAP_TYPE_PNG);
	img.Rescale(150, 105);
	wxBitmap shrunkenBitmap(img);
	logo = new wxStaticBitmap(panel, wxID_ANY, shrunkenBitmap);

	welcomeMessage->SetFont(boldAndLarge);
	usernamePrompt->SetFont(regualFont);
	passwordPrompt->SetFont(regualFont);
	usernameInput->SetFont(regualFont);
	passwordInput->SetFont(regualFont);
}

void MainFrame::bindEventHandlers()
{
	usernameInput->Bind(wxEVT_TEXT_ENTER, &MainFrame::onUserPressEnterInUsername, this);
	passwordInput->Bind(wxEVT_TEXT_ENTER, &MainFrame::onUserPressEnterInPassword, this);
}

void MainFrame::setUpSizers()
{

	mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(welcomeMessage, wxSizerFlags().CenterHorizontal());
	mainSizer->AddSpacer(25);
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
	mainSizer->Add(logInButton, wxSizerFlags().CenterHorizontal());
	mainSizer->AddSpacer(5);
	mainSizer->Add(signUpHyperLink, wxSizerFlags().CenterHorizontal());

	wxGridSizer* outerSizer = new wxGridSizer(1);
	outerSizer->Add(mainSizer, wxSizerFlags().Border(wxALL, 25).Expand().CenterVertical());
	
	panel->SetSizer(outerSizer);
	outerSizer->SetSizeHints(this);
}

void MainFrame::onUserPressEnterInUsername(wxCommandEvent& evt)
{
	passwordInput->SetFocus();
}

void MainFrame::onUserPressEnterInPassword(wxCommandEvent& evt)
{
	string buffer = "";
	bool success = client.login(usernameInput->GetValue().ToStdString(), passwordInput->GetValue().ToStdString(), buffer);
	if (!success)
	{
		wxMessageBox(buffer, "Error", wxOK);
	}
	else
	{
		wxMessageBox("Correct!", "Success", wxOK);
	}
}
