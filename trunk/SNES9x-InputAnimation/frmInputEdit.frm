VERSION 5.00
Begin VB.Form frmInputEdit 
   BackColor       =   &H8000000B&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "SNES9x Input Animator"
   ClientHeight    =   5820
   ClientLeft      =   150
   ClientTop       =   720
   ClientWidth     =   7170
   BeginProperty Font 
      Name            =   "Arial"
      Size            =   9.75
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H00000000&
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5820
   ScaleWidth      =   7170
   StartUpPosition =   3  'Windows Default
   Begin VB.CheckBox chkRoll 
      Caption         =   "Roll Input?"
      Height          =   510
      Left            =   270
      TabIndex        =   8
      Top             =   3330
      Width           =   1410
   End
   Begin VB.CommandButton cmdText 
      Caption         =   "&Text"
      Height          =   510
      Left            =   1755
      TabIndex        =   9
      Top             =   3330
      Width           =   1545
   End
   Begin VB.CommandButton cmdInvert 
      Caption         =   "In&vert"
      Height          =   510
      Left            =   1755
      TabIndex        =   13
      Top             =   4590
      Width           =   1545
   End
   Begin VB.CommandButton cmdSave 
      Caption         =   "Save Pattern"
      Height          =   510
      Left            =   1755
      TabIndex        =   15
      Top             =   5220
      Width           =   1545
   End
   Begin VB.CommandButton cmdRight 
      Caption         =   "Shift &Right"
      Height          =   510
      Left            =   1755
      TabIndex        =   11
      Top             =   3960
      Width           =   1545
   End
   Begin VB.CommandButton cmdReset 
      Caption         =   "Rese&t"
      Height          =   510
      Left            =   90
      TabIndex        =   12
      Top             =   4590
      Width           =   1545
   End
   Begin VB.CommandButton cmdLoad 
      Caption         =   "Load Pattern"
      Height          =   510
      Left            =   90
      TabIndex        =   14
      Top             =   5220
      Width           =   1545
   End
   Begin VB.TextBox txtDuration 
      BackColor       =   &H80000013&
      Enabled         =   0   'False
      Height          =   360
      Left            =   5040
      TabIndex        =   20
      Top             =   4275
      Width           =   2040
   End
   Begin VB.CommandButton cmdOpen 
      Caption         =   "&Open"
      Height          =   510
      Left            =   5535
      TabIndex        =   2
      Top             =   90
      Width           =   1545
   End
   Begin VB.TextBox txtFile 
      BackColor       =   &H80000013&
      Enabled         =   0   'False
      Height          =   360
      Left            =   1665
      Locked          =   -1  'True
      TabIndex        =   1
      TabStop         =   0   'False
      Top             =   180
      Width           =   3750
   End
   Begin VB.CheckBox chkPlayer 
      Caption         =   "Player 5"
      Height          =   465
      Index           =   5
      Left            =   270
      TabIndex        =   3
      Top             =   855
      Value           =   1  'Checked
      Width           =   1365
   End
   Begin VB.CheckBox chkPlayer 
      Caption         =   "Player 4"
      Height          =   465
      Index           =   4
      Left            =   270
      TabIndex        =   4
      Top             =   1305
      Value           =   1  'Checked
      Width           =   1365
   End
   Begin VB.CheckBox chkPlayer 
      Caption         =   "Player 3"
      Height          =   465
      Index           =   3
      Left            =   270
      TabIndex        =   5
      Top             =   1755
      Value           =   1  'Checked
      Width           =   1365
   End
   Begin VB.CheckBox chkPlayer 
      Caption         =   "Player 2"
      Height          =   465
      Index           =   2
      Left            =   270
      TabIndex        =   6
      Top             =   2205
      Value           =   1  'Checked
      Width           =   1365
   End
   Begin VB.CheckBox chkPlayer 
      Caption         =   "Player 1"
      Height          =   465
      Index           =   1
      Left            =   270
      TabIndex        =   7
      Top             =   2655
      Value           =   1  'Checked
      Width           =   1365
   End
   Begin VB.CommandButton cmdExit 
      Cancel          =   -1  'True
      Caption         =   "E&xit"
      Height          =   510
      Left            =   5535
      TabIndex        =   24
      Top             =   5220
      Width           =   1545
   End
   Begin VB.CommandButton cmdInsert 
      Caption         =   "&Insert"
      Enabled         =   0   'False
      Height          =   510
      Left            =   3870
      TabIndex        =   23
      Top             =   5220
      Width           =   1545
   End
   Begin VB.CommandButton cmdLeft 
      Caption         =   "Shift &Left"
      Height          =   510
      Left            =   90
      TabIndex        =   10
      Top             =   3960
      Width           =   1545
   End
   Begin VB.TextBox txtEnd 
      BackColor       =   &H80000013&
      Enabled         =   0   'False
      Height          =   360
      Left            =   5040
      Locked          =   -1  'True
      TabIndex        =   22
      TabStop         =   0   'False
      Top             =   4725
      Width           =   2040
   End
   Begin VB.TextBox txtStart 
      BackColor       =   &H80000013&
      Enabled         =   0   'False
      Height          =   360
      Left            =   5040
      TabIndex        =   18
      Top             =   3825
      Width           =   2040
   End
   Begin VB.Label lblFrame 
      Caption         =   "Movie Length:"
      Height          =   285
      Index           =   3
      Left            =   3600
      TabIndex        =   16
      Top             =   3375
      Width           =   3390
   End
   Begin VB.Label lblFrame 
      Caption         =   "Duration"
      Height          =   285
      Index           =   2
      Left            =   4095
      TabIndex        =   19
      Top             =   4320
      Width           =   870
   End
   Begin VB.Label lblFile 
      Caption         =   "Movie File"
      Height          =   285
      Left            =   495
      TabIndex        =   0
      Top             =   225
      Width           =   1050
   End
   Begin VB.Label lblFrame 
      Caption         =   "Ending Frame"
      Height          =   285
      Index           =   1
      Left            =   3555
      TabIndex        =   21
      Top             =   4770
      Width           =   1410
   End
   Begin VB.Label lblFrame 
      Caption         =   "Starting Frame"
      Height          =   285
      Index           =   0
      Left            =   3465
      TabIndex        =   17
      Top             =   3870
      Width           =   1500
   End
   Begin VB.Line Line2 
      BorderColor     =   &H8000000E&
      Index           =   5
      X1              =   1665
      X2              =   7065
      Y1              =   3105
      Y2              =   3105
   End
   Begin VB.Line Line2 
      BorderColor     =   &H8000000E&
      Index           =   4
      X1              =   1665
      X2              =   7065
      Y1              =   2655
      Y2              =   2655
   End
   Begin VB.Line Line2 
      BorderColor     =   &H8000000E&
      Index           =   3
      X1              =   1665
      X2              =   7065
      Y1              =   2205
      Y2              =   2205
   End
   Begin VB.Line Line2 
      BorderColor     =   &H8000000E&
      Index           =   2
      X1              =   1665
      X2              =   7065
      Y1              =   1755
      Y2              =   1755
   End
   Begin VB.Line Line2 
      BorderColor     =   &H8000000E&
      Index           =   1
      X1              =   1665
      X2              =   7065
      Y1              =   1305
      Y2              =   1305
   End
   Begin VB.Line Line2 
      BorderColor     =   &H8000000E&
      Index           =   0
      X1              =   1665
      X2              =   7065
      Y1              =   855
      Y2              =   855
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   15
      X1              =   7065
      X2              =   7065
      Y1              =   855
      Y2              =   3105
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   14
      X1              =   1665
      X2              =   1665
      Y1              =   855
      Y2              =   3105
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   13
      X1              =   6705
      X2              =   6705
      Y1              =   855
      Y2              =   3105
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   12
      X1              =   6345
      X2              =   6345
      Y1              =   855
      Y2              =   3105
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   11
      X1              =   5985
      X2              =   5985
      Y1              =   855
      Y2              =   3105
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   10
      X1              =   5625
      X2              =   5625
      Y1              =   855
      Y2              =   3105
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   9
      X1              =   5265
      X2              =   5265
      Y1              =   855
      Y2              =   3105
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   8
      X1              =   4905
      X2              =   4905
      Y1              =   855
      Y2              =   3105
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   7
      X1              =   4545
      X2              =   4545
      Y1              =   855
      Y2              =   3105
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   6
      X1              =   4185
      X2              =   4185
      Y1              =   855
      Y2              =   3105
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   5
      X1              =   3825
      X2              =   3825
      Y1              =   855
      Y2              =   3105
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   4
      X1              =   3465
      X2              =   3465
      Y1              =   855
      Y2              =   3105
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   3
      X1              =   3105
      X2              =   3105
      Y1              =   855
      Y2              =   3105
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   2
      X1              =   2745
      X2              =   2745
      Y1              =   855
      Y2              =   3105
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   1
      X1              =   2385
      X2              =   2385
      Y1              =   855
      Y2              =   3105
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   0
      X1              =   2025
      X2              =   2025
      Y1              =   855
      Y2              =   3105
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "s"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   75
      Left            =   6705
      TabIndex        =   99
      Top             =   855
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "s"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   60
      Left            =   6705
      TabIndex        =   98
      Top             =   1305
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "s"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   45
      Left            =   6705
      TabIndex        =   97
      Top             =   1755
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "s"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   30
      Left            =   6705
      TabIndex        =   96
      Top             =   2205
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "s"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   15
      Left            =   6705
      TabIndex        =   95
      Top             =   2655
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "S"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   74
      Left            =   6345
      TabIndex        =   94
      Top             =   855
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "S"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   59
      Left            =   6345
      TabIndex        =   93
      Top             =   1305
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "S"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   44
      Left            =   6345
      TabIndex        =   92
      Top             =   1755
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "S"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   29
      Left            =   6345
      TabIndex        =   91
      Top             =   2205
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "S"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   14
      Left            =   6345
      TabIndex        =   90
      Top             =   2655
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "2"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   73
      Left            =   5985
      TabIndex        =   89
      Top             =   855
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "2"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   58
      Left            =   5985
      TabIndex        =   88
      Top             =   1305
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "2"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   43
      Left            =   5985
      TabIndex        =   87
      Top             =   1755
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "2"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   28
      Left            =   5985
      TabIndex        =   86
      Top             =   2205
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "2"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   13
      Left            =   5985
      TabIndex        =   85
      Top             =   2655
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "R"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   72
      Left            =   5625
      TabIndex        =   84
      Top             =   855
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "R"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   57
      Left            =   5625
      TabIndex        =   83
      Top             =   1305
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "R"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   42
      Left            =   5625
      TabIndex        =   82
      Top             =   1755
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "R"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   27
      Left            =   5625
      TabIndex        =   81
      Top             =   2205
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "R"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   12
      Left            =   5625
      TabIndex        =   80
      Top             =   2655
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "L"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   71
      Left            =   5265
      TabIndex        =   79
      Top             =   855
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "L"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   56
      Left            =   5265
      TabIndex        =   78
      Top             =   1305
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "L"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   41
      Left            =   5265
      TabIndex        =   77
      Top             =   1755
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "L"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   26
      Left            =   5265
      TabIndex        =   76
      Top             =   2205
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "L"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   11
      Left            =   5265
      TabIndex        =   75
      Top             =   2655
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "1"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   70
      Left            =   4905
      TabIndex        =   74
      Top             =   855
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "1"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   55
      Left            =   4905
      TabIndex        =   73
      Top             =   1305
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "1"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   40
      Left            =   4905
      TabIndex        =   72
      Top             =   1755
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "1"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   25
      Left            =   4905
      TabIndex        =   71
      Top             =   2205
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "1"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   10
      Left            =   4905
      TabIndex        =   70
      Top             =   2655
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "X"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   69
      Left            =   4545
      TabIndex        =   69
      Top             =   855
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "X"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   54
      Left            =   4545
      TabIndex        =   68
      Top             =   1305
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "X"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   39
      Left            =   4545
      TabIndex        =   67
      Top             =   1755
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "X"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   24
      Left            =   4545
      TabIndex        =   66
      Top             =   2205
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "X"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   9
      Left            =   4545
      TabIndex        =   65
      Top             =   2655
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "Y"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   68
      Left            =   4185
      TabIndex        =   64
      Top             =   855
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "Y"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   53
      Left            =   4185
      TabIndex        =   63
      Top             =   1305
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "Y"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   38
      Left            =   4185
      TabIndex        =   62
      Top             =   1755
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "Y"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   23
      Left            =   4185
      TabIndex        =   61
      Top             =   2205
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "Y"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   8
      Left            =   4185
      TabIndex        =   60
      Top             =   2655
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "B"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   67
      Left            =   3825
      TabIndex        =   59
      Top             =   855
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "B"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   52
      Left            =   3825
      TabIndex        =   58
      Top             =   1305
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "B"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   37
      Left            =   3825
      TabIndex        =   57
      Top             =   1755
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "B"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   22
      Left            =   3825
      TabIndex        =   56
      Top             =   2205
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "B"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   7
      Left            =   3825
      TabIndex        =   55
      Top             =   2655
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "A"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   66
      Left            =   3465
      TabIndex        =   54
      Top             =   855
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "A"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   51
      Left            =   3465
      TabIndex        =   53
      Top             =   1305
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "A"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   36
      Left            =   3465
      TabIndex        =   52
      Top             =   1755
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "A"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   21
      Left            =   3465
      TabIndex        =   51
      Top             =   2205
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "A"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   6
      Left            =   3465
      TabIndex        =   50
      Top             =   2655
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "0"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   65
      Left            =   3105
      TabIndex        =   49
      Top             =   855
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "0"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   50
      Left            =   3105
      TabIndex        =   48
      Top             =   1305
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "0"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   35
      Left            =   3105
      TabIndex        =   47
      Top             =   1755
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "0"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   20
      Left            =   3105
      TabIndex        =   46
      Top             =   2205
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "0"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   5
      Left            =   3105
      TabIndex        =   45
      Top             =   2655
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "v"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   64
      Left            =   2745
      TabIndex        =   44
      Top             =   855
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "v"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   49
      Left            =   2745
      TabIndex        =   43
      Top             =   1305
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "v"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   34
      Left            =   2745
      TabIndex        =   42
      Top             =   1755
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "v"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   19
      Left            =   2745
      TabIndex        =   41
      Top             =   2205
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "v"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   4
      Left            =   2745
      TabIndex        =   40
      Top             =   2655
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   ">"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   63
      Left            =   2385
      TabIndex        =   39
      Top             =   855
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   ">"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   48
      Left            =   2385
      TabIndex        =   38
      Top             =   1305
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   ">"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   33
      Left            =   2385
      TabIndex        =   37
      Top             =   1755
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   ">"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   18
      Left            =   2385
      TabIndex        =   36
      Top             =   2205
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   ">"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   3
      Left            =   2385
      TabIndex        =   35
      Top             =   2655
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "^"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   62
      Left            =   2025
      TabIndex        =   34
      Top             =   855
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "^"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   47
      Left            =   2025
      TabIndex        =   33
      Top             =   1305
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "^"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   32
      Left            =   2025
      TabIndex        =   32
      Top             =   1755
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "^"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   17
      Left            =   2025
      TabIndex        =   31
      Top             =   2205
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "^"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   2
      Left            =   2025
      TabIndex        =   30
      Top             =   2655
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "<"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   61
      Left            =   1665
      TabIndex        =   29
      Top             =   855
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "<"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   46
      Left            =   1665
      TabIndex        =   28
      Top             =   1305
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "<"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   31
      Left            =   1665
      TabIndex        =   27
      Top             =   1755
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "<"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   16
      Left            =   1665
      TabIndex        =   26
      Top             =   2205
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "<"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   12
         Charset         =   0
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000000&
      Height          =   465
      Index           =   1
      Left            =   1665
      TabIndex        =   25
      Top             =   2655
      Width           =   375
   End
   Begin VB.Menu mnuMovie 
      Caption         =   "Movie"
      Begin VB.Menu mnuUDLR 
         Caption         =   "Toggle U+D/L+R"
         Enabled         =   0   'False
      End
      Begin VB.Menu mnuControllers 
         Caption         =   "Controllers"
         Enabled         =   0   'False
      End
   End
   Begin VB.Menu mnuAbout 
      Caption         =   "About"
   End
