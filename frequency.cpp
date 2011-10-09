#include <wx/wx.h>
#include <wx/listctrl.h>
#include <cstdlib>
#include <iostream>
#include "frequency.h"
#include "ids.h"
#include "english.h"


BEGIN_EVENT_TABLE(WA_Frequency, wxPanel)
	EVT_TEXT(frequency_txt, WA_Frequency::on_txt_changed)
END_EVENT_TABLE()


WA_Frequency::WA_Frequency(wxWindow * parent) : wxPanel(parent, wxID_ANY){
	txt = new wxTextCtrl(this, frequency_txt, wxT(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);

	list = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);

	list->InsertColumn(0, wxT("Char"), wxLIST_FORMAT_CENTER);
	list->InsertColumn(1, wxT("Freq"), wxLIST_FORMAT_CENTER);
	list->InsertColumn(2, wxT("Match"), wxLIST_FORMAT_CENTER);

	main_sizer = new wxBoxSizer(wxHORIZONTAL);
	main_sizer->Add(txt, 2, wxEXPAND | wxALL, 3);
	main_sizer->Add(list, 1, wxEXPAND | wxALL, 3);

	SetAutoLayout(TRUE);

	SetSizer(main_sizer);
}

static int wxCALLBACK freq_comp(long item1, long item2, long sortData){
	if(item1 > item2){
		return -1;
	}else if(item1 < item2){
		return 1;
	}else{
		return 0;
	}
	return 0;
}

void WA_Frequency::on_txt_changed(wxCommandEvent& event){
	unsigned int freq[256];
	wxString data = txt->GetValue();
	unsigned int len = data.Len();
	unsigned int next_id = 0;
	unsigned int total_freq = 0;
	//char a, b;

	/*double eng_freq[] = {0.12702, 0.09056, 0.08167, 0.07507,
						0.06966, 0.06749, 0.06327, 0.06094,
						0.05987, 0.04253, 0.04025, 0.02782,
						0.02758, 0.02406, 0.02360, 0.02228,
						0.02015, 0.01974, 0.01929, 0.01492,
						0.00978, 0.00772, 0.00153, 0.00150,
						0.00095, 0.00074};*/

	char eng_cfreq[] = {'e', 't', 'a', 'o', 'i', 'n', 's', 'h', 'r', 'd', 'l', 'c', 'u',
						'm', 'w', 'f', 'g', 'y', 'p', 'b', 'v', 'k', 'j', 'x', 'q', 'z'};

	list->DeleteAllItems();

	//initialise frequency array to 0
	for(unsigned int p = 0; p < 256; p++){
		freq[p] = 0;
	}

	//calculate the frequency of each character
	for(unsigned int i = 0; i < len; i++){
		if(!isspace(data[i])){
			freq[int(data[i])]++;
			total_freq++;
		}
	}

	//add frequencies to list
	for(unsigned int w = 0; w < 256; w++){
		if(freq[w] > 0){
			list->InsertItem(next_id, wxString::Format(wxT("%c"), char(w)));
			list->SetItemData(next_id, long(freq[w]));
			list->SetItem(next_id, 1, wxString::Format(wxT("%.2f%%"), float(100 * freq[w]) / float(total_freq)));
			list->SetItem(next_id, 2, wxT("?"));
			next_id++;
		}
	}

	list->SortItems(freq_comp, 0);

	for(unsigned int q = 0; q < 26; q++){
		list->SetItem(q, 2, wxString::Format(wxT("%c"), eng_cfreq[q]));
	}
}

WA_Frequency::~WA_Frequency(){

}
