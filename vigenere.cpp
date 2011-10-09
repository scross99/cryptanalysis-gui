#include <wx/wx.h>
#include <cstdlib>
#include "vigenere.h"
#include "ids.h"
#include "english.h"

#define VG_GEN_KEYS 40

BEGIN_EVENT_TABLE(WA_Vigenere, wxPanel)
	EVT_TEXT(vigenere_txt, WA_Vigenere::on_txt_changed)
	EVT_TEXT(vigenere_current_key, WA_Vigenere::on_key_changed)
	EVT_LISTBOX(vigenere_keys, WA_Vigenere::on_key_select)
	EVT_BUTTON(vigenere_calc_key, WA_Vigenere::on_calc_key)
END_EVENT_TABLE()

struct vigenere_key{
	wxString text;
	double score;
};

// Quick algorithm to solve caesar shift.
std::size_t quick_caesar(const wxString& txt, std::size_t offset, std::size_t key_len){

	// Standard english frequency of each character.
	double eng_freq[] = {0.08167, 0.01492, 0.02782, 0.04253,
				0.12702, 0.02228, 0.02015, 0.06094,
				0.06966, 0.00153, 0.00772, 0.04025,
				0.02406, 0.06749, 0.07507, 0.01929,
				0.00095, 0.05987, 0.06327, 0.09056,
				0.02758, 0.00978, 0.02360, 0.00150,
				0.01974, 0.00074};

	double bestAccuracy = 0.0;
	std::size_t bestShift = 0;

	std::size_t intfreq[26];
	double freq[26];
	std::size_t total = 0;

	// Clear the character frequency table.
	for(std::size_t letter = 0; letter < 26; letter++){
		intfreq[letter] = 0;
	}

	// Calculate the frequency of each character.
	for(std::size_t pos = offset; pos < txt.size(); pos += key_len){
		if(txt[pos] >= 'a' && txt[pos] <= 'z'){
			intfreq[txt[pos] - 'a']++;
			total++;
		}else if(txt[pos] >= 'A' && txt[pos] <= 'Z'){
			intfreq[txt[pos] - 'A']++;
			total++;
		}
	}

	if(total == 0){
		return 0;
	}

	// Convert frequency from integer to floating point (0.0 <= value <= 1.0).
	for(std::size_t letter = 0; letter < 26; letter++){
		freq[letter] = double(intfreq[letter]) / double(total);
	}

	// Identify the solution by comparing shifts to standard english frequency.
	for(std::size_t shift = 0; shift < 26; shift++){
		double actualAccuracy = 0.0;
		double perfectAccuracy = 0.0;
		for(std::size_t letter = 0; letter < 26; letter++){
			actualAccuracy += eng_freq[letter] * freq[(letter + shift) % 26];
			perfectAccuracy += eng_freq[letter] * eng_freq[letter];
		}
		
		const double accuracy = actualAccuracy / perfectAccuracy;
		
		if(accuracy > bestAccuracy){
			bestShift = shift;
			bestAccuracy = accuracy;
		}
	}

	return bestShift;
}

