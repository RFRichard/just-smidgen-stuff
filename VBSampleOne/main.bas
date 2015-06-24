Attribute VB_Name = "Controller"
Public colPartsData As Collection
Public colCustomerData As Collection

Public Sub main()
' WidgetEvaluation
' RRichard
'
'
' Set Error Handlers
On Error GoTo ExceptionHandler

' create collections for holding parts and customer data
Set colPartsData = New Collection
Set colCustomerData = New Collection


'Load PartRates
Dim strFilename As String
strFilename = "c:\PartRates.txt"
Call LoadPartRates(colPartsData, strFilename)

'Load Customers
Dim strDSN As String
'strDSN = "TextData"
Call LoadCustomerData(colCustomerData, strDSN)

' Run Tests
TestWidgetCalculations

' Set control data
Call frmOrderCalc.SetFormControls(colPartsData, colCustomerData)

' Show Forms
frmOrderCalc.Show

'err.Raise 1000

Exit Sub
ExceptionHandler:
    Call ProcessExceptions(err)
End Sub

Public Sub ProcessExceptions(err As ErrObject)
    Call LogError(err)
    Call DisplayErrorMessage(err)
End Sub

Public Sub DisplayErrorMessage(err As ErrObject)

MsgBox "An Error Has Occured " _
        & vbCrLf & " Error Number " & CStr(err.Number) _
        & vbCrLf & err.Description, vbCritica

End Sub

Public Sub LogError(err As ErrObject)
    'TODO: Place logging services
End Sub

Public Sub LoadPartRates(colPartsData As Collection, strFilename)

On Error GoTo ExceptionHandler

Dim strPartRecord As String
Dim intRecordCount As Integer
Dim strChar As String * 1
Dim aryPartRecordFields() As String


Open strFilename For Input As #1

' parts file records are delimited by newline (\n) !(\r\n)
' so input must be one char at a time until newline
' read parts records into collection
Do While Not EOF(1)
    strChar = Input(1, 1)
    If strChar = Chr(10) Then
        ' create array of the record fields
        ' add the fields to the collection
        ' check for empty string or first record
        ' first record is the col names record
        If strPartRecord <> "" Then
            If intRecordCount > 0 Then
                aryPartRecordFields = Split(strPartRecord, "*!*")
                
                ' add to collection set Part column as key
                Call colPartsData.Add(aryPartRecordFields, aryPartRecordFields(0))
                
            End If
        End If
        intRecordCount = intRecordCount + 1
        strPartRecord = ""
    Else
        strPartRecord = strPartRecord & strChar
    End If
Loop

Close #1
Exit Sub
ExceptionHandler:
    Close #1
    Call ProcessExceptions(err)
End Sub

Public Sub LoadCustomerData(colCustomerData As Collection, strDSN As String)
'   Load Customer Data from RDBMS or othe Datasource using ADO
'
' This subroutine works with ADO and the Microsoft Text Driver
' DSN to enable testing with a text file rather than an actual RDBMS.
' The text file should be place in c:\ and be called customers.txt
' Replace the connection parameters with those of the RDBMS DSN
' containing the ERP database and the CUSTOMERS table.


On Error GoTo ExceptionHandler

Dim con As New ADODB.Connection
Dim cmd As New ADODB.Command
Dim rs As New ADODB.Recordset
Dim intRCnt As Integer
Dim aryCustomerData As Variant
Dim intTotalFields As Integer
Dim arySubArray() As String
Dim strConnectionParameters As String
Dim strPathforTextFileforTesting As String

strPathforTextFileforTesting = "c:\"
strConnectionParameters = "Driver={Microsoft Text Driver (*.txt; *.csv)};" _
                        & "Dbq=" & strPathforTextFileforTesting & ";"

Call con.Open(strConnectionParameters)
Set cmd.ActiveConnection = con
cmd.CommandText = "SELECT * FROM CUSTOMERS.txt;"
rs.CursorLocation = adUseClient

Call rs.Open(cmd)

intRCnt = rs.RecordCount
rs.MoveFirst

If Not rs.EOF Then
    aryCustomerData = rs.GetRows()
    intTotalFields = UBound(aryCustomerData)
    ReDim arySubArray(intTotalFields) As String
    
    ' iterate through records and place them into collection
    For i = 0 To intRCnt - 1
        intTotalFields = UBound(aryCustomerData)
        ' copy the record fields to another array
        For j = 0 To intTotalFields
            arySubArray(j) = aryCustomerData(j, i)
        Next
        ' insert array of fields into customer collection
        ' with customer name as the key
        Call colCustomerData.Add(arySubArray, arySubArray(0))
        
    Next
End If

Exit Sub

ExceptionHandler:
    Call ProcessExceptions(err)

End Sub


