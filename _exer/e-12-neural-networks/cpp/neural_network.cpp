#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <string>

class Neuron;

// ─────────────────────────────────────────────────────────────
// Connection
// ─────────────────────────────────────────────────────────────
class Connection {
public:
    double weight        = 0.0;
    double prevDeltaWeight = 0.0; // for momentum
    double deltaWeight   = 0.0;

    Neuron* leftNeuron;
    Neuron* rightNeuron;

    static int counter;
    int id;

    Connection(Neuron* fromN, Neuron* toN)
        : leftNeuron(fromN), rightNeuron(toN), id(counter++) {}

    double getWeight() const { return weight; }
    void   setWeight(double w) { weight = w; }

    void setDeltaWeight(double w) {
        prevDeltaWeight = deltaWeight;
        deltaWeight = w;
    }

    double getPrevDeltaWeight() const { return prevDeltaWeight; }

    Neuron* getFromNeuron() const { return leftNeuron; }
    Neuron* getToNeuron()   const { return rightNeuron; }
};

int Connection::counter = 0;

// ─────────────────────────────────────────────────────────────
// Neuron
// ─────────────────────────────────────────────────────────────
class Neuron {
public:
    static int counter;
    int id;

    Connection* biasConnection = nullptr;
    const double bias = -1.0;
    double output = 0.0;

    std::vector<Connection*> inConnections;
    std::map<int, Connection*> connectionLookup;

    Neuron() : id(counter++) {}

    ~Neuron() {
        for (auto* c : inConnections) delete c;
    }

    static double sigmoid(double x) {
        return 1.0 / (1.0 + std::exp(-x));
    }

    void calculateOutput() {
        double s = 0.0;
        for (auto* con : inConnections) {
            Neuron* left = con->getFromNeuron();
            s += con->getWeight() * left->output;
        }
        // bias handled by its own connection in inConnections already
        output = sigmoid(s);
    }

    void addInConnectionsS(std::vector<Neuron*>& inNeurons) {
        for (auto* n : inNeurons) {
            auto* con = new Connection(n, this);
            inConnections.push_back(con);
            connectionLookup[n->id] = con;
        }
    }

    void addBiasConnection(Neuron* n) {
        auto* con = new Connection(n, this);
        biasConnection = con;
        inConnections.push_back(con);
    }

    Connection* getConnection(int neuronIndex) {
        return connectionLookup.count(neuronIndex) ? connectionLookup[neuronIndex] : nullptr;
    }
};

int Neuron::counter = 0;

// ─────────────────────────────────────────────────────────────
// NeuralNetwork
// ─────────────────────────────────────────────────────────────
class NeuralNetwork {
public:
    int layers[3];
    double learningRate;
    double momentum;

    std::vector<Neuron*> inputLayer;
    std::vector<Neuron*> hiddenLayer;
    std::vector<Neuron*> outputLayer;
    Neuron* bias;

    std::vector<std::vector<double>> inputs;
    std::vector<std::vector<double>> expectedOutputs;
    std::vector<std::vector<double>> resultOutputs;
    std::vector<double> output;

    NeuralNetwork(int inputN, int hiddenN, int outputN,
                  double lr, double mom)
        : learningRate(lr), momentum(mom)
    {
        layers[0] = inputN;
        layers[1] = hiddenN;
        layers[2] = outputN;

        bias = new Neuron();
        bias->output = 1.0; // bias output is +1; bias value in Neuron is -1 (absorbed into weight)

        for (int j = 0; j < inputN; j++)
            inputLayer.push_back(new Neuron());

        for (int j = 0; j < hiddenN; j++) {
            auto* n = new Neuron();
            n->addInConnectionsS(inputLayer);
            n->addBiasConnection(bias);
            hiddenLayer.push_back(n);
        }

        for (int j = 0; j < outputN; j++) {
            auto* n = new Neuron();
            n->addInConnectionsS(hiddenLayer);
            n->addBiasConnection(bias);
            outputLayer.push_back(n);
        }

        initWeights();

        Neuron::counter   = 0;
        Connection::counter = 0;
    }

    ~NeuralNetwork() {
        for (auto* n : inputLayer)  delete n;
        for (auto* n : hiddenLayer) delete n;
        for (auto* n : outputLayer) delete n;
        delete bias;
    }

    double getRandom() {
        return 0.1 * ((double)std::rand() / RAND_MAX) - 0.05;
    }

    void initWeights() {
        for (auto* n : hiddenLayer)
            for (auto* c : n->inConnections)
                c->setWeight(getRandom());
        for (auto* n : outputLayer)
            for (auto* c : n->inConnections)
                c->setWeight(getRandom());
    }

    void setInput(const std::vector<double>& in) {
        for (int i = 0; i < (int)inputLayer.size(); i++)
            inputLayer[i]->output = in[i];
    }

    std::vector<double> getOutput() {
        std::vector<double> out;
        for (auto* n : outputLayer) out.push_back(n->output);
        return out;
    }

    void activate() {
        for (auto* n : hiddenLayer) n->calculateOutput();
        for (auto* n : outputLayer) n->calculateOutput();
    }

