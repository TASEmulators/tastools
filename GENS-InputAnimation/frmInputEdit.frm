VERSION 5.00
Begin VB.Form frmInputEdit 
   BackColor       =   &H8000000B&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Gens Input Animator"
   ClientHeight    =   4740
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
   ScaleHeight     =   4740
   ScaleWidth      =   7170
   StartUpPosition =   3  'Windows Default
   Begin VB.CheckBox chkByte3 
      Caption         =   "Byte 3"
      Enabled         =   0   'False
      Height          =   330
      Left            =   4590
      TabIndex        =   45
      Top             =   585
      Width           =   915
   End
   Begin VB.CheckBox chkByte2 
      Caption         =   "Byte 2"
      Enabled         =   0   'False
      Height          =   330
      Left            =   3150
      TabIndex        =   44
      Top             =   585
      Width           =   915
   End
   Begin VB.CheckBox chkByte1 
      Caption         =   "Byte 1"
      Enabled         =   0   'False
      Height          =   330
      Left            =   1710
      TabIndex        =   43
      Top             =   585
      Width           =   960
   End
   Begin VB.CheckBox chk3Player 
      Caption         =   "3-Player movie format"
      Height          =   465
      Left            =   135
      TabIndex        =   42
      Top             =   2205
      Value           =   1  'Checked
      Width           =   3030
   End
   Begin VB.CheckBox chkRoll 
      Caption         =   "Roll Input?"
      Height          =   510
      Left            =   135
      TabIndex        =   3
      Top             =   2880
      Width           =   1410
   End
   Begin VB.CommandButton cmdSave 
      Caption         =   "Save Pattern"
      Height          =   510
      Left            =   1755
      TabIndex        =   8
      Top             =   4140
      Width           =   1545
   End
   Begin VB.CommandButton cmdRight 
      Caption         =   "Shift &Right"
      Height          =   510
      Left            =   1755
      TabIndex        =   5
      Top             =   3510
      Width           =   1545
   End
   Begin VB.CommandButton cmdReset 
      Caption         =   "Rese&t"
      Height          =   510
      Left            =   1755
      TabIndex        =   6
      Top             =   2880
      Width           =   1545
   End
   Begin VB.CommandButton cmdLoad 
      Caption         =   "Load Pattern"
      Height          =   510
      Left            =   90
      TabIndex        =   7
      Top             =   4140
      Width           =   1545
   End
   Begin VB.TextBox txtDuration 
      BackColor       =   &H80000013&
      Enabled         =   0   'False
      Height          =   360
      Left            =   5040
      TabIndex        =   13
      Top             =   3195
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
   Begin VB.CommandButton cmdExit 
      Cancel          =   -1  'True
      Caption         =   "E&xit"
      Height          =   510
      Left            =   5535
      TabIndex        =   17
      Top             =   4140
      Width           =   1545
   End
   Begin VB.CommandButton cmdInsert 
      Caption         =   "&Insert"
      Enabled         =   0   'False
      Height          =   510
      Left            =   3870
      TabIndex        =   16
      Top             =   4140
      Width           =   1545
   End
   Begin VB.CommandButton cmdLeft 
      Caption         =   "Shift &Left"
      Height          =   510
      Left            =   90
      TabIndex        =   4
      Top             =   3510
      Width           =   1545
   End
   Begin VB.TextBox txtEnd 
      BackColor       =   &H80000013&
      Enabled         =   0   'False
      Height          =   360
      Left            =   5040
      Locked          =   -1  'True
      TabIndex        =   15
      TabStop         =   0   'False
      Top             =   3645
      Width           =   2040
   End
   Begin VB.TextBox txtStart 
      BackColor       =   &H80000013&
      Enabled         =   0   'False
      Height          =   360
      Left            =   5040
      TabIndex        =   11
      Top             =   2745
      Width           =   2040
   End
   Begin VB.Label Label1 
      Alignment       =   1  'Right Justify
      Caption         =   "Controller Input:"
      Height          =   285
      Left            =   45
      TabIndex        =   41
      Top             =   1080
      Width           =   1545
   End
   Begin VB.Label lblColors 
      Alignment       =   1  'Right Justify
      Caption         =   "Pallette:"
      Height          =   285
      Left            =   585
      TabIndex        =   40
      Top             =   1710
      Width           =   960
   End
   Begin VB.Line Line2 
      BorderColor     =   &H8000000E&
      Index           =   3
      X1              =   6345
      X2              =   7065
      Y1              =   1575
      Y2              =   1575
   End
   Begin VB.Line Line2 
      BorderColor     =   &H8000000E&
      Index           =   2
      X1              =   6345
      X2              =   7065
      Y1              =   2025
      Y2              =   2025
   End
   Begin VB.Label lblCurColor 
      Alignment       =   1  'Right Justify
      Caption         =   "Current Color:"
      Height          =   285
      Left            =   4860
      TabIndex        =   39
      Top             =   1710
      Width           =   1365
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   31
      X1              =   2025
      X2              =   2025
      Y1              =   990
      Y2              =   1440
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   30
      X1              =   2385
      X2              =   2385
      Y1              =   990
      Y2              =   1440
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   29
      X1              =   2745
      X2              =   2745
      Y1              =   990
      Y2              =   1440
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   28
      X1              =   3105
      X2              =   3105
      Y1              =   990
      Y2              =   1440
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   27
      X1              =   3465
      X2              =   3465
      Y1              =   990
      Y2              =   1440
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   26
      X1              =   3825
      X2              =   3825
      Y1              =   990
      Y2              =   1440
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   25
      X1              =   4185
      X2              =   4185
      Y1              =   990
      Y2              =   1440
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   24
      X1              =   4545
      X2              =   4545
      Y1              =   990
      Y2              =   1440
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   23
      Visible         =   0   'False
      X1              =   4905
      X2              =   4905
      Y1              =   990
      Y2              =   1440
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   22
      Visible         =   0   'False
      X1              =   5265
      X2              =   5265
      Y1              =   990
      Y2              =   1440
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   21
      Visible         =   0   'False
      X1              =   5625
      X2              =   5625
      Y1              =   990
      Y2              =   1440
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   20
      Visible         =   0   'False
      X1              =   5985
      X2              =   5985
      Y1              =   990
      Y2              =   1440
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   17
      X1              =   1665
      X2              =   1665
      Y1              =   990
      Y2              =   1440
   End
   Begin VB.Label lblFrame 
      Caption         =   "Movie Length:"
      Height          =   285
      Index           =   3
      Left            =   3600
      TabIndex        =   9
      Top             =   2295
      Width           =   3390
   End
   Begin VB.Label lblFrame 
      Caption         =   "Duration"
      Height          =   285
      Index           =   2
      Left            =   4095
      TabIndex        =   12
      Top             =   3240
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
      TabIndex        =   14
      Top             =   3690
      Width           =   1410
   End
   Begin VB.Label lblFrame 
      Caption         =   "Starting Frame"
      Height          =   285
      Index           =   0
      Left            =   3465
      TabIndex        =   10
      Top             =   2790
      Width           =   1500
   End
   Begin VB.Line Line2 
      BorderColor     =   &H8000000E&
      Index           =   5
      X1              =   1665
      X2              =   4545
      Y1              =   2025
      Y2              =   2025
   End
   Begin VB.Line Line2 
      BorderColor     =   &H8000000E&
      Index           =   4
      X1              =   1665
      X2              =   4545
      Y1              =   1575
      Y2              =   1575
   End
   Begin VB.Line Line2 
      BorderColor     =   &H8000000E&
      Index           =   1
      X1              =   1665
      X2              =   4545
      Y1              =   1440
      Y2              =   1440
   End
   Begin VB.Line Line2 
      BorderColor     =   &H8000000E&
      Index           =   0
      X1              =   1665
      X2              =   4545
      Y1              =   990
      Y2              =   990
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   15
      X1              =   7065
      X2              =   7065
      Y1              =   1575
      Y2              =   2025
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   14
      X1              =   1665
      X2              =   1665
      Y1              =   1575
      Y2              =   2025
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   12
      X1              =   6345
      X2              =   6345
      Y1              =   1575
      Y2              =   2025
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   7
      X1              =   4545
      X2              =   4545
      Y1              =   1575
      Y2              =   2025
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   6
      X1              =   4185
      X2              =   4185
      Y1              =   1575
      Y2              =   2025
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   5
      X1              =   3825
      X2              =   3825
      Y1              =   1575
      Y2              =   2025
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   4
      X1              =   3465
      X2              =   3465
      Y1              =   1575
      Y2              =   2025
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   3
      X1              =   3105
      X2              =   3105
      Y1              =   1575
      Y2              =   2025
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   2
      X1              =   2745
      X2              =   2745
      Y1              =   1575
      Y2              =   2025
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   1
      X1              =   2385
      X2              =   2385
      Y1              =   1575
      Y2              =   2025
   End
   Begin VB.Line Line1 
      BorderColor     =   &H8000000E&
      Index           =   0
      X1              =   2025
      X2              =   2025
      Y1              =   1575
      Y2              =   2025
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
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
      Left            =   6345
      TabIndex        =   38
      Top             =   1575
      Width           =   735
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "M"
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
      TabIndex        =   37
      Top             =   990
      Visible         =   0   'False
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "Z"
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
      TabIndex        =   36
      Top             =   990
      Visible         =   0   'False
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
      Index           =   10
      Left            =   4905
      TabIndex        =   35
      Top             =   990
      Visible         =   0   'False
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
      TabIndex        =   34
      Top             =   990
      Visible         =   0   'False
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
      Index           =   8
      Left            =   4185
      TabIndex        =   33
      Top             =   990
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00FFFFFF&
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
      Left            =   4185
      TabIndex        =   32
      Top             =   1575
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
      Caption         =   "C"
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
      TabIndex        =   31
      Top             =   990
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H0000FFFF&
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
      Left            =   3825
      TabIndex        =   30
      Top             =   1575
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
      Index           =   6
      Left            =   3465
      TabIndex        =   29
      Top             =   990
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00FF00FF&
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
      Left            =   3465
      TabIndex        =   28
      Top             =   1575
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
      Index           =   5
      Left            =   3105
      TabIndex        =   27
      Top             =   990
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H000000FF&
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
      Left            =   3105
      TabIndex        =   26
      Top             =   1575
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
      Index           =   4
      Left            =   2745
      TabIndex        =   25
      Top             =   990
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00FFFF00&
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
      Left            =   2745
      TabIndex        =   24
      Top             =   1575
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
      Index           =   3
      Left            =   2385
      TabIndex        =   23
      Top             =   990
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H0000FF00&
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
      Left            =   2385
      TabIndex        =   22
      Top             =   1575
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
      Index           =   2
      Left            =   2025
      TabIndex        =   21
      Top             =   990
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00FF0000&
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
      Left            =   2025
      TabIndex        =   20
      Top             =   1575
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
      Index           =   1
      Left            =   1665
      TabIndex        =   19
      Top             =   990
      Width           =   375
   End
   Begin VB.Label lblInputLabel 
      Alignment       =   2  'Center
      BackColor       =   &H00000000&
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
      Left            =   1665
      TabIndex        =   18
      Top             =   1575
      Width           =   375
   End
   Begin VB.Menu mnuControl 
      Caption         =   "Controller"
      Enabled         =   0   'False
      Begin VB.Menu mnu2Player 
         Caption         =   "2-Player format"
         Enabled         =   0   'False
      End
      Begin VB.Menu mnu3Player 
         Caption         =   "3-Player format"
         Checked         =   -1  'True
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
' Gens 9 (GMV) Input Editor
' Author: JXQ
' Started 11-15-2007
' Finished 11-16-2007
' Finished v2 12-14-2007

