#ifndef AFFINE_H
#define AFFINE_H

#include <wx/wx.h>

class WA_Affine : public wxPanel{
	public:
		WA_Affine(wxWindow * parent);
		~WA_Affine();
		
		void on_txt_changed(wxCommandEvent& event);
		
		wxBoxSizer * main_sizer;
		wxBoxSizer * num_sizer;
		
		wxTextCtrl * txt;
		wxTextCtrl * result;
		wxTextCtrl * num_a;
		wxTextCtrl * num_b;

		DECLARE_EVENT_TABLE();
};

#endif
