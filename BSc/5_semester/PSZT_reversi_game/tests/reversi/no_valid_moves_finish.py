from app.reversi import Reversi


def run_test():
    reversi = Reversi()

    assert reversi.make_a_move(2, 3)
    assert reversi.make_a_move(2, 4)
    assert reversi.make_a_move(3, 5)
    assert reversi.make_a_move(2, 6)

    assert reversi.make_a_move(2, 5)
    assert reversi.make_a_move(4, 2)
    assert reversi.make_a_move(2, 7)
    assert reversi.make_a_move(1, 5)

    assert reversi.make_a_move(3, 2)
    assert reversi.make_a_move(2, 2)
    assert reversi.make_a_move(1, 4)
    assert reversi.make_a_move(0, 4)

    assert reversi.make_a_move(2, 1)
    assert reversi.make_a_move(3, 7)
    assert reversi.make_a_move(4, 7)
    assert reversi.make_a_move(2, 0)

    assert reversi.player1_points == 3
    assert reversi.player2_points == 17
    assert reversi.num_of_valid_moves == 0

    board = [[0, 0, 0, 0, 2, 0, 0, 0],
             [0, 0, 0, 0, 2, 2, 0, 0],
             [2, 2, 2, 2, 2, 2, 2, 1],
             [0, 0, 2, 2, 2, 2, 0, 1],
             [0, 0, 2, 2, 2, 0, 0, 1],
             [0, 0, 0, 0, 0, 0, 0, 0],
             [0, 0, 0, 0, 0, 0, 0, 0],
             [0, 0, 0, 0, 0, 0, 0, 0]]

    assert board == reversi.board

    assert reversi.is_finished
