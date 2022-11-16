import struct

from dataclasses import dataclass
from enum import IntEnum
from math import degrees
from typing import Union
 
import numpy as np
 
 
class MsgTypes(IntEnum):
    ERROR_ANS = 0
    DETECTION_REQ = 1
    DETECTION_ANS = 2
    SETTINGS_WRITE_REQ = 3
    SETTINGS_READ_ANS = 4


@dataclass
class Circle:
    center_x: float = 0
    center_y: float = 0
    r: float = 0


@dataclass
class Ellipse:
    center_x: float = 0
    center_y: float = 0
    theta_d: float = 0
    width: float = 0
    height: float = 0
 
 
@dataclass
class Message():
    msg_type: MsgTypes
    msg_no: int = 0
    payload_size: int = 0
 
 
class DetectionRequest(Message):
    def __init__(self, msg_no, img: 'np.ndarray'):
        super().__init__(
            msg_type=MsgTypes.DETECTION_REQ,
        )
        self.msg_no = msg_no
        self.img = img
        self._img_b = img.tobytes()
 
    def _get_img_shape(self, byte=False) -> Union[bytes, 'tuple[int]']:
        rows, cols = self.img.shape
        if (not byte):
            return (rows, cols)
 
        rows_b = rows.to_bytes(2, 'little')
        cols_b = cols.to_bytes(2, 'little')
 
        return rows_b + cols_b
 
    def _get_payload_size(self, byte=False) -> Union[bytes, int]:
        img_size = len(self._img_b)
        payload_size = img_size + 4  # 2*2 bytes for rows&cols
        if (not byte):
            return payload_size
        payload_size_b = payload_size.to_bytes(4, 'little')
 
        return payload_size_b
 
    def to_bytes(self) -> bytes:
        msg_type_b = self.msg_type.value.to_bytes(1, 'little')
        msg_no_b = self.msg_no.to_bytes(2, 'little')
        img_size_b = self._get_img_shape(True)
        payload_size_b = self._get_payload_size(True)
        msg = msg_type_b + msg_no_b + payload_size_b + img_size_b + self._img_b
 
        return msg
 
    def __str__(self):
        txt = (
            f"Message type: {self.msg_type}\n"
            f"Message no:   {self.msg_no}\n"
            f"Payload size: {self._get_payload_size()}\n"
            f"Payload:      {type(self.img)} with shape: {self.img.shape}"
        )
 
        return txt
 
    def __repr__(self) -> str:
        return self.to_bytes().hex(sep=':')
 

class DetectionAnswer(Message):
    def __init__(self, msg: 'list[bytes]') -> None:
        self.msg = msg
        header = self.parse_header()
        super().__init__(
            msg_type=header[0],
            msg_no=header[1],
            payload_size=header[2]
        )

        self.circle_no = 0
        self.ellipse_no = 0

    def get_shape_no(self):
        self.circle_no = int.from_bytes(self.msg[7:9], 'little')
        self.ellipse_no = int.from_bytes(self.msg[9:11], 'little')

        # print(self.circle_no)
        # print(self.ellipse_no)

    def get_shapes(self) -> 'tuple[list, list]':
        self.get_shape_no()

        circles = []
        ellipses = []
        idx_circle_begin = 7 + 4  # header + circle_no + ellipse_no
        idx_ellipse_begin = idx_circle_begin + self.circle_no * 24
        idx_ellipse_end = idx_ellipse_begin + self.ellipse_no * 32

        for idx in range(idx_circle_begin, idx_ellipse_begin, 24):
            center_x, center_y, r = struct.unpack('<ddd', self.msg[idx:idx+24])
            circle = Circle(center_x, center_y, r=r)
            circles.append(circle)
            # print(center_x, center_y, r)

        for idx in range(idx_ellipse_begin, idx_ellipse_end, 32):
            center_x, center_y, width, height, theta_r = struct.unpack('<ddIId', self.msg[idx:idx+32])
            theta_d = degrees(theta_r)
            ellipse = Ellipse(center_x, center_y, theta_d, width, height)
            ellipses.append(ellipse)

        return circles, ellipses


class SettingsSetRequest(Message):
    def __init__(self, msg_no):
        super().__init__(
            msg_type=MsgTypes.SETTINGS_WRITE_REQ,
        )
        self.msg_no = msg_no


class SettingsSetAnswer(Message):
    pass


class SettingsGetRequest(Message):
    pass


class SettingsGetAnswer(Message):
    pass


class DetectionAnswer(Message):
    def __init__(self, msg: 'list[bytes]') -> None:
        self.msg = msg
        header = self.parse_header()
        super().__init__(
            msg_type=header[0],
            msg_no=header[1],
            payload_size=header[2]
        )

    # TODO use abstract Answer class
    def parse_header(self) -> 'tuple[int]':
        msg_type = MsgTypes(self.msg[0])
        msg_no = int.from_bytes(self.msg[1:3], 'little')
        payload_size = int.from_bytes(self.msg[3:7], 'little')

        return msg_type, msg_no, payload_size
 
if __name__ == '__main__':
    img = np.array([[1, 2, 3], [4, 5, 6]], np.uint8)
    det_req = DetectionRequest(2, img)
    print(det_req)
    print(det_req.to_bytes())
 
    import cv2 as cv
 
    img = cv.imread('grayscale.png', cv.IMREAD_GRAYSCALE)
    det_req = DetectionRequest(2, img)
    print(det_req)
    print(repr(det_req))