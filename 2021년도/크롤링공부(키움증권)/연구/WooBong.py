import sys
from PyQt5.QtWidgets import *
from PyQt5.QAxContainer import *
from PyQt5.QtCore import *
from operator import *
import numpy as np
import time

TR_REQ_TIME_INTERVAL = 0.2

class Kiwoom (QAxWidget):
    """ 키움 시작 함수 (Request -> Response) """
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

    #trans data를 넘겨주는 함수.
    def _receive_tr_data (self, screen_no, rqname, trcode, record_name, next, unused1, unused2, unused3, unused4):
        if next == '2':
            self.remained_data = True
        else:
            self.remained_data = False

        if rqname == "opt10081_req":
            self._opt10081(rqname, trcode)
        elif rqname == "opt10082_req":
            self._opt10081(rqname, trcode)

        try:
            self.tr_event_loop.exit()
        except AttributeError:
            pass


    """ Requestion + Response"""
    # Request에 보내는함수
    def set_input_value (self, id, value):
        self.dynamicCall("SetInputValue(QString, QString)", id, value)

    """
        sRQName   – 사용자구분명 
        sTrCode   - Tran명 입력 
        nPrevNext - 0:조회, 2:연속 
        sScreenNo - 4자리의 화면번호 : 기본값 0101
            ex) CommRqData( “RQ_1”, “OPT00001”, 0, “0101”); 
    """
    # Requestion1 : 앞의 SetInputValue()를 송신하는 함수
    def comm_rq_data (self, rqname, trcode, next, screen_no):
        self.dynamicCall("CommRqData(QString, QString, int, QString)", rqname, trcode, next, screen_no)
        self.tr_event_loop = QEventLoop()
        self.tr_event_loop.exec_()


    """
        (str)  sArrCode   – 종목리스트 
        (bool) bNext      – 연속조회요청 (bool 타입)
        (int)  nCodeCount – 종목개수 
        (int)  nTypeFlag  – 조회구분 (0:주식관심종목정보, 3:선물옵션관심종목정보)
        (str)  sRQName    – 사용자구분명 
        (str)  sScreenNo  – 화면번호[4] -> 0101
            ex) CommKwRqData(“000660;005930”, 0, 2, 0, “RQ_1”, “0101”)
    """
    # Requestion2 : 복수개의 Tran을 요청하는 함수. (type_flag = 조회 구분, next = 연속조회 요청 유무) -> 사용 예정 없음.
    def comm_kw_rq_data(self, code_arr, next, code_count, type_flag, rqname, screen_no):
        check_val = self.dynamicCall("CommKwRqData(QString, bool, int, int, QString, QString)",
                         code_arr, next, code_count, type_flag, rqname, screen_no)
        self.tr_event_loop = QEventLoop()
        self.tr_event_loop.exec_()
        return check_val

    # Response :Data를 받아오는 함수
    def get_comm_data (self, Trancode, reqname, index, item_name):
        ret = self.dynamicCall("GetCommData(QString, QString, int, QString)",
                                Trancode, reqname, index, item_name)
        return ret

    #레코드 반복횟수를 반환한다. (Record Repeat Count)
    def _get_repeat_cnt (self, trcode, rqname):
        ret = self.dynamicCall("GetRepeatCnt(QString, QString)", trcode, rqname)
        return ret


    #일봉+주봉 차트 기준
    def _opt10081(self, rqname, trcode):
        data_list = []
        data_cnt = self._get_repeat_cnt(trcode, rqname)   #데이터 개수 반환
        #print("DataCnt = ", data_cnt)

        #print("data = [일자, 시가, 고가, 저가, 현재가, 거래량]")
        for index_num in range(data_cnt):
            temp_list = []
            date = self.get_comm_data(trcode, rqname, index_num, "일자")
            open = self.get_comm_data(trcode, rqname, index_num, "시가")
            high = self.get_comm_data(trcode, rqname, index_num, "고가")
            low = self.get_comm_data(trcode, rqname, index_num, "저가")
            close = self.get_comm_data(trcode, rqname, index_num, "현재가")
            volume = self.get_comm_data(trcode, rqname, index_num, "거래량")
            #print(date, open, high, low, close, volume)
            temp_list.append(date)
            temp_list.append(open)
            temp_list.append(high)
            temp_list.append(low)
            temp_list.append(close)
            temp_list.append(volume)
            data_list.append(temp_list)

        self.temp_list = data_list
