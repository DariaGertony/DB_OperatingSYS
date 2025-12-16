import zipfile
import sys
import os

name = sys.argv[1]
dist = "."
proj = name+".zip"

with zipfile.ZipFile(proj, 'r', zipfile.ZIP_DEFLATED) as zipf:
    zipf.extractall(dist)

try:
    os.remove(proj)
except FileNotFoundError:
    pass