End
Attribute VB_Name = "frmInputEdit"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
' SNES9x Input Editor
' Author: JXQ
' Started 2-13-2007
' Finished 3-6-2007

' To improve:

'Converting text to a pattern

'Currently this is done, but in a pretty sloppy way.

'My idea was to change the load pattern function to
'first call the reset function, and then only toggle
'the lights that are on.  This way, only the right four
'columns could be toggled, moved to the left, and the
'next letter could be loaded without affecting the ones
'that have already been loaded.

'Unfortunately, these routines also included the file
'opening code, and so I basically copy-pasted the code
'to another routine to accomplish this goal.  It works,
'but could be written more cleanly.


Option Explicit

'InputLight represents one of the fifteen buttons
'for one of the five controllers.  1-15 is controller 1,
'16-30 controller 2, etc.

Dim InputLight(1 To 75) As Boolean

'NumControllers can be different than the number of
'the highest controller, for example, if controller 1
'and 3 are used, but 2 is not.

Dim NumControllers As Integer

'This is the offset of the controller data in the
'movie file.

Dim ControllerOffset As Long

'Frame count from the movie file.  Used to ensure
'that writing past EOF is not a problem.

Dim FrameCount As Long

'Flag or a movie having Up+Down/Left+Right enabled

Dim IsUDLR As Boolean

