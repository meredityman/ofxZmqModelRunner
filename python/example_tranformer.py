import modelrunner as mr
import numpy as np
from transformers import pipeline

class DistilbertEmotion ( mr.Model, mr.TextInput, mr.TextOutput ):

    def setup(self):
        self.classifier = pipeline("text-classification",model='bhadresh-savani/distilbert-base-uncased-emotion', return_all_scores=False)

    def forward(self, input):
        result = self.classifier(input, )
        label = result[0]["label"]
        return label


if __name__ == "__main__":
    args = mr.parse_args()
    print("Making model")
    model = DistilbertEmotion()

    modelServer = mr.ModelServer(args.socket_addr, model)
    modelServer.run()