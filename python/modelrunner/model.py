import numpy as np
from numpy.lib.financial import npv
import zmq

class Model:
    
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.setup(**kwargs)

    def setup(self, **kwargs):
        raise NotImplementedError()

    def forward(self, input):
        raise NotImplementedError()

    def _forward(self, input):
        return self.forward(input)


class ImageInput:

    def __init__(self, input_shape, **kwargs):
        super().__init__(**kwargs)
        self.input_shape = np.array(input_shape)

    def receive(self, socket, flags=0, copy=True, track=False):
        msg = socket.recv(flags=flags)
        buf =  memoryview(msg)
        arr = np.frombuffer(buf, dtype=np.intc)
        batch_size = arr[0]
        shape      = arr[1:]

        if not np.array_equal(shape,self.input_shape):
            pass
    
        msg = socket.recv(flags=flags|zmq.RCVMORE, copy=copy, track=track)
        buf = memoryview(msg)
        image = np.frombuffer(buf, dtype=np.single)
        return image.reshape(shape)

class ImageOutput:

    def __init__(self, output_shape, **kwargs):
        super().__init__(**kwargs)
        self.output_shape = np.array(output_shape)

    def send(self, socket, data, flags=0, copy=True, track=False):
        assert(data.dtype == np.single)

        if not np.array_equal(data.shape,self.output_shape):
            pass

        desc = np.array([1] + list(data.shape), dtype=np.intc)
        print(desc)

        socket.send(desc, flags|zmq.SNDMORE, track=track)

        return socket.send(data, flags=flags, copy=copy, track=track)

class TextInput:

    def receive(self, socket, flags=0, copy=False, track=False):
        return socket.recv_string(flags=flags)

class TextOutput:

    def send(self, data, socket, flags=0, copy=False, track=False):
        socket.send_string(data, flags=flags)

