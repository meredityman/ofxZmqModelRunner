import modelrunner as mr
import numpy as np

import tensorflow as tf
import tensorflow_hub as hub
import urllib

class ESRGANModel( mr.Model, mr.ImageInput, mr.ImageOutput):

    def setup(self, **kwargs):
        
        # the runtime initialization will not allocate all memory on the device to avoid out of GPU memory
        gpus = tf.config.experimental.list_physical_devices('GPU')
        for gpu in gpus:
            #tf.config.experimental.set_memory_growth(gpu, True)
            tf.config.experimental.set_virtual_device_configuration(gpu,
            [tf.config.experimental.VirtualDeviceConfiguration(memory_limit=4000)])

        # load model
        self.module = hub.load("https://tfhub.dev/captain-pool/esrgan-tf2/1")

    def forward(self, input):
        #input = tf.transpose(input, [2, 0, 1])
        input = tf.expand_dims(input, axis=0)
        tensor = tf.convert_to_tensor(input * 255, dtype=tf.float32)

        output =self. module.signatures['serving_default'](tensor)

        prediction = np.clip(output['rrdb_net'].numpy() / 255, 0.0, 1.0)

        print(prediction.shape)
        output = prediction.reshape(1, *self.output_shape)

        return output



if __name__ == "__main__":
    args = mr.parse_args()

    model = ESRGANModel(input_shape  = [128, 128, 3], output_shape = [512, 512, 3])

    modelServer = mr.ModelServer(args.socket_addr, model)
    modelServer.run()

