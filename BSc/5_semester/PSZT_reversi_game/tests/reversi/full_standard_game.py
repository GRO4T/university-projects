from app.reversi import Reversi


def run_test():
    reversi = Reversi()

    assert reversi.player1_points == 2
    assert reversi.player2_points == 2
    assert reversi.num_of_valid_moves == 4
    assert reversi.valid_moves_list == [(2, 3), (3, 2), (4, 5), (5, 4)]

    assert reversi.make_a_move(2, 3)
    assert reversi.make_a_move(2, 2)
    assert reversi.make_a_move(2, 1)
    assert reversi.make_a_move(3, 5)

    assert reversi.player1_points == 4
    assert reversi.player2_points == 4
    assert reversi.num_of_valid_moves == 3
    assert len(reversi.valid_moves_list) == reversi.num_of_valid_moves

    assert reversi.make_a_move(5, 5)
    assert reversi.make_a_move(5, 2)
    assert reversi.make_a_move(3, 6)
    assert reversi.make_a_move(2, 5)

    assert reversi.player1_points == 8
    assert reversi.player2_points == 4
    assert reversi.num_of_valid_moves == 6
    assert len(reversi.valid_moves_list) == reversi.num_of_valid_moves

    assert reversi.make_a_move(5, 3)
    assert reversi.make_a_move(3, 7)
    assert reversi.make_a_move(1, 6)
    assert reversi.make_a_move(0, 7)

    assert reversi.player1_points == 7
    assert reversi.player2_points == 9
    assert reversi.num_of_valid_moves == 5
    assert len(reversi.valid_moves_list) == reversi.num_of_valid_moves

    assert reversi.make_a_move(4, 5)
    assert reversi.make_a_move(6, 5)
    assert reversi.make_a_move(5, 6)
    assert reversi.make_a_move(5, 7)

    assert reversi.player1_points == 8
    assert reversi.player2_points == 12
    assert reversi.num_of_valid_moves == 9
    assert len(reversi.valid_moves_list) == reversi.num_of_valid_moves

    assert reversi.make_a_move(7, 5)
    assert reversi.make_a_move(3, 2)
    assert reversi.make_a_move(1, 5)
    assert reversi.make_a_move(5, 4)

    assert reversi.player1_points == 8
    assert reversi.player2_points == 16
    assert reversi.num_of_valid_moves == 10
    assert len(reversi.valid_moves_list) == reversi.num_of_valid_moves

    board = [[0, 0, 0, 0, 0, 0, 0, 2],
             [0, 0, 0, 0, 0, 1, 2, 0],
             [0, 1, 1, 1, 0, 1, 0, 0],
             [0, 0, 2, 2, 2, 1, 2, 2],
             [0, 0, 0, 2, 2, 2, 0, 0],
             [0, 0, 2, 2, 2, 2, 2, 2],
             [0, 0, 0, 0, 0, 1, 0, 0],
             [0, 0, 0, 0, 0, 1, 0, 0]]

    assert board == reversi.board

    assert reversi.make_a_move(4, 7)
    assert reversi.make_a_move(0, 5)
    assert reversi.make_a_move(2, 7)
    assert reversi.make_a_move(1, 7)

    assert reversi.player1_points == 7
    assert reversi.player2_points == 21
    assert reversi.num_of_valid_moves == 7

    assert reversi.make_a_move(6, 6)
    assert reversi.make_a_move(1, 2)
    assert reversi.make_a_move(2, 6)
    assert reversi.make_a_move(7, 7)

    assert reversi.player1_points == 7
    assert reversi.player2_points == 25
    assert reversi.num_of_valid_moves == 12

    assert reversi.make_a_move(6, 7)
    assert reversi.make_a_move(2, 0)
    assert reversi.make_a_move(0, 1)
    assert reversi.make_a_move(7, 4)

    assert reversi.player1_points == 11
    assert reversi.player2_points == 25
    assert reversi.num_of_valid_moves == 15

    assert reversi.make_a_move(3, 1)
    assert reversi.make_a_move(2, 4)
    assert reversi.make_a_move(1, 0)
    assert reversi.make_a_move(0, 0)

    assert reversi.player1_points == 13
    assert reversi.player2_points == 27
    assert reversi.num_of_valid_moves == 12

    assert reversi.make_a_move(0, 6)
    assert reversi.make_a_move(0, 2)
    assert reversi.make_a_move(7, 3)
    assert reversi.make_a_move(3, 0)

    assert reversi.player1_points == 13
    assert reversi.player2_points == 31
    assert reversi.num_of_valid_moves == 9

    assert reversi.make_a_move(1, 1)
    assert reversi.make_a_move(1, 3)
    assert reversi.make_a_move(1, 4)
    assert reversi.make_a_move(4, 6)

    assert reversi.player1_points == 16
    assert reversi.player2_points == 32
    assert reversi.num_of_valid_moves == 9

    assert reversi.make_a_move(6, 4)
    assert reversi.make_a_move(0, 3)
    assert reversi.make_a_move(0, 4)
    assert reversi.make_a_move(6, 3)

    assert reversi.player1_points == 20
    assert reversi.player2_points == 32
    assert reversi.num_of_valid_moves == 6

    assert reversi.make_a_move(4, 2)
    assert reversi.make_a_move(4, 1)
    assert reversi.make_a_move(4, 0)
    assert reversi.make_a_move(5, 0)

    assert reversi.player1_points == 23
    assert reversi.player2_points == 33
    assert reversi.num_of_valid_moves == 3

    assert reversi.make_a_move(5, 1)
    assert reversi.make_a_move(7, 6)
    assert reversi.make_a_move(6, 2)
    assert reversi.make_a_move(7, 2)

    assert reversi.player1_points == 20
    assert reversi.player2_points == 40
    assert reversi.num_of_valid_moves == 2

    assert reversi.make_a_move(7, 1)
    assert reversi.make_a_move(7, 0)
    assert reversi.make_a_move(6, 1)
    assert reversi.make_a_move(6, 0)

    assert reversi.player1_points == 21
    assert reversi.player2_points == 43
    assert reversi.num_of_valid_moves == 0

    board = [[2, 2, 2, 2, 1, 1, 1, 2],
             [2, 1, 2, 1, 1, 1, 1, 2],
             [2, 2, 1, 2, 1, 2, 1, 2],
             [2, 1, 2, 1, 1, 2, 2, 2],
             [2, 1, 2, 1, 2, 2, 2, 2],
             [2, 2, 1, 1, 2, 1, 2, 2],
             [2, 2, 2, 2, 1, 2, 2, 1],
             [2, 2, 2, 2, 2, 2, 2, 2]]

    assert board == reversi.board

    assert reversi.is_finished
