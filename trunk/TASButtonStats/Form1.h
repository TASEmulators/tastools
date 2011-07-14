#pragma once

/////////////////////////////////////////////
// INCLUDES
/////////////////////////////////////////////
#include <time.h>
#include <string>
using std::string;
#include "movie.h"

/////////////////////////////////////////////
// GLOBALS
/////////////////////////////////////////////
CMovie* theMovie = 0;

/////////////////////////////////////////////
// Go Bill Go!!!
/////////////////////////////////////////////
namespace PG3702_RL2 {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
		// my stuff
	private: char * szDataPrintBuff;

	public:
		Form1(void)
		{
			szDataPrintBuff = new char[128];
			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			delete [] szDataPrintBuff;
			if (components)
			{
				delete components;
			}
		}




		// form stuff
	private: System::Windows::Forms::Button^  button1;
	private: System::Windows::Forms::TextBox^  txtTASname;
	private: System::Windows::Forms::ListBox^  movieData;
	private: System::Windows::Forms::Label^  label0;

	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Panel^  panelOneButton;

	private: System::Windows::Forms::Panel^  panelZeroButtons;
	private: System::Windows::Forms::Panel^  panelFivePlusButtons;



	private: System::Windows::Forms::Panel^  panelFourButtons;

	private: System::Windows::Forms::Label^  label2;

	private: System::Windows::Forms::Panel^  panelThreeButtons;
	private: System::Windows::Forms::Panel^  panelTwoButtons;


