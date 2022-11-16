import time

import win32pipe, win32file, pywintypes

from .messages import (
    Circle,
    DetectionAnswer,
    DetectionRequest,
    Ellipse
)



class PipeClient():
    def __init__(self) -> None:
        self.handle: object

    def connect(self):
        try:
            self.handle = win32file.CreateFile(
                r'\\.\\pipe\\Foo',
                win32file.GENERIC_READ | win32file.GENERIC_WRITE,
                0,
                None,
                win32file.OPEN_EXISTING,
                0,
                None
            )
            res = win32pipe.SetNamedPipeHandleState(self.handle, None, None, None)
            if res == 0:
                print(f"SetNamedPipeHandleState return code: {res}")
        except pywintypes.error as err:
            print("PipeClient.send(): ", err)


    def send(self, img) -> 'tuple[list[Circle], list[Ellipse]]':
        img_b = img.tobytes()

        det_req = DetectionRequest(1, img)
        try:
            win32file.WriteFile(self.handle, det_req.to_bytes())
            retval, data = win32file.ReadFile(self.handle, 64*1024)
            # print("Response length: ", len(data))
            det_ans = DetectionAnswer(data)
            shapes = det_ans.get_shapes()
   
        except pywintypes.error as e:
            if e.args[0] == 2:
                print("no pipe, trying again in a sec")
                time.sleep(1)
            elif e.args[0] == 109:
                print("broken pipe, bye bye")
            print(e)

        return shapes

