from app.reversi import Reversi


def run_test():
    reversi = Reversi()

    reversi.board = [[0, 0, 0, 0, 0, 0, 0, 0],
                     [0, 0, 0, 0, 0, 0, 0, 0],
                     [0, 0, 2, 2, 2, 0, 0, 0],
                     [0, 0, 2, 1, 2, 0, 0, 0],
                     [0, 0, 2, 2, 2, 0, 0, 0],
                     [0, 0, 2, 0, 2, 0, 0, 0],
                     [0, 0, 2, 2, 2, 0, 0, 0],
                     [0, 0, 0, 0, 0, 0, 0, 0]]

    assert reversi.current_player == 1

    assert reversi.make_a_move(5, 3)

    assert reversi.current_player == 1

    assert reversi.player1_points == 3
    assert reversi.player2_points == 12
    assert reversi.num_of_valid_moves == 16

    board = [[0, 0, 0, 0, 0, 0, 0, 0],
             [0, 0, 0, 0, 0, 0, 0, 0],
             [0, 0, 2, 2, 2, 0, 0, 0],
             [0, 0, 2, 1, 2, 0, 0, 0],
             [0, 0, 2, 1, 2, 0, 0, 0],
             [0, 0, 2, 1, 2, 0, 0, 0],
             [0, 0, 2, 2, 2, 0, 0, 0],
             [0, 0, 0, 0, 0, 0, 0, 0]]

    assert board == reversi.board

    assert not reversi.is_finished
