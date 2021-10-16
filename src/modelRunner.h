#include "ofMain.h"
#include "ofxZmq.h"

class ModelRunner {

    public:
    ModelRunner(){}

    void setup( std::string & address ){
        ofLogNotice("ModelRunner::ModelRunner") << "Connecting to: " << address;
        this->address = address;
        request.connect(address);
        ofLogNotice("ModelRunner::ModelRunner") << "Connection successful!";
    }

    protected:
        std::string address;
		ofxZmqRequest request;
        bool isSetup;

};

class TextModelRunner : public ModelRunner {

    public:
    TextModelRunner() : ModelRunner(){};

    void sendText(const std::string& text){
        ofLogNotice("TextModelRunner::sendText") << address << " -> " << text;
        request.send(text);
    }

    void receiveText(std::string& text){
        request.receive(text);
        ofLogNotice("TextModelRunner::sendText") << address << " <- " << text;
    }


};

class ImageModelRunner : public  ModelRunner{

    public:

    ImageModelRunner() : ModelRunner(){};

    void sendPixels(const ofFloatPixels& pixels){
        ofLogNotice("ImageModelRunner::sendPixels") << address << " -> " << 
                    "[" <<  pixels.getWidth()       << ", " <<
                            pixels.getHeight()      << ", " <<
                            pixels.getNumChannels() << "]";

        int desc[4]{
            1,
            (int)pixels.getWidth(),  
            (int)pixels.getHeight(),
            (int)pixels.getNumChannels()
        };
        request.send((void*)&desc[0], sizeof(desc), false, true);

        request.send((void*)pixels.getData(), pixels.getTotalBytes());
    }

    void receivePixels(ofFloatPixels& pixels){
        
   
        request.receive(desc_buf);

        int *desc = (int*)desc_buf.getData();

        int batchSize = desc[0];
        std::vector<int> shape;
        size_t desc_size = desc_buf.size() / sizeof(int);
        shape.insert(shape.end(), &desc[1], &desc[desc_size]);

        ofPixelFormat format = (ofPixelFormat)(shape[2] - 1);

        request.receive(data_buf);

        pixels.setFromExternalPixels((float*)data_buf.getData(), shape[0], shape[1], format);

        ofLogNotice("ImageModelRunner::receivePixels") << address << " <- " << 
                    "[" << batchSize <<  ", " 
                    << shape[0] << ", " 
                    << shape[1] << ", " 
                    << ((int)format) + 1 << "]";
    }

    private:
     ofBuffer desc_buf;
    ofBuffer data_buf;
};
