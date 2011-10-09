#include <wx/wx.h>
#include <cstdlib>
#include "affine.h"
#include "ids.h"
#include "english.h"


BEGIN_EVENT_TABLE(WA_Affine, wxPanel)
	EVT_TEXT(affine_txt, WA_Affine::on_txt_changed)
END_EVENT_TABLE()


WA_Affine::WA_Affine(wxWindow * parent) : wxPanel(parent, wxID_ANY){
	txt = new wxTextCtrl(this, affine_txt, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	result = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
	num_a = new wxTextCtrl(this, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	num_b = new wxTextCtrl(this, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, wxTE_READONLY);

	num_sizer = new wxBoxSizer(wxHORIZONTAL);
	num_sizer->Add(num_a, 1, wxEXPAND);
	num_sizer->Add(num_b, 1, wxEXPAND);

	main_sizer = new wxBoxSizer(wxVERTICAL);
	main_sizer->Add(txt, 1, wxEXPAND | wxALL, 5);
	main_sizer->Add(num_sizer, 0, wxEXPAND);
	main_sizer->Add(result, 1, wxEXPAND | wxALL, 5);

	SetAutoLayout(TRUE);

	SetSizer(main_sizer);
}

static int divide(int a, int b){
	while(a < 0 || (a % b) != 0){
		a += 26;
	}
	return a / b;
}

void WA_Affine::on_txt_changed(wxCommandEvent& event){
	wxString data = txt->GetValue();
	wxString ndata(data);

	double lowest_freq = 0.0f;
	double fval;
	wxString lowest_data;

	for(int a = 1; a < 26; a += 2){
		if(a == 13){
			a = 15;
			continue;
		}
		for(int b = 0; b < 26; b++){
			for(unsigned int i = 0; i < data.Len(); i++){
				if(data[i] >= 'A' && data[i] <= 'Z'){
					ndata[i] = 'A' + divide(data[i] - 'A' - b, a);
				}else if(data[i] >= 'a' && data[i] <= 'z'){
					ndata[i] = 'a' + divide(data[i] - 'a' - b, a);
				}
			}
			//fval = english_frequency(ndata);
			fval = english_score(ndata);
			if(fval > lowest_freq){
				num_a->SetValue(wxString::Format(wxT("%u"), a));
				num_b->SetValue(wxString::Format(wxT("%u"), b));
				lowest_freq = fval;
				lowest_data = ndata;
			}
		}
	}

	result->SetValue(lowest_data);
}

WA_Affine::~WA_Affine(){

}
