#pragma once

#include "gesture.hh"
#include "position.hh"
#include "network.hh"

#include "wx/wx.h"

class Simple : public wxFrame {
public:
	Simple(const wxString& title);
	
	void OnClose(wxCloseEvent& event);
	void OnMotion(wxMouseEvent& event);
	void OnLeftDown(wxMouseEvent& event);
	void OnLeftUp(wxMouseEvent& event);
private:
	std::vector<Position> positions;
	std::vector<Gesture> gestures;

	wxClientDC* dc;

	bool is_left_down{false};
	int repre_cnt{20};

	Network* network;
};
