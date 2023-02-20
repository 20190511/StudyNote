import shutil
import os
from pathlib import Path

#prev 와 next 는 독립적인 폴더로 제작할 것 같은 폴더영역 안에 있으면 로직이 꼬일 수 있다.
def moving (prev, next):
    prev_all = list(os.walk(prev))
    next_all = list(os.walk(next))

    prev_file_set = [] #files name, mother_folder, original_path
    find_folder_name = set()
    for a,b,c in prev_all:
        for files in c:
            prev_original_path = a+"\\"+files
            parent_folder_name = a.split("\\")[-1]
            prev_file_set.append([files, parent_folder_name, prev_original_path])
            find_folder_name.add(parent_folder_name)


    fold_dictionary = dict()
    #dictionary 에는 "\\"가 추가되지 않은 버전임.
    for a,b,c in next_all:
        folder_names = a.split("\\")[-1]
        if folder_names in find_folder_name:
            fold_dictionary.update({folder_names:a})

    for files_name, mother_folder, original_path in prev_file_set:
        path = next+"\\"+mother_folder
        if mother_folder in fold_dictionary:
            path = fold_dictionary[mother_folder]

        file_path = path + "\\" + files_name
        if not os.path.isdir(path):
            print("make_folder: -->" + path)
            os.mkdir(path)

        if os.path.isfile(original_path) and  os.path.isdir(path):
            print(original_path + " --> " + file_path)
            shutil.move(original_path, file_path)
        else:
            print("X")


prev = "C:\\Users\\baejh\\PycharmProjects\\pythonProject\\AA"
next = "C:\\Users\\baejh\\PycharmProjects\\pythonProject\\A"
moving(prev,next)
