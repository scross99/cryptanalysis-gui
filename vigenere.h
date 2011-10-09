#ifndef VIGENERE_H
#define VIGENERE_H

#include <wx/wx.h>

class WA_Vigenere : public wxPanel{
	public:
		WA_Vigenere(wxWindow * parent);
		~WA_Vigenere();
		
		void on_calc_key(wxCommandEvent& event);
		
		void on_key_select(wxCommandEvent& event);
		
		void on_key_changed(wxCommandEvent& event);
		
		void on_txt_changed(wxCommandEvent& event);
		
		wxBoxSizer * main_sizer;
		
		wxTextCtrl * txt;
		wxGauge * txt_gauge;
		wxButton * calc_key;
		
		wxTextCtrl * current_key;
		wxListBox * keys;
		
		wxString txt_data;
		bool show_data;

		DECLARE_EVENT_TABLE();
};

#endif
