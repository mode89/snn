Feature: Recognize audio

    Scenario: Perform task on audio command
        Given recognizer
        When I say command
        Then perform task