'Size the form correctly and center it on the screen

Private Sub Form_Load()
  Me.Height = 6480
  Me.Width = 7260
  Me.Left = (Screen.Width - Me.Width) / 2
  Me.Top = (Screen.Height - Me.Height) / 2
End Sub

'When disabling a certain player, clear all the
'input in that row that is turned on.

Private Sub chkPlayer_Click(Index As Integer)
  
  Dim Index2 As Integer
  
  If chkPlayer(Index).Value = 0 Then
    For Index2 = 15 * (Index - 1) + 1 To 15 * Index
      InputLight(Index2) = False
      lblInputLabel(Index2).ForeColor = &H0&
    Next Index2
  End If

End Sub

'Toggles whether the left and right buttons will Shift or
'Roll (Rolling will wrap one side's input to the other)

Private Sub chkRoll_Click()
  
  If chkRoll.Value = 1 Then
    cmdLeft.Caption = "Roll &Left"
    cmdRight.Caption = "Roll &Right"
  Else
    cmdLeft.Caption = "Shift &Left"
    cmdRight.Caption = "Shift &Right"
  End If

End Sub

'Reset all input to "off"

Private Sub cmdReset_Click()
  
  Dim Index As Integer
  
  For Index = 1 To 75
    InputLight(Index) = False
    lblInputLabel(Index).ForeColor = &H0&
  Next Index

End Sub

'Invert input on all enabled controller rows

Private Sub cmdInvert_Click()
  
  Dim Index As Integer
  Dim Index2 As Integer
  Dim TempIndex As Integer
  
  For Index = 1 To 5
    
    'If the controller row is enabled,
    
    If chkPlayer(Index).Value = 1 Then
      
      For Index2 = 1 To 15
        TempIndex = (Index - 1) * 15 + Index2
        
        'Invert each input on that row
        
        InputLight(TempIndex) = Not InputLight(TempIndex)
        
        If InputLight(TempIndex) Then
          lblInputLabel(TempIndex).ForeColor = &HFFFFFF
        Else
          lblInputLabel(TempIndex).ForeColor = &H0&
        End If
      
      Next Index2
    
    End If
  
  Next Index

End Sub

'Shift/Roll all input one space to the left

Private Sub cmdLeft_Click()
  
  Dim Index As Integer
  Dim Temp As Integer
  Dim TempColor As Long
  
  For Index = 1 To 75
    
    'If the "Roll Input?" checkbox is selected, save the left side
    'in a temporary variable
    
    If Index Mod 15 = 1 Then
      Temp = InputLight(Index)
      TempColor = lblInputLabel(Index).ForeColor
    End If
    
    If Index Mod 15 = 0 Then
      
      'If the "Roll Input?" checkbox is selected, assign the left side
      'temporary variables to the right side, otherwise make them "off"
   
      If chkRoll.Value = 1 Then
        InputLight(Index) = Temp
        lblInputLabel(Index).ForeColor = TempColor
      Else
        InputLight(Index) = False
        lblInputLabel(Index).ForeColor = &H0&
      End If
    
    Else
      
      'Grab the value from the adjacent position
      
      InputLight(Index) = InputLight(Index + 1)
      lblInputLabel(Index).ForeColor = lblInputLabel(Index + 1).ForeColor
    
    End If
  
  Next Index

