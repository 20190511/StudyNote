import sys
from PyQt5.QtWidgets import *
from PyQt5.QAxContainer import *
from PyQt5.QtCore import *
import time

TR_REQ_TIME_INTERVAL = 0.2

class Kiwoom (QAxWidget):
    def __init__(self):
        super().__init__()
        self._kiwoom_instance()
        self._set_signal_slot()

    def _kiwoom_instance(self):
        self.setControl("KHOPENAPI.KHOpenAPICtrl.1")         #setControl()은 메소드를 객체없이 사용 가능한 함수.

    def _set_signal_slot(self):
        self.OnEventConnect.connect(self._event_connect)        # 로그인 상태와 함께 연결 시도
        self.OnReceiveTrData.connect(self._receive_tr_data)      # tran값들을 넘겨주는 함수.

    """ 이벤트 함수  """
    #로그인 함수
    def comm_connect(self):
        self.dynamicCall("CommConnect()")
        self.login_event_loop = QEventLoop()
        self.login_event_loop.exec_()

    # 로그인 상태 함수.
    def _event_connect (self, err_code):
        if err_code == 0:
            print("연결됨")
        else:
            print("연결되지 않음")

        self.login_event_loop.exit()

    #시장구분에 따른 종목 코드 반환 (리스트 반환)
    def get_code_list_by_market (self, market):
        code_list = self.dynamicCall("GetCodeListByMarket(QString)", "0")
        code_list = code_list.split(";")
        return code_list[:-1]

    #종목코드의 한글 이름 반환 (하나 반환)
    def get_master_code_name (self, market_code):
        code_name = self.dynamicCall("GetMasterCodeName(QString)", market_code)
        return code_name

    def set_input_value (self, id, value):
        self.dynamicCall("SetInputValue(QString, QString)", id, value)

    def comm_rq_data (self, rqname, trcode, next, screen_no):
        self.dynamicCall("CommRqData(QString, QString, int, QString)", rqname, trcode, next, screen_no)
        self.tr_event_loop = QEventLoop()
        self.tr_event_loop.exec_()

    def get_comm_data (self, Trancode, reqname, index, item_name):
        ret = self.dynamicCall("GetCommData(QString, QString, int, QString)",
                                Trancode, reqname, index, item_name)
        return ret

    #레코드 반복횟수를 반환한다. (Record Repeat Count)
    def _get_repeat_cnt (self, trcode, rqname):
        ret = self.dynamicCall("GetRepeatCnt(QString, QString)", trcode, rqname)
        return ret

    #trans data를 넘겨주는 함수.
    def _receive_tr_data (self, screen_no, rqname, trcode, record_name, next, unused1, unused2, unused3, unused4):
        if next == '2':
            self.remained_data = True
        else:
            self.remained_data = False

        if rqname == "opt10081_req":
            self._opt10081(rqname, trcode)

        try:
            self.tr_event_loop.exit()
        except AttributeError:
            pass

    def _opt10081(self, rqname, trcode):
        data_cnt = self._get_repeat_cnt(trcode, rqname)   #데이터 개수 반환
        print("DataCnt = ", data_cnt)

        print("data = [일자, 시가, 고가, 저가, 현재가, 거래량]")
        for index_num in range(data_cnt):
            date = self.get_comm_data(trcode, rqname, index_num, "일자")
            open = self.get_comm_data(trcode, rqname, index_num, "시가")
            high = self.get_comm_data(trcode, rqname, index_num, "고가")
            low = self.get_comm_data(trcode, rqname, index_num, "저가")
            close = self.get_comm_data(trcode, rqname, index_num, "현재가")
            volume = self.get_comm_data(trcode, rqname, index_num, "거래량")
            print(date, open, high, low, close, volume)


    def login_and_input(self):
            #기본값 설정



        market_code = input("종목 코드 :")
        std_date = input("기준 일자 :")
        if market_code == "":
            market_code = "039490"
        if std_date == "":
            std_date = "20210320"


        self.comm_connect()
        name = self.get_master_code_name(market_code)
        print("해당 종목코드의 회사 이름 : ",name)
            #opt10081요청
        self.set_input_value("종목코드", market_code)
        self.set_input_value("기준일자", std_date)
        self.set_input_value("수정주가구분", 1)

        self.comm_rq_data("opt10081_req", "opt10081", 0, "0101")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    kiwoom = Kiwoom()
    kiwoom.login_and_input()
