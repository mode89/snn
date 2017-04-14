from network import Monitor, Network
import matplotlib.pyplot as plt

@given("network")
def step_impl(context):
    context.network = Network()

@given("step")
def step_impl(context):
    context.network.step()

@given("monitor")
def step_impl(context):
    context.monitor = Monitor(context.network)

@when("step {n:d} times")
def step_impl(context, n):
    for i in range(n):
        context.network.step()
        if hasattr(context, "monitor"):
            context.monitor.step()

@then("plot output")
def step_impl(context):
    plt.scatter(
        context.monitor.x,
        context.monitor.y,
        color="black",
        marker=".")
    plt.show()