Option Explicit

'InputLight represents one of the eight buttons
'for one of the three controllers.  1-8 is controller 1,
'9-16 controller 2, 17-24 for controller 3 (or XYZM).

Dim PInput(1 To 3, 1 To 8) As Boolean

'This is the offset of the controller data in the
'movie file.

Const ControllerOffset = 64

'Frame count from the movie file.  Used to ensure
'that writing past EOF is not a problem.

Dim FrameCount As Long

'Flag for whether a movie is 2-player or 3-player.

Dim Is3Player As Boolean

'Value for keeping track of the current color.

Dim m_CurColor As Long

'Array for quick access to color hex strings.

Dim m_Color(2 To 3, 0 To 7) As Long


'Size the form correctly and center it on the screen

Private Sub Form_Load()
  Me.Height = 5400
  Me.Width = 7260
  Me.Left = (Screen.Width - Me.Width) / 2
  Me.Top = (Screen.Height - Me.Height) / 2
  
  m_Color(2, 0) = &H0&     'Black -   no buttons pressed - 00
  m_Color(2, 1) = &HB04000 'Sonic -   Player 1 only      - 01
  m_Color(2, 2) = &H80FF&  'Tails -   Player 2 only      - 10
  m_Color(2, 3) = &HFFFFFF 'White -   Player 1 and 2     - 11
  m_Color(2, 4) = &H0&     'Dummy color, not used
  m_Color(2, 5) = &H0&     'Dummy color, not used
  m_Color(2, 6) = &H0&     'Dummy color, not used
  m_Color(2, 7) = &H0&     'Dummy color, not used
  
  m_Color(3, 0) = &H0&     'Black -   no buttons pressed - 000
  m_Color(3, 1) = &HFF0000 'Blue -    Player 1 only      - 001
  m_Color(3, 2) = &HFF00&  'Green -   Player 2 only      - 010
  m_Color(3, 3) = &HFFFF00 'Cyan -    Player 1 and 2     - 011
  m_Color(3, 4) = &HFF&    'Red -     Player 3 only      - 100
  m_Color(3, 5) = &HFF00FF 'Magenta - Player 1 and 3     - 101
  m_Color(3, 6) = &HFFFF&  'Yellow -  Player 2 and 3     - 110
  m_Color(3, 7) = &HFFFFFF 'White -   Player 1, 2, and 3 - 111
  
  Is3Player = True
  m_CurColor = 1
  
  Call UpdateColors

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
  Dim Index2 As Integer
  
  For Index = 1 To 3
    For Index2 = 1 To 8
      PInput(Index, Index2) = False
    Next Index2
  Next Index
  
  Call UpdateColors

