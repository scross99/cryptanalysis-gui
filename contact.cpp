#include <wx/wx.h>
#include <wx/grid.h>
#include <cstdlib>
#include <iostream>
#include "contact.h"
#include "ids.h"
#include "english.h"


BEGIN_EVENT_TABLE(WA_Contact, wxPanel)
	EVT_TEXT(contact_txt, WA_Contact::on_txt_changed)
	EVT_CHECKBOX(contact_asym, WA_Contact::on_check)
END_EVENT_TABLE()


WA_Contact::WA_Contact(wxWindow * parent) : wxPanel(parent, wxID_ANY){
	is_asym = false;

	txt = new wxTextCtrl(this, contact_txt, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

	grid = new wxGrid(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);

	grid->CreateGrid(26, 26);

	grid->AutoSize();
	grid->DisableCellEditControl();
	grid->DisableDragColMove();
	grid->DisableDragColSize();
	grid->DisableDragGridSize();
	grid->DisableDragRowSize();
	grid->EnableEditing(false);

	grid->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);

	for(char i = 0; i < 26; i++){
		grid->SetRowLabelValue(i, wxString::Format(wxT("%c"), char('A' + i)));
		grid->SetCellBackgroundColour(i, i, wxColour(200, 200, 200));
	}

	asym = new wxCheckBox(this, contact_asym, wxT("Asymmetry"));

	grid_sizer = new wxBoxSizer(wxVERTICAL);
	grid_sizer->Add(grid, 1, wxEXPAND);
	grid_sizer->Add(asym, 0, wxEXPAND | wxALL, 2);

	main_sizer = new wxBoxSizer(wxHORIZONTAL);
	main_sizer->Add(txt, 1, wxEXPAND | wxALL, 2);
	main_sizer->Add(grid_sizer, 0, wxEXPAND);

	SetAutoLayout(TRUE);

	SetSizer(main_sizer);

	main_sizer->SetSizeHints(this);
}

void WA_Contact::on_txt_changed(wxCommandEvent& event){
	int cont[26][26];
	wxString data = txt->GetValue();
	unsigned int len = data.Len();
	char a, b;

	//initialise contact array to 0
	for(unsigned int i = 0; i < 26; i++){
		for(unsigned int j = 0; j < 26; j++){
			cont[i][j] = 0;
		}
	}

	//perform contact analysis
	for(unsigned int i = 0; i < len; i++){
		if(i != (len - 1)){
			if(islower(data[i])){
				a = data[i] - 'a';
			}else if(isupper(data[i])){
				a = data[i] - 'A';
			}else{
				continue;
			}
			if(islower(data[i+1])){
				b = data[i+1] - 'a';
			}else if(isupper(data[i + 1])){
				b = data[i+1] - 'A';
			}else{
				continue;
			}
			cont[(std::size_t) b][(std::size_t) a]++;
		}
	}

	if(is_asym){
		for(unsigned int i = 0; i < 26; i++){
			for(unsigned int j = 0; j < 26; j++){
				if(i < j){
					cont[i][j] -= cont[j][i];
				}
			}
		}

		for(unsigned int i = 0; i < 26; i++){
			for(unsigned int j = 0; j < 26; j++){
				if(i >= j){
					cont[i][j] = 0;
				}
			}
		}
	}

	grid->BeginBatch();

	//copy data to grid
	for(unsigned int i = 0; i < 26; i++){
		for(unsigned int j = 0; j < 26; j++){
			if(cont[i][j] != 0){
				grid->SetCellValue(i, j, wxString::Format(wxT("%d"), cont[i][j]));
			}else{
				grid->SetCellValue(i, j, wxT(""));
			}
		}
	}

	grid->EndBatch();

	grid->AutoSize();
}

void WA_Contact::on_check(wxCommandEvent& event){
	if(event.IsChecked()){
		is_asym = true;
	}else{
		is_asym = false;
	}
	on_txt_changed(event);
}

WA_Contact::~WA_Contact(){

}
