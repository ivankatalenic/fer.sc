#include "app.hh"

#include "simple.hh"

wxIMPLEMENT_APP(App);

bool App::OnInit() {
	Simple* simple{new Simple(wxT("Simple Application Frame"))};

	simple->Show(true);

	return true;
}