End Sub

'Shift/Roll all input one space to the left

Private Sub cmdLeft_Click()
  
  Dim Index As Integer
  Dim P1Temp As Boolean
  Dim P2Temp As Boolean
  Dim P3Temp As Boolean
  Dim P3Temp2 As Boolean
  Dim RBound As Integer
  
  'Get right boundary based on controller format
    
  If Is3Player Then
    RBound = 8
  
    'If the "Roll Input?" checkbox is selected, save the left side
    'in temporary variables.
  
    If chkRoll.Value = 1 Then
      P1Temp = PInput(1, 1)
      P2Temp = PInput(2, 1)
      P3Temp = PInput(3, 1)
    End If
  
  Else
    RBound = 12
    
    'If the "Roll Input?" checkbox is selected, save the left side
    'in temporary variables.
  
    If chkRoll.Value = 1 Then
      P1Temp = PInput(1, 1)
      P2Temp = PInput(2, 1)
    End If
    
    'Also save special 3-player input, since it is used for
    'P1 and P2 input for this format type.
    
    P3Temp = PInput(3, 1)
    P3Temp2 = PInput(3, 5)
    
  End If
  
  For Index = 1 To RBound
    
    'For space 8
    
    If Index = 8 Then
            
      'For 3-player format
      
      If Is3Player Then
        
        'If rolling the values, grab saved variables
        
        If chkRoll.Value = 1 Then
        
          PInput(1, Index) = P1Temp
          PInput(2, Index) = P2Temp
          PInput(3, Index) = P3Temp
        
        'Otherwise, these are blank
        
        Else
        
          PInput(1, Index) = False
          PInput(2, Index) = False
          PInput(3, Index) = False
        
        End If
          
      'For 2-player format, grab special saved values
      '(regardless of rolling, this is in space 8)
      
      Else
      
        PInput(1, Index) = P3Temp
        PInput(2, Index) = P3Temp2
        
      End If
        
    'For space 12, we know it's 2-player
    
    ElseIf Index = 12 Then
    
      'if rolling the values, grab from saved variables
      
      If chkRoll.Value = 1 Then
      
        PInput(3, 4) = P1Temp
        PInput(3, 8) = P2Temp
        
      Else
       
        PInput(3, 4) = False
        PInput(3, 8) = False
       
      End If
      
    'For the last four spaces, we know it is two player
    'so move P3 Input used for XYZM correctly
    
    ElseIf Index > 8 Then
    
      PInput(3, Index - 8) = PInput(3, Index - 7)
      PInput(3, Index - 4) = PInput(3, Index - 3)
        
    'Grab the value from the adjacent position in most cases
    
    Else
      
      'Two player case grabs only P1 and P2 input
      
      PInput(1, Index) = PInput(1, Index + 1)
      PInput(2, Index) = PInput(2, Index + 1)
      
      If Is3Player Then
        PInput(3, Index) = PInput(3, Index + 1)
      End If
    
    End If
    
  Next Index
    
  Call UpdateColors