End Sub

'Shift/Roll all input one space to the right

Private Sub cmdRight_Click()
  Dim Index As Integer
  Dim Temp As Integer
  Dim TempColor As Long
  
  For Index = 75 To 1 Step -1
    
    'If the "Roll Input?" checkbox is selected, save the right side
    'in a temporary variable
    
    If Index Mod 15 = 0 Then
      Temp = InputLight(Index)
      TempColor = lblInputLabel(Index).ForeColor
    End If
    
    If Index Mod 15 = 1 Then
      
      'If the "Roll Input?" checkbox is selected, assign the right side
      'temporary variables to the left side, otherwise make them "off"
      
      If chkRoll.Value = 1 Then
        InputLight(Index) = Temp
        lblInputLabel(Index).ForeColor = TempColor
      Else
        InputLight(Index) = False
        lblInputLabel(Index).ForeColor = &H0&
      End If
    
    Else
      
      'Grab the value from the adjacent position
      
      InputLight(Index) = InputLight(Index - 1)
      lblInputLabel(Index).ForeColor = lblInputLabel(Index - 1).ForeColor
    
    End If
  
  Next Index

End Sub

'This is the routine that writes the appropriate data to the movie file.

Private Sub cmdInsert_Click()
  
  Dim FrameStart As Long
  Dim FrameEnd As Long
  Dim CurrentController As Integer
  Dim CurrentRealController As Integer
  Dim CurrentFrame As Long
  Dim ControllerBinaryData(1 To 5) As Integer
  Dim Index As Integer
  Dim TempOffset As Integer
  Dim NumActiveControllers As Integer
  
  'Due to previous error checking, checking validity of
  'user inputs can be matched against the End Frame text field.
    
  If txtEnd.Text = "" Then
    
    Call MsgBox("Please enter valid numbers for Starting frame and/or Duration", vbOKOnly, "Invalid entry")
    txtStart.SetFocus
    Exit Sub
  
  End If
    
  'Check to see if the frame range is valid for the movie.
  'If the end value is too large, warn the user, correct
  'the value to the last possible frame.
  
  FrameStart = Val(txtStart.Text)
  FrameEnd = Val(txtEnd.Text)
    
  If FrameEnd > FrameCount Then
    
    'Display error when the given frames to place the data extend past
    'the frame count given in the movie file.
    
    Call MsgBox("This movie is only " & Str$(FrameCount) & " frames - input cannot be inserted beyond that frame", _
      vbOKOnly, "Invalid frame range")
    
    'Adjust the duration and/or starting frame to be valid values
    
    If FrameStart > FrameCount Then
      txtStart.Text = Trim$(Str$(FrameCount))
      txtDuration.Text = Trim$(Str$(1))
    Else
      txtDuration.Text = Trim$(Str$(FrameCount - FrameStart + 1))
      txtDuration.SetFocus
    End If
    
    Exit Sub
  End If
  
  'Alright, everything's good, so now let's calculate the
  'binary values needed that correspond to the desired input.
  
  For Index = 1 To 5
    
    'Only calculate values for the controllers that are checked.
    
    If chkPlayer(Index).Value = 1 Then
      
      TempOffset = (Index - 1) * 15 + 1
      
      ' Bit values for each button for VB (reverse bit order from SMV.html)
      ' < 512
      ' ^ 2048
      ' > 256
      ' v 1024
      ' 0 2
      ' a 128
      ' b -32768
      ' y 16384
      ' x 64
      ' 1 4
      ' L 32
      ' R 16
      ' 2 8
      ' S 4096
      ' s 8192
    
      If InputLight(TempOffset) Then ControllerBinaryData(Index) = ControllerBinaryData(Index) + 512
      If InputLight(TempOffset + 1) Then ControllerBinaryData(Index) = ControllerBinaryData(Index) + 2048
      If InputLight(TempOffset + 2) Then ControllerBinaryData(Index) = ControllerBinaryData(Index) + 256
      If InputLight(TempOffset + 3) Then ControllerBinaryData(Index) = ControllerBinaryData(Index) + 1024
      If InputLight(TempOffset + 4) Then ControllerBinaryData(Index) = ControllerBinaryData(Index) + 2
      If InputLight(TempOffset + 5) Then ControllerBinaryData(Index) = ControllerBinaryData(Index) + 128
      If InputLight(TempOffset + 7) Then ControllerBinaryData(Index) = ControllerBinaryData(Index) + 16384
      If InputLight(TempOffset + 8) Then ControllerBinaryData(Index) = ControllerBinaryData(Index) + 64
      If InputLight(TempOffset + 9) Then ControllerBinaryData(Index) = ControllerBinaryData(Index) + 4
      If InputLight(TempOffset + 10) Then ControllerBinaryData(Index) = ControllerBinaryData(Index) + 32
      If InputLight(TempOffset + 11) Then ControllerBinaryData(Index) = ControllerBinaryData(Index) + 16
      If InputLight(TempOffset + 12) Then ControllerBinaryData(Index) = ControllerBinaryData(Index) + 8
      If InputLight(TempOffset + 13) Then ControllerBinaryData(Index) = ControllerBinaryData(Index) + 4096
      If InputLight(TempOffset + 14) Then ControllerBinaryData(Index) = ControllerBinaryData(Index) + 8192
      
      'This value is checked last so that possible overflow is avoided
      
      If InputLight(TempOffset + 6) Then ControllerBinaryData(Index) = ControllerBinaryData(Index) - 32768
    
    End If
  
  Next Index
  
  'CurrentRealController is the number of the controller in the file
  '(1st piece, 2nd, etc.)  This can be different than CurrentController
  'if some in-between controllers are disabled.
  
  CurrentRealController = 0
  
  'CurrentController is the number of the controller in the game
  '(Player 1, 2, etc.)
   
  For CurrentController = 1 To 5
    
    CurrentRealController = CurrentRealController + 1
    
    'If a controller is not active, increment CurrentController and
    'check the next player's controller.  By not incrementing
    'CurrentRealController, we will still have the correct offset
    'when writing the data to the file.
            
    Do While chkPlayer(CurrentController) <> 1
      
      CurrentController = CurrentController + 1
      
      'If the controller is enabled but not being used for this
      'insert, then be sure to increment the file offset
      
      If CurrentController = 6 Then Exit For
      
      If chkPlayer(CurrentController).Enabled Then
        CurrentRealController = CurrentRealController + 1
      End If
      
    Loop
    
    'A piece of data goes in the following spot:
    'Start with the ControllerOffset of the entire movie.
    'Advance from there to the first frame desired.  Each
    'frame is 2 * (NumControllers) bytes.  The first
    'controller's input goes in this position, and
    'each subsequent controller will go 2 bytes later.
        
    On Error GoTo FileWriteError
        
      For CurrentFrame = FrameStart To FrameEnd
        Put #1, ControllerOffset + 2 * ((CurrentFrame * NumControllers) + (CurrentRealController - 1)), ControllerBinaryData(CurrentController)
      Next CurrentFrame
   
    On Error GoTo 0
  
  Next CurrentController
  
  'The user will most likely want to not place data on
  'this frame again, so the starting frame is automatically
  'incremented to the frame following the last frame of
  'this insert.
  
  txtStart.Text = Trim$(Str$(CurrentFrame))
  
  Exit Sub
  
