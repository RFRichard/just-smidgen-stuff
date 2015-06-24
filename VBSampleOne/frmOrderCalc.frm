VERSION 5.00
Begin VB.Form frmOrderCalc 
   Caption         =   "Widget Dimension and Order Calculator"
   ClientHeight    =   5985
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   5325
   LinkTopic       =   "Form1"
   ScaleHeight     =   5985
   ScaleWidth      =   5325
   StartUpPosition =   3  'Windows Default
   Begin VB.Frame frame 
      Height          =   2415
      Left            =   2880
      TabIndex        =   13
      Top             =   2280
      Width           =   2175
      Begin VB.Label lblOutput 
         Height          =   2055
         Left            =   120
         TabIndex        =   14
         Top             =   240
         Width           =   1935
      End
   End
   Begin VB.TextBox txtDepth 
      Height          =   285
      Left            =   600
      TabIndex        =   12
      Text            =   "0.0"
      Top             =   4320
      Width           =   1575
   End
   Begin VB.TextBox txtWidth 
      Height          =   285
      Left            =   600
      TabIndex        =   10
      Text            =   "0.0"
      Top             =   2640
      Width           =   1575
   End
   Begin VB.TextBox txtHeight 
      Height          =   285
      Left            =   600
      TabIndex        =   9
      Text            =   "0.0"
      Top             =   3480
      Width           =   1575
   End
   Begin VB.ComboBox cmbParts 
      Height          =   315
      Left            =   360
      TabIndex        =   8
      Top             =   1320
      Width           =   4695
   End
   Begin VB.ComboBox cmbCustomer 
      Height          =   315
      ItemData        =   "frmOrderCalc.frx":0000
      Left            =   360
      List            =   "frmOrderCalc.frx":0002
      TabIndex        =   7
      Top             =   600
      Width           =   4695
   End
   Begin VB.CommandButton cmdButtons 
      Caption         =   "Print Receipt"
      Height          =   495
      Index           =   1
      Left            =   3240
      TabIndex        =   1
      Top             =   5160
      Width           =   1575
   End
   Begin VB.CommandButton cmdButtons 
      Caption         =   "Calculate Discounted Price"
      Height          =   495
      Index           =   0
      Left            =   600
      TabIndex        =   0
      Top             =   5160
      Width           =   1575
   End
   Begin VB.Label Label1 
      Caption         =   "Width"
      Height          =   255
      Left            =   600
      TabIndex        =   11
      Top             =   2400
      Width           =   1935
   End
   Begin VB.Label Label6 
      Caption         =   "Widget Dimensions"
      Height          =   255
      Left            =   360
      TabIndex        =   6
      Top             =   2040
      Width           =   1935
   End
   Begin VB.Label Label5 
      Caption         =   "Part"
      Height          =   375
      Left            =   360
      TabIndex        =   5
      Top             =   1080
      Width           =   2415
   End
   Begin VB.Label Label4 
      Caption         =   "Customer"
      Height          =   255
      Left            =   360
      TabIndex        =   4
      Top             =   360
      Width           =   2655
   End
   Begin VB.Label Label3 
      Caption         =   "Depth"
      Height          =   375
      Left            =   600
      TabIndex        =   3
      Top             =   4080
      Width           =   2415
   End
   Begin VB.Label Label2 
      Caption         =   "Height"
      Height          =   375
      Left            =   600
      TabIndex        =   2
      Top             =   3240
      Width           =   2415
   End
End
Attribute VB_Name = "frmOrderCalc"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Enum FormButtons
    CALCULATE_PRICE
    PRINT_RECEIPT
End Enum

Public Sub SetFormControls(colPartsData As Collection, colCustomerData As Collection)

Dim aryData As Variant
' fill parts combo
For Each aryData In colPartsData
    Call Me.cmbParts.AddItem(aryData(0))
Next

' fill parts combo
For Each aryData In colCustomerData
    Call Me.cmbCustomer.AddItem(aryData(0))
Next


End Sub

Public Sub GetFormControlData()


End Sub

Private Sub cmdButtons_Click(Index As Integer)

Dim curWidgetPrice As Currency
Dim strOutput As String

    Select Case Index
    
        Case CALCULATE_PRICE:
                                   
            curWidgetPrice = CalculateWidgetPrice(cmbParts.Text _
                                , cmbCustomer.Text _
                                , CSng(Me.txtWidth.Text) _
                                , CSng(Me.txtHeight.Text) _
                                , CSng(Me.txtDepth.Text))

            strOutput = PrintReceipt(cmbCustomer.Text, cmbParts.Text, curWidgetPrice, False)
            Me.lblOutput.Caption = strOutput
                        
            
        Case PRINT_RECEIPT:
                        
            curWidgetPrice = CalculateWidgetPrice(cmbParts.Text _
                                , cmbCustomer.Text _
                                , CSng(Me.txtWidth.Text) _
                                , CSng(Me.txtHeight.Text) _
                                , CSng(Me.txtDepth.Text))
 
            strOutput = PrintReceipt(cmbCustomer.Text, cmbParts.Text, curWidgetPrice, True)
            Me.lblOutput.Caption = strOutput

    End Select

End Sub

Private Sub Form_Load()

End Sub