End Sub

'Shift/Roll all input one space to the right

Private Sub cmdRight_Click()
  Dim Index As Integer
  Dim P1Temp As Boolean
  Dim P2Temp As Boolean
  Dim P3Temp As Boolean
  Dim P3Temp2 As Boolean
  Dim RBound As Integer
  
  'Get right boundary based on controller format
    
  If Is3Player Then
    RBound = 8
  
    'If the "Roll Input?" checkbox is selected, save the right side
    'in temporary variables.
  
    If chkRoll.Value = 1 Then
      P1Temp = PInput(1, 8)
      P2Temp = PInput(2, 8)
      P3Temp = PInput(3, 8)
    End If
  
  Else
    RBound = 12
    
    'If the "Roll Input?" checkbox is selected, save special
    '3-player input, since it is used for P1 and P2 input for
    'this controller format.
    
    If chkRoll.Value = 1 Then
    
      P3Temp = PInput(3, 4)
      P3Temp2 = PInput(3, 8)
    
    End If
    
    'Also save the right side of P1 and P2 normal input
    'in temporary variables.
  
    P1Temp = PInput(1, 8)
    P2Temp = PInput(2, 8)
    
  End If
  
  For Index = RBound To 1 Step -1
    
    'For space 1
    
    If Index = 1 Then
            
      'For 3-player format
      
      If Is3Player Then
        
        'If rolling the values, grab saved variables
        
        If chkRoll.Value = 1 Then
        
          PInput(1, Index) = P1Temp
          PInput(2, Index) = P2Temp
          PInput(3, Index) = P3Temp
        
        'Otherwise, these are blank
        
        Else
        
          PInput(1, Index) = False
          PInput(2, Index) = False
          PInput(3, Index) = False
        
        End If
          
      'For 2-player format,
      
      Else
      
        'If rolling the values, grab special saved variables
        
        If chkRoll.Value = 1 Then
        
          PInput(1, Index) = P3Temp
          PInput(2, Index) = P3Temp2
        
        'Otherwise, these are blank
        
        Else
        
          PInput(1, Index) = False
          PInput(2, Index) = False
        
        End If
        
      End If
        
    'For space 9, we know it's 2-player
    
    ElseIf Index = 9 Then
    
      'grab from special saved variables
      
      PInput(3, 1) = P1Temp
      PInput(3, 5) = P2Temp
     
    'For other values over 8, we know it is 2-player
    'so move P3 Input used for XYZM correctly
    
    ElseIf Index > 8 Then
    
      PInput(3, Index - 8) = PInput(3, Index - 9)
      PInput(3, Index - 4) = PInput(3, Index - 5)
        
    'Grab the value from the adjacent position in most cases
    
    Else
      
      'Two player case grabs only P1 and P2 input
      
      PInput(1, Index) = PInput(1, Index - 1)
      PInput(2, Index) = PInput(2, Index - 1)
      
      If Is3Player Then
        PInput(3, Index) = PInput(3, Index - 1)
      End If
    
    End If
    
  Next Index
    
  Call UpdateColors
  
