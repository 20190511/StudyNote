import sys
from PyQt5.QtWidgets import *
from PyQt5.QAxContainer import *
from PyQt5.QtCore import *
from pykiwoom.kiwoom import *
from operator import *
import os
import pandas as pd
import datetime
import time

class KiwoomEvent():
    def __init__(self):
        self.today = self.today_date()
        self.__data_path = "D:/PycharmProject/Test/PyKiwoomProject/dayData"

        """ 멤버 변수 """
        self.merge_filename = []
        self.code_company_date = []
        self.sorted_code_company_date = [] #상장일 순으로 정렬된 Code 리스트
        self.code_list = []
        self.test_aggregate = "empty"

        #로그인
        self.kiwoom = Kiwoom()
        self.kiwoom.ocx.OnReceiveRealData.connect(self._handler_real_data)
        self.kiwoom.CommConnect(block=True)     #block는 자동으로 True 처리 (Blocking 처리)


        #오늘 날짜 얻기
    def today_date(self):
        now = datetime.datetime.now()
        today = now.strftime("%Y%m%d")
        return today

        #현 로그인 상태
    def connect_code(self):
        err_code = self.kiwoom.GetConnectState()
        if err_code == 1:
            print("연결됨.")
        elif err_code == 0:
            print("연결되지 않음.")

        #전 종목의 코드 받아오기 (block_request함수를 이용해서 한꺼번에 데이터를 받아오는 것이 가능)
    def all_chart_code_extract(self):
        kospi = self.kiwoom.GetCodeListByMarket("0")
        kosdaq = self.kiwoom.GetCodeListByMarket("10")
        all_chart = kospi+kosdaq

        #파이썬 enumerate 사용법 : https://wikidocs.net/16045
        for i, code in enumerate(all_chart):
            print(f"{i}/{len(all_chart)} {all_chart}")
            dataframe_ = self.kiwoom.block_request("opt10081",
                                                   종목코드=code,
                                                   기준일자=self.today,
                                                   수정주가구분=1,
                                                   output="주식일봉차트조회",
                                                   next=0)
            out_name = f"{code}.xlsx"
            dataframe_.to_excel(self.__data_path, sheet_name=out_name)
            time.sleep(3.6)

        # 엑셀파일에서 데이터 추출.
    def all_chart_code_intract(self):
        file_list = os.listdir(self.__data_path)     #데이터 경로 설정.
        xlst_list = [num for num in file_list if num.endswith(".xlsx")]
        close_data = []
        about_data = input("데이터 종류 ex) 고가 or 저가 or 현재가 : ")

            # (종가)로 데이터 추출.
        for index_num, xls in enumerate (xlst_list):
            print("현재 작업 파일 = ", f"{index_num+1}/{len(xlst_list)}")
            code = xls.split(".")[0]
            data_name = self.__data_path+"/"+xls
            data_frame_ = pd.read_excel(data_name)
            data_frame2_ = data_frame_[["일자", about_data]].copy()
            data_frame2_.rename(columns={about_data: code}, inplace=True)
            data_frame2_ = data_frame2_.set_index("일자")
            data_frame2_ = data_frame2_[::-1]
            close_data.append(data_frame2_)

        data_frame_ = pd.concat(close_data, axis=1)
        file_name = "merge("f"{self.today}of{about_data}).xlsx"
        self.merge_filename.append(file_name)
        data_frame_.to_excel(file_name)

    """
        <Err_code>
             0   : 정상
            -200 : 시세조회 과부화
            -201 : 입력 구조체 생성 실패
            -202 : 전문작성 입력값 오류
    """
        #일봉차트 조회시 Send하는 역할 --> 후에 self.kiwoom.GetCommData()를 이용해서 데이터를 받아와야함.
    def _opt10001_send(self, market=""):
        if market == "":
            market = "039490"
        self.kiwoom.SetInputValue("종목코드", market)
        err_code = self.kiwoom.CommRqData("opt10001_req", "opt10001", 0, "0101")
        return err_code

    def _opt10081_send(self, market="", date="today"):
        if market == "":
            market = "039490"
        if date == "today":
            date = self.today
        self.kiwoom.SetInputValue("종목코드", market)
        self.kiwoom.SetInputValue("기준일자", date)
        self.kiwoom.SetInputValue("수정주가구분", 1)

        err_code = self.kiwoom.CommRqData("opt10081_req", "opt10081", 0, "0101")
        return err_code

    def _opt10082_send (self, market="", date="today", end_date="today"):
        if market == "":
            market = "039490"
        if date == "today":
            date = self.today
        if end_date == "today":
            end_date = self.today - 20000

        self.kiwoom.SetInputValue("종목코드", market)
        self.kiwoom.SetInputValue("기준일자", date)
        self.kiwoom.SetInputValue("끝일자", end_date)
        self.kiwoom.SetInputValue("수정주가구분", 1)

        err_code = self.kiwoom.CommRqData("opt10082_req", "opt10082", 0, "0101")
        return err_code

    # 종목코드(0) + 회사명(1) + 상장일(2) -> 멤버변수 self.code_company_date에 저장.
    def _basic_data_(self):
        kospi = self.kiwoom.GetCodeListByMarket("0")
        kosdaq = self.kiwoom.GetCodeListByMarket("10")
        all_chart = kospi + kosdaq
        self.code_list = all_chart      #모든 코드 차트 저장
        code_len = len(all_chart)

        for index_num, code in enumerate(all_chart):
            temp_list = []
            company_name = self.kiwoom.GetMasterCodeName(code)
            company_date = self.kiwoom.GetMasterListedStockDate_original(code)
            temp_list.append(code)
            temp_list.append(company_name)
            temp_list.append(company_date)
            self.code_company_date.append(temp_list)
        self.sorted_code_company_date = sorted(self.code_company_date, key=itemgetter(2))  #상장일 순으로 정렬


    """ 실시간 데이터 사용시 -> 현재 사용불가 
        실시간 데이터는 SetRealReg()함수로 연결 -> 데이터전달 -> DisConnectRealData()로 연결 끊기
        이후 GetCommRealData로 데이터 받아옴.
    """
        #실시간 이벤트 처리 핸들러
    def _handler_real_data(self, market_code, real_type, real_data):
        if real_type == "시가총액":
            aggregate_value = self.kiwoom.GetCommRealData(market_code, 311)

        print("handler")
        try:
            self.real_loop.exit()
        except AttributeError:
            pass

        self.test_aggregate = aggregate_value


    """
        (str) screen_no : 실시간 등록할 화면 번호
        (str) code_list : 실시간 등록할 종목 코드 (; 구분자로 전달)
        (str) fid_list  : 실시간 등록할 FID (":FID1;FID2:FID3;...")
        (str) real_type : "0", "1" 타입
            -> "0" : 같은 화면에서 다른 종목 실시간 등록을 할 때 앞의 항목이 자동 "연결해제"됨
            -> "1" : 같은 화면에서 다른 종목 실시간 등록을 할 때 기존 종목도 실시간 시세를 받을 수 있음.
                !- 꼭 같은 화면이어야 하고 최초 등록은 "0"으로 하고 이후부터 "1"이 가능함 -!
    """
    # 실시간 데이터 연결 -> 2021_03-21 제작중,
    def real_data_open(self, code_arr, fid_list, real_type):
        self.kiwoom.SetRealReg("1000", code_arr, fid_list, real_type)
        print("real_data_open")
        self.real_loop = QEventLoop()
        self.real_loop.exec_()

    """
        screen_no : 실시간 해제할 화면 번호
        del_code  : 실시간 해제할 종목 코드.
    """
    # 실시간 데이터 연결해제
    def real_data_close(self, screen_no, del_code):
        self.SetRealRemove("1000", screen_no, del_code)
    """ 실시간 데이터 사용시"""


        #실시간 시가총액 데이터 불러오기.
    def _aggregate_value_company(self):
        self._basic_data_()   #선 리스트 제작.
        code_list = ";".join(self.code_list)
        fid_list = list("311")
        """
        data_num = len(self.code_list) - 1
        for i in range(data_num):
            fid_list.append(";311")
        """
        real_type = "0"

        self.kiwoom.SetRealReg("0101", code_list, fid_list, real_type)

        print("check_point")
        aggregate_list = []
        for index, code in enumerate(self.code_list):
            data = self.kiwoom.GetCommRealData(code, "311")
            aggregate_list.append(data)
            print(index," : ", data)

        self.kiwoom.SetRealRemove("ALL", "ALL")


    def real_test(self):
        test_code = "039490"
        test_fid = "311"
        real_type = "0"

        self.real_data_open(test_code, test_fid, real_type)
        print(self.test_aggregate)



kiwooms = KiwoomEvent()
    #주의 all_chart_code는 3시간가량 소요됨
#kiwooms.all_chart_code()
#kiwooms.all_chart_code_intract()
kiwooms.real_test()
