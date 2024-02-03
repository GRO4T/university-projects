from tests.reversi import full_standard_game
from tests.reversi import no_valid_moves_pass
from tests.reversi import no_valid_moves_finish

import tests.ai as ai


if __name__ == "__main__":
    print("Running all reversi engine tests...")
    full_standard_game.run_test()
    no_valid_moves_pass.run_test()
    no_valid_moves_finish.run_test()

    print("Running all algorithm tests...")

    print("Running 1 vs 1...")
    ai.run_test(1, 1)
    print("Running 1 vs 3...")
    ai.run_test(1, 3)
    print("Running 2 vs 3...")
    ai.run_test(2, 3)
    print("Running 4 vs 3...")
    ai.run_test(4, 3)

    print("All tests passed successfully!")
