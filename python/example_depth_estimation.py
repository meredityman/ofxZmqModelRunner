import modelrunner as mr
import numpy as np

import tensorflow as tf
import tensorflow_hub as hub
import urllib

class MiDaSModel( mr.Model, mr.ImageInput, mr.ImageOutput):

    def setup(self, **kwargs):
        
        # the runtime initialization will not allocate all memory on the device to avoid out of GPU memory
        gpus = tf.config.experimental.list_physical_devices('GPU')
        for gpu in gpus:
            #tf.config.experimental.set_memory_growth(gpu, True)
            tf.config.experimental.set_virtual_device_configuration(gpu,
            [tf.config.experimental.VirtualDeviceConfiguration(memory_limit=4000)])

        # load model
        self.module = hub.load("https://tfhub.dev/intel/midas/v2_1_small/1", tags=['serve'])

    def forward(self, input):
        input = tf.transpose(input, [2, 0, 1])
        input = tf.expand_dims(input, axis=0)

        tensor = tf.convert_to_tensor(input, dtype=tf.float32)
        output =self. module.signatures['serving_default'](tensor)

        prediction = output['default'].numpy()
        prediction = prediction.reshape(1, *self.output_shape)

        depth_min = prediction.min()
        depth_max = prediction.max()
        output = ((prediction - depth_min) / (depth_max - depth_min))

        return output


if __name__ == "__main__":
    args = mr.parse_args()

    model = MiDaSModel(input_shape  = [256, 256, 3], output_shape = [256, 256, 1])

    modelServer = mr.ModelServer(args.socket_addr, model)
    modelServer.run()