End Sub

'This is the routine that writes the appropriate data to the movie file.

Private Sub cmdInsert_Click()
  
'  Exit Sub
  
  Dim FrameStart As Long
  Dim FrameEnd As Long
  Dim CurrentFrame As Long
  Dim Index As Integer
  Dim Index2 As Integer
  Dim ControllerBinaryData(1 To 3) As Byte
  
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

  For Index = 1 To 3

    'Unlike most other formats, this starts at FF and removes values,
    'rather than starting at 0 and adding them.
    
    ControllerBinaryData(Index) = 255
    
    For Index2 = 1 To 8
      
      ' (Negative) bit values for each button for VB for GMV format.
      
      '   Player/Byte 1    Player/Byte 2    Player/Byte 3
      ' ^ 1                1                1
      ' v 2                2                2
      ' < 4                4                4
      ' > 8                8                8
      ' a 16               16               16
      ' b 32               32               32
      ' c 64               64               64
      ' s 128              128              128
      
      'X, Y, Z, M are used in 2-player format, in the third byte.
      
      '   Player 1/Byte 3  Player 2/Byte 3
      ' x 1                16
      ' y 2                32
      ' z 4                64
      ' m 8                128
      
      If PInput(Index, Index2) Then
        ControllerBinaryData(Index) = ControllerBinaryData(Index) - (2 ^ (Index2 - 1))
      End If
      
    Next Index2
  
  Next Index

  'A piece of data goes in the following spot:
  'Start with the ControllerOffset of the entire movie = 64.
  'Advance from there to the first frame desired.  Each
  'frame is 3 bytes.  The first controller's input goes
  'in this position, and each subsequent controller will go
  '1 byte later.  In a 2-player format movie, X, Y, Z, Mode
  'for Player 1 and 2 are combined into this third byte.

  'Only write data for bytes that are selected.
  
  On Error GoTo FileWriteError

    For CurrentFrame = FrameStart - 1 To FrameEnd - 1
      If chkByte1.Value = vbChecked Then
        Put #1, 65 + CurrentFrame * 3, ControllerBinaryData(1)
      End If
      
      If chkByte2.Value = vbChecked Then
        Put #1, 66 + CurrentFrame * 3, ControllerBinaryData(2)
      End If
      
      If chkByte3.Value = vbChecked Then
        Put #1, 67 + CurrentFrame * 3, ControllerBinaryData(3)
      End If
    Next CurrentFrame

  On Error GoTo 0

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
  
  'First byte of file signature for GMV files
  
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
  
    frmNew.Caption = "Open GMV file"
      
    Call frmNew.Show(vbModal)
    
    'A non-null text file indicates that
    'a file was loaded successfully
    
    If Len(frmNew.strFileName) > 0 Then
      
      'Check for SMV signature of 47 65 6E 73 (hex, little-endian)
      '736E6547 (hex) = 1936614727 (decimal)
      
      On Error GoTo ErrorInFile
        Get #1, 1, Signature
      On Error GoTo 0
      
      If Signature = 1936614727 Then
        
        'Valid GMV file, so enable appropriate functions and
        'update the filename textbox
        
        txtFile.Text = frmNew.strFileName
        Call FileSetup(True)
      
      Else
        
        Call MsgBox("File is not a valid GMV file", vbOKOnly, "Invalid File")
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
  Dim Index2 As Integer
  Dim sMessage As String
  
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
    '001-004: 4 byte signature 4A 58 51 3F
    '005    : 1 byte (1 bit) representing if the
    '         movie is 3-player format (1) or not (0).
    '006-008: 3 bytes (24 bits) of data.  The 24 bits refer
    '         to the buttons pressed on each controller.

    'Check for PAT signature of 4A 58 51 3F (hex, little-endian)
    '3F51584A (hex) = 1062295626 (decimal)

      On Error GoTo ErrorInFileTwo
        Get #2, 1, Signature
      On Error GoTo 0

      If Signature = 1062295626 Then

        'Valid PAT file, so load controller toggles into grid

        Get #2, 5, PatternData
        
        'If the pattern controller type doesn't match,
        'warn the user only if a movie is open.
        
        If (PatternData = 1) <> Is3Player Then
                    
          If chk3Player.Value = 2 Then
          
            sMessage = "This pattern is for use with " & CStr(PatternData + 2) & "-player format." _
              & "Do you want to change to this format?"
          
            'If they proceed, switch the setting via the menu toggles.
            'This will give them the appropriate warning and switch
            'settings for us.  Otherwise, exit.
          
            If MsgBox(sMessage, vbYesNo, "Inconsistent format") = vbYes Then
          
              If Is3Player Then
                Call mnu2Player_Click
              Else
                Call mnu3Player_Click
              End If
            
            Else
            
              Close #2
              Exit Sub
          
            End If
          
          'If a movie isn't loaded, just convert it automatically
          Else
        
            Is3Player = Not Is3Player
            Call UpdateForm
            
          End If
            
        End If
        
        'Get the appropriate values, and set them in the grid
        
        For Index = 6 To 8
          
          'Get data from file
          
          Get #2, Index, PatternData
          
          'Check each bit for its value
           
          For Index2 = 1 To 8
          
            If (PatternData \ (2 ^ (Index2 - 1))) Mod 2 = 1 Then
              PInput(Index - 5, Index2) = True
            Else
              PInput(Index - 5, Index2) = False
            End If
          
          Next Index2
        
        Next Index

        Call UpdateColors
        
      Else

        Call MsgBox("File is not a valid Gens PAT file", vbOKOnly, "Invalid File")

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
  Dim Index2 As Integer

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
    '001-004: 4 byte signature 4A 58 51 3F
    '005    : 1 byte (1 bit) representing if the
    '         movie is 3-player format (1) or not (0).
    '006-008: 3 bytes (24 bits) of data.  The 24 bits refer
    '         to the buttons pressed on each controller.

    'Check for PAT signature of 4A 58 51 3F (hex, little-endian)
    '3F51584A (hex) = 1062295626 (decimal)

    On Error GoTo ErrorInFileThree
      Put #3, 1, 1062295626
    On Error GoTo 0

    'Set controller format (1: 3-player, 0: 2-player)
    
    If Is3Player Then
      PatternData = 1
    Else
      PatternData = 0
    End If
    
    Put #3, 5, PatternData

    PatternData = 0

    'Now save each button press into the file

    For Index = 1 To 3

      For Index2 = 1 To 8
      
        If PInput(Index, Index2) Then PatternData = PatternData + 2 ^ ((Index2 - 1) Mod 8)

      Next Index2
      
      'Every 8 bits, write the byte to the file

      On Error GoTo ErrorInFileThree
        Put #3, 5 + Index, PatternData
      On Error GoTo 0
      
      PatternData = 0

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