FileWriteError:
  Call MsgBox("Error " & Err.Number & " when writing to the file", vbOKOnly, "File I/O Error")
  
End Sub

'Pop up open dialog box and update filename
'if a valid one is given.

Private Sub cmdOpen_Click()
  
  'File signature for SMV files
  
  Dim Signature As Long
  
  'File name retrieved from open dialog for display
  
  Dim NewFileName As String
  
  'Keep track of the button's function by its caption
  
  If cmdOpen.Caption = "&Open" Then
    
    'Pop up Open dialog
    
    Dim frmNew As frmFileOpen
    Set frmNew = New frmFileOpen
    Call Load(frmNew)
    
    'Center open form on top of current form
    
    frmNew.Left = Me.Left + (Me.Width - frmNew.Width) / 2
    frmNew.Top = Me.Top + (Me.Height - frmNew.Height) / 2
      
    'Set FileNumber to 1 (opening SMV file)
    
    frmNew.FileNumber = 1
  
    'Title and show the form
  
    frmNew.Caption = "Open SMV file"
      
    Call frmNew.Show(vbModal)
    
    'A non-null text file indicates that
    'a file was loaded successfully
    
    If Len(frmNew.strFileName) > 0 Then
      
      'Check for SMV signature of 53 4D 56 1A (hex, little-endian)
      '1A564D53 (hex) = 441863507 (decimal)
      
      On Error GoTo ErrorInFile
        Get #1, 1, Signature
      On Error GoTo 0
      
      If Signature = 441863507 Then
        
        'Valid SMV file, so enable appropriate functions and
        'update the filename textbox
        
        txtFile.Text = frmNew.strFileName
        Call FileSetup(True)
      
      Else
        
        Call MsgBox("File is not a valid SMV file", vbOKOnly, "Invalid File")
        Close #1
      
      End If
    
    End If
  
    'Unload open dialog
  
    Call Unload(frmNew)
    Set frmNew = Nothing
  
  Else
    
    'If the caption is "cl&ose", close the current file.  Disable
    'appropriate options and remove filename from text box.
    
    On Error GoTo 0
    
    Close #1
    txtFile.Text = ""
    Call FileSetup(False)
  
  End If

  Exit Sub
  
ErrorInFile:
  Call MsgBox("Error " & Err.Number & " when trying to read file", vbOKOnly, "File I/O Error")
  Close #1

End Sub

'Load a pattern into the program from a file

Private Sub cmdLoad_Click()
  
  'File signature for PAT files
  
  Dim Signature As Long
  Dim Index As Integer
  
  'Pattern Data to be loaded from the file into the grid
  
  Dim PatternData As Byte
  
  'Set up the Open dialog box if filename is not given
  
  Dim frmNew As frmFileOpen
  Set frmNew = New frmFileOpen
  Call Load(frmNew)
    
  'Center open form on top of current form
      
  frmNew.Left = Me.Left + (Me.Width - frmNew.Width) / 2
  frmNew.Top = Me.Top + (Me.Height - frmNew.Height) / 2
    
  'Set FileNumber to 2 (opening pattern file)
    
  frmNew.FileNumber = 2
      
  'Title and show the form
  
  frmNew.Caption = "Open PAT file"
  frmNew.filDialog.Pattern = "*.pat"
  
  Call frmNew.Show(vbModal)
    
  'A non-null text file indicates that
  'a file was loaded successfully
    
  If Len(frmNew.strFileName) > 0 Then
      
    'Specs for PAT file:
    '001-004: 4 byte signature 4A 58 51 21
    '005    : 1 byte (5 bits) representing which
    '         controllers are enabled.
    '006-00D: 10 bytes (75 bits) of data.  The first
    '         75 bits refer to the spaces on the grid
    '         being lighted or not.  The last five
    '         bits are the toggles for each of the
    '         five controllers.
    
    'Check for PAT signature of 4A 58 51 21 (hex, little-endian)
    '2151584A (hex) = 558979146 (decimal)
      
      On Error GoTo ErrorInFileTwo
        Get #2, 1, Signature
      On Error GoTo 0
      
      If Signature = 558979146 Then
        
        'Valid PAT file, so load controller toggles into grid
                
        Get #2, 5, PatternData
        
        For Index = 1 To 5
        
          'If the controller capability is not disabled (this is
          'different than enabled but unchecked)
        
          If chkPlayer(Index).Enabled Then
              
            'Get the controller toggle from the corresponding bit in the file
              
            chkPlayer(Index).Value = (PatternData \ (2 ^ (Index - 1))) Mod 2
        
          End If
        
        Next Index
        
        'Call a function to toggle the appropriate button presses.
        'In this case, send 15, as a complete load will load all columns.
        
        Call ToggleButtons(15)
          
      Else
        
        Call MsgBox("File is not a valid PAT file", vbOKOnly, "Invalid File")
        
      End If
    
      Close #2
    
    End If
  
  'Unload open dialog
  
  Call Unload(frmNew)
  Set frmNew = Nothing
   
  Exit Sub
  
ErrorInFileTwo:
  Call MsgBox("Error reading from file", vbOKOnly, "File I/O Error")
  
End Sub

'Save a pattern from the program into a file

