VERSION 5.00
Begin VB.Form frmAbout 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "About Gens 9.5 Input Animator"
   ClientHeight    =   3000
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   3630
   BeginProperty Font 
      Name            =   "Arial"
      Size            =   9.75
      Charset         =   0
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3000
   ScaleWidth      =   3630
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton cmdOK 
      Caption         =   "Twombly"
      Default         =   -1  'True
      Height          =   510
      Left            =   1170
      TabIndex        =   0
      Top             =   2430
      Width           =   1230
   End
   Begin VB.Image Image1 
      Height          =   1200
      Left            =   1215
      Picture         =   "frmAbout.frx":0000
      Top             =   720
      Width           =   1155
   End
   Begin VB.Label Label2 
      Alignment       =   2  'Center
      Caption         =   "http://tasvideos.org/JXQ.html"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   285
      Left            =   45
      TabIndex        =   3
      Top             =   2070
      Width           =   3570
   End
   Begin VB.Label Label1 
      Alignment       =   2  'Center
      Caption         =   "Developed by JXQ"
      BeginProperty Font 
         Name            =   "Arial"
         Size            =   9.75
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   285
      Left            =   45
      TabIndex        =   2
      Top             =   360
      Width           =   3570
   End
   Begin VB.Label lblAbout 
      Alignment       =   2  'Center
      Caption         =   "Gens 9.5 Input Animator v2.1"
      Height          =   285
      Left            =   45
      TabIndex        =   1
      Top             =   45
      Width           =   3570
   End
End
Attribute VB_Name = "frmAbout"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

'It would be silly to put a comment here

Private Sub cmdOK_Click()
  
  Call Me.Hide

End Sub
