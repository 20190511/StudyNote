import sys
from PyQt5.QtWidgets import *
from PyQt5.QAxContainer import *
from PyQt5.QtCore import *
from operator import *
import numpy as np
import time
from weekBong import *

TR_REQ_TIME_INTERVAL = 0.2

class KiwoomApplication (Kiwoom):
    def __init__(self):
        super().__init__()
        self.temp_list = []
        self.total_dict = {}


        # 1. 시장구분에 따른 종목 코드 반환 (리스트 반환)
    def get_code_list_by_market(self, market):
        code_list = self.dynamicCall("GetCodeListByMarket(QString)", market)
        code_list = code_list.split(";")
        return code_list[:-1]

        # 2. 종목코드의 한글 이름 반환 (하나 반환)
    def get_master_code_name(self, market_code):
        code_name = self.dynamicCall("GetMasterCodeName(QString)", market_code)
        return code_name

        # 3. 주식 회사 "상장일"을 반환해줌.
    def get_master_listed_stock_date(self, market_code):
        date_of_company = self.dynamicCall("GetMasterListedStockDate (QString)", market_code)
        return date_of_company





    """ 함수 활용 함수들. """

        # 주식차트 정렬 함수.
    def _list_of_code(self):
        code_list = self.get_code_list_by_market(0)
        name_list = []

        cnt = len(code_list)
        for index_num in range(cnt):
            total_list = []
            company_name = self.get_master_code_name(code_list[index_num])
            date_of_company = self.get_master_listed_stock_date(code_list[index_num])
            # print(code_list[index_num], " : ", company_name)
            # print("상장일 : ", date_of_company, "\n")
            total_list.append(code_list[index_num])
            total_list.append(company_name)
            total_list.append(date_of_company)
            name_list.append(total_list)
        return name_list

        # 상장일로 정렬된 차트.
    def sort_of_stock_listing_date(self):
        name_dict = self._list_of_code()
        sort_list = sorted(name_dict, key=itemgetter(2))
        return sort_list

        #시가 총액을 가져오는 함수
    def pickup_capitalization


    ## 모든 종목코드의 차트를 가져오기. -> API 호출 제약 때문에 100개까지 밖에 불가능함.
    def pick_up_all_chart(self):
        code_list = self.get_code_list_by_market(0)
        code_num = len(code_list)
        print(code_num)
        code_dict = {}
        check_point = 1
            ##키움 API에서 한번 조회당 100회까지 제한되어있음.
        for code_index in range (100):
            time.sleep(TR_REQ_TIME_INTERVAL)
            market_code = code_list[code_index]
            std_date = "20210320"
            end_date = str(int(std_date)-20000)

                #주봉기준으로 작성
            self.set_input_value("종목코드", market_code)
            self.set_input_value("기준일자", std_date)
            self.set_input_value("끝일자", end_date)
            self.set_input_value("수정주가구분", 1)

            self.comm_rq_data("opt10082_req", "opt10082", 0, "0101")
            code_dict[market_code] = self.temp_list

            print(check_point)
            check_point += 1

        self.total_dict = code_dict

        #주봉 일봉 호출 함수.
    def login_and_input(self):
            #기본값 설정
        check = input("(일봉=1, 주봉=2)) : ")
        check = check.strip()

        market_code = input("종목 코드 (ex)039490)   :")
        std_date = input("기준 일자 (ex)20210320) : ")
        end_date = input("시작 날짜 (ex)20180320) : ")

        if market_code == "":
            market_code = "039490"
        if std_date == "":
            std_date = "20210320"
        if end_date == "":
            end_date = str(int(std_date)-20000)

        name = self.get_master_code_name(market_code)
        print("해당 종목코드의 회사 이름 : ",name)
            #opt10081요청
        self.set_input_value("종목코드", market_code)
        self.set_input_value("기준일자", std_date)
            #opt10082요청의 경우
        if check == "2":
            self.set_input_value("끝일자", end_date)
        self.set_input_value("수정주가구분", 1)

        if check == "1":
            self.comm_rq_data("opt10081_req", "opt10081", 0, "0101")
        elif check == "2":
            self.comm_rq_data("opt10082_req", "opt10082", 0, "0101")


if __name__ == "__main__":
    app = QApplication(sys.argv)
    kiwoom = KiwoomApplication()
    kiwoom.comm_connect()   #로그인 창 띄우기.
    kiwoom.pickup_all_chart()