	private: System::Windows::Forms::Label^  label3;

	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label7;
	private: System::Windows::Forms::Label^  label8;
	private: System::Windows::Forms::Label^  label9;
	private: System::Windows::Forms::Label^  label10;
	private: System::Windows::Forms::Label^  label11;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  helpToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  aboutToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;





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
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->txtTASname = (gcnew System::Windows::Forms::TextBox());
			this->movieData = (gcnew System::Windows::Forms::ListBox());
			this->label0 = (gcnew System::Windows::Forms::Label());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->panelZeroButtons = (gcnew System::Windows::Forms::Panel());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->panelOneButton = (gcnew System::Windows::Forms::Panel());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->panelTwoButtons = (gcnew System::Windows::Forms::Panel());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->panelThreeButtons = (gcnew System::Windows::Forms::Panel());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->panelFourButtons = (gcnew System::Windows::Forms::Panel());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->panelFivePlusButtons = (gcnew System::Windows::Forms::Panel());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->helpToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->aboutToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->panel1->SuspendLayout();
			this->panelZeroButtons->SuspendLayout();
			this->panelOneButton->SuspendLayout();
			this->panelTwoButtons->SuspendLayout();
			this->panelThreeButtons->SuspendLayout();
			this->panelFourButtons->SuspendLayout();
			this->panelFivePlusButtons->SuspendLayout();
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(12, 28);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(90, 23);
			this->button1->TabIndex = 9;
			this->button1->Text = L"Open movie file";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
			// 
			// txtTASname
			// 
			this->txtTASname->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtTASname->Location = System::Drawing::Point(13, 58);
			this->txtTASname->Name = L"txtTASname";
			this->txtTASname->ReadOnly = true;
			this->txtTASname->Size = System::Drawing::Size(262, 20);
			this->txtTASname->TabIndex = 10;
			// 
			// movieData
			// 
			this->movieData->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->movieData->FormattingEnabled = true;
			this->movieData->Location = System::Drawing::Point(13, 101);
			this->movieData->Name = L"movieData";
			this->movieData->Size = System::Drawing::Size(468, 355);
			this->movieData->TabIndex = 11;
			// 
			// label0
			// 
			this->label0->AutoSize = true;
			this->label0->Location = System::Drawing::Point(13, 85);
			this->label0->Name = L"label0";
			this->label0->Size = System::Drawing::Size(50, 13);
			this->label0->TabIndex = 12;
			this->label0->Text = L"Summary";
			// 
			// panel1
			// 
			this->panel1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->panel1->Controls->Add(this->panelZeroButtons);
			this->panel1->Controls->Add(this->panelOneButton);
			this->panel1->Controls->Add(this->panelTwoButtons);
			this->panel1->Controls->Add(this->panelThreeButtons);
			this->panel1->Controls->Add(this->panelFourButtons);
			this->panel1->Controls->Add(this->panelFivePlusButtons);
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->label2);
			this->panel1->Controls->Add(this->label3);
			this->panel1->Controls->Add(this->label4);
			this->panel1->Controls->Add(this->label5);
			this->panel1->Controls->Add(this->label6);
			this->panel1->Location = System::Drawing::Point(281, 28);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(200, 60);
			this->panel1->TabIndex = 13;
			// 
			// panelZeroButtons
			// 
			this->panelZeroButtons->BackColor = System::Drawing::Color::Red;
			this->panelZeroButtons->Controls->Add(this->label7);
			this->panelZeroButtons->Location = System::Drawing::Point(0, 0);
			this->panelZeroButtons->Name = L"panelZeroButtons";
			this->panelZeroButtons->Size = System::Drawing::Size(200, 10);
			this->panelZeroButtons->TabIndex = 0;
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->BackColor = System::Drawing::Color::Red;
			this->label7->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label7->Location = System::Drawing::Point(2, 1);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(29, 12);
			this->label7->TabIndex = 15;
			this->label7->Text = L"label7";
			// 
			// panelOneButton
			// 
			this->panelOneButton->BackColor = System::Drawing::Color::DarkOrange;
			this->panelOneButton->Controls->Add(this->label8);
			this->panelOneButton->Location = System::Drawing::Point(0, 10);
			this->panelOneButton->Name = L"panelOneButton";
			this->panelOneButton->Size = System::Drawing::Size(200, 10);
			this->panelOneButton->TabIndex = 1;
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->BackColor = System::Drawing::Color::Orange;
			this->label8->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label8->Location = System::Drawing::Point(2, 1);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(29, 12);
			this->label8->TabIndex = 16;
			this->label8->Text = L"label8";
			// 
			// panelTwoButtons
			// 
			this->panelTwoButtons->BackColor = System::Drawing::Color::Yellow;
			this->panelTwoButtons->Controls->Add(this->label9);
			this->panelTwoButtons->Location = System::Drawing::Point(0, 20);
			this->panelTwoButtons->Name = L"panelTwoButtons";
			this->panelTwoButtons->Size = System::Drawing::Size(200, 10);
			this->panelTwoButtons->TabIndex = 2;
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->BackColor = System::Drawing::Color::Yellow;
			this->label9->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label9->Location = System::Drawing::Point(2, 1);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(29, 12);
			this->label9->TabIndex = 17;
			this->label9->Text = L"label9";
			// 
			// panelThreeButtons
			// 
			this->panelThreeButtons->BackColor = System::Drawing::Color::LightGreen;
			this->panelThreeButtons->Controls->Add(this->label10);
			this->panelThreeButtons->Location = System::Drawing::Point(0, 30);
			this->panelThreeButtons->Name = L"panelThreeButtons";
			this->panelThreeButtons->Size = System::Drawing::Size(200, 10);
			this->panelThreeButtons->TabIndex = 2;
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->BackColor = System::Drawing::Color::LightGreen;
			this->label10->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label10->Location = System::Drawing::Point(2, 1);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(34, 12);
			this->label10->TabIndex = 18;
			this->label10->Text = L"label10";
			// 
			// panelFourButtons
			// 
			this->panelFourButtons->BackColor = System::Drawing::Color::LightSkyBlue;
			this->panelFourButtons->Controls->Add(this->label11);
			this->panelFourButtons->Location = System::Drawing::Point(0, 40);
			this->panelFourButtons->Name = L"panelFourButtons";
			this->panelFourButtons->Size = System::Drawing::Size(200, 10);
			this->panelFourButtons->TabIndex = 3;
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->BackColor = System::Drawing::Color::LightSkyBlue;
			this->label11->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label11->Location = System::Drawing::Point(2, 1);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(34, 12);
			this->label11->TabIndex = 19;
			this->label11->Text = L"label11";
			// 
			// panelFivePlusButtons
			// 
			this->panelFivePlusButtons->BackColor = System::Drawing::Color::Violet;
			this->panelFivePlusButtons->Controls->Add(this->label12);
			this->panelFivePlusButtons->Location = System::Drawing::Point(0, 50);
			this->panelFivePlusButtons->Name = L"panelFivePlusButtons";
			this->panelFivePlusButtons->Size = System::Drawing::Size(200, 10);
			this->panelFivePlusButtons->TabIndex = 4;
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->BackColor = System::Drawing::Color::Violet;
			this->label12->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label12->Location = System::Drawing::Point(2, 1);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(34, 12);
			this->label12->TabIndex = 20;
			this->label12->Text = L"label12";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::SystemColors::Control;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(2, 1);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(29, 12);
			this->label1->TabIndex = 14;
			this->label1->Text = L"label1";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(2, 1);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(29, 12);
			this->label2->TabIndex = 15;
			this->label2->Text = L"label2";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(2, 1);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(29, 12);
			this->label3->TabIndex = 15;
			this->label3->Text = L"label3";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(2, 1);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(29, 12);
			this->label4->TabIndex = 16;
			this->label4->Text = L"label4";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label5->Location = System::Drawing::Point(2, 1);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(29, 12);
			this->label5->TabIndex = 15;
			this->label5->Text = L"label5";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 6.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label6->Location = System::Drawing::Point(2, 1);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(29, 12);
			this->label6->TabIndex = 16;
			this->label6->Text = L"label6";
			// 
			// button2
			// 
			this->button2->Enabled = false;
			this->button2->Location = System::Drawing::Point(108, 28);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(65, 23);
			this->button2->TabIndex = 14;
			this->button2->Text = L"Save stats";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {this->fileToolStripMenuItem, 
				this->helpToolStripMenuItem});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(493, 24);
			this->menuStrip1->TabIndex = 0;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->exitToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(35, 20);
			this->fileToolStripMenuItem->Text = L"&File";
			// 
			// helpToolStripMenuItem
			// 
			this->helpToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->aboutToolStripMenuItem});
			this->helpToolStripMenuItem->Name = L"helpToolStripMenuItem";
			this->helpToolStripMenuItem->Size = System::Drawing::Size(40, 20);
			this->helpToolStripMenuItem->Text = L"&Help";
			// 
			// aboutToolStripMenuItem
			// 
			this->aboutToolStripMenuItem->Name = L"aboutToolStripMenuItem";
			this->aboutToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->aboutToolStripMenuItem->Text = L"&About";
			this->aboutToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::aboutToolStripMenuItem_Click);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(152, 22);
			this->exitToolStripMenuItem->Text = L"E&xit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::exitToolStripMenuItem_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(493, 471);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->label0);
			this->Controls->Add(this->movieData);
			this->Controls->Add(this->txtTASname);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->Text = L"TAS Button Stats";
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &Form1::Form1_FormClosed);
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->panelZeroButtons->ResumeLayout(false);
			this->panelZeroButtons->PerformLayout();
			this->panelOneButton->ResumeLayout(false);
			this->panelOneButton->PerformLayout();
			this->panelTwoButtons->ResumeLayout(false);
			this->panelTwoButtons->PerformLayout();
			this->panelThreeButtons->ResumeLayout(false);
			this->panelThreeButtons->PerformLayout();
			this->panelFourButtons->ResumeLayout(false);
			this->panelFourButtons->PerformLayout();
			this->panelFivePlusButtons->ResumeLayout(false);
			this->panelFivePlusButtons->PerformLayout();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

		///////////////////////////////////////////////////////////////////
		//	My code starts here. Thanks for your help, Bill.
		///////////////////////////////////////////////////////////////////


		///////////////////////////////////////////////////////////////////
		//	Easy way to get the text out of a textbox - trust me
		///////////////////////////////////////////////////////////////////
		string GetTextFromTextBox( System::Windows::Forms::TextBox^ txtBox )
		{
			string strTemp;
			cli::array<wchar_t,1> ^chars = txtBox->Text->ToCharArray();
			for( int i=0, j=chars->Length; i < j; i++ )
				strTemp += (char)chars[i];
			strTemp += (char)0;
			return strTemp;
		}

		///////////////////////////////////////////////////////////////////
		//	Easy way to put text into a textbox - trust me
		///////////////////////////////////////////////////////////////////
		void SetTextToTextBox( System::Windows::Forms::TextBox^ txtBox, const char* szString )
		{
			txtBox->Text = gcnew String( szString );
		}

		///////////////////////////////////////////////////////////////////
		// EventHandler - occurs when the form loads (just after the constructor)
		///////////////////////////////////////////////////////////////////
		System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e) 
		{
			srand( (unsigned int)time(0) );				// seed rand
			CleanForm();
		}

		//
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog();
				 openFileDialog1->Filter = "All supported emulator movies|*.fcm;*fm2;*.gmv;*.smv;*.vbm;*.m64|FCE Ultra movies (*.fcm)|*.fcm|FCE Ultra X movies (*.fm2)|*.fm2|Gens movies (*.gmv)|*.gmv|SNES9x movies (*.smv)|*.smv|Visual Boy Advance movies (*.vbm)|*.vbm|Mupen64 movies (*.m64)|*.m64";
				 openFileDialog1->FilterIndex = 1;
				 openFileDialog1->RestoreDirectory = true;

				 if ( openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				 {
					 {
						 string strTemp;
						 cli::array<wchar_t,1> ^chars = openFileDialog1->FileName->ToCharArray();
						 for( int i=0, j=chars->Length; i < j; i++ )
							 strTemp += (char)chars[i];
						 strTemp += (char)0;
						 strTemp;

						 if (theMovie)
							 delete theMovie;

						 theMovie = new CMovie(strTemp.c_str());
					 }

					 CleanForm();
					 txtTASname->Text = System::IO::Path::GetFileName(openFileDialog1->FileName);

					 if (theMovie->GetType() == 0)
						 System::Windows::Forms::MessageBox::Show(gcnew String("Invalid file"), gcnew String("Error"), MessageBoxButtons::OK, MessageBoxIcon::Error);
					 else
					 {
						 PopulateStats();
						 CreateGraph();
						 button2->Enabled = true;
					 }
				 }
			 }
	private: System::Void Form1_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e)
			 {
				 if (theMovie)
					 delete theMovie;
			 }
	private: void CleanForm()
			 {
				 movieData->Items->Clear();
				 txtTASname->Text = String::Empty;
				 button2->Enabled = false;

				 System::Drawing::Point pt = System::Drawing::Point(-2,-1);

				 label1->Location = pt;
				 label1->Text = String::Empty;
				 label2->Location = System::Drawing::Point(-2,9);
				 label2->Text = String::Empty;
				 label3->Location = System::Drawing::Point(-2,19);
				 label3->Text = String::Empty;
				 label4->Location = System::Drawing::Point(-2,29);
				 label4->Text = String::Empty;
				 label5->Location = System::Drawing::Point(-2,39);
				 label5->Text = String::Empty;
				 label6->Location = System::Drawing::Point(-2,49);
				 label6->Text = String::Empty;
				 label7->Location = pt;
				 label7->Text = String::Empty;
				 label8->Location = pt;
				 label8->Text = String::Empty;
				 label9->Location = pt;
				 label9->Text = String::Empty;
				 label10->Location = pt;
				 label10->Text = String::Empty;
				 label11->Location = pt;
				 label11->Text = String::Empty;
				 label12->Location = pt;
				 label12->Text = String::Empty;

				 panelZeroButtons->Size = System::Drawing::Size(0,10);
				 panelOneButton->Size = System::Drawing::Size(0,10);
				 panelTwoButtons->Size = System::Drawing::Size(0,10);
				 panelThreeButtons->Size = System::Drawing::Size(0,10);
				 panelFourButtons->Size = System::Drawing::Size(0,10);
				 panelFivePlusButtons->Size = System::Drawing::Size(0,10);
			 }
	private: void PopulateStats()
			 {
				 char buffer[128];
				 sprintf_s( buffer, 128, "Movie Input Frames: %d", theMovie->GetFrames());
				 movieData->Items->Add(gcnew String( buffer ));
				 sprintf_s( buffer, 128, "");
				 movieData->Items->Add(gcnew String( buffer ));
				 sprintf_s( buffer, 128, "Button Presses: %d, which is %.4f per frame", theMovie->GetTotalPresses(), theMovie->GetPPF());
				 movieData->Items->Add(gcnew String( buffer ));
				 if (theMovie->GetTotalPresses() != theMovie->GetTotalReleases())
				 {
					 sprintf_s( buffer, 128, "Button Releases: %d, (Because %d buttons (starred) were still down on the final frame)", theMovie->GetTotalReleases(), theMovie->GetTotalPresses() - theMovie->GetTotalReleases());
					 movieData->Items->Add(gcnew String( buffer ));
				 }
				 sprintf_s( buffer, 128, "Frames With Any Buttons Held: %d (%.1f%%)", theMovie->GetFrames() - theMovie->GetFramesWithButtonsHeld(0), 100 - theMovie->GetFramesWithButtonsHeldPercent(0));
				 movieData->Items->Add(gcnew String( buffer ));
				 sprintf_s( buffer, 128, "");
				 movieData->Items->Add(gcnew String( buffer ));
				 sprintf_s( buffer, 128, "button\tpresses\t\theld down\t\tavg hold/press");
				 movieData->Items->Add(gcnew String( buffer ));
				 sprintf_s( buffer, 128, "-----------------------------------------------------------------------------------------------------------------------");
				 movieData->Items->Add(gcnew String( buffer ));

				 switch(theMovie->GetType())
				 {
				 case FCM:
				 case FM2:
					 sprintf_s( buffer, 128, "Left\t%s", ButtonData(0, 6));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Right\t%s", ButtonData(0, 7));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Up\t%s", ButtonData(0, 4));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Down\t%s", ButtonData(0, 5));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Start\t%s", ButtonData(0, 3));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Select\t%s", ButtonData(0, 2));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "A\t%s", ButtonData(0, 0));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "B\t%s", ButtonData(0, 1));
					 movieData->Items->Add(gcnew String( buffer ));
					 break;
				 case SMV:
					 sprintf_s( buffer, 128, "Left\t%s", ButtonData(0, 1));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Right\t%s", ButtonData(0, 0));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Up\t%s", ButtonData(0, 3));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Down\t%s", ButtonData(0, 2));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Start\t%s", ButtonData(0, 4));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Select\t%s", ButtonData(0, 5));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "A\t%s", ButtonData(0, 15));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "B\t%s", ButtonData(0, 7));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "X\t%s", ButtonData(0, 14));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Y\t%s", ButtonData(0, 16));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "L\t%s", ButtonData(0, 13));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "R\t%s", ButtonData(0, 12));
					 movieData->Items->Add(gcnew String( buffer ));
					 break;
				 case GMV:
					 sprintf_s( buffer, 128, "Left\t%s", ButtonData(0, 2));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Right\t%s", ButtonData(0, 3));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Up\t%s", ButtonData(0, 0));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Down\t%s", ButtonData(0, 1));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "A\t%s", ButtonData(0, 4));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "B\t%s", ButtonData(0, 5));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "C\t%s", ButtonData(0, 6));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Start\t%s", ButtonData(0, 7));
					 movieData->Items->Add(gcnew String( buffer ));
					 break;
				 case VBM:
					 sprintf_s( buffer, 128, "Left\t%s", ButtonData(0, 5));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Right\t%s", ButtonData(0, 4));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Up\t%s", ButtonData(0, 6));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Down\t%s", ButtonData(0, 7));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Start\t%s", ButtonData(0, 3));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Select\t%s", ButtonData(0, 2));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "A\t%s", ButtonData(0, 0));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "B\t%s", ButtonData(0, 1));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "L\t%s", ButtonData(0, 9));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "R\t%s", ButtonData(0, 8));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "---MOTION SENSOR---");
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Left\t%s", ButtonData(0, 12));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Right\t%s", ButtonData(0, 13));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Up\t%s", ButtonData(0, 15));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Down\t%s", ButtonData(0, 14));
					 movieData->Items->Add(gcnew String( buffer ));

					 break;
				 case M64:
					 sprintf_s( buffer, 128, "---LEFT SIDE---");
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Left\t%s", ButtonData(0, 25));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Right\t%s", ButtonData(0, 24));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Up\t%s", ButtonData(0, 27));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Down\t%s", ButtonData(0, 26));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "L\t%s", ButtonData(0, 21));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "---CENTER---");
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Stick\t%s", ButtonData(0, 0));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Start\t%s", ButtonData(0, 28));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Z\t%s", ButtonData(0, 29));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "---RIGHT SIDE---");
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "A\t%s", ButtonData(0, 31));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "B\t%s", ButtonData(0, 30));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "C^\t%s", ButtonData(0, 9));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "Cv\t%s", ButtonData(0, 18));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "C<\t%s", ButtonData(0, 17));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "C>\t%s", ButtonData(0, 16));
					 movieData->Items->Add(gcnew String( buffer ));
					 sprintf_s( buffer, 128, "R\t%s", ButtonData(0, 20));
					 movieData->Items->Add(gcnew String( buffer ));
				 default:
					 break;
				 }

			 }
	private: void CreateGraph()
			 {
				 int longest = 0;
				 for (int i = 1; i < 6; ++i)
					 if (theMovie->GetFramesWithButtonsHeld(i) > theMovie->GetFramesWithButtonsHeld(longest))
						 longest = i;
				 float multiplication = 200.0f / theMovie->GetFramesWithButtonsHeldPercent(longest);

				 char buffer[128];
				 if (theMovie->GetFramesWithButtonsHeld(0))
				 {
					 sprintf_s( buffer, 128, "No input: %d (%.1f%%)", theMovie->GetFramesWithButtonsHeld(0), theMovie->GetFramesWithButtonsHeldPercent(0));
					 label1->Text = gcnew String(buffer);
					 label7->Text = gcnew String(buffer);
				 }
				 if (theMovie->GetFramesWithButtonsHeld(1))
				 {
					 sprintf_s( buffer, 128, "1: %d (%.1f%%)", theMovie->GetFramesWithButtonsHeld(1), theMovie->GetFramesWithButtonsHeldPercent(1));
					 label2->Text = gcnew String(buffer);
					 label8->Text = gcnew String(buffer);
				 }
				 if (theMovie->GetFramesWithButtonsHeld(2))
				 {
					 sprintf_s( buffer, 128, "2: %d (%.1f%%)", theMovie->GetFramesWithButtonsHeld(2), theMovie->GetFramesWithButtonsHeldPercent(2));
					 label3->Text = gcnew String(buffer);
					 label9->Text = gcnew String(buffer);
				 }
				 if (theMovie->GetFramesWithButtonsHeld(3))
				 {
					 sprintf_s( buffer, 128, "3: %d (%.1f%%)", theMovie->GetFramesWithButtonsHeld(3), theMovie->GetFramesWithButtonsHeldPercent(3));
					 label4->Text = gcnew String(buffer);
					 label10->Text = gcnew String(buffer);
				 }
				 if (theMovie->GetFramesWithButtonsHeld(4))
				 {
					 sprintf_s( buffer, 128, "4: %d (%.1f%%)", theMovie->GetFramesWithButtonsHeld(4), theMovie->GetFramesWithButtonsHeldPercent(4));
					 label5->Text = gcnew String(buffer);
					 label11->Text = gcnew String(buffer);
				 }
				 if (theMovie->GetFramesWithButtonsHeld(5))
				 {
					 sprintf_s( buffer, 128, "5+: %d (%.1f%%)", theMovie->GetFramesWithButtonsHeld(5), theMovie->GetFramesWithButtonsHeldPercent(5));
					 label6->Text = gcnew String(buffer);
					 label12->Text = gcnew String(buffer);
				 }
				 panelZeroButtons->Size = System::Drawing::Size(int(theMovie->GetFramesWithButtonsHeldPercent(0)*multiplication),10);
				 panelOneButton->Size = System::Drawing::Size(int(theMovie->GetFramesWithButtonsHeldPercent(1)*multiplication),10);
				 panelTwoButtons->Size = System::Drawing::Size(int(theMovie->GetFramesWithButtonsHeldPercent(2)*multiplication),10);
				 panelThreeButtons->Size = System::Drawing::Size(int(theMovie->GetFramesWithButtonsHeldPercent(3)*multiplication),10);
				 panelFourButtons->Size = System::Drawing::Size(int(theMovie->GetFramesWithButtonsHeldPercent(4)*multiplication),10);
				 panelFivePlusButtons->Size = System::Drawing::Size(int(theMovie->GetFramesWithButtonsHeldPercent(5)*multiplication),10);
			 }
	private: char* ButtonData(unsigned int pad, unsigned int button)
			 {
				 sprintf_s( szDataPrintBuff, 128, "%d (%.3f%%)\t%d (%.3f%%)\t\t%.1f", theMovie->GetPresses(button), theMovie->GetPressesPercent(button), theMovie->GetHeld(button), theMovie->GetHeldPercent(button), theMovie->HoldsPerPress(button));
				 return (char*)szDataPrintBuff;
			 }
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 // save to txt file
				 SaveFileDialog^ sfd = gcnew SaveFileDialog();
				 sfd->FileName = txtTASname->Text->Substring(0, txtTASname->Text->Length - 4);
				 sfd->FileName += "-stats";
				 sfd->Filter = "Text file (*.txt)|*.txt";
				 if (sfd->ShowDialog() == System::Windows::Forms::DialogResult::OK)
				 {
					 FileStream^ fs = gcnew FileStream(sfd->FileName, FileMode::OpenOrCreate, FileAccess::Write);
					 System::Text::StringBuilder^ sb = gcnew System::Text::StringBuilder();
					 for (int i = 0; i < movieData->Items->Count; ++i)
					 {
						 sb->Append(movieData->Items[i]->ToString());
						 sb->Append("\r\n");
					 }
					 sb->Append(gcnew String("\r\n\r\nAdditional info...\r\n"));
					 sb->Append(label1->Text);
					 sb->Append("\r\n");
					 sb->Append(label2->Text);
					 sb->Append("\r\n");
					 sb->Append(label3->Text);
					 sb->Append("\r\n");
					 sb->Append(label4->Text);
					 sb->Append("\r\n");
					 sb->Append(label5->Text);
					 sb->Append("\r\n");
					 sb->Append(label6->Text);

					 cli::array<unsigned char,1>^ chars = gcnew cli::array<unsigned char,1>(sb->Length);
					 for( int i=0, j=sb->Length; i < j; i++ )
						 chars[i] = (unsigned char)sb->ToString()->ToCharArray()[i];

					 fs->Write(chars, 0, chars->Length);
					 //done
					 fs->Close();

				 }
			 }
	private: System::Void aboutToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
			 {
				 MessageBox::Show(
					 "Idea: TASVideos' Tool-Assisted Laboratory\nProgramming: Djilani \"bkDJ\" Kebaili\nBugtesting: Randil",
					 "TAS Button Stats",
					 MessageBoxButtons::OK,
					 MessageBoxIcon::Information);
			 }
private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
		 {
			 this->Close();
		 }
};
}