from network import Network

@given("network")
def step_impl(context):
    context.network = Network()