Function CalculateWidgetPrice(strPart As String, strCustomer As String _
        , sngWidth As Single, sngHeight As Single, sngDepth As Single) As Currency
     
     
     ' set default of zero
     CalculateWidgetPrice = CCur(0#)
     
     ' check for valid user input
     If strCustomer = "" Then
        MsgBox "Please Select A Customer"
     ElseIf strPart = "" Then
        MsgBox "Please Select A Part"
     ElseIf sngWidth = 0 Or sngHeight = 0 Or sngDepth = 0 Then
        MsgBox "Please Enter Valid Widget Dimensions."
     Else
        ' calculate price
        ' get widget volume
        Dim sngVolumeUnits As Single
        sngVolumeUnits = CalculateWidgetTotalVolume(strPart _
                    , sngWidth, sngHeight, sngDepth)
                
        ' get part price
        Dim sngPartRate As Single
        sngPartRate = colPartsData(strPart)(4)
                
        ' caluclate extended price based on volume
        Dim curExtendedPrice As Currency
        curExtendedPrice = CalculateWidgetExtendedPrice(sngPartRate, sngVolumeUnits)
                
        ' calucluate discount price
        Dim curDiscountedPrice As Currency
        Dim sngDiscountRate As Single
        sngDiscountRate = colCustomerData(strCustomer)(5)
        curDiscountedPrice = CalculateWidgetDiscountedPrice(CSng(curExtendedPrice), sngDiscountRate)
        CalculateWidgetPrice = curDiscountedPrice
                
     End If

End Function

Function CalculateWidgetTotalVolume(strPart As String, sngWidth As Single, sngHeight As Single, sngDepth As Single) As Single
     ' This funtion assumes that the application is knowlegable of how to
     ' calculate a specific widgets volume.
     ' It would be best to use an abstraction that can be placed with the parts file.
     Dim sngVolume As Single
     
     Select Case strPart
          Case "ABC":
          Case "XYZ":
               ' Assuming the XYZ widget top protrusion length is one fifth of the widget length.
               ' Assuming the XYZ widget top protrusion height is one third of the widget height.
               ' Assuming the XYZ widget top protrusion depth is equal to the widget depth.
               ' Total volume of xyz widget top protrusion is two tenths of the widget main body.
               ' Two tenths of main body volume + main body volume = eleven fifteenths of h*w*d
               sngVolume = (sngWidth * sngHeight * sngDepth) * (11# / 15#)
               CalculateWidgetTotalVolume = sngVolume
               
          Case "DEF":
     End Select
     
End Function

Public Function CalculateWidgetExtendedPrice(sngPriceRate As Single, sngUnits As Single) As Currency
               
    CalculateWidgetExtendedPrice = CCur(sngPriceRate) * sngUnits
End Function


Public Function CalculateWidgetDiscountedPrice(sngPrice As Single, sngDiscount As Single) As Currency
               
    CalculateWidgetDiscountedPrice = CCur(sngPrice - (sngPrice * sngDiscount))
End Function

Public Function TestWidgetCalculations()
    Dim sngResult As Single
    Dim curResult As Currency
    
    Debug.Print vbCrLf & vbCrLf
    Debug.Print " Running Widget Calculation Tests "
    Debug.Print "----------------------"
    
    ' Test CalculateWidgetTotalVolume
    sngResult = CalculateWidgetTotalVolume("XYZ", 5#, 3#, 1#)
    Debug.Print "CalculateWidgetTotalVolume(""XYZ"", 5#, 3#, 1#)"
    Debug.Print "Result: " & Format(sngResult, "0.0000")
    Debug.Assert (sngResult = 11#)
    
    ' Test CalculateWidgetExtendedPrice
    curResult = CalculateWidgetExtendedPrice(10#, 2#)
    Debug.Print "CalculateWidgetExtendedPrice(10#, 2#)"
    Debug.Print "Result: " & Format(curResult, "0.00")
    Debug.Assert (curResult = 20#)

    ' Test CalculateWidgetDiscountedPrice
    curResult = CalculateWidgetDiscountedPrice(10#, 0.2)
    Debug.Print "CalculateWidgetDiscountedPrice(10#, 0.2)"
    Debug.Print "Result: " & Format(curResult, "0.00")
    Debug.Assert (curResult = 8#)
                
    ' Test CalculateWidgetPrice
    curResult = CalculateWidgetPrice("XYZ", "Infisy", 1#, 1#, 1#)
    Debug.Print "CalculateWidgetPrice(""XYZ"", ""Infisy"", 1#, 1#, 1#)"
    Debug.Print "Result: " & Format(curResult, "0.00")
    
        

End Function

Public Function PrintReceipt(strCustomer As String, strPart As String _
                        , curPrice As Currency _
                        , bolToFile As Boolean) As String

     Dim strOutput, strHeader, strFooter, strDetail As String
     
     strOutput = ""
     strHeader = "Receipt" & vbCrLf & vbCrLf
     strHeader = strHeader & "---------------------" & vbCrLf
     strDetail = "Customer: " & strCustomer & vbCrLf
     strDetail = strDetail & "Part: " & strPart & vbCrLf
     strDetail = strDetail & "Price: " & Format(curPrice, "0.00") & vbCrLf
     strFooter = "---------------------" & vbCrLf
          
     strOutput = strHeader & strDetail & strFooter
     
     If bolToFile Then
        Open "C:\Receipt.txt" For Output As #1
             Print #1, strOutput
        Close #1
     End If
     
     PrintReceipt = strOutput
     
Exit Function

ExceptionHandler:
    Close #1
    Call ProcessExceptions(err)
     
End Function
