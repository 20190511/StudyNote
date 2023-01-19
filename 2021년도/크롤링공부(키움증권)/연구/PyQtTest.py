import sys
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QAxContainer import *


class MyWindow (QMainWindow):
    def __init__(self):
        super().__init__()

        # login 처리
        self.kiwoom = QAxWidget("KHOPENAPI.KHOpenAPICtrl.1")
        self.kiwoom.dynamicCall("CommConnect()")

        self.setWindowTitle("상장일 반환.")
        self.setGeometry(300, 300, 460, 1000)

        btn1 = QPushButton("종목 코드 반환", self)
        btn1.move(290, 20)
        btn1.resize(150, 30)
        btn1.clicked.connect(self.btn1_clicked)

        self.listWidget = QListWidget(self)
        self.listWidget.setGeometry(10, 10, 300, 900)  # 크기 조절.


    def btn1_clicked(self):
        ret = self.kiwoom.dynamicCall("GetCodeListByMarket(QString)", ["0"])
        list_code = ret.split(';')
        list_code_name = []

        for i in list_code:
            name = self.kiwoom.dynamicCall("GetMasterCodeName(QString)", [i])
            date = self.kiwoom.dynamicCall("GetMasterListedStockDate(QString)", [i])

            list_code_name.append(i+" : "+name+" \n상장일 : "+date+"\n")
        self.listWidget.addItems(list_code_name)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    myWindow = MyWindow()
    myWindow.show()
    app.exit(app.exec_())
