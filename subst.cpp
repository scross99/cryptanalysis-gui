#include <wx/wx.h>
#include <wx/grid.h>
#include <cstdlib>
#include <iostream>
#include "subst.h"
#include "ids.h"
#include "english.h"


BEGIN_EVENT_TABLE(WA_Subst, wxPanel)
	EVT_BUTTON(subst_select, WA_Subst::on_select)
	EVT_TEXT(subst_txt, WA_Subst::on_txt_changed)
	EVT_GRID_CMD_CELL_CHANGE(subst_grid, WA_Subst::on_cell_change)
END_EVENT_TABLE()


WA_Subst::WA_Subst(wxWindow * parent) : wxPanel(parent, wxID_ANY){
	txt_cipher = new wxTextCtrl(this, subst_txt, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

	txt_plain = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

	select = new wxButton(this, subst_select, wxT("Selection"));

	grid = new wxGrid(this, subst_grid, wxDefaultPosition, wxDefaultSize, 0);

	grid->CreateGrid(1, 26);
	grid->AutoSize();
	grid->SetRowLabelSize(0);
	grid->SetDefaultCellAlignment(wxALIGN_CENTRE, wxALIGN_CENTRE);

	txt_sizer = new wxBoxSizer(wxHORIZONTAL);
	txt_sizer->Add(txt_cipher, 1, wxEXPAND | wxALL, 2);
	txt_sizer->Add(txt_plain, 1, wxEXPAND | wxALL, 2);

	main_sizer = new wxBoxSizer(wxVERTICAL);
	main_sizer->Add(txt_sizer, 1, wxEXPAND | wxALL, 2);
	main_sizer->Add(select, 0, wxEXPAND);
	main_sizer->Add(grid, 0, wxEXPAND);

	SetAutoLayout(TRUE);

	SetSizer(main_sizer);

	main_sizer->SetSizeHints(this);

	for(int i = 0; i < 256; i++){
		conv_table[i] = char(~0);
	}
}

void WA_Subst::on_select(wxCommandEvent& event){
	long from, to;

	txt_plain->SetFocus();
	txt_plain->GetSelection(&from, &to);

	txt_cipher->SetFocus();
	txt_cipher->SetSelection(from, to);
}

void WA_Subst::on_txt_changed(wxCommandEvent& event){
	txt_update();
}

void WA_Subst::on_cell_change(wxGridEvent& event){
	int col = event.GetCol();
	int row = event.GetRow();

	wxString data = grid->GetCellValue(row, col);
	wxString ndata;

	for(std::size_t p = 0; p < data.Len(); p++){
		if(conv_table[data[p]] == col || conv_table[data[p]] == char(~0)){
			ndata += data[p];
		}
	}

	grid->SetCellValue(row, col, ndata);

	txt_update();
}

void WA_Subst::txt_update(){
	wxString data;

	//init conversion table
	for(std::size_t i = 0; i < 256; i++){
		conv_table[i] = char(~0);
	}

	for(std::size_t i = 0; i < 26; i++){
		data = grid->GetCellValue(0, i);
		for(std::size_t p = 0; p < data.Len(); p++){
			conv_table[(std::size_t) data[p]] = i;
		}
	}

	data = txt_cipher->GetValue();

	wxString ndata(data);

	for(std::size_t i = 0; i < data.Len(); i++){
		if(isspace(data[i])){
			continue;
		}
		if(conv_table[data[i]] != char(~0)){
			ndata[i] = 'A' + conv_table[data[i]];
		}else{
			ndata[i] = '*';
		}
	}

	txt_plain->SetValue(ndata);
}

WA_Subst::~WA_Subst(){

}
