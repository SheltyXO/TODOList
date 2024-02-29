#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
	// contructor for the mainframe class
	MainFrame(const wxString& title);
private:
	void CreateControls();
	void SetupSizers();
	void BindEventHandels();
	void OnAddButtonClicked(wxCommandEvent& evt);
	void AddTaskFromInput();
	void OnInputEnter(wxCommandEvent& evt);
	void OnListKeyDown(wxKeyEvent& evt);
	void DeleteSelectedTask();
	void MoveSelectedTask(int offset);
	void SwapTasks(int i, int j);
	void OnClearButtonClicked(wxCommandEvent& evt);
	void OnWindowClosed(wxCloseEvent& evt);
	void AddSavedTasks();
	
	

	wxPanel* panel;
	wxStaticText* headlineText;
	wxTextCtrl* inputField;
	wxButton* addButton;
	wxCheckListBox* checkListBox;
	wxButton* clearButton;
};

