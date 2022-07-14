# -*- coding: utf-8 -*-
# Copyright (c) Megvii, Inc. and its affiliates.

import cv2
import os
import time
from onnx_inference import chess_test

if __name__ == '__main__':
    
    while True:
        # model = input("ready\n")
        model = input()
        if model == '1':
            print("setBoard64")
            cap = cv2.VideoCapture(0)
            ret, frame = cap.read()
            cv2.imwrite("./000.jpg", frame)
            cap.release()
            chess_test()
            proc1="./setBoard64"
            os.system(proc1)
        elif model == '2':
            cap = cv2.VideoCapture(0)
            print("findMove")
            ret, frame = cap.read()
            cv2.imwrite("./000.jpg", frame)
            cap.release()
            chess_test()
            proc2="./findMove"
            os.system(proc2)
            

