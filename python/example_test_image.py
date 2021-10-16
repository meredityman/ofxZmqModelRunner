import modelrunner as mr
import numpy as np


class ImageTestModel( mr.Model, mr.ImageInput, mr.ImageOutput ):

    def setup(self, shape):
        pass

    def forward(self, input):
        return np.random.random(self.output_shape).astype(dtype=np.single)


if __name__ == "__main__":
    args = mr.parse_args()

    model = ImageTestModel(input_shape = [64, 64, 3], output_shape = [64, 64, 3])

    modelServer = mr.ModelServer(args.socket_addr, model)
    modelServer.run()