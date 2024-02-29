#include "Mainframe.h"
#include <wx/wx.h>
#include <vector>
#include <string>
#include "Task.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{
	
	CreateControls();
	SetupSizers();
	BindEventHandels();
	AddSavedTasks();

}

void MainFrame::CreateControls()
{
	
	wxFont headlineFont(wxFontInfo(wxSize(0, 36)).Bold());
	wxFont mainFont(wxFontInfo(wxSize(0, 24)));

	panel = new wxPanel(this);
	panel->SetFont(mainFont);

	headlineText = new wxStaticText(panel, wxID_ANY, "To-Do List");
	headlineText->SetFont(headlineFont);
	
	inputField = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	addButton = new wxButton(panel, wxID_ANY, "Add");
	checkListBox = new wxCheckListBox(panel, wxID_ANY);
	clearButton = new wxButton(panel, wxID_ANY, "Clear");

}

void MainFrame::SetupSizers()
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	mainSizer->Add(headlineText, wxSizerFlags().CenterHorizontal());
	mainSizer->AddSpacer(20);

	wxBoxSizer* inputSizer = new wxBoxSizer(wxHORIZONTAL);
	inputSizer->Add(inputField, wxSizerFlags().Proportion(1));
	inputSizer->AddSpacer(10);
	inputSizer->Add(addButton);
	
	mainSizer->Add(inputSizer, wxSizerFlags().Expand()); 
	mainSizer->AddSpacer(10);
	mainSizer->Add(checkListBox, wxSizerFlags().Expand().Proportion(1));
	mainSizer->AddSpacer(10);
	mainSizer->Add(clearButton);
	
	wxGridSizer* outerSizer = new wxGridSizer(1);
	outerSizer->Add(mainSizer, wxSizerFlags().Border(wxALL, 25).Expand());

	panel->SetSizer(outerSizer);
	outerSizer->SetSizeHints(this);
}
// binding event handlers to controls
void MainFrame::BindEventHandels()
{
	addButton->Bind(wxEVT_BUTTON, &MainFrame::OnAddButtonClicked, this);
	inputField->Bind(wxEVT_TEXT_ENTER, &MainFrame::OnInputEnter, this);
	checkListBox->Bind(wxEVT_KEY_DOWN, &MainFrame::OnListKeyDown, this);
	clearButton->Bind(wxEVT_BUTTON, &MainFrame::OnClearButtonClicked, this);
	this->Bind(wxEVT_CLOSE_WINDOW, &MainFrame::OnWindowClosed, this);
	
}
// event handler for clicking the Add button
void MainFrame::OnAddButtonClicked(wxCommandEvent& evt)
{
	AddTaskFromInput();
}
// add a task from the input field to the task list
void MainFrame::AddTaskFromInput()
{
	wxString description = inputField->GetValue();

	if (!description.IsEmpty()) {
		checkListBox->Insert(description, checkListBox->GetCount());
		inputField->Clear();
	}

	inputField->SetFocus();
}
// event handler for pressing the enter key in the input field
void MainFrame::OnInputEnter(wxCommandEvent& evt)
{
	AddTaskFromInput();
}
//  moving down and up and deleting tasks in the list with delete or backspace key
void MainFrame::OnListKeyDown(wxKeyEvent& evt)
{
	if (evt.GetKeyCode() == WXK_BACK || evt.GetKeyCode() == WXK_DELETE) {
		DeleteSelectedTask();
	}
	else if (evt.GetKeyCode() == WXK_UP) {
		MoveSelectedTask(-1);
	}
	else if (evt.GetKeyCode() == WXK_DOWN) {
		MoveSelectedTask(1);
	}
}
// deletes task from the task list
void MainFrame::DeleteSelectedTask()
{
	int selectedIndex = checkListBox->GetSelection();

	if (selectedIndex == wxNOT_FOUND) {
		return;
	}
	checkListBox->Delete(selectedIndex);
}

void MainFrame::MoveSelectedTask(int offset)
{
	int selectedIndex = checkListBox->GetSelection();

	if (selectedIndex == wxNOT_FOUND) {
		return;
	}
	else {
		int newIndex = selectedIndex + offset;

		if (newIndex >= 0 && newIndex < checkListBox->GetCount()) {
			SwapTasks(selectedIndex, newIndex);
			checkListBox->SetSelection(newIndex, true);
		}
	}

}

void MainFrame::SwapTasks(int i, int j)
{
	Task taskI{ checkListBox->GetString(i).ToStdString(), checkListBox->IsChecked(i) };
	Task taskJ{ checkListBox->GetString(j).ToStdString(), checkListBox->IsChecked(j) };
	checkListBox->SetString(i, taskJ.description);
	checkListBox->Check(i, taskJ.done);
	checkListBox->SetString(j, taskI.description);
	checkListBox->Check(j, taskI.done);
}
// event handler for the clear button
void MainFrame::OnClearButtonClicked(wxCommandEvent& evt)
{
	if (checkListBox->IsEmpty()) {
		return;
	}
	else {
		wxMessageDialog dialog(this, "Are you sure?", "Clear", wxYES_NO);
		int result = dialog.ShowModal();

		if (result == wxID_YES) {
			checkListBox->Clear();
		}
	}
}
// event handler for when the window is closed
void MainFrame::OnWindowClosed(wxCloseEvent& evt)
{
	std::vector<Task> tasks;
	// saves the current tasks to a file when the windows is closed
	for (int i = 0; i < checkListBox->GetCount(); i++) {
		Task task;
		task.description = checkListBox->GetString(i);
		task.done = checkListBox->IsChecked(i);
		tasks.push_back(task);
	}
	SaveTasksToFile(tasks, "saveTasks.txt");
	evt.Skip(); // without this, the windows cannot close
}
// add tasks that were saved from a file
void MainFrame::AddSavedTasks()
{
	std::vector<Task> tasks = loadTasksFromFile("saveTasks.txt");

	for (const Task& task : tasks) {
		int index = checkListBox->GetCount();
		checkListBox->Insert(task.description, index);
		checkListBox->Check(index, task.done);
	}
}

