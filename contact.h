#ifndef CONTACT_H
#define CONTACT_H

#include <wx/wx.h>
#include <wx/grid.h>

class WA_Contact : public wxPanel{
	public:
		WA_Contact(wxWindow * parent);
		~WA_Contact();
		
		void on_txt_changed(wxCommandEvent& event);
		
		void on_check(wxCommandEvent& event);

		wxBoxSizer * main_sizer;
		wxBoxSizer * grid_sizer;
		
		wxTextCtrl * txt;
		wxGrid * grid;
		wxCheckBox * asym;
		
		bool is_asym;

		DECLARE_EVENT_TABLE();
};

#endif