Private Sub cmdSave_Click()

  Dim Index As Integer
  
  'Pattern Data to be loaded from the file into the grid
  
  Dim PatternData As Byte
  
  'Set up the Open dialog box
  
  Dim frmNew As frmFileOpen
  Set frmNew = New frmFileOpen
  Call Load(frmNew)
    
  'Center open form on top of current form
  
  frmNew.Left = Me.Left + (Me.Width - frmNew.Width) / 2
  frmNew.Top = Me.Top + (Me.Height - frmNew.Height) / 2
    
  'Set FileNumber to 2 (opening pattern file)
    
  frmNew.FileNumber = 3
      
  'Title and show the form.  Change "open" button caption to "save"
  
  frmNew.Caption = "Save PAT file"
  frmNew.cmdOpen.Caption = "&Save"
  frmNew.filDialog.Pattern = "*.pat"
      
  Call frmNew.Show(vbModal)
    
  'A non-null text file indicates that
  'a file was loaded successfully
    
  If Len(frmNew.strFileName) > 0 Then
      
    'Specs for PAT file:
    '001-004: 4 byte signature 4A 58 51 21
    '005    : 1 byte (5 bits) representing which
    '         controllers are enabled.
    '006-00D: 10 bytes (75 bits) of data.  The first
    '         75 bits refer to the spaces on the grid
    '         being lighted or not.  The last five
    '         bits are the toggles for each of the
    '         five controllers.
    
    'Write PAT signature of 4A 58 51 21 (hex, little-endian)
    '2151584A (hex) = 558979146 (decimal)
      
    On Error GoTo ErrorInFileThree
      Put #3, 1, 558979146
    On Error GoTo 0
      
    'Save controller toggles into file
    
    For Index = 1 To 5
    
      'If the controller capability is not disabled (this is
      'different than enabled but unchecked)
       
      If chkPlayer(Index).Enabled Then
              
        'Save the controller toggle in the corresponding bit for the file
              
        If chkPlayer(Index).Value = 1 Then
          
          PatternData = PatternData + 2 ^ (Index - 1)
        
        End If
      
      End If
    
    Next Index
        
    Put #3, 5, PatternData
    
    PatternData = 0
    
    'Now save each button press into the file
    
    For Index = 1 To 75
          
      'Put controller data in appropriate bit position
        
      If InputLight(Index) Then PatternData = PatternData + 2 ^ ((Index - 1) Mod 8)
      
      'Every 8 bits, write the byte to the file
          
      If Index Mod 8 = 0 Then
                
        On Error GoTo ErrorInFileThree
          Put #3, 6 + ((Index - 1) \ 8), PatternData
        On Error GoTo 0
          
        PatternData = 0
        
      End If
        
    Next Index
          
  End If
    
  Close #3
    
  'Unload save dialog
  
  Call Unload(frmNew)
  Set frmNew = Nothing
   
  Exit Sub
  
ErrorInFileThree:
  Call MsgBox("Error writing to file", vbOKOnly, "File I/O Error")
  
End Sub

'Opens up a seperate window to type in a small phrase
'that will be "translated" onto the game input grid

'Currently this code is sloppy, though it works.
'I copied a lot of code from different routines because
'those routines I planned on using assumed to grab the
'information from a file, and this was a quick way to
'implement this without popping up the open dialog box.

Private Sub cmdText_Click()
  
  'Text to be gotten from input form
  
  Dim InputText As String
  
  'Index used for number of characters in InputText
  
  Dim Index As Integer
  Dim Index2 As Integer
  Dim Signature As Long
  
  'Current character, turned into pattern filename
  
  Dim CurChar As String
  
  'Pattern Info from file
  
  Dim PatternData As Byte
  
  
  'Pop up input text form
  
  Dim frmNew As frmTextInput
  Set frmNew = New frmTextInput
  Call Load(frmNew)
    
  'Center open form on top of current form
  
  frmNew.Left = Me.Left + (Me.Width - frmNew.Width) / 2
  frmNew.Top = Me.Top + (Me.Height - frmNew.Height) / 2

  'Only allow character strings up to 4 in length
  
  frmNew.txtInput.MaxLength = 4

  Call frmNew.Show(vbModal)

  'Get the text entered from the form
  
  InputText = UCase$(frmNew.sInput)

  'Unload the form
    
  Call Unload(frmNew)
  Set frmNew = Nothing

  'If the input is invalid, do not attempt to insert it
  'to the grid
  
  If Len(InputText) = 0 Then Exit Sub

  'Erase the current grid
  
  Call cmdReset_Click
  
  'Update each character in InputText, starting on the right
  
  For Index = Len(InputText) To 1 Step -1
  
    'Get the current character
  
    CurChar = Mid$(InputText, Index, 1)
  
    'If the current character is not A-Z or 0-9,
    'change the file name appropriately.
  
    Select Case Asc(CurChar)
      Case Is < 33, Is > 126
        CurChar = "Space"
      Case 33
        CurChar = "Exclamation Point"
      Case 34
        CurChar = "Quotes"
      Case 35
        CurChar = "Pound"
      Case 36
        CurChar = "Dollar"
      Case 37
        CurChar = "Percent"
      Case 38
        CurChar = "Ampersand"
      Case 39
        CurChar = "Apostrophe"
      Case 40
        CurChar = "Left Parenthesis"
      Case 41
        CurChar = "Right Parenthesis"
      Case 42
        CurChar = "Asterisk"
      Case 43
        CurChar = "Plus"
      Case 44
        CurChar = "Comma"
      Case 45
        CurChar = "Hyphen"
      Case 46
        CurChar = "Period"
      Case 47
        CurChar = "Slash"
      Case 58
        CurChar = "Colon"
      Case 59
        CurChar = "Semicolon"
      Case 60
        CurChar = "Less Than"
      Case 61
        CurChar = "Equals"
      Case 62
        CurChar = "Greater Than"
      Case 63
        CurChar = "Question Mark"
      Case 64
        CurChar = "At Sign"
      Case 91
        CurChar = "Left Bracket"
      Case 92
        CurChar = "Backslash"
      Case 93
        CurChar = "Right Bracket"
      Case 94
        CurChar = "Caret"
      Case 95
        CurChar = "Underscore"
      Case 96
        CurChar = "Left Quote"
      Case 123
        CurChar = "Left Brace"
      Case 124
        CurChar = "Pipe"
      Case 125
        CurChar = "Right Brace"
      Case 126
        CurChar = "Tilde"
      Case Else
        CurChar = UCase$(CurChar)
    End Select
    
    'Load the pattern file that corresponds to that character
    'This code is nearly the same as cmdLoad_Click
    
    On Error GoTo LetterOpenError
     
      Open CurChar & ".pat" For Binary Lock Read Write As #2
  
    On Error GoTo 0
    
    'Specs for PAT file:
    '001-004: 4 byte signature 4A 58 51 21
    '005    : 1 byte (5 bits) representing which
    '         controllers are enabled.
    '006-00D: 10 bytes (75 bits) of data.  The first
    '         75 bits refer to the spaces on the grid
    '         being lighted or not.  The last five
    '         bits are the toggles for each of the
    '         five controllers.
    
    'Check for PAT signature of 4A 58 51 21 (hex, little-endian)
    '2151584A (hex) = 558979146 (decimal)
      
    Get #2, 1, Signature
    
    If Signature = 558979146 Then
        
      'Valid PAT file, so load controller toggles into grid
                
      Get #2, 5, PatternData
        
      For Index2 = 1 To 5
        
        'If the controller capability is not disabled (this is
        'different than enabled but unchecked)
        
        If chkPlayer(Index2).Enabled Then
              
          'Get the controller toggle from the corresponding bit in the file
              
          chkPlayer(Index2).Value = (PatternData \ (2 ^ (Index2 - 1))) Mod 2
        
        End If
        
      Next Index2
        
      'Call a function to toggle the appropriate button presses.
      'In this case, send 3, so only the letter on the left is loaded.
        
      Call ToggleButtons(3)
          
    End If
  
