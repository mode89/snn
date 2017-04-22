Feature: Core functionality

    Scenario: Construct network
        Given network with 100 neurons

    Scenario: Step network
        Given network with 100 neurons
        And step

    Scenario: Plot
        Given network with 100 neurons
        And monitor
        When step 1000 times
        Then plot output