'Each box can be clicked on to toggle the input.
'If the spot is not enabled due to controller settings,
'the click will be ignored.

Private Sub lblInputLabel_MouseDown(Index As Integer, Button As Integer, Shift As Integer, X As Single, Y As Single)
  
  'For 3-player movies, ignore clicks to the last four input spots.
  
  If Is3Player Then
    
    If Index <= 8 Then
    
      If m_CurColor Mod 2 = 1 Then
        PInput(1, Index) = True
      Else
        PInput(1, Index) = False
      End If
  
      If (m_CurColor \ 2) Mod 2 = 1 Then
        PInput(2, Index) = True
      Else
        PInput(2, Index) = False
      End If
  
      If (m_CurColor \ 4) = 1 Then
        PInput(3, Index) = True
      Else
        PInput(3, Index) = False
      End If
    
    End If
  
  'For 2-player movies, ignore clicks to the later pallette spots.
  
  Else
      
    If Index <= 8 Then
    
      If m_CurColor Mod 2 = 1 Then
        PInput(1, Index) = True
      Else
        PInput(1, Index) = False
      End If
  
      If (m_CurColor \ 2) Mod 2 = 1 Then
        PInput(2, Index) = True
      Else
        PInput(2, Index) = False
      End If
    
    'Now check P1 X,Y,Z,M and P2 X,Y,Z,M
    
    ElseIf Index <= 12 Then
      
      If m_CurColor Mod 2 = 1 Then
        PInput(3, Index - 8) = True
      Else
        PInput(3, Index - 8) = False
      End If
  
      If (m_CurColor \ 2) Mod 2 = 1 Then
        PInput(3, Index - 4) = True
      Else
        PInput(3, Index - 4) = False
      End If
  
    End If
  
  End If
  
  'Otherwise, toggle the appropriate boolean value based
  'on the current color, then call the update routine.
    
  If Index >= 20 And Index < 30 Then
  
    m_CurColor = Index - 20
  
  End If
  
  Call UpdateColors