LetterOpenError:
   
    Close #2
    
    'If this is not the last letter, move everything to the right
    'to prepare for the next letter
    
    If Index > 1 Then
    
      Call cmdRight_Click
      Call cmdRight_Click
      Call cmdRight_Click
      Call cmdRight_Click
    
    End If
     
  Next Index

End Sub

'Each box can be clicked on to toggle the input.
'If the row is on a controller than is not enabled,
'the click will be ignored.

Private Sub lblInputLabel_MouseDown(Index As Integer, Button As Integer, Shift As Integer, X As Single, Y As Single)
  
  'If the label clicked is on a disabled row, ignore it
  
  If chkPlayer(((Index - 1) \ 15) + 1).Value = 0 Then Exit Sub
  
  'Otherwise, toggle the boolean value and also make
  'the label forecolor visible/invisible
  
  InputLight(Index) = Not InputLight(Index)
  If InputLight(Index) Then
    lblInputLabel(Index).ForeColor = &HFFFFFF
  Else
    lblInputLabel(Index).ForeColor = &H0&
  End If

End Sub

'Brings up the "about" screen

Private Sub mnuAbout_Click()
  
  'Pop up "about" form
  
  Dim frmNew As frmAbout
  Set frmNew = New frmAbout
  Call Load(frmNew)
  
  'Place "about" screen in the middle of the active window
  'before showing it
  
  frmNew.Left = Me.Left + (Me.Width - frmNew.Width) / 2
  frmNew.Top = Me.Top + (Me.Height - frmNew.Height) / 2
  
  Call frmNew.Show(vbModal)
  
  'Unload the form when control is returned
  
  Call Unload(frmNew)
  Set frmNew = Nothing

End Sub

'Up+Down/Left+Right can be toggled when a movie is loaded
'by clicking on this menu option.

Private Sub mnuUDLR_Click()
  
  Dim SyncData As Byte
  Dim YNResult As Integer
    
  On Error GoTo MenuError
    
    'Get the movie synchronization data from the file
    
    Get #1, 24, SyncData
    
    'If the user is disabling Up+Down/Left+right, display a
    'warning message that this may cause unexpected results
    'when using this program.
    
    If IsUDLR Then
      
      YNResult = MsgBox("It is recommended to leave this feature enabled when adding input in this program." _
          & vbCrLf & "Are you sure you want to disable it?", vbYesNo, "Disable Left+Right/Up+Down?")
      
      'Write new data to the file.  The bit is position 2
      '(starting from 0) should be turned off, if the user
      'has selected "yes" on the warning dialog.
      
      If YNResult = vbYes Then
        
        'Removing the third bit cannot be done directly - it
        'converts to an integer and corrupts the next position
        
        SyncData = SyncData - 4
        
        Put #1, 24, SyncData
      Else
        Exit Sub
      End If
    
    Else
      
      'Write new data to the file.  The bit is position 2
      '(starting from 0) should be turned on.
      
      'This cannot be done directly - it converts to an
      'integer and corrupts the next position.
      
      SyncData = SyncData + 4
      
      Put #1, 24, SyncData
    
    End If
  
  On Error GoTo 0
    
  'Toggle the variable and menu check as well
  
  IsUDLR = Not IsUDLR
  mnuUDLR.Checked = IsUDLR
    
  Exit Sub

MenuError:
  Call MsgBox("Error writing data to file", vbOKOnly, "File I/O Error")

End Sub

'When clicking on the 'Controllers' option in the
'movie menu, pop up a form that allows for toggling
'which controllers are active in the movie file.
'These changes will be saved to a new file.

Private Sub mnuControllers_Click()

  Dim Index As Integer
  Dim YNResult As Integer
  
  Dim frmNew As frmControllers
  Set frmNew = New frmControllers
  Call Load(frmNew)
    
  'Center open form on top of current form
  
  frmNew.Left = Me.Left + (Me.Width - frmNew.Width) / 2
  frmNew.Top = Me.Top + (Me.Height - frmNew.Height) / 2
  
  'Send controller data to the form
  
  For Index = 1 To 5
  
    If chkPlayer(Index).Enabled Then
      
      frmNew.chkPlayer(Index).Value = 1
      frmNew.txtCurrent.Text = frmNew.txtCurrent.Text & Trim$(Str$(Index)) & ", "
    
    Else
      
      frmNew.chkPlayer(Index).Value = 0
    
    End If
  
  Next Index
    
  frmNew.txtCurrent.Text = Mid$(frmNew.txtCurrent.Text, 1, Len(frmNew.txtCurrent.Text) - 2)
  frmNew.txtFile.Text = txtFile.Text
  
  frmNew.ControllerOffset = ControllerOffset
  frmNew.FrameCount = FrameCount
    
  'Pop up the Controllers form
  
  Call frmNew.Show(vbModal)

  'If successful, prompt success and ask the user if they
  'want to open this new file now
  
  If frmNew.Success Then
  
    YNResult = MsgBox("New file " & frmNew.NewFileName & " created successfully!" & vbCrLf & _
      "Would you like to open this file now?", vbYesNo, "Open new file now?")
    
    If YNResult = vbYes Then
    
      On Error GoTo FileErrorLabel
      
        Close #1
        Call FileSetup(False)
        
        Open frmNew.NewFileName For Binary Lock Read Write As #1
        Call FileSetup(True)
        
        txtFile.Text = frmNew.NewFileName
              
      On Error GoTo 0
        
    End If
  
  End If
    
  'Unload the form
    
  Call Unload(frmNew)
  Set frmNew = Nothing
  
  Exit Sub
  
FileErrorLabel:
  
  Call MsgBox("There was an error opening the file.", vbOKOnly, "File I/O Error")
  
  Call Unload(frmNew)
  Set frmNew = Nothing


End Sub

' If the duration is being updated, real-time
' update the ending frame if starting frame is given.

Private Sub txtDuration_Change()
  
  If (VerifyNumeric(txtStart.Text) Or txtStart.Text = "0") And VerifyNumeric(txtDuration.Text) Then
    txtEnd.Text = Trim$(Str$(Val(txtStart.Text) + Val(txtDuration.Text) - 1))
  Else
    txtEnd.Text = ""
  End If

End Sub

Private Sub txtStart_Change()
  
  'If duration isn't given, change to 1 (default)
   
  If Not VerifyNumeric(txtDuration.Text) Then
    txtDuration.Text = "1"
  End If

  'If the start frame is being updated, real-time
  'update the ending frame if duration is given.
  'Clear the value if an input is invalid.
    
  If VerifyNumeric(txtStart.Text) Or txtStart.Text = "0" Then
    txtEnd.Text = Trim$(Str$(Val(txtStart.Text) + Val(txtDuration.Text) - 1))
  Else
    txtEnd.Text = ""
  End If
  
End Sub

