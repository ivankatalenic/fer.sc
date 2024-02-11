#include "simple.hh"

#include "processing.hh"
#include "import.hh"
#include "export.hh"
#include "mapper.hh"
#include "vector_utils.hh"

#include "wx/window.h"
#include "wx/splitter.h"

#include <iostream>
#include <cstdlib>

void save_gestures(const std::string& filename, const std::vector<Gesture> gestures);

Simple::Simple(const wxString& title):
	wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1200, 800)) {

	Centre();
	CreateStatusBar();
	SetStatusText("Drawing application");

	wxSplitterWindow* window_splitter{
		new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_BORDER)
	};

	wxWindow* window_draw{new wxWindow(
		window_splitter,
		wxID_ANY,
		wxDefaultPosition,
		wxDefaultSize,
		wxBORDER_SIMPLE
	)};
	wxPanel* panel_control{new wxPanel(
		window_splitter,
		wxID_ANY,
		wxDefaultPosition,
		wxDefaultSize,
		wxBORDER_SIMPLE
	)};

	window_splitter->SplitVertically(window_draw, panel_control, 500);

	dc = new wxClientDC(window_draw);
	const wxPen* pen{wxBLACK_PEN};
	dc->SetPen(*pen);

	wxBoxSizer* sizer_panel_ctrl{new wxBoxSizer(wxVERTICAL)};

	wxStaticBoxSizer* sizer_gesture{new wxStaticBoxSizer(wxVERTICAL, panel_control, "Gesture")};
	
	wxButton* button_gesture_display{new wxButton(sizer_gesture->GetStaticBox(), wxID_ANY, wxString("Display"))};
	sizer_gesture->Add(button_gesture_display, 0, wxEXPAND | wxALL, 5);

	wxStaticBoxSizer* sizer_labels{new wxStaticBoxSizer(wxHORIZONTAL, sizer_gesture->GetStaticBox(), "Label")};
	wxButton*         button_alpha{new wxButton(sizer_labels->GetStaticBox(), wxID_ANY, wxString("Alpha"))};
	wxButton*         button_beta{new wxButton(sizer_labels->GetStaticBox(), wxID_ANY, wxString("Beta"))};
	wxButton*         button_gamma{new wxButton(sizer_labels->GetStaticBox(), wxID_ANY, wxString("Gamma"))};
	wxButton*         button_delta{new wxButton(sizer_labels->GetStaticBox(), wxID_ANY, wxString("Delta"))};
	wxButton*         button_epsilon{new wxButton(sizer_labels->GetStaticBox(), wxID_ANY, wxString("Epsilon"))};
	sizer_gesture->Add(sizer_labels, 0, wxEXPAND | wxALL, 5);
	sizer_labels->Add(button_alpha, 0, wxEXPAND | wxALL, 5);
	sizer_labels->Add(button_beta, 0, wxEXPAND | wxALL, 5);
	sizer_labels->Add(button_gamma, 0, wxEXPAND | wxALL, 5);
	sizer_labels->Add(button_delta, 0, wxEXPAND | wxALL, 5);
	sizer_labels->Add(button_epsilon, 0, wxEXPAND | wxALL, 5);

	wxStaticBoxSizer* sizer_file{new wxStaticBoxSizer(wxHORIZONTAL, sizer_gesture->GetStaticBox(), "Save/Load")};
	wxStaticText*     text_filename{new wxStaticText(sizer_file->GetStaticBox(), wxID_ANY, "Filename:")};
	wxTextCtrl*       input_filename{new wxTextCtrl(sizer_file->GetStaticBox(), wxID_ANY)};
	wxButton*         button_file_load{new wxButton(sizer_file->GetStaticBox(), wxID_ANY, wxString("Load"))};
	wxButton*         button_file_save{new wxButton(sizer_file->GetStaticBox(), wxID_ANY, wxString("Save"))};
	sizer_file->Add(text_filename, 0, wxALL, 5);
	sizer_file->Add(input_filename, 0, wxEXPAND | wxALL, 5);
	sizer_file->Add(button_file_load, 0, wxEXPAND | wxALL, 5);
	sizer_file->Add(button_file_save, 0, wxEXPAND | wxALL, 5);
	sizer_gesture->Add(sizer_file, 0, wxEXPAND | wxALL, 5);

	wxStaticBoxSizer* sizer_network{new wxStaticBoxSizer(wxVERTICAL, panel_control, "Network")};
	
	wxBoxSizer*   sizer_architecture{new wxBoxSizer(wxHORIZONTAL)};
	wxStaticText* text_architecture{new wxStaticText(sizer_network->GetStaticBox(), wxID_ANY, "Architecture:")};
	wxTextCtrl*   input_architecture{new wxTextCtrl(sizer_network->GetStaticBox(), wxID_ANY)};
	sizer_architecture->Add(text_architecture, 0, wxALL, 5);
	sizer_architecture->Add(input_architecture, 0, wxEXPAND | wxALL, 5);
	sizer_network->Add(sizer_architecture, 0, wxEXPAND | wxALL, 5);

	wxBoxSizer*   sizer_learning_rate{new wxBoxSizer(wxHORIZONTAL)};
	wxStaticText* text_learning_rate{new wxStaticText(sizer_network->GetStaticBox(), wxID_ANY, "Learning rate:")};
	wxTextCtrl*   input_learning_rate{new wxTextCtrl(sizer_network->GetStaticBox(), wxID_ANY)};
	sizer_learning_rate->Add(text_learning_rate, 0, wxALL, 5);
	sizer_learning_rate->Add(input_learning_rate, 0, wxEXPAND | wxALL, 5);
	sizer_network->Add(sizer_learning_rate, 0, wxEXPAND | wxALL, 5);

	wxBoxSizer*   sizer_epochs_max{new wxBoxSizer(wxHORIZONTAL)};
	wxStaticText* text_epochs_max{new wxStaticText(sizer_network->GetStaticBox(), wxID_ANY, "Epochs max:")};
	wxTextCtrl*   input_epochs_max{new wxTextCtrl(sizer_network->GetStaticBox(), wxID_ANY)};
	sizer_epochs_max->Add(text_epochs_max, 0, wxALL, 5);
	sizer_epochs_max->Add(input_epochs_max, 0, wxEXPAND | wxALL, 5);
	sizer_network->Add(sizer_epochs_max, 0, wxEXPAND | wxALL, 5);

	wxBoxSizer*   sizer_batch_size{new wxBoxSizer(wxHORIZONTAL)};
	wxStaticText* text_batch_size{new wxStaticText(sizer_network->GetStaticBox(), wxID_ANY, "Batch size:")};
	wxTextCtrl*   input_batch_size{new wxTextCtrl(sizer_network->GetStaticBox(), wxID_ANY)};
	sizer_batch_size->Add(text_batch_size, 0, wxALL, 5);
	sizer_batch_size->Add(input_batch_size, 0, wxEXPAND | wxALL, 5);
	sizer_network->Add(sizer_batch_size, 0, wxEXPAND | wxALL, 5);
	
	wxButton* button_network_train{new wxButton(sizer_network->GetStaticBox(), wxID_ANY, wxString("Train"))};
	wxButton* button_network_predict{new wxButton(sizer_network->GetStaticBox(), wxID_ANY, wxString("Predict"))};
	
	wxStaticText* text_loss{new wxStaticText(sizer_network->GetStaticBox(), wxID_ANY, "Loss:")};
	wxTextCtrl* output_loss{new wxTextCtrl(
		sizer_network->GetStaticBox(),
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	)};
	wxStaticText* text_prediction{new wxStaticText(sizer_network->GetStaticBox(), wxID_ANY, "Prediction:")};
	wxTextCtrl* output_prediction{new wxTextCtrl(
		sizer_network->GetStaticBox(),
		wxID_ANY,
		wxEmptyString,
		wxDefaultPosition,
		wxDefaultSize,
		wxTE_READONLY
	)};
	sizer_network->Add(button_network_train, 0, wxEXPAND | wxALL, 5);
	sizer_network->Add(button_network_predict, 0, wxEXPAND | wxALL, 5);
	sizer_network->Add(text_loss, 0, wxEXPAND | wxALL, 5);
	sizer_network->Add(output_loss, 0, wxEXPAND | wxALL, 5);
	sizer_network->Add(text_prediction, 0, wxEXPAND | wxALL, 5);
	sizer_network->Add(output_prediction, 0, wxEXPAND | wxALL, 5);

	sizer_panel_ctrl->Add(sizer_gesture, 0, wxEXPAND | wxALL, 5);
	sizer_panel_ctrl->Add(sizer_network, 0, wxEXPAND | wxALL, 5);
	panel_control->SetSizerAndFit(sizer_panel_ctrl);

	// Setting events handlers
	button_alpha->Bind(wxEVT_BUTTON, [=](wxCommandEvent& e) {
		const std::string& label{button_alpha->GetLabel().ToStdString()};
		gestures.push_back({label, positions});
		std::cout << "saved: " << label << std::endl;
	});
	button_beta->Bind(wxEVT_BUTTON, [=](wxCommandEvent& e) {
		const std::string& label{button_beta->GetLabel().ToStdString()};
		gestures.push_back({label, positions});
		std::cout << "saved: " << label << std::endl;
	});
	button_gamma->Bind(wxEVT_BUTTON, [=](wxCommandEvent& e) {
		const std::string& label{button_gamma->GetLabel().ToStdString()};
		gestures.push_back({label, positions});
		std::cout << "saved: " << label << std::endl;
	});
	button_delta->Bind(wxEVT_BUTTON, [=](wxCommandEvent &e) {
		const std::string& label{button_delta->GetLabel().ToStdString()};
		gestures.push_back({label, positions});
		std::cout << "saved: " << label << std::endl;
	});
	button_epsilon->Bind(wxEVT_BUTTON, [=](wxCommandEvent& e) {
		const std::string& label{button_epsilon->GetLabel().ToStdString()};
		gestures.push_back({label, positions});
		std::cout << "saved: " << label << std::endl;
	});
	button_gesture_display->Bind(wxEVT_BUTTON, [=](wxCommandEvent& e) {
		dc->Clear();
		wxSize size{dc->GetSize()};
		double scale{static_cast<double>(std::min(size.GetWidth(), size.GetHeight())) / 2};

		for (const Position& pos : positions) {
			wxRealPoint p{pos.x * scale + size.GetWidth() / 2, pos.y * scale + size.GetHeight() / 2};
			dc->DrawCircle(p, 2);
		}
	});
	button_file_save->Bind(wxEVT_BUTTON, [=](wxCommandEvent& e) {
		const std::string filename{input_filename->GetLineText(0).ToStdString()};
		Export::save_gestures(filename, gestures);
	});
	button_file_load->Bind(wxEVT_BUTTON, [=](wxCommandEvent& e) {
		const std::string filename{input_filename->GetLineText(0).ToStdString()};
		gestures = Import::gestures_from_file(filename);
	});
	button_network_train->Bind(wxEVT_BUTTON, [=](wxCommandEvent& e) {
		// Get parameters
		const std::string arch_str{input_architecture->GetLineText(0).ToStdString()};
		const std::vector<int> arch{Import::config_arch_from_string(arch_str)};

		bool is_valid{false};
		double learning_rate{0};
		is_valid = input_learning_rate->GetLineText(0).ToDouble(&learning_rate);
		if (!is_valid) {
			throw std::invalid_argument("can't parse the learning rate");
		}

		long epoch_max_l{0};
		is_valid = input_epochs_max->GetLineText(0).ToLong(&epoch_max_l);
		if (!is_valid) {
			throw std::invalid_argument("can't parse the epochs max");
		}
		const int epoch_max{static_cast<int>(epoch_max_l)};

		long batch_size_l{0};
		is_valid = input_epochs_max->GetLineText(0).ToLong(&batch_size_l);
		if (!is_valid) {
			throw std::invalid_argument("can't parse the batch size");
		}
		const int batch_size{static_cast<int>(batch_size_l)};

		// Prepare data
		std::pair<std::vector<Matrix>, std::vector<Matrix>> data{Mapper::map_gestures(gestures)};

		// Create the network
		network = new Network(arch);

		// Train the network
		try {
			network->fit(
				data.first,
				data.second,
				learning_rate,
				epoch_max,
				batch_size
			);
		} catch (const std::exception& e) {
			std::cout << e.what() << std::endl;
			return;
		}

		const double loss{network->loss(data.first, data.second)};
		output_loss->Clear();
		(*output_loss) << loss;
	});
	button_network_predict->Bind(wxEVT_BUTTON, [=](wxCommandEvent& e) {
		if (network == nullptr) {
			std::cout << "can't predict because the network hasn't been trained yet" << std::endl;
			return;
		}

		const Matrix sample{Mapper::map_positions(positions)};
		const Matrix label_matrix{network->predict(sample)};
		std::cout << VectorUtils::format(label_matrix) << std::endl;
		const std::string label{Mapper::map_to_label(label_matrix)};
		output_prediction->Clear();
		(*output_prediction) << label;
	});

	window_draw->Bind(wxEVT_LEFT_DOWN, Simple::OnLeftDown, this);
	window_draw->Bind(wxEVT_LEFT_UP, Simple::OnLeftUp, this);
	window_draw->Bind(wxEVT_MOTION, Simple::OnMotion, this);
	Bind(wxEVT_CLOSE_WINDOW, Simple::OnClose, this);
}

void Simple::OnClose(wxCloseEvent& event) {
	if (!event.CanVeto()) {
		Destroy();
	}
	Destroy();
}

void Simple::OnMotion(wxMouseEvent& event) {
	if (!is_left_down) {
		return;
	}

	const wxPoint& point{event.GetPosition()};
	dc->DrawPoint(point);
	positions.push_back({static_cast<double>(point.x), static_cast<double>(point.y)});
}

void Simple::OnLeftDown(wxMouseEvent& event) {
	is_left_down = true;

	dc->Clear();
	positions.clear();
}

void Simple::OnLeftUp(wxMouseEvent& event) {
	is_left_down = false;

	if (positions.size() < 2) {
		return;
	}

	try {
		const auto& positions_processed{Processing::process_positions(positions, repre_cnt)};
		positions = positions_processed;
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
}
