#
import sys
from PyQt5.QtWidgets import *
from PyQt5.QtGui import  *
from PyQt5.QAxContainer import *

class MyWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        """ 키움 관련 이벤트 """
        # Kiwoom Login
        self.kiwoom = QAxWidget("KHOPENAPI.KHOpenAPICtrl.1") #키움 증권 레제시트리 값을 가져오는 함수코드.
        self.kiwoom.dynamicCall("CommConnect()")

        #OpenAPI + Event
        self.kiwoom.OnEventConnect.connect(self.event_connect)
        self.kiwoom.OnReceiveTrData.connect(self.receive_trdata)
        """ --- """

        self.setWindowTitle("PyStock")
        self.setGeometry(300, 300, 1000, 750)

        label = QLabel('종목코드: ', self)   #텍스트나 이미지 출력시 사용.
        label.move(20, 20) #출력 위치 조절


        #기본적으로 들어가 있는 값.
        self.code_edit = QLineEdit(self)
        self.code_edit.move(80, 20)
        self.code_edit.setText("039490")

        btn1 = QPushButton("조회", self)
        btn1.move(190, 20)
        btn1.clicked.connect(self.btn1_clicked) #btn1을 클릭하면-> 연결

        self.text_edit = QTextEdit(self)
        self.text_edit.setGeometry(10, 60, 980, 730)
        self.text_edit.setEnabled(False)

    def event_connect(self, err_code):
        if err_code == 0:
            self.text_edit.append("로그인 성공")

    def btn1_clicked(self):
        code = self.code_edit.text()                       ##현재 입력중인 코드.
        self.text_edit.append("종목코드: " + code)

        #SetInputValue("종목코드", String code) -> TR 입력값 설정.
        self.kiwoom.dynamicCall("SetInputValue(QString, QString)", "종목코드", code)

            #서버로 데이터 송신.
        #CommRqData("Rqname(request name = 사용자이름 명)", "opt10001", 0, "화면크기" = (기본값)"0101") -> 데이터 송신.
        self.kiwoom.dynamicCall("CommRqData(QString, QString, int, QString)", "opt10001_req", "opt10001", 0, "0101")
            #rqname을 opt10001_req로 설정함.


    #onReceiveTrData() 함수 활용. -> 서버로부터 데이터를 받았음을 의미함.
    def receive_trdata(self, screen_no, rqname, trcode, recordname, prev_next, data_len, err_code, msg1, msg2):
        if rqname == "opt10001_req": #reqname이 일치하는지 확인.
                #CommGetData(trcode(인자값), "", rqname(인자값), 0, "종목명" or "거래량")

            name = self.kiwoom.dynamicCall("CommGetData(QString, QString, QString, int , QString)", trcode, "", rqname, 0 , "종목명")
            self.text_edit.append("종목명: "+name.strip())
            data = self.kiwoom.dynamicCall("GetCommData(QString, QString, int, QString)", trcode, "", 1, "현재가")
            self.text_edit.append("현재가 : " +data.strip())


"""
            for i in range(600):
                data = self.kiwoom.dynamicCall("GetCommData(QString, QString, int, QString)", trcode, "", i, "현재가")
                date = self.kiwoom.dynamicCall("GetCommData(QString, QString, int, QString)", trcode, "", i, "일자")
                self.text_edit.append("현재가 : "+ data)
                self.text_edit.append("일자 : "+ date)
"""

if __name__ == "__main__":
    app = QApplication(sys.argv)
    myWindow = MyWindow()
    myWindow.show()
    app.exec_()