    void applyBackpropagation(const std::vector<double>& expectedOutput) {
        // Clamp expected to [0,1]
        std::vector<double> target = expectedOutput;
        for (auto& d : target) {
            if (d < 0) d = 0;
            if (d > 1) d = 1;
        }

        // Output layer
        int i = 0;
        for (auto* n : outputLayer) {
            for (auto* con : n->inConnections) {
                double ak = n->output;
                double ai = con->getFromNeuron()->output;
                double desired = target[i];

                double partialDerivative = -ak * (1.0 - ak) * ai * (desired - ak);
                double dw = -learningRate * partialDerivative;
                double newWeight = con->getWeight() + dw;
                con->setDeltaWeight(dw);
                con->setWeight(newWeight + momentum * con->getPrevDeltaWeight());
            }
            i++;
        }

        // Hidden layer
        for (auto* n : hiddenLayer) {
            for (auto* con : n->inConnections) {
                double aj = n->output;
                double ai = con->getFromNeuron()->output;
                double sumKDeltas = 0.0;
                int outIdx = 0;
                for (auto* outN : outputLayer) {
                    Connection* wjk = outN->getConnection(n->id);
                    if (!wjk) { outIdx++; continue; }
                    double ak      = outN->output;
                    double desired = target[outIdx];
                    double dk = -ak * (1.0 - ak) * (desired - ak);
                    sumKDeltas += wjk->getWeight() * dk;
                    outIdx++;
                }
                double partialDerivative = aj * (1.0 - aj) * ai * sumKDeltas;
                double dw = -learningRate * partialDerivative;
                double newWeight = con->getWeight() + dw;
                con->setDeltaWeight(dw);
                con->setWeight(newWeight + momentum * con->getPrevDeltaWeight());
            }
        }
    }

    void run(int maxSteps, double minError, const std::string& funcName) {
        resultOutputs.assign(inputs.size(), std::vector<double>(layers[2], -1));

        int i;
        double error = 1.0;
        for (i = 0; i < maxSteps && error > minError; i++) {
            error = 0.0;
            for (int p = 0; p < (int)inputs.size(); p++) {
                setInput(inputs[p]);
                activate();
                output = getOutput();
                resultOutputs[p] = output;

                for (int j = 0; j < (int)expectedOutputs[p].size(); j++) {
                    double err = std::pow(output[j] - expectedOutputs[p][j], 2);
                    error += err;
                }
                applyBackpropagation(expectedOutputs[p]);
            }
        }
        printResult(funcName, i, error, maxSteps);
    }

    void printResult(const std::string& funcName, int epoch, double error, int maxSteps) {
        std::cout << "Training " << funcName
                  << " for " << maxSteps << " epochs max"
                  << " (H=" << layers[1] << ", lr=" << std::defaultfloat << learningRate << ")\n";

        for (int p = 0; p < (int)inputs.size(); p++) {
            std::cout << "  INPUTS:";
            for (double v : inputs[p]) std::cout << " " << (int)v;
            std::cout << "  EXPECTED:";
            for (double v : expectedOutputs[p]) std::cout << " " << (int)v;
            std::cout << "  ACTUAL:";
            for (double v : resultOutputs[p])
                std::cout << " " << std::fixed << std::setprecision(8) << v;
            std::cout << "\n";
        }

        std::cout << "  Sum of squared errors = " << std::scientific << error << "\n";
        std::cout << "  EPOCH " << epoch << "\n";
        if (epoch == maxSteps)
            std::cout << "  !Error: failed to converge\n\n";
        else
            std::cout << "  SUCCESS: converged\n\n";
    }
};

// ─────────────────────────────────────────────────────────────
// train one boolean function
// ─────────────────────────────────────────────────────────────
void trainFunction(const std::string& name,
                   const std::vector<std::vector<double>>& ins,
                   const std::vector<std::vector<double>>& outs,
                   int H, double lr, double mom,
                   int maxRuns, double minError)
{
    Neuron::counter     = 0;
    Connection::counter = 0;

    NeuralNetwork nn(2, H, 1, lr, mom);
    nn.inputs          = ins;
    nn.expectedOutputs = outs;
    nn.run(maxRuns, minError, name);
}

int main() {
    std::srand(42);

    const int    H        = 4;
    const double LR       = 0.9;
    const double MOM      = 0.7;
    const int    MAX_RUNS = 50000;
    const double MIN_ERR  = 0.001;

    std::vector<std::vector<double>> inputs = {
        {0, 0}, {0, 1}, {1, 0}, {1, 1}
    };

    // AND
    trainFunction("AND", inputs,
        {{0},{0},{0},{1}},
        H, LR, MOM, MAX_RUNS, MIN_ERR);

    // OR
    trainFunction("OR", inputs,
        {{0},{1},{1},{1}},
        H, LR, MOM, MAX_RUNS, MIN_ERR);

    // XOR
    trainFunction("XOR", inputs,
        {{0},{1},{1},{0}},
        H, LR, MOM, MAX_RUNS, MIN_ERR);

    return 0;
}