WA_Vigenere::WA_Vigenere(wxWindow * parent) : wxPanel(parent, wxID_ANY){
	txt = new wxTextCtrl(this, vigenere_txt, wxT("<Vigenere Cipher Here>"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	txt_gauge = new wxGauge(this, wxID_ANY, 1000);
	calc_key = new wxButton(this, vigenere_calc_key, wxT("Calculate Keys"));
	current_key = new wxTextCtrl(this, vigenere_current_key);
	keys = new wxListBox(this, vigenere_keys);
	keys->Append(wxT("<ORIGINAL>"));

	wxBoxSizer * left_sizer = new wxBoxSizer(wxVERTICAL);
	left_sizer->Add(txt, 1, wxEXPAND);
	left_sizer->Add(txt_gauge, 0, wxEXPAND);
	left_sizer->Add(calc_key, 0, wxEXPAND);

	wxBoxSizer * right_sizer = new wxBoxSizer(wxVERTICAL);
	right_sizer->Add(current_key, 0, wxEXPAND);
	right_sizer->Add(keys, 1, wxEXPAND);

	main_sizer = new wxBoxSizer(wxHORIZONTAL);
	main_sizer->Add(left_sizer, 2, wxEXPAND);
	main_sizer->Add(right_sizer, 1, wxEXPAND);

	SetAutoLayout(TRUE);

	SetSizer(main_sizer);

	show_data = true;
}

// Performs decryption, given a key.
wxString vigenere_decrypt(wxString dat, wxString key){
	wxString data(dat);
	unsigned int w = 0;
	for(unsigned int i = 0; i < data.Len() ; i++){
		if(data[i] >= 'A' && data[i] <= 'Z'){
			data[i] = data[i] - 'A';
		}else if(data[i] >= 'a' && data[i] <= 'z'){
			data[i] = data[i] - 'a';
		}else{
			continue;
		}
		data[i] = data[i] - (key[w % key.size()] - 'a');
		if(data[i] < 0){
			data[i] += 26;
		}
		data[i] += 'a';
		w++;
	}
	return data;
}

int compare_vigenere_keys(const void * a, const void * b){
	vigenere_key * ka = (vigenere_key *) a;
	vigenere_key * kb = (vigenere_key *) b;
	if(ka->score > kb->score){
		return -1;
	}else if(ka->score < kb->score){
		return 1;
	}else{
		// Prefer shorter keys.
		return (ka->text).size() < (kb->text).size() ? -1 : 1;
	}
}

void WA_Vigenere::on_calc_key(wxCommandEvent& event){
	wxString key_txt[VG_GEN_KEYS + 1];
	vigenere_key key[VG_GEN_KEYS];
	wxString data = txt->GetValue();

	wxString filtered;
	for(std::size_t i = 0; i < data.Len(); i++){
		if(data[i] >= 'A' && data[i] <= 'Z'){
			filtered += data[i] - 'A' + 'a';
		}else if(data[i] >= 'a' && data[i] <= 'z'){
			filtered += data[i];
		}else{
			continue;
		}
	}

    	std::size_t len = data.Len();
	// Split the text into multiple caesar shifts to obtain a likely key.
	for(unsigned int key_len = 0; key_len < VG_GEN_KEYS; key_len++){
		for(unsigned int i = 0; i < key_len+1; i++){
			key[key_len].text += wxChar(quick_caesar(filtered, i, key_len+1) + 'a');
		}
		key[key_len].score = english_score(vigenere_decrypt(data, key[key_len].text));
	}

	// Sort the generated keys in order of quality (best to worst).
	qsort((void *) key, VG_GEN_KEYS, sizeof(vigenere_key), compare_vigenere_keys);

	key_txt[0] = wxT("<ORIGINAL>");

	for(unsigned int g = 0; g < VG_GEN_KEYS; g++){
		key_txt[g+1] = (key[g]).text;
	}

	keys->Set(VG_GEN_KEYS, key_txt);
}

void WA_Vigenere::on_key_select(wxCommandEvent& event){
	if(keys->GetSelection() == wxNOT_FOUND){
		return;
	}
	if(keys->GetSelection() == 0){
		current_key->SetValue(wxT(""));
		return;
	}
	wxString key = keys->GetString(keys->GetSelection());
	current_key->SetValue(key);
}

void WA_Vigenere::on_key_changed(wxCommandEvent& event){
	if(show_data){
		txt_data = txt->GetValue();
	}
	if(current_key->GetValue().Len() == 0){
		show_data = true;
		txt->SetEditable(true);
		txt->SetValue(txt_data);
	}else{
		show_data = false;
		txt->SetValue(vigenere_decrypt(txt_data, current_key->GetValue()));
		txt->SetEditable(false);
	}
}

void WA_Vigenere::on_txt_changed(wxCommandEvent& event){
	const std::size_t score = english_score(txt->GetValue()) * 1000.0;
	txt_gauge->SetValue(score);
}

WA_Vigenere::~WA_Vigenere(){

}

