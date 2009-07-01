VERSION 5.00
Begin VB.Form frmTextInput 
   BorderStyle     =   4  'Fixed ToolWindow
   Caption         =   "Text Input"
   ClientHeight    =   1275
   ClientLeft      =   45
   ClientTop       =   285
   ClientWidth     =   3360
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
   ScaleHeight     =   1275
   ScaleWidth      =   3360
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox txtInput 
      Height          =   375
      Left            =   90
      TabIndex        =   0
      Top             =   180
      Width           =   3165
   End
   Begin VB.CommandButton cmdOK 
      Caption         =   "&OK"
      Default         =   -1  'True
      Height          =   510
      Left            =   90
      TabIndex        =   1
      Top             =   675
      Width           =   1545
   End
   Begin VB.CommandButton cmdCancel 
      Cancel          =   -1  'True
      Caption         =   "&Cancel"
      Height          =   510
      Left            =   1710
      TabIndex        =   2
      Top             =   675
      Width           =   1545
   End
End
Attribute VB_Name = "frmTextInput"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

'Variable to give back to main form

Public sInput As String

'When clicking 'OK', return to the main form
'and store the text given in sInput

Private Sub cmdOK_Click()
  
  'If the input is valid, update it as such
  
  If Len(txtInput.Text) > 0 Then
    
    sInput = txtInput.Text
    
  End If
  
  'All done here
  
  Call Me.Hide
    
End Sub

'Store nothing in sInput and leave the form

Private Sub cmdCancel_Click()

  sInput = ""
  Call Me.Hide
  
End Sub


