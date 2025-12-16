import zipfile
import sys
import os

name = sys.argv[1]

def add_folder_to_zip(zip_handle, folder_path, arcname=""):
    for root, dirs, files in os.walk(folder_path):
        for file in files:
            file_path = os.path.join(root, file)
            arc_path = os.path.relpath(file_path, os.path.dirname(folder_path))
            if arcname:
                arc_path = os.path.join(arcname, arc_path)
            zip_handle.write(file_path, arc_path)

# Использование:
proj = name + '.zip'
fold = name+'/'
fold2 = name+'/'
file = name+'.txt'
file2 = name+'.txt'
with zipfile.ZipFile(proj, 'w', zipfile.ZIP_DEFLATED) as zipf:
    add_folder_to_zip(zipf, fold, fold2)
    zipf.write(file, file2)
