#!/usr/bin/env python3
# -*- coding: utf-8 -*-
# Copyright (c) Megvii, Inc. and its affiliates.

import os

import cv2
import numpy as np

import onnxruntime

from yolox.data.data_augment import preproc as preprocess
from yolox.data.datasets import VOC_CLASSES
from yolox.utils import mkdir, multiclass_nms, demo_postprocess, vis, coo  # vis has boxes

def chess_test():
    input_shape = tuple(map(int, "640,640".split(',')))
    origin_img = cv2.imread("000.jpg")
    img, ratio = preprocess(origin_img, input_shape)

    session = onnxruntime.InferenceSession("chess.onnx")

    ort_inputs = {session.get_inputs()[0].name: img[None, :, :, :]}
    output = session.run(None, ort_inputs)
    predictions = demo_postprocess(output[0], input_shape, p6=False)[0]

    boxes = predictions[:, :4]
    scores = predictions[:, 4:5] * predictions[:, 5:]

    boxes_xyxy = np.ones_like(boxes)
    boxes_xyxy[:, 0] = boxes[:, 0] - boxes[:, 2] / 2.
    boxes_xyxy[:, 1] = boxes[:, 1] - boxes[:, 3] / 2.
    boxes_xyxy[:, 2] = boxes[:, 0] + boxes[:, 2] / 2.
    boxes_xyxy[:, 3] = boxes[:, 1] + boxes[:, 3] / 2.
    boxes_xyxy /= ratio
    dets = multiclass_nms(boxes_xyxy, scores, nms_thr=0.45, score_thr=0.1)
    if dets is not None:
        final_boxes, final_scores, final_cls_inds = dets[:, :4], dets[:, 4], dets[:, 5]
        origin_img = vis(origin_img, final_boxes, final_scores, final_cls_inds,
                         conf=0.3, class_names=VOC_CLASSES)
        coo(final_boxes, final_scores, final_cls_inds, conf=0.25, class_names=VOC_CLASSES)
    mkdir("YOLOX_outputs")
    output_path = os.path.join("YOLOX_outputs", os.path.basename("000.jpg"))
    cv2.imwrite(output_path, origin_img)


if __name__ == '__main__':
    chess_test()
