import math
import random


class Connection:
    counter = 0

    def __init__(self, from_neuron, to_neuron):
        self.weight = 0.0
        self.prev_delta_weight = 0.0
        self.delta_weight = 0.0
        self.left_neuron = from_neuron
        self.right_neuron = to_neuron
        self.id = Connection.counter
        Connection.counter += 1

    def get_weight(self):
        return self.weight

    def set_weight(self, w):
        self.weight = w

    def set_delta_weight(self, w):
        self.prev_delta_weight = self.delta_weight
        self.delta_weight = w

    def get_prev_delta_weight(self):
        return self.prev_delta_weight

    def get_from_neuron(self):
        return self.left_neuron

    def get_to_neuron(self):
        return self.right_neuron


class Neuron:
    counter = 0

    def __init__(self):
        self.id = Neuron.counter
        Neuron.counter += 1
        self.bias = -1.0
        self.output = 0.0
        self.bias_connection = None
        self.in_connections = []
        self.connection_lookup = {}

    @staticmethod
    def sigmoid(x):
        return 1.0 / (1.0 + math.exp(-x))

    def calculate_output(self):
        s = sum(c.get_weight() * c.get_from_neuron().output for c in self.in_connections)
        self.output = self.sigmoid(s)

    def add_in_connections(self, in_neurons):
        for n in in_neurons:
            con = Connection(n, self)
            self.in_connections.append(con)
            self.connection_lookup[n.id] = con

    def add_bias_connection(self, n):
        con = Connection(n, self)
        self.bias_connection = con
        self.in_connections.append(con)

    def get_connection(self, neuron_index):
        return self.connection_lookup.get(neuron_index)


class NeuralNetwork:
    def __init__(self, input_n, hidden_n, output_n, lr=0.9, mom=0.7):
        self.layers = [input_n, hidden_n, output_n]
        self.learning_rate = lr
        self.momentum = mom

        self.bias = Neuron()
        self.bias.output = 1.0

        self.input_layer  = [Neuron() for _ in range(input_n)]
        self.hidden_layer = []
        self.output_layer = []

        for _ in range(hidden_n):
            n = Neuron()
            n.add_in_connections(self.input_layer)
            n.add_bias_connection(self.bias)
            self.hidden_layer.append(n)

        for _ in range(output_n):
            n = Neuron()
            n.add_in_connections(self.hidden_layer)
            n.add_bias_connection(self.bias)
            self.output_layer.append(n)

        self._init_weights()
        Neuron.counter = 0
        Connection.counter = 0

    def _get_random(self):
        return random.uniform(-0.05, 0.05)

    def _init_weights(self):
        for n in self.hidden_layer + self.output_layer:
            for c in n.in_connections:
                c.set_weight(self._get_random())

    def set_input(self, inputs):
        for i, n in enumerate(self.input_layer):
            n.output = inputs[i]

    def get_output(self):
        return [n.output for n in self.output_layer]

    def activate(self):
        for n in self.hidden_layer:
            n.calculate_output()
        for n in self.output_layer:
            n.calculate_output()

    def apply_backpropagation(self, expected_output):
        target = [max(0.0, min(1.0, t)) for t in expected_output]

        # Output layer
        for i, n in enumerate(self.output_layer):
            for con in n.in_connections:
                ak = n.output
                ai = con.get_from_neuron().output
                desired = target[i]
                partial = -ak * (1.0 - ak) * ai * (desired - ak)
                dw = -self.learning_rate * partial
                new_w = con.get_weight() + dw
                con.set_delta_weight(dw)
                con.set_weight(new_w + self.momentum * con.get_prev_delta_weight())

        # Hidden layer
        for n in self.hidden_layer:
            for con in n.in_connections:
                aj = n.output
                ai = con.get_from_neuron().output
                sum_k = 0.0
                for out_idx, out_n in enumerate(self.output_layer):
                    wjk = out_n.get_connection(n.id)
                    if wjk is None:
                        continue
                    ak = out_n.output
                    desired = target[out_idx]
                    dk = -ak * (1.0 - ak) * (desired - ak)
                    sum_k += wjk.get_weight() * dk
                partial = aj * (1.0 - aj) * ai * sum_k
                dw = -self.learning_rate * partial
                new_w = con.get_weight() + dw
                con.set_delta_weight(dw)
                con.set_weight(new_w + self.momentum * con.get_prev_delta_weight())

    def run(self, inputs, expected_outputs, max_steps, min_error, func_name):
        result_outputs = [[-1.0] * self.layers[2] for _ in inputs]

        error = 1.0
        i = 0
        while i < max_steps and error > min_error:
            error = 0.0
            for p, inp in enumerate(inputs):
                self.set_input(inp)
                self.activate()
                out = self.get_output()
                result_outputs[p] = out
                for j in range(len(expected_outputs[p])):
                    error += (out[j] - expected_outputs[p][j]) ** 2
                self.apply_backpropagation(expected_outputs[p])
            i += 1

        self._print_result(func_name, inputs, expected_outputs, result_outputs,
                           i, error, max_steps)

    def _print_result(self, func_name, inputs, expected, results, epoch, error, max_steps):
        print(f"Training {func_name} for {max_steps} epochs max (H={self.layers[1]}, lr={self.learning_rate})")
        for p in range(len(inputs)):
            inp_str  = " ".join(str(int(v)) for v in inputs[p])
            exp_str  = " ".join(str(int(v)) for v in expected[p])
            act_str  = " ".join(f"{v:.8f}" for v in results[p])
            print(f"  INPUTS: {inp_str}  EXPECTED: {exp_str}  ACTUAL: {act_str}")
        print(f"  Sum of squared errors = {error:.2e}")
        print(f"  EPOCH {epoch}")
        if epoch == max_steps:
            print("  !Error: failed to converge\n")
        else:
            print("  SUCCESS: converged\n")


def train_function(name, inputs, expected_outputs, H=4, lr=0.9, mom=0.7,
                   max_runs=50000, min_error=0.001):
    Neuron.counter = 0
    Connection.counter = 0
    nn = NeuralNetwork(2, H, 1, lr, mom)
    nn.run(inputs, expected_outputs, max_runs, min_error, name)


if __name__ == "__main__":
    random.seed(42)

    inputs = [[0, 0], [0, 1], [1, 0], [1, 1]]

    train_function("XOR", inputs, [[0], [1], [1], [0]])
