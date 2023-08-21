#pragma once

namespace vlc_controller {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for help
	/// </summary>
	public ref class help : public System::Windows::Forms::Form
	{
	public:
		help(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~help()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^  label17;
	protected: 
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::Label^  label15;
	private: System::Windows::Forms::Label^  label16;

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(help::typeid));
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->tableLayoutPanel1->SuspendLayout();
			this->SuspendLayout();
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->BackColor = System::Drawing::Color::Transparent;
			this->label17->Font = (gcnew System::Drawing::Font(L"Cambria", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label17->Location = System::Drawing::Point(12, 9);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(198, 25);
			this->label17->TabIndex = 3;
			this->label17->Text = L"Features provided:";
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->BackColor = System::Drawing::Color::Transparent;
			this->tableLayoutPanel1->ColumnCount = 2;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				15)));
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				85)));
			this->tableLayoutPanel1->Controls->Add(this->label1, 1, 0);
			this->tableLayoutPanel1->Controls->Add(this->label2, 1, 1);
			this->tableLayoutPanel1->Controls->Add(this->label3, 1, 2);
			this->tableLayoutPanel1->Controls->Add(this->label4, 1, 3);
			this->tableLayoutPanel1->Controls->Add(this->label5, 1, 4);
			this->tableLayoutPanel1->Controls->Add(this->label6, 1, 5);
			this->tableLayoutPanel1->Controls->Add(this->label7, 1, 6);
			this->tableLayoutPanel1->Controls->Add(this->label8, 1, 7);
			this->tableLayoutPanel1->Controls->Add(this->label9, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->label10, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->label11, 0, 2);
			this->tableLayoutPanel1->Controls->Add(this->label12, 0, 3);
			this->tableLayoutPanel1->Controls->Add(this->label13, 0, 4);
			this->tableLayoutPanel1->Controls->Add(this->label14, 0, 5);
			this->tableLayoutPanel1->Controls->Add(this->label15, 0, 6);
			this->tableLayoutPanel1->Controls->Add(this->label16, 0, 7);
			this->tableLayoutPanel1->Font = (gcnew System::Drawing::Font(L"Cambria", 14.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->tableLayoutPanel1->Location = System::Drawing::Point(12, 48);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 8;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 12.5F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 12.5F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 12.5F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 12.5F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 12.5F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 12.5F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 12.5F)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 12.5F)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(401, 451);
			this->tableLayoutPanel1->TabIndex = 2;
			// 
			// label1
			// 
			this->label1->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(63, 17);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(113, 22);
			this->label1->TabIndex = 0;
			this->label1->Text = L"open movies";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label2
			// 
			this->label2->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(63, 73);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(122, 22);
			this->label2->TabIndex = 1;
			this->label2->Text = L"window open";
			// 
			// label3
			// 
			this->label3->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(63, 129);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(121, 22);
			this->label3->TabIndex = 2;
			this->label3->Text = L"window close";
			// 
			// label4
			// 
			this->label4->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(63, 185);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(103, 22);
			this->label4->TabIndex = 3;
			this->label4->Text = L"window up";
			// 
			// label5
			// 
			this->label5->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(63, 241);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(128, 22);
			this->label5->TabIndex = 4;
			this->label5->Text = L"window down";
			this->label5->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label6
			// 
			this->label6->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(63, 297);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(77, 22);
			this->label6->TabIndex = 5;
			this->label6->Text = L"vlc close";
			this->label6->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label7
			// 
			this->label7->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(63, 353);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(124, 22);
			this->label7->TabIndex = 6;
			this->label7->Text = L"vlc volume up";
			this->label7->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label8
			// 
			this->label8->Anchor = System::Windows::Forms::AnchorStyles::Left;
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(63, 410);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(149, 22);
			this->label8->TabIndex = 7;
			this->label8->Text = L"vlc volume down";
			this->label8->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label9
			// 
			this->label9->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(16, 17);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(28, 22);
			this->label9->TabIndex = 8;
			this->label9->Text = L"1)";
			this->label9->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label10
			// 
			this->label10->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(16, 73);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(28, 22);
			this->label10->TabIndex = 9;
			this->label10->Text = L"2)";
			this->label10->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label11
			// 
			this->label11->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(16, 129);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(28, 22);
			this->label11->TabIndex = 10;
			this->label11->Text = L"3)";
			this->label11->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label12
			// 
			this->label12->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(16, 185);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(28, 22);
			this->label12->TabIndex = 11;
			this->label12->Text = L"4)";
			this->label12->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label13
			// 
			this->label13->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(16, 241);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(28, 22);
			this->label13->TabIndex = 12;
			this->label13->Text = L"5)";
			this->label13->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label14
			// 
			this->label14->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(16, 297);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(28, 22);
			this->label14->TabIndex = 13;
			this->label14->Text = L"6)";
			this->label14->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label15
			// 
			this->label15->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(16, 353);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(28, 22);
			this->label15->TabIndex = 14;
			this->label15->Text = L"7)";
			this->label15->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// label16
			// 
			this->label16->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(16, 410);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(28, 22);
			this->label16->TabIndex = 15;
			this->label16->Text = L"8)";
			this->label16->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// help
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::White;
			this->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"$this.BackgroundImage")));
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->ClientSize = System::Drawing::Size(437, 523);
			this->Controls->Add(this->label17);
			this->Controls->Add(this->tableLayoutPanel1);
			this->DoubleBuffered = true;
			this->Name = L"help";
			this->Text = L"Command guide";
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	};
}
