#pragma once
#include "vlc_controller.h"

namespace vlc_controller {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Runtime::InteropServices;

	/// <summary>
	/// Summary for Training_Form
	/// </summary>
	public ref class Training_Form : public System::Windows::Forms::Form
	{
	public:
		bool is_data_already_exists;
		String^ p_word;
		String^ l_digit;

	private: System::Windows::Forms::Label^  training_complete_msg;
	public: 
	private: System::Windows::Forms::Label^  training_msg;
	private: System::Windows::Forms::Label^  digit;
	private: System::Windows::Forms::Label^  training_word;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker1;
	private: System::Windows::Forms::Label^  utterance_no;
	private: System::Windows::Forms::Label^  speak_word;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  utterance_input;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::TextBox^  speaker_input;
	private: System::Windows::Forms::CheckBox^  checkBox1;
	private: System::Windows::Forms::Button^  record_btn;

	public: 
		//String^ msg;
		Training_Form(void)
		{
			InitializeComponent();
			
			speak_word->Visible=false;
			training_word->Visible=false;
			utterance_no->Visible=false;
			digit->Visible=false;
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Training_Form()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 









	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Training_Form::typeid));
			this->training_complete_msg = (gcnew System::Windows::Forms::Label());
			this->training_msg = (gcnew System::Windows::Forms::Label());
			this->digit = (gcnew System::Windows::Forms::Label());
			this->training_word = (gcnew System::Windows::Forms::Label());
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->utterance_no = (gcnew System::Windows::Forms::Label());
			this->speak_word = (gcnew System::Windows::Forms::Label());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->utterance_input = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->speaker_input = (gcnew System::Windows::Forms::TextBox());
			this->checkBox1 = (gcnew System::Windows::Forms::CheckBox());
			this->record_btn = (gcnew System::Windows::Forms::Button());
			this->tableLayoutPanel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// training_complete_msg
			// 
			this->training_complete_msg->AutoSize = true;
			this->training_complete_msg->BackColor = System::Drawing::Color::Transparent;
			this->training_complete_msg->Font = (gcnew System::Drawing::Font(L"Cambria", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->training_complete_msg->Location = System::Drawing::Point(0, 309);
			this->training_complete_msg->Name = L"training_complete_msg";
			this->training_complete_msg->Size = System::Drawing::Size(378, 38);
			this->training_complete_msg->TabIndex = 31;
			this->training_complete_msg->Text = L"Training is completed...\r\nNow, say the words and it will obey your commands...";
			this->training_complete_msg->Visible = false;
			// 
			// training_msg
			// 
			this->training_msg->AutoSize = true;
			this->training_msg->Font = (gcnew System::Drawing::Font(L"Cambria", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->training_msg->Location = System::Drawing::Point(-1, 309);
			this->training_msg->Name = L"training_msg";
			this->training_msg->Size = System::Drawing::Size(430, 38);
			this->training_msg->TabIndex = 30;
			this->training_msg->Text = L"Training is in progress...\r\nThis might take some time based on the size of your d" 
				L"ataset...";
			this->training_msg->Visible = false;
			// 
			// digit
			// 
			this->digit->AutoSize = true;
			this->digit->BackColor = System::Drawing::Color::Transparent;
			this->digit->Font = (gcnew System::Drawing::Font(L"Cambria", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->digit->Location = System::Drawing::Point(226, 329);
			this->digit->Name = L"digit";
			this->digit->Size = System::Drawing::Size(18, 19);
			this->digit->TabIndex = 29;
			this->digit->Text = L"5";
			// 
			// training_word
			// 
			this->training_word->AutoSize = true;
			this->training_word->BackColor = System::Drawing::Color::Transparent;
			this->training_word->Font = (gcnew System::Drawing::Font(L"Cambria", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->training_word->Location = System::Drawing::Point(224, 307);
			this->training_word->Name = L"training_word";
			this->training_word->Size = System::Drawing::Size(47, 19);
			this->training_word->TabIndex = 28;
			this->training_word->Text = L"Close";
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->WorkerReportsProgress = true;
			this->backgroundWorker1->WorkerSupportsCancellation = true;
			this->backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Training_Form::backgroundWorker1_DoWork);
			this->backgroundWorker1->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler(this, &Training_Form::backgroundWorker1_ProgressChanged);
			this->backgroundWorker1->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &Training_Form::backgroundWorker1_RunWorkerCompleted);
			// 
			// utterance_no
			// 
			this->utterance_no->AutoSize = true;
			this->utterance_no->BackColor = System::Drawing::Color::Transparent;
			this->utterance_no->Font = (gcnew System::Drawing::Font(L"Cambria", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->utterance_no->Location = System::Drawing::Point(136, 329);
			this->utterance_no->Name = L"utterance_no";
			this->utterance_no->Size = System::Drawing::Size(84, 19);
			this->utterance_no->TabIndex = 27;
			this->utterance_no->Text = L"Utterance :";
			// 
			// speak_word
			// 
			this->speak_word->AutoSize = true;
			this->speak_word->BackColor = System::Drawing::Color::Transparent;
			this->speak_word->Font = (gcnew System::Drawing::Font(L"Cambria", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->speak_word->Location = System::Drawing::Point(119, 307);
			this->speak_word->Name = L"speak_word";
			this->speak_word->Size = System::Drawing::Size(105, 19);
			this->speak_word->TabIndex = 26;
			this->speak_word->Text = L"Speak Word : ";
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->BackColor = System::Drawing::Color::Transparent;
			this->tableLayoutPanel1->ColumnCount = 2;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				40.21448F)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				59.78552F)));
			this->tableLayoutPanel1->Controls->Add(this->label1, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->utterance_input, 1, 1);
			this->tableLayoutPanel1->Controls->Add(this->label2, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->speaker_input, 1, 0);
			this->tableLayoutPanel1->Location = System::Drawing::Point(29, 34);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 1;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(373, 86);
			this->tableLayoutPanel1->TabIndex = 25;
			// 
			// label1
			// 
			this->label1->Anchor = System::Windows::Forms::AnchorStyles::Right;
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Cambria", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(42, 12);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(105, 19);
			this->label1->TabIndex = 0;
			this->label1->Text = L"Dataset Name";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// utterance_input
			// 
			this->utterance_input->AcceptsTab = true;
			this->utterance_input->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->utterance_input->Font = (gcnew System::Drawing::Font(L"Cambria", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->utterance_input->Location = System::Drawing::Point(153, 51);
			this->utterance_input->Name = L"utterance_input";
			this->utterance_input->Size = System::Drawing::Size(193, 26);
			this->utterance_input->TabIndex = 3;
			// 
			// label2
			// 
			this->label2->Anchor = System::Windows::Forms::AnchorStyles::Right;
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Cambria", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(26, 55);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(121, 19);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Total Utterances";
			// 
			// speaker_input
			// 
			this->speaker_input->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->speaker_input->Font = (gcnew System::Drawing::Font(L"Cambria", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->speaker_input->Location = System::Drawing::Point(152, 8);
			this->speaker_input->Margin = System::Windows::Forms::Padding(2);
			this->speaker_input->Name = L"speaker_input";
			this->speaker_input->Size = System::Drawing::Size(194, 26);
			this->speaker_input->TabIndex = 6;
			// 
			// checkBox1
			// 
			this->checkBox1->AutoSize = true;
			this->checkBox1->BackColor = System::Drawing::Color::Transparent;
			this->checkBox1->Font = (gcnew System::Drawing::Font(L"Cambria", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->checkBox1->Location = System::Drawing::Point(35, 157);
			this->checkBox1->Name = L"checkBox1";
			this->checkBox1->Size = System::Drawing::Size(367, 23);
			this->checkBox1->TabIndex = 24;
			this->checkBox1->Text = L"Do you want to train on already existing dataset\?";
			this->checkBox1->UseVisualStyleBackColor = false;
			this->checkBox1->CheckedChanged += gcnew System::EventHandler(this, &Training_Form::checkBox1_CheckedChanged);
			// 
			// record_btn
			// 
			this->record_btn->BackColor = System::Drawing::Color::DarkOrange;
			this->record_btn->FlatAppearance->BorderSize = 0;
			this->record_btn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->record_btn->Font = (gcnew System::Drawing::Font(L"Cambria", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->record_btn->ForeColor = System::Drawing::Color::White;
			this->record_btn->Location = System::Drawing::Point(140, 233);
			this->record_btn->Name = L"record_btn";
			this->record_btn->Size = System::Drawing::Size(142, 32);
			this->record_btn->TabIndex = 23;
			this->record_btn->Text = L"Start Recording";
			this->record_btn->UseVisualStyleBackColor = false;
			this->record_btn->Click += gcnew System::EventHandler(this, &Training_Form::record_btn_Click);
			// 
			// Training_Form
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->ClientSize = System::Drawing::Size(429, 408);
			this->Controls->Add(this->training_complete_msg);
			this->Controls->Add(this->training_msg);
			this->Controls->Add(this->digit);
			this->Controls->Add(this->training_word);
			this->Controls->Add(this->utterance_no);
			this->Controls->Add(this->speak_word);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Controls->Add(this->checkBox1);
			this->Controls->Add(this->record_btn);
			this->DoubleBuffered = true;
			this->Name = L"Training_Form";
			this->Text = L"Training Form";
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

private: System::Void record_btn_Click(System::Object^  sender, System::EventArgs^  e) {
			record_btn->Enabled=false;
			training_complete_msg->Visible=false;
			backgroundWorker1->RunWorkerAsync();
		 }
private: System::Void backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
			fp_log = fopen("log_debug_live_training.txt", "w");
			int total_utterances = int::Parse(utterance_input->Text);
			IntPtr p = Marshal::StringToHGlobalAnsi(speaker_input->Text);
			char *speaker_name = static_cast<char*>(p.ToPointer());
			is_data_already_exists = checkBox1->CheckState==CheckState::Checked;
			if(!is_data_already_exists){
				create_directory_structure(speaker_name);
				for(int i=0; i<NO_OF_WORDS; i++){
					char *word = dictionary[i];
					for(int utterance = 1; utterance <= total_utterances; utterance++){
						p_word=gcnew String(word);
						l_digit=utterance.ToString();
						Console::Beep();
						backgroundWorker1->ReportProgress(i*total_utterances + utterance);
						record_data(speaker_name, word, utterance);
					}
				}
			}
			backgroundWorker1->ReportProgress(NO_OF_WORDS * total_utterances + 1);
			train_on_dataset(speaker_name, total_utterances);
			if (backgroundWorker1->WorkerSupportsCancellation == true)
			{
				// Cancel the asynchronous operation.
				e->Cancel = true;
			}
		 }
private: System::Void backgroundWorker1_ProgressChanged(System::Object^  sender, System::ComponentModel::ProgressChangedEventArgs^  e) {
			int total_utterances = int::Parse(utterance_input->Text);
			if(e->ProgressPercentage <= NO_OF_WORDS * total_utterances){			
				speak_word->Visible=true;
				training_word->Visible=true;
				training_word->Text=p_word;
				utterance_no->Visible=true;
				digit->Visible=true;
				digit->Text=l_digit;
			}
			else{
				speak_word->Visible=false;
				training_word->Visible=false;
				utterance_no->Visible=false;
				digit->Visible=false;
				training_msg->Visible=true;
			}
		 }
private: System::Void backgroundWorker1_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) {
			if (e->Cancelled == true)
			{
				training_msg->Visible=false;
				training_complete_msg->Visible=true;
				record_btn->Enabled=true;
				fclose(fp_log);
			}
			else if (e->Error)
			{
				//resultLabel->Text = "Error: " + e->Error->Message;
			}
			else
			{
				//resultLabel->Text = "Done!";
			}
		 }
private: System::Void checkBox1_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 if(checkBox1->CheckState==CheckState::Checked){
				record_btn->Text="Start Training";
			 }
			 else{
				 record_btn->Text="Start Recording";
			 }
		 }
};
}
