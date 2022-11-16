from typing import Union

from .named_pipe import PipeClient
from .messages import Circle, Ellipse

class EDLibDetector():
    def __init__(self, r_range: tuple) -> None:
        self._r_min = r_range[0]
        self._r_max = r_range[-1]

        self._client = PipeClient()
        self._client.connect()

    def set_r_range(self, new_range: tuple):
        self._r_min = new_range[0]
        self._r_max = new_range[-1]

    def _radius_filter(self, shape: Union[Circle, Ellipse]) -> bool:
        if (isinstance(shape, Circle)):
            print(f"Circle: {shape.r}")
            if (shape.r >= self._r_min and shape.r <= self._r_max):
                return True

        elif (isinstance(shape, Ellipse)):
            print(f"Ellipse: {shape.r1}, {shape.r2}, {shape.rz}")
            if (shape.r1 >= self._r_min and shape.r1 <= self._r_max and
                shape.r2 >= self._r_min and shape.r2 <= self._r_max):
                return True
        return False

    def _filter_results(self, shapes: 'tuple[list, list]'):
        circles, ellipses = shapes
        # filter by radius
        for ellipse in ellipses:
            if (self._radius_filter(ellipse)):
                return ellipse
        
        for circle in circles:
            if (self._radius_filter(circle)):
                return circle


    def detect(self, img):
        shapes = self._client.send(img)
        # print(*shapes, sep="\n")
        if (self._r_min < 0 or self._r_max < 0):
            return shapes
        shape = self._filter_results(shapes)
        # print(shape)
        return shape