End Sub

'Change movie format to 2-players through the menu.
'This is enabled only when a movie file is open.

Private Sub mnu2Player_Click()
  Dim sMessage As String
  Dim SyncData As Byte
  
  'If already 2-player, no need to do anything.
  If Not Is3Player Then Exit Sub
  
  'Warn user of format change.
  sMessage = "Changing this setting will change the controller format " _
    & "in the movie file." & vbCrLf & "This can have undesired effects if " _
    & "the controller data uses the third player.  Continue?"
    
  If MsgBox(sMessage, vbYesNo, "Warning") = vbYes Then
    On Error GoTo MenuFileError
      Get #1, 23, SyncData
    On Error GoTo 0
    
    'Double check actual data from file.
    'This shouldn't ever be false, but just in case.
    
    If (SyncData \ 32) Mod 2 = 1 Then
      
      SyncData = SyncData - 32
      On Error GoTo MenuFileError
        Put #1, 23, SyncData
      On Error GoTo 0
    
      Is3Player = False
      
      mnu2Player.Checked = True
      mnu3Player.Checked = False
    
      Call UpdateForm
      
    End If
  
  End If

  Exit Sub

MenuFileError:
  Call MsgBox("Error " & Err.Number & " when trying to read file", vbOKOnly, "File I/O Error")
  Close #1

End Sub

'Change movie format to 3-players through the menu.
'This is enabled only when a movie file is open.

Private Sub mnu3Player_Click()
  
  Dim sMessage As String
  Dim SyncData As Byte
  
  'If already 3-player, no need to do anything.
  If Is3Player Then Exit Sub
  
  'Warn user of format change.
  sMessage = "Changing this setting will change the controller format " _
    & "in the movie file." & vbCrLf & "This can have undesired effects if " _
    & "the controller data uses the X, Y, Z, or Mode buttons.  Continue?"
    
  If MsgBox(sMessage, vbYesNo, "Warning") = vbYes Then
    On Error GoTo MenuFileError2
      Get #1, 23, SyncData
    On Error GoTo 0
    
    'Double check actual data from file.
    'This shouldn't ever be false, but just in case.
    
    If (SyncData \ 32) Mod 2 = 0 Then
      
      SyncData = SyncData + 32
      On Error GoTo MenuFileError2
        Put #1, 23, SyncData
      On Error GoTo 0
    
      Is3Player = True
        
      mnu2Player.Checked = False
      mnu3Player.Checked = True
    
      Call UpdateForm
      
    End If
  
  End If
  
  Exit Sub
  
MenuFileError2:
  Call MsgBox("Error " & Err.Number & " when trying to read file", vbOKOnly, "File I/O Error")
  Close #1

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

' If the duration is being updated, real-time
' update the ending frame if starting frame is given.

Private Sub txtDuration_Change()
  
  If (VerifyNumeric(txtStart.Text)) And VerifyNumeric(txtDuration.Text) Then
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
    
  If VerifyNumeric(txtStart.Text) Then
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
  Dim ControllerFormat As Byte
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
    
    chkByte1.Enabled = True
    chkByte2.Enabled = True
    chkByte3.Enabled = True
    chkByte1.Value = vbChecked
    chkByte2.Value = vbChecked
    chkByte3.Value = vbChecked
    
    'Here, get 2 or 3 player format flag
    'from the file's metadata
    
    On Error GoTo FileReadError
      Get #1, 23, SyncData
    On Error GoTo 0
        
    'Display the frame count
    
    FrameCount = (LOF(1) - 64) / 3
    lblFrame(3).Caption = "Movie Length: " & Str$(FrameCount) & " frames"
    
    'Enable the controller format toggle menu
    
    chk3Player.Enabled = False
    
    mnuControl.Enabled = True
    mnu2Player.Enabled = True
    mnu3Player.Enabled = True
    
    'Set the Up+Down/Left+Right flags based on movie data
    
    If (SyncData \ 32) Mod 2 = 1 Then
      Is3Player = True
      mnu3Player.Checked = True
      mnu2Player.Checked = False
    Else
      Is3Player = False
      mnu3Player.Checked = False
      mnu2Player.Checked = True
    End If
        
    Call UpdateForm
    
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
    
    chkByte1.Enabled = False
    chkByte2.Enabled = False
    chkByte3.Enabled = False
    chkByte1.Value = vbUnchecked
    chkByte2.Value = vbUnchecked
    chkByte3.Value = vbUnchecked
    
    txtFile.Enabled = False
    
    mnuControl.Enabled = False
    mnu2Player.Enabled = False
    mnu3Player.Enabled = False
    
    chk3Player.Enabled = True
    If Is3Player Then
      chk3Player.Value = 1
    Else
      chk3Player.Value = 0
    End If
    
    'Remove label showing the frame count
    
    lblFrame(3).Caption = ""
        
    'Change button caption to "&Open" for the next file
    
    cmdOpen.Caption = "&Open"
   
  End If

  Exit Sub

