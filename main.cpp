#include <wx/wx.h>
#include <wx/notebook.h>
#include "ids.h"
#include "main.h"
#include "vigenere.h"
#include "affine.h"
#include "frequency.h"
#include "contact.h"
#include "subst.h"

IMPLEMENT_APP(WINAPP);

WA_Frame * window = NULL;

bool WINAPP::OnInit(){
	window = new WA_Frame();
	window->Show(true);
	SetTopWindow(window);


	return true;
}

BEGIN_EVENT_TABLE(WA_Frame, wxFrame)

END_EVENT_TABLE()

WA_Frame::WA_Frame() : wxFrame((wxFrame *)NULL, wxID_ANY, wxT("Basic CryptAnalysis GUI")){

	notebook = new wxNotebook(this, wxID_ANY);

	WA_Frequency * frequency = new WA_Frequency(notebook);
	frequency->Show(true);

	notebook->AddPage(frequency, wxT("Frequency"));

	WA_Contact * contact = new WA_Contact(notebook);
	contact->Show(true);

	notebook->AddPage(contact, wxT("Contact"));

	WA_Vigenere * vigenere = new WA_Vigenere(notebook);
	vigenere->Show(true);

	notebook->AddPage(vigenere, wxT("Vigenere"));

	WA_Affine * affine = new WA_Affine(notebook);
	affine->Show(true);

	notebook->AddPage(affine, wxT("Affine"));

	WA_Subst * subst = new WA_Subst(notebook);
	subst->Show(true);

	notebook->AddPage(subst, wxT("Substitution"));

	main_sizer = new wxBoxSizer(wxHORIZONTAL);
	main_sizer->Add(notebook, 1, wxEXPAND);

	SetSizer(main_sizer);

	main_sizer->SetSizeHints(this);

	SetSize(wxSize(800,600));
}

WA_Frame::~WA_Frame(){

}


