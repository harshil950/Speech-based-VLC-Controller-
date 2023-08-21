#pragma once
#include "help.h"
#include "Training_Form.h"
namespace vlc_controller {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		String^ msg;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Label^  label1;

	public: 
		bool isStart;
		Form1(void)
		{
			InitializeComponent();
			isStart = true;
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 
	private: System::Windows::Forms::Button^  help_btn;
	public: System::Windows::Forms::Button^  start;
	private: 

	private: System::Windows::Forms::Button^  training_btn;

	private: System::Windows::Forms::Label^  resultLabel;
	private: System::Windows::Forms::RichTextBox^  richTextBox1;
	private: System::ComponentModel::BackgroundWorker^  backgroundWorker1;
	private: System::ComponentModel::IContainer^  components;

	protected: 





	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(Form1::typeid));
			this->help_btn = (gcnew System::Windows::Forms::Button());
			this->start = (gcnew System::Windows::Forms::Button());
			this->training_btn = (gcnew System::Windows::Forms::Button());
			this->resultLabel = (gcnew System::Windows::Forms::Label());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->backgroundWorker1 = (gcnew System::ComponentModel::BackgroundWorker());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// help_btn
			// 
			this->help_btn->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->help_btn->BackColor = System::Drawing::Color::DarkOrange;
			this->help_btn->FlatAppearance->BorderSize = 0;
			this->help_btn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->help_btn->Font = (gcnew System::Drawing::Font(L"Cambria", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->help_btn->ForeColor = System::Drawing::Color::White;
			this->help_btn->Location = System::Drawing::Point(248, 231);
			this->help_btn->Name = L"help_btn";
			this->help_btn->Size = System::Drawing::Size(145, 34);
			this->help_btn->TabIndex = 4;
			this->help_btn->TabStop = false;
			this->help_btn->Text = L"Command Guide";
			this->help_btn->UseVisualStyleBackColor = false;
			this->help_btn->Click += gcnew System::EventHandler(this, &Form1::help_btn_Click);
			// 
			// start
			// 
			this->start->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->start->BackColor = System::Drawing::Color::DarkOrange;
			this->start->FlatAppearance->BorderColor = System::Drawing::Color::White;
			this->start->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->start->Font = (gcnew System::Drawing::Font(L"Arial", 8.5F));
			this->start->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"start.Image")));
			this->start->Location = System::Drawing::Point(182, 287);
			this->start->Name = L"start";
			this->start->Size = System::Drawing::Size(56, 52);
			this->start->TabIndex = 0;
			this->start->UseVisualStyleBackColor = false;
			this->start->Click += gcnew System::EventHandler(this, &Form1::start_Click);
			// 
			// training_btn
			// 
			this->training_btn->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->training_btn->BackColor = System::Drawing::Color::DarkOrange;
			this->training_btn->FlatAppearance->BorderColor = System::Drawing::Color::Black;
			this->training_btn->FlatAppearance->BorderSize = 0;
			this->training_btn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->training_btn->Font = (gcnew System::Drawing::Font(L"Cambria", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->training_btn->ForeColor = System::Drawing::Color::White;
			this->training_btn->Location = System::Drawing::Point(27, 231);
			this->training_btn->Name = L"training_btn";
			this->training_btn->Size = System::Drawing::Size(145, 34);
			this->training_btn->TabIndex = 6;
			this->training_btn->Text = L"Train Dataset";
			this->training_btn->UseVisualStyleBackColor = false;
			this->training_btn->Click += gcnew System::EventHandler(this, &Form1::training_btn_Click);
			// 
			// resultLabel
			// 
			this->resultLabel->AutoSize = true;
			this->resultLabel->Location = System::Drawing::Point(21, 126);
			this->resultLabel->Name = L"resultLabel";
			this->resultLabel->Size = System::Drawing::Size(0, 13);
			this->resultLabel->TabIndex = 9;
			// 
			// richTextBox1
			// 
			this->richTextBox1->Font = (gcnew System::Drawing::Font(L"Cambria", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->richTextBox1->Location = System::Drawing::Point(12, 345);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(413, 166);
			this->richTextBox1->TabIndex = 8;
			this->richTextBox1->Text = L"";
			this->richTextBox1->TextChanged += gcnew System::EventHandler(this, &Form1::richTextBox1_TextChanged);
			// 
			// backgroundWorker1
			// 
			this->backgroundWorker1->WorkerReportsProgress = true;
			this->backgroundWorker1->WorkerSupportsCancellation = true;
			this->backgroundWorker1->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &Form1::backgroundWorker1_DoWork);
			this->backgroundWorker1->ProgressChanged += gcnew System::ComponentModel::ProgressChangedEventHandler(this, &Form1::backgroundWorker1_ProgressChanged);
			this->backgroundWorker1->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &Form1::backgroundWorker1_RunWorkerCompleted);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Interval = 500;
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// pictureBox1
			// 
			this->pictureBox1->Image = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.Image")));
			this->pictureBox1->InitialImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.InitialImage")));
			this->pictureBox1->Location = System::Drawing::Point(24, 27);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(163, 152);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 10;
			this->pictureBox1->TabStop = false;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Cambria", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(199, 27);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(226, 152);
			this->label1->TabIndex = 11;
			this->label1->Text = resources->GetString(L"label1.Text");
			this->label1->Click += gcnew System::EventHandler(this, &Form1::label1_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->ClientSize = System::Drawing::Size(437, 523);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->help_btn);
			this->Controls->Add(this->start);
			this->Controls->Add(this->resultLabel);
			this->Controls->Add(this->training_btn);
			this->Controls->Add(this->richTextBox1);
			this->Name = L"Form1";
			this->Text = L"VLC controller";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void start_Click(System::Object^  sender, System::EventArgs^  e) {
			if(isStart){
				isStart=false;
				start->BackColor=Color::Red;
				if (backgroundWorker1->IsBusy != true){
					// Start the asynchronous operation.
					backgroundWorker1->RunWorkerAsync();
					DateTime datetime = DateTime::Now;
 					msg=richTextBox1->Text;
					msg+=datetime.ToString()+"\tStarted Recording!\n";
					richTextBox1->Text=msg;
				}
				
			}
			else{
				isStart=true;
				start->BackColor=Color::DarkOrange;
				if (backgroundWorker1->WorkerSupportsCancellation == true){
					// Cancel the asynchronous operation.
					backgroundWorker1->CancelAsync();
				}
			}

			/*if (backgroundWorker1->IsBusy != true){
				// Start the asynchronous operation.
				backgroundWorker1->RunWorkerAsync();
			}
			start->Enabled=false;
			stop->Enabled=true;
			DateTime datetime = DateTime::Now;
 			msg=richTextBox1->Text;
			msg+=datetime.ToString()+"\tStarted Recording!\n";
			richTextBox1->Text=msg;*/
		}
/*
private: System::Void stop_Click(System::Object^  sender, System::EventArgs^  e) {
			if (backgroundWorker1->WorkerSupportsCancellation == true){
				// Cancel the asynchronous operation.
				backgroundWorker1->CancelAsync();
			}
			stop->Enabled=false;
			start->Enabled=true;
		 }
*/
private: System::Void help_btn_Click(System::Object^  sender, System::EventArgs^  e) {
			help^ obj=gcnew help();
			obj->ShowDialog();
		 }

private: System::Void training_btn_Click(System::Object^  sender, System::EventArgs^  e) {
			Training_Form^ obj=gcnew Training_Form();
			obj->ShowDialog();
		 }

private: System::Void backgroundWorker1_DoWork(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e) {
			fp_log = fopen("log_debug.txt", "w");
	
			int i=0;
			while(true)
			{
				if (backgroundWorker1->CancellationPending == true)
				{
					e->Cancel = true;
					break;
				}
				else
				{
					Console::Beep();
					main_functionality(i);
					msg+="predicted : ";
					for(int j=0;j<words;j++){
						msg+=gcnew String(dictionary[predicted_words[j]]) + ",";
					}
					msg+="\n";
					i++;
					backgroundWorker1->ReportProgress(i);
				}
			}
		 }

private: System::Void backgroundWorker1_ProgressChanged(System::Object^  sender, System::ComponentModel::ProgressChangedEventArgs^  e) {
			richTextBox1->Text=msg;
		 }
private: System::Void backgroundWorker1_RunWorkerCompleted(System::Object^  sender, System::ComponentModel::RunWorkerCompletedEventArgs^  e) {
			if (e->Cancelled == true)
			{
				//resultLabel->Text = "Canceled!";
				DateTime datetime = DateTime::Now;
				msg=richTextBox1->Text;
				msg+=datetime.ToString()+"\tStopped Recording!\n";
				richTextBox1->Text=msg;
				fclose(fp_log);
			}
			else if (e->Error)
			{
				resultLabel->Text = "Error: " + e->Error->Message;
			}
			else
			{
				//resultLabel->Text = "Done!";
			}
		 }
private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
			 if(start->BackColor==Color::Red){
				start->BackColor=Color::Black;
			 } else if(start->BackColor==Color::Black){
				start->BackColor=Color::Red;				
			 }
		 }
private: System::Void richTextBox1_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
};
}