'Enable/disable various parts of the form based
'on whether or not a file is open currently.
'Accepts boolean parameter which determines
'whether or not a file has been loaded successfully.

Private Sub FileSetup(Success As Boolean)
  Dim Index As Integer
  Dim Controllers As Byte
  Dim SyncData As Byte
  Dim YNResult As Integer
  
  If Success Then
    
    'Enable various relevant controls
    
    cmdInsert.Enabled = True
    
    With txtStart
      .Enabled = True
      .BackColor = &H80000005
    End With
    
    With txtDuration
      .Enabled = True
      .BackColor = &H80000005
    End With
    
    txtEnd.Enabled = True
    txtFile.Enabled = True
    
    mnuUDLR.Enabled = True
    mnuControllers.Enabled = True
       
    'Get relevant metadata from movie file
    
    On Error GoTo FileReadError
      Get #1, 17, FrameCount
      Get #1, 21, Controllers
      Get #1, 24, SyncData
    On Error GoTo 0
    
    'Display the frame count
    
    lblFrame(3).Caption = "Movie Length: " & Str$(FrameCount) & " frames"
    
    'Set the Up+Down/Left+Right flags based on movie data
    
    IsUDLR = ((SyncData \ 4) Mod 2 = 1)
    mnuUDLR.Checked = IsUDLR
    
    'If the movie does not have this flag set currently,
    'prompt a warning that it is recommended with the
    'option to enable it now.
    
    If Not IsUDLR Then
      YNResult = MsgBox("Allow Left+Right/Up+Down is not currently enabled for this movie." _
          & vbCrLf & "It is recommended to enable this feature when adding input in this program." _
          & vbCrLf & "Would you like to enable it now?", vbYesNo, "Allow Left+Right/Up+Down?")
            
      If YNResult = vbYes Then
        
        'If user wishes to enable Up+Down/Left+Right now,
        'turn on bit position 2 (starting from 0) and write
        'the sync data back to the file.
        
        'This cannot be done directly - it converts to an
        'integer and corrupts the next position.
        
        SyncData = SyncData + 4
                
        On Error GoTo FileReadError
          Put #1, 24, SyncData
        On Error GoTo 0
        
        'Update variables as well
        
        IsUDLR = True
        mnuUDLR.Checked = True
      
      End If
    
    End If
        
    'Determine how many active controllers are in this movie file.
    'This is not necessarily the same as the number of the highest
    'active controller.
    
    NumControllers = 0
    
    For Index = 1 To 5
      
      'If this controller is enabled,
      
      If (Controllers \ (2 ^ (Index - 1))) Mod 2 = 1 Then
        
        'Enable the checkbox (semi-redundant) and add
        'one to the number of active controllers
        
        chkPlayer(Index).Enabled = True
        NumControllers = NumControllers + 1
      
      Else
        
        'Otherwise, remove the check mark from that
        'controller's checkbox, and disable the use
        'of it completely (as the current movie does
        'not support this controller)
        
        chkPlayer(Index).Value = 0
        chkPlayer(Index).Enabled = False
        
        'Call the click function to remove any input
        'on the grid that is now on an inactive controller
        
        Call chkPlayer_Click(Index)
                
      End If
    
    Next Index
    
    'Get controller data offset
    
    On Error GoTo FileReadError
      Get #1, 29, ControllerOffset
    On Error GoTo 0
     
    ControllerOffset = ControllerOffset + 1
    
    'Switch button caption now that a file is open
       
    cmdOpen.Caption = "Cl&ose"
  
  Else
    
    'Disable various relevant controls
    
    cmdInsert.Enabled = False
    
    With txtStart
      .Text = ""
      .Enabled = False
      .BackColor = &H80000013
    End With
    
    With txtDuration
      .Text = ""
      .Enabled = False
      .BackColor = &H80000013
    End With
    
    With txtEnd
      .Text = ""
      .Enabled = False
    End With
    
    txtFile.Enabled = False
    
    With mnuUDLR
      .Checked = False
      .Enabled = False
    End With
    
    mnuControllers.Enabled = False
    
    'Remove label showing the frame count
    
    lblFrame(3).Caption = ""
        
    'Set Up+Down/Left+Right to false (this flag has
    'no meaning when a movie is not loaded)
    
    IsUDLR = False
    
    'With no movie file loaded, allow all controllers to be
    'accessed.  (A "play-around" mode of sorts)
    
    For Index = 1 To 5
      chkPlayer(Index).Enabled = True
      chkPlayer(Index).Value = 1
    Next Index
    
    'Change button caption to "&Open" for the next file
    
    cmdOpen.Caption = "&Open"
   
  End If

  Exit Sub

FileReadError:
  Call MsgBox("Error " & Err.Number & " when trying to read file", vbOKOnly, "File I/O Error")
  Close #1

End Sub

'This subroutine is used to actually toggle the appropriate
'buttons after loading a pattern file.  The parameter NumColumns
'can be adjusted to indicate how many of the columns (starting
'on the left) should be overwritten by the pattern.  This method
'was chosen to allow easier insertion of letters, one at a time.

Private Sub ToggleButtons(NumColumns As Integer)

  Dim Index As Integer
  Dim PatternData As Byte
  
  'Load each button press into the grid
  
  For Index = 1 To 75
          
    'Every 8 bits, grab the next byte
          
    If Index Mod 8 = 1 Then
                
      On Error GoTo ErrorInFileFour
        Get #2, 6 + (Index \ 8), PatternData
      On Error GoTo 0
          
    End If
          
    'If the corresponding controller is enabled,
            
    If chkPlayer(((Index - 1) \ 15) + 1).Value = 1 Then
                        
      'and the column has been chosen to write into,
                        
      If (Index - 1) Mod 15 <= NumColumns - 1 Then
      
        'Get the input from the corresponding bit in the file
              
        InputLight(Index) = (PatternData \ (2 ^ ((Index - 1) Mod 8)) Mod 2 = 1)
        
        If InputLight(Index) Then
          lblInputLabel(Index).ForeColor = &HFFFFFF
        Else
          lblInputLabel(Index).ForeColor = &H0&
        End If
              
      End If
      
    End If
        
  Next Index
          
  Exit Sub
  
ErrorInFileFour:
  Call MsgBox("Error reading from file", vbOKOnly, "File I/O Error")
  
End Sub

'Return true if StrToVerify is numeric, larger than zero, and an integer
'Otherwise return false

Private Function VerifyNumeric(StrToVerify As String)
  If (Len(StrToVerify) > 0 And Trim$(Str$(Val(StrToVerify))) = StrToVerify) Then
    If Val(StrToVerify) = Int(Val(StrToVerify)) And Val(StrToVerify) > 0 Then
      VerifyNumeric = True
    Else
      VerifyNumeric = False
    End If
  Else
    VerifyNumeric = False
  End If
End Function

'Exit the program via the exit button

Private Sub cmdExit_Click()
  Call Unload(Me)
End Sub

