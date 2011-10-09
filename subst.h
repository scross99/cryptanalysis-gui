#ifndef SUBST_H
#define SUBST_H

#include <wx/wx.h>
#include <wx/grid.h>

class WA_Subst : public wxPanel{
	public:
		WA_Subst(wxWindow * parent);
		~WA_Subst();
		
		void on_select(wxCommandEvent& event);
		
		void on_txt_changed(wxCommandEvent& event);
		
		void on_cell_change(wxGridEvent& event);
		
		void txt_update();

		wxBoxSizer * main_sizer;
		
		wxBoxSizer * txt_sizer;
		
		wxTextCtrl * txt_cipher;
		
		wxTextCtrl * txt_plain;
		
		wxButton * select;
		
		wxGrid * grid;
		
		char conv_table[256];

		DECLARE_EVENT_TABLE();
};

#endif
