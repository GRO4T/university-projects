from app.reversi import Reversi
from app.minimax import Minimax
import app.minimax_nopruning as nopruning


def run_test(depth1: int, depth2: int) -> None:
    reversi = Reversi()
    minimax = Minimax()

    while not reversi.is_finished:
        move = (-1, -1)
        move_np = (-1, -1)

        if reversi.current_player == 1:
            move = minimax.get_optimal_move(reversi, depth1)
            move_np = nopruning.get_optimal_move(reversi, depth1)
        elif reversi.current_player == 2:
            move = minimax.get_optimal_move(reversi, depth2)
            move_np = nopruning.get_optimal_move(reversi, depth2)

        assert move == move_np
        assert reversi.make_a_move(move[0], move[1])