FileReadError:
  Call MsgBox("Error " & Err.Number & " when trying to read file", vbOKOnly, "File I/O Error")
  Close #1

End Sub

'Toggles whether 2-Player or 3-Player mode is active.
Private Sub chk3Player_Click()
  
  If chk3Player.Value = vbChecked Then
    Is3Player = True
  Else
    Is3Player = False
  End If
  
  Call UpdateForm
  Call UpdateColors
  
End Sub

'This routine will update the display with correct colors.

Private Function UpdateColors()
  Dim Index As Integer
  Dim Temp As Integer
  
  '3 Player case is just case by case
  
  If Is3Player Then
    
    For Index = 1 To 8
    
      Temp = 0
      If PInput(1, Index) Then Temp = Temp + 1
      If PInput(2, Index) Then Temp = Temp + 2
      If PInput(3, Index) Then Temp = Temp + 4
    
      lblInputLabel(Index).ForeColor = m_Color(3, Temp)
    
    Next Index
    
    'Finally, update current color
  
    lblInputLabel(30).BackColor = m_Color(3, m_CurColor)
    
  Else
    
    '2 Player case also uses P3 Input as X, Y, Z, Mode input
    For Index = 1 To 8
      Temp = 0
      If PInput(1, Index) Then Temp = Temp + 1
      If PInput(2, Index) Then Temp = Temp + 2
  
      lblInputLabel(Index).ForeColor = m_Color(2, Temp)
    Next Index
        
    For Index = 9 To 12
      Temp = 0
      If PInput(3, Index - 8) Then Temp = Temp + 1
      If PInput(3, Index - 4) Then Temp = Temp + 2
      lblInputLabel(Index).ForeColor = m_Color(2, Temp)
    Next Index
  
    'Finally, update current color
  
    lblInputLabel(30).BackColor = m_Color(2, m_CurColor)
  End If
  
End Function

'This will set appropriate objects availability
'based on the given format of the movie.

Private Sub UpdateForm()
  Dim Index As Integer
  
  'For 3-player mode, make 8 colors available, but only
  '8 spaces for input available.
  
  If Is3Player Then
    
    chk3Player.Value = 1
    
    For Index = 0 To 7
      lblInputLabel(Index + 20).BackColor = m_Color(3, Index)
      lblInputLabel(Index + 20).Visible = True
      lblInputLabel(Index + 20).Enabled = True
      
      If Index > 3 Then
        Line1(Index).Visible = True
      Else
        Line1(Index + 20).Visible = False
      End If
      
    Next Index
    
    For Index = 8 To 11
      Line1(Index + 12).Visible = False
      lblInputLabel(Index + 1).Visible = False
      lblInputLabel(Index + 1).Enabled = False
    Next Index
    
    Line2(0).X2 = 4545
    Line2(1).X2 = 4545
    Line2(4).X2 = 4545
    Line2(5).X2 = 4545
    
  'For 2-player mode, make only 4 colors available, but
  '12 spaces for input available.
    
  Else
    
    chk3Player.Value = 0
    
    For Index = 0 To 3
      lblInputLabel(Index + 20).BackColor = m_Color(2, Index)
      lblInputLabel(Index + 20).Visible = True
      lblInputLabel(Index + 20).Enabled = True
    Next Index
    
    For Index = 4 To 7
      lblInputLabel(Index + 20).BackColor = &H0&
      lblInputLabel(Index + 20).Visible = False
      lblInputLabel(Index + 20).Enabled = False
      Line1(Index).Visible = False
    Next Index
    
    For Index = 8 To 11
      Line1(Index + 12).Visible = True
      lblInputLabel(Index + 1).Visible = True
      lblInputLabel(Index + 1).Enabled = True
    Next Index
    
    Line2(0).X2 = 5985
    Line2(1).X2 = 5985
    Line2(4).X2 = 3105
    Line2(5).X2 = 3105
    
    If m_CurColor > 3 Then m_CurColor = 1
    
    Call UpdateColors
  
  End If
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

