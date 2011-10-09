#ifndef MAIN_H
#define MAIN_H

class WINAPP : public wxApp{
	public:
		virtual bool OnInit();
};

class WA_Frame : public wxFrame{
	public:
		WA_Frame();
		~WA_Frame();
		
		wxNotebook * notebook;
		
		wxBoxSizer * main_sizer;

		DECLARE_EVENT_TABLE();
};

#endif
