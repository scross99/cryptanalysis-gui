#ifndef FREQUENCY_H
#define FREQUENCY_H

#include <wx/wx.h>
#include <wx/listctrl.h>

class WA_Frequency : public wxPanel{
	public:
		WA_Frequency(wxWindow * parent);
		~WA_Frequency();
		
		void on_txt_changed(wxCommandEvent& event);
		
		wxBoxSizer * main_sizer;
		
		wxTextCtrl * txt;
		wxListCtrl * list;

		DECLARE_EVENT_TABLE();
};

#endif
