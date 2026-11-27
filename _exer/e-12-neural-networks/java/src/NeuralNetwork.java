import java.text.DecimalFormat;
import java.util.*;

public class NeuralNetwork {

    final int[] layers;
    private DecimalFormat df;

    private ArrayList<Neuron> inputLayer = new ArrayList<Neuron>();
    private ArrayList<Neuron> hiddenLayer = new ArrayList<Neuron>();
    private ArrayList<Neuron> outputLayer = new ArrayList<Neuron>();
    private Neuron bias = new Neuron();

    final double learningRate = 0.9f;
    final double momentum = 0.7f;

    // Inputs for xor problem
    final double inputs[][] = { { 1, 1 }, { 1, 0 }, { 0, 1 }, { 0, 0 } };

    // Corresponding outputs, xor training data
    final double expectedOutputs[][] = { { 0 }, { 1 }, { 1 }, { 0 } };
    double resultOutputs[][] = { { -1 }, { -1 }, { -1 }, { -1 } }; // dummy init
    double output[];

    // for weight update all
    final HashMap<String, Double> weightUpdate = new HashMap<String, Double>();

    public static void main(String[] args) {
        NeuralNetwork nn = new NeuralNetwork(2, 4, 1);
        int maxRuns = 50000;
        double minErrorCondition = 0.001;
        nn.run(maxRuns, minErrorCondition);
    }

    public NeuralNetwork(int input, int hidden, int output) {
        this.layers = new int[] { input, hidden, output };
        df = new DecimalFormat("#.########");

        /**
         * Create all neurons and connections Connections are created in the
         * neuron class
         */
        for (int i = 0; i < layers.length; i++) {
            if (i == 0) { // input layer
                for (int j = 0; j < layers[i]; j++) {
                    Neuron neuron = new Neuron();
                    inputLayer.add(neuron);
                }
            } else if (i == 1) { // hidden layer
                for (int j = 0; j < layers[i]; j++) {
                    Neuron neuron = new Neuron();
                    neuron.addInConnectionsS(inputLayer);
                    neuron.addBiasConnection(bias);
                    hiddenLayer.add(neuron);
                }
            } else if (i == 2) { // output layer
                for (int j = 0; j < layers[i]; j++) {
                    Neuron neuron = new Neuron();
                    neuron.addInConnectionsS(hiddenLayer);
                    neuron.addBiasConnection(bias);
                    outputLayer.add(neuron);
                }
            } else {
                System.out.println("!Error NeuralNetwork init");
            }
        }

        // initialize random weights
        for (Neuron neuron : hiddenLayer) {
            ArrayList<Connection> connections = neuron.getAllInConnections();
            for (Connection conn : connections) {
                double newWeight = getRandom();
                conn.setWeight(newWeight);
            }
        }
        for (Neuron neuron : outputLayer) {
            ArrayList<Connection> connections = neuron.getAllInConnections();
            for (Connection conn : connections) {
                double newWeight = getRandom();
                conn.setWeight(newWeight);
            }
        }

        // reset id counters
        Neuron.counter = 0;
        Connection.counter = 0;
    }

    double getRandom() {
        return 0.1 * Math.random() - 0.05; // random weight in (-0.05, 0.05)
    }

    public void setInput(double inputs[]) {
        for (int i = 0; i < inputLayer.size(); i++) {
            inputLayer.get(i).setOutput(inputs[i]);
        }
    }

    public double[] getOutput() {
        double[] outputs = new double[outputLayer.size()];
        for (int i = 0; i < outputLayer.size(); i++)
            outputs[i] = outputLayer.get(i).getOutput();
        return outputs;
    }

    /**
     * Calculate the output of the neural network based on the input The forward
     * operation
     */
    public void activate() {
        for (Neuron n : hiddenLayer)
            n.calculateOutput();
        for (Neuron n : outputLayer)
            n.calculateOutput();
    }

    /**
     * all output propagate back
     *
     * @param expectedOutput
     *            first calculate the partial derivative of the error with
     *            respect to each of the weight leading into the output neurons
     *            bias is also updated here
     */
    public void applyBackpropagation(double expectedOutput[]) {

        // error check, normalize value [0;1]
        for (int i = 0; i < expectedOutput.length; i++) {
            double d = expectedOutput[i];
            if (d < 0 || d > 1) {
                if (d < 0)
                    expectedOutput[i] = 0;
                else
                    expectedOutput[i] = 1;
            }
        }

        int i = 0;
        for (Neuron n : outputLayer) {
            ArrayList<Connection> connections = n.getAllInConnections();
            for (Connection con : connections) {
                double ak = n.getOutput();
                double ai = con.getFromNeuron().getOutput();
                double desiredOutput = expectedOutput[i];

                double partialDerivative = -ak * (1 - ak) * ai * (desiredOutput - ak);
                double deltaWeight = -learningRate * partialDerivative;
                double newWeight = con.getWeight() + deltaWeight;
                con.setDeltaWeight(deltaWeight);
                con.setWeight(newWeight + momentum * con.getPrevDeltaWeight());
            }
            i++;
        }

        // update weights for hidden layer
        for (Neuron n : hiddenLayer) {
            ArrayList<Connection> connections = n.getAllInConnections();
            for (Connection con : connections) {
                double aj = n.getOutput();
                double ai = con.getFromNeuron().getOutput();
                double sumKDeltas = 0;
                int outputIndex = 0;
                for (Neuron outputNeuron : outputLayer) {
                    double wjk = outputNeuron.getConnection(n.id).getWeight();
                    double ak = outputNeuron.getOutput();
                    double desiredOutput = expectedOutput[outputIndex];
                    double dk = -ak * (1 - ak) * (desiredOutput - ak);
                    sumKDeltas += wjk * dk;
                    outputIndex++;
                }
                double partialDerivative = aj * (1 - aj) * ai * sumKDeltas;
                double deltaWeight = -learningRate * partialDerivative;
                double newWeight = con.getWeight() + deltaWeight;
                con.setDeltaWeight(deltaWeight);
                con.setWeight(newWeight + momentum * con.getPrevDeltaWeight());
            }
        }
    }

    void run(int maxSteps, double minError) {
        int i;
        // Train neural network until minError reached or maxSteps exceeded
        double error = 1;
        for (i = 0; i < maxSteps && error > minError; i++) {
            error = 0;
            for (int p = 0; p < inputs.length; p++) {
                setInput(inputs[p]);

                activate();

                output = getOutput();
                resultOutputs[p] = output;

                for (int j = 0; j < expectedOutputs[p].length; j++) {
                    double err = Math.pow(output[j] - expectedOutputs[p][j], 2);
                    error += err;
                }

                applyBackpropagation(expectedOutputs[p]);
            }
        }
        printResult();

        System.out.println("Sum of squared errors = " + error);
        System.out.println("##### EPOCH " + i + "\n");
        if (i == maxSteps) {
            System.out.println("!Error: failed to converge");
        } else {
            System.out.println("SUCCESS: converged");
        }
    }

    void printResult() {
        System.out.println("NN example with XOR");
        for (int p = 0; p < inputs.length; p++) {
            System.out.print("INPUTS: ");
            for (int x = 0; x < layers[0]; x++) {
                System.out.print(inputs[p][x] + " ");
            }

            System.out.print("EXPECTED: ");
            for (int x = 0; x < layers[2]; x++) {
                System.out.print(expectedOutputs[p][x] + " ");
            }

            System.out.print("ACTUAL: ");
            for (int x = 0; x < layers[2]; x++) {
                System.out.print(df.format(resultOutputs[p][x]) + " ");
            }
            System.out.println();
        }
        System.out.println();
    }
}
