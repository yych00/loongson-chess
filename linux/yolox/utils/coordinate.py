#!/usr/bin/env python3
# -*- coding:utf-8 -*-
# Copyright (c) 2014-2021 Megvii Inc. All rights reserved.

__all__ = ["coo"]


def coo(boxes, scores, cls_ids, conf=0.5, class_names=None):
    file_handle = open('coordinate.txt', mode='w')
    file_handle.writelines(['x0\ty0\tx1\ty1\tname\tscore\n'])
    for i in range(len(boxes)):
        box = boxes[i]
        cls_id = int(cls_ids[i])
        score = scores[i]

        if score < conf:
            continue
        x0 = int(box[0])
        y0 = int(box[1])
        x1 = int(box[2])
        y1 = int(box[3])

        text_names = '{}'.format(class_names[cls_id])
        text_score = '{:.1f}%'.format(score * 100)
        file_handle.writelines([str(x0), '\t', str(y0), '\t', str(x1), '\t', str(y1), '\t'])
        file_handle.writelines([text_names, '\t', text_score, '\n'])
    file_handle.close()
