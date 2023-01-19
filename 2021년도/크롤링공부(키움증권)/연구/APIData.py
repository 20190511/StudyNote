import sys
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QAxContainer import *

class MyWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.kiwoom = QAxWidget("KHOPENAPI.KHOpenAPICtrl.1")
        self.kiwoom.dynamicCall("CommConnect()")

        self.setWindowTitle("종목 코드")
        self.setGeometry(300, 300, 450, 1000)

        btn1 = QPushButton("종목코드 얻기", self)
        btn1.move(290, 20)
        btn1.clicked.connect(self.btn1_clicked)

            #QListWidget 객체 : 얻어온 종목코드리스트를 띄우는 칸을 의미함.
        self.listWidget = QListWidget(self)
        self.listWidget.setGeometry(10, 10, 270, 900) #크기 조절.

    def btn1_clicked(self):
            #GetDCodeListByMarket(["0"]) : 장내 종목 코드를 가져오는 메소드.
        ret = self.kiwoom.dynamicCall("GetCodeListByMarket(QString)", ["0"])
        kospi_code_list = ret.split(';')
        kospi_code_name_list = []


        for x in kospi_code_list:
                # 종목 코드->종목 한글 이름 : 리스트 형태로 데이터 가져오기.
            name = self.kiwoom.dynamicCall("GetMasterCodeName(QString)", [x])
            kospi_code_name_list.append(x + " : " + name)
        #가져온 리스트를 listWidget에 추가하기 listWidget.addItems()
        self.listWidget.addItems(kospi_code_name_list)

        return kospi_code_name_list


if __name__ == "__main__":
    app = QApplication(sys.argv)
    myWindow = MyWindow()
    myWindow.show()
    app.exit(app.exec_())


    list = myWindow.btn1_clicked()
    num = list.__sizeof__()
    for i in range (num):
        print(list[i])